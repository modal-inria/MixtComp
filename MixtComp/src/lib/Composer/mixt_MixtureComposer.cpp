/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 2, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Matthieu Marbac <matthieu.marbac-lourdelle@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#include "mixt_MixtureComposer.h"
#include "../Mixture/mixt_IMixture.h"
#include "../IO/mixt_IO.h"
#include "../Various/mixt_Timer.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Various/mixt_Various.h"

namespace mixt {

MixtureComposer::MixtureComposer(Index nbInd, Index nbClass,
		Real confidenceLevel) :
		idName_("z_class"), nClass_(nbClass), nInd_(nbInd), nVar_(0), prop_(
				nbClass), tik_(nbInd, nbClass), sampler_(zClassInd_, tik_,
				nbClass), paramStat_(prop_, confidenceLevel), dataStat_(
				zClassInd_), confidenceLevel_(confidenceLevel), completedProbabilityCache_(
				nInd_) {
	std::cout << "MixtureComposer::MixtureComposer, nbInd: " << nbInd
			<< ", nbClass: " << nbClass << std::endl;
	zClassInd_.setIndClass(nbInd, nbClass);

	std::stringstream sstm;
	sstm << "nbModality: " << nbClass;
	paramStr_ = sstm.str();
}

MixtureComposer::~MixtureComposer() {
	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		delete (*it);
	}
}

Real MixtureComposer::lnObservedProbability(int i, int k) const {
	Real sum = std::log(prop_[k]); // the joint probability p(x, z) is computed here, and will be marginalized over z later, for example in observedTik method

	for (Index j = 0; j < nVar_; ++j) { // use the cache
		sum += observedProbabilityCache_(j)(i, k);
	}

	return sum;
}

void MixtureComposer::printTik() const {
	std::cout << "Sampled t_ik" << std::endl;
	std::cout << tik_ << std::endl;
}

void MixtureComposer::observedTik(Vector<Real>& oZMode) const {
	oZMode.resize(nInd_);
	Matrix<Real> observedTikMat(nInd_, nClass_);

	Matrix<Real> lnComp(nInd_, nClass_);

	for (Index k = 0; k < nClass_; ++k) {
		for (Index i = 0; i < nInd_; ++i) {
			lnComp(i, k) = lnObservedProbability(i, k);
		}
	}

	Index mode;
	for (Index i = 0; i < nInd_; ++i) { // sum is inside a log, hence the numerous steps for the computation
		RowVector<Real> dummy;
		observedTikMat.row(i).logToMulti(lnComp.row(i));
		observedTikMat.row(i).maxCoeff(&mode);

		oZMode(i) = mode;
	}

	//  std::cout << "Computed t_ik" << std::endl;
	//  std::cout << observedTik << std::endl;
}

Real MixtureComposer::lnObservedLikelihood() {
	Real lnLikelihood = 0.;
	Matrix<Real> lnComp(nInd_, nClass_);

	for (Index k = 0; k < nClass_; ++k) {
		for (Index i = 0; i < nInd_; ++i) {
			lnComp(i, k) = lnObservedProbability(i, k);
		}
	}

	for (Index i = 0; i < nInd_; ++i) { // sum is inside a log, hence the numerous steps for the computation
		RowVector<Real> dummy;
		lnLikelihood += dummy.logToMulti(lnComp.row(i));
	}

	return lnLikelihood;
}

Real MixtureComposer::lnCompletedLikelihood() {
	Real lnLikelihood = 0.;

	for (Index i = 0; i < nInd_; ++i) { // completion is only on the latent class, latent data in models is marginalized over
		lnLikelihood += lnObservedProbability(i, zClassInd_.zi().data_(i));
	}

	return lnLikelihood;
}

Real MixtureComposer::lnCompletedProbability(int i, int k) const {
	Real sum = std::log(prop_[k]); // the joint probability p(x, z) is computed

	for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end();
			++it) {
		sum += (*it)->lnCompletedProbability(i, k);
	}

	return sum;
}

std::string MixtureComposer::mStep(const Vector<std::set<Index>>& classInd) {
	mStepPi(); // computation of z_ik frequencies, which correspond to ML estimator of proportions

	std::vector<std::string> vecWarnLog(nVar_);

#pragma omp parallel for // note that this is the only case where parallelism is not performed over observations, but over individuals
	for (Index v = 0; v < nVar_; ++v) {
		std::string currLog;
		currLog = v_mixtures_[v]->mStep(classInd); // call mStep on each variable
		if (0 < currLog.size()) {
			vecWarnLog[v] = "mStep error in variable: "
					+ v_mixtures_[v]->idName() + eol + currLog + eol;
		}
	}

	std::string warnLog;
	for (std::vector<std::string>::const_iterator i = vecWarnLog.begin();
			i != vecWarnLog.end(); ++i)
		warnLog += *i;

	return warnLog;
}

std::string MixtureComposer::mStep() {
	return mStep(zClassInd_.classInd());
}

void MixtureComposer::sampleZ() {
#pragma omp parallel for
	for (Index i = 0; i < nInd_; ++i) {
		sampleZ(i);
	}
}

void MixtureComposer::sampleZ(int i) {
	sampler_.sStepNoCheck(i);
}

void MixtureComposer::eStepCompleted() {
	bool correct[nInd_]; // std::vector<bool> causes errors in parallel writes: https://stackoverflow.com/questions/33617421/write-concurrently-vectorbool and http://www.cplusplus.com/reference/vector/

#pragma omp parallel for
	for (Index i = 0; i < nInd_; ++i) {
		RowVector<Real> lnComp(nClass_);

		bool correctVal = true;

		for (Index k = 0; k < nClass_; k++) {
			lnComp(k) = lnCompletedProbability(i, k);
		}

		if (minInf == lnComp.maxCoeff()) { // completed proba is non 0 in at least one class
			correctVal = false;
		}

		completedProbabilityCache_(i) = tik_.row(i).logToMulti(lnComp);
		correct[i] = correctVal;
	}
	std::list<Index> listIndErr;
	for (Index i = 0; i < nInd_; ++i) {
		if (correct[i] == false)
			listIndErr.push_back(i);
	}

	if (0 < listIndErr.size()) {
		std::string listInd, warnLog;
		for (std::list<Index>::const_iterator it = listIndErr.begin(), itEnd =
				listIndErr.end(); it != itEnd; ++it) {
			listInd += std::to_string(*it) + " ";
		}
		warnLog =
				"An unexpected error has occurred in the computation of the completed probability of the individuals "
						+ listInd
						+ ". This kind of error is most likely the result of a bug. Please contact the maintainer and provide your data, descriptors and lauch script.";
		throw(warnLog);
	}

//	std::cout << "MixtureComposer::eStepCompleted, tik" << std::endl;
//	std::cout << tik_ << std::endl;
}

void MixtureComposer::mStepPi() {
	prop_ = 0.;
	for (Index i = 0; i < zClassInd_.zi().data_.rows(); ++i) {
		prop_(zClassInd_.zi().data_(i)) += 1.;
	}
	prop_ = prop_ / prop_.sum();
}

void MixtureComposer::writeParameters() const {
	std::cout << "Composer nbFreeParameter = " << nbFreeParameters()
			<< std::endl;
	std::cout << "Composer proportions = " << itString(prop_) << std::endl;

	for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end();
			++it) {
		std::cout << "Parameters of " << (*it)->idName() << "\n";
		(*it)->writeParameters();
	}
}

int MixtureComposer::nbFreeParameters() const {
	int sum = nClass_ - 1; // proportions
	for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end();
			++it) {
		sum += (*it)->nbFreeParameter();
	}
	return sum;
}

void MixtureComposer::sampleUnobservedAndLatent() {
#pragma omp parallel for
	for (Index i = 0; i < nInd_; ++i) {
		sampleUnobservedAndLatent(i);
	}
}

void MixtureComposer::sampleUnobservedAndLatent(int i) {
	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->sampleUnobservedAndLatent(i, zClassInd_.zi().data_(i));
	}
}

std::string MixtureComposer::checkSampleCondition(
		const Vector<std::set<Index>>& classInd) const {
	std::string warnLog = checkNbIndPerClass(classInd);
	for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end();
			++it) {
		warnLog += (*it)->checkSampleCondition(classInd);
	}

	return warnLog;
}

std::string MixtureComposer::checkSampleCondition() const {
	return checkSampleCondition(zClassInd_.classInd());
}

std::string MixtureComposer::checkNbIndPerClass(
		const Vector<std::set<Index>>& classInd) const {
	for (Index k = 0; k < nClass_; ++k) {
		if (0 < classInd(k).size()) {
			continue;
		} else {
			std::stringstream sstm;
			sstm
					<< "MixtureComposer::checkNbIndPerClass: at least one class is empty. Maybe you asked for more classes "
					<< "than the number of observations you provided. Or the constraints on the classes of each observations "
					<< "in a (semi) supervised case are too tight."
					<< std::endl;
			return sstm.str();
		}
	}

	return "";
}

std::string MixtureComposer::checkNbIndPerClass() const {
	return checkNbIndPerClass(zClassInd_.classInd());
}

void MixtureComposer::storeSEMRun(int iteration, int iterationMax,
		RunType runType) {
	if (runType == burnIn_) {
		if (iteration == 0) {
			completedProbabilityLogBurnIn_.resize(iterationMax + 1);
		}
		completedProbabilityLogBurnIn_(iteration) =
				completedProbabilityCache_.sum();
	}

	if (runType == run_) {
		if (iteration == 0) {
			completedProbabilityLogRun_.resize(iterationMax + 1);
		}
		completedProbabilityLogRun_(iteration) =
				completedProbabilityCache_.sum();

		for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end();
				++it) {
			(*it)->storeSEMRun(iteration, iterationMax);
		}

		paramStat_.sampleParam(iteration, iterationMax);

		if (iteration == iterationMax) {
			paramStat_.normalizeParam(paramStr_); // enforce that estimated proportions sum to 1, but only if paramStr is of the form "nModality: x"
			paramStat_.setExpectationParam(); // replace pi by the median values
		}
	}
}

void MixtureComposer::setObservedProbaCache() {
	std::cout
			<< "MixtureComposer::setObservedProbaCache, this operation could take some time..."
			<< std::endl;
	observedProbabilityCache_.resize(nVar_);

	for (Index j = 0; j < nVar_; ++j) {
		observedProbabilityCache_(j) = Matrix<Real>(nInd_, nClass_);
		observedProbabilityCache_(j) = 0.;
	}

	for (Index j = 0; j < nVar_; ++j) {
#pragma omp parallel for
		for (Index i = 0; i < nInd_; ++i) {
			for (Index k = 0; k < nClass_; ++k) {
				observedProbabilityCache_(j)(i, k) =
						v_mixtures_[j]->lnObservedProbability(i, k);
			}
		}
	}
}

void MixtureComposer::storeGibbsRun(Index iteration, Index iterationMax) {
	for (Index ind = 0; ind < nInd_; ++ind) {
		dataStat_.sampleVals(ind, iteration, iterationMax);

		if (iteration == iterationMax) {
			dataStat_.imputeData(ind, tik_); // impute the missing values using empirical mean or mode, depending of the model. Latest completed tik are replaced by observed tik
		}

		for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end();
				++it) {
			(*it)->storeGibbsRun(ind, iteration, iterationMax);
		}
	}
}

void MixtureComposer::registerMixture(IMixture* p_mixture) {
	v_mixtures_.push_back(p_mixture);
	++nVar_;
}

std::vector<std::string> MixtureComposer::paramName() const {
	std::vector<std::string> names(nClass_);
	for (Index k = 0; k < nClass_; ++k) {
		std::stringstream sstm;
		sstm << "k: " << k + minModality;
		names[k] = sstm.str();
	}

	return names;
}

std::vector<std::string> MixtureComposer::mixtureName() const {
	std::vector<std::string> names(nVar_);
	for (Index j = 0; j < nVar_; ++j) {
		names[j] = v_mixtures_[j]->idName();
	}

	return names;
}

void MixtureComposer::initData() {
	tik_ = 1. / nClass_;
	sampleZ(); // since tik are uniform, this sStep corresponds to an uniform initialization of z. It takes into account the supervised / semisupervised constraints

	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
#pragma omp parallel for
		for (Index i = 0; i < nInd_; ++i) {
			(*it)->initData(i);
		}
	}
}

void MixtureComposer::initParam() {
	prop_ = 1. / nClass_; // this is roughly equivalent to an estimation by maximization of likelihood, considering that proportions in all t_ik are equal

	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->initParam();
	}
}

std::string MixtureComposer::initParamSubPartition(Index nInitPerClass) {
	std::string warnLog;

	Vector<std::set<Index>> partialClassInd(nClass_);
	Index nSubset = std::min(nInitPerClass * nClass_, nInd_);

	std::cout << "MixtureComposer::initParamSubPartition, " << nSubset
			<< " observations used in parameter initialization." << std::endl;

	Vector<Index> allInd(nInd_);
	for (Index i = 0; i < nInd_; ++i) {
		allInd(i) = i;
	}

	MultinomialStatistic multi;
	multi.shuffle(allInd);

	for (Index i = 0; i < nSubset; ++i) {
		Index currInd = allInd(i);
		partialClassInd(zClassInd_.zi().data_(currInd)).insert(currInd);
	}

//	for (Index k = 0; k < nClass_; ++k) {
//		std::cout << "k: " << k << ", " << itString(partialClassInd(k)) << std::endl;
//	}

	warnLog += checkSampleCondition(partialClassInd);
	if (0 < warnLog.size())
		return warnLog;

	warnLog += mStep(partialClassInd);

	return warnLog;
}

void MixtureComposer::E_kj(Matrix<Real>& ekj) const {
	ekj.resize(nClass_, nVar_);
	ekj = 0.;

	for (Index i = 0; i < nInd_; ++i) {
		for (Index j = 0; j < nVar_; ++j) {
			Vector<Real> lnP(nClass_); // ln(p(z_i = k, x_i^j))
			Vector<Real> t_ik_j(nClass_); // p(z_i = k / x_i^j)
			for (Index k = 0; k < nClass_; ++k) {
				lnP(k) = std::log(prop_(k))
						+ observedProbabilityCache_(j)(i, k);
			}
			t_ik_j.logToMulti(lnP); // "observed" t_ik, for the variable j
			Vector<Real> t_ink_j = 1. - t_ik_j; // The nj means: "all classes but k".

			for (Index k = 0; k < nClass_; ++k) {
				Real p;
//				Real nP;

				if (epsilon < t_ik_j(k)) {
					p = -t_ik_j(k) * std::log(t_ik_j(k));
				} else {
					p = 0.;
				}

				if (epsilon < t_ink_j(k)) {
//					nP = -t_ink_j(k) * std::log(t_ink_j(k));
				} else {
//					nP = 0.;
				}

				ekj(k, j) += p;
			}
		}
	}
}

// Added by Matthieu
void MixtureComposer::Delta(Matrix<Real>& delta) const {
	delta.resize(nVar_, nVar_);
	delta = 0.;
	for (Index i = 0; i < nInd_; ++i) {
		Matrix<Real> probacond(nClass_, nVar_); // P(Z_i=k|x_{ij}) k=1,...,K in row and j=1,...,d in column
		for (Index j = 0; j < nVar_; ++j) {
			Vector<Real> lnP(nClass_); // ln(p(z_i = k, x_i^j))
			for (Index k = 0; k < nClass_; ++k) {
				lnP(k) = std::log(prop_(k))
						+ observedProbabilityCache_(j)(i, k);
			}
			probacond.col(j).logToMulti(lnP); // "observed" t_ik, for the variable j
		}

		for (Index k = 0; k < nClass_; ++k) {
			for (Index j = 0; j < nVar_; ++j) {
				for (Index h = j; h < nVar_; ++h) {
					// Contribution of observation i to the distance between variables j and h for component k
					delta(j, h) += (probacond(k, j) - probacond(k, h))
							* (probacond(k, j) - probacond(k, h));
				}
			}
		}
	}
	for (Index j = 0; j < nVar_; ++j) {
		for (Index h = j; h < nVar_; ++h) {
			delta(j, h) = std::sqrt(delta(j, h) / nInd_); // Normalisation
			delta(h, j) = delta(j, h); // symetric matrix
		}
	}
}

void MixtureComposer::IDClass(Matrix<Real>& idc) const {
	idc.resize(nClass_, nVar_);

	if (nClass_ > 1) {
		Matrix<Real> ekj;
		E_kj(ekj);
		Real den = nInd_ * std::log(nClass_);

		for (Index k = 0; k < nClass_; ++k) {
			//    Real min = ekj.row(k).minCoeff();
			//    Real max = ekj.row(k).maxCoeff();

			for (Index j = 0; j < nVar_; ++j) {
				//      idc(k, j) = (max - ekj(k, j)) / (max - min);
				//      idc(k, j) = 1. - ekj(k, j) / ekj.row(k).sum();
				idc(k, j) = ekj(k, j) / den;
			}
		}
	} else {
		idc = 1.;
	}
}

void MixtureComposer::lnProbaGivenClass(Matrix<Real>& pGC) const {
	pGC.resize(nInd_, nClass_);

	for (Index i = 0; i < nInd_; ++i) {
		for (Index k = 0; k < nClass_; ++k) {
			pGC(i, k) = lnObservedProbability(i, k);
		}
	}
}

void MixtureComposer::printClassInd() const {
	zClassInd_.printState();
}

void MixtureComposer::computeObservedProba() {
	for (Index j = 0; j < nVar_; ++j) {
		v_mixtures_[j]->computeObservedProba();
	}
}

std::string MixtureComposer::initializeLatent() {
	std::string warnLog;

	computeObservedProba(); // whether the Gibbs comes after a SEM or is used in prediction, parameters are known at that point
	setObservedProbaCache();
	warnLog = eStepObserved();
	if (0 < warnLog.size()) {
		std::cout << "initializeLatent, eStepObserved failed." << std::endl;
		return warnLog;
	}

	sampleZ();
	initializeMarkovChain();
	sampleUnobservedAndLatent();
	warnLog = checkSampleCondition(); // TODO: might be useless, in this case, remove it
	if (0 < warnLog.size()) {
		std::cout << "initializeLatent, checkSampleCondition failed."
				<< std::endl;
		return warnLog;
	}

	return "";
}

void MixtureComposer::initializeMarkovChain() {
#pragma omp parallel for
	for (Index i = 0; i < nInd_; ++i) {
		for (Index j = 0; j < nVar_; ++j) {
			v_mixtures_[j]->initializeMarkovChain(i, zClassInd_.zi().data_(i));
		}
	}
}

std::string MixtureComposer::eStepObserved() {
	std::vector<bool> vecWarnLog(nInd_); // since the for loop can be executed in parallel, the individual results are stored in a vector to avoid race conditions

#pragma omp parallel for
	for (Index i = 0; i < nInd_; ++i) {
		vecWarnLog[i] = eStepObservedInd(i);
	}

	std::string tempWarnLog;
	for (Index i = 0; i < nInd_; ++i) {
		if (!vecWarnLog[i]) {
			tempWarnLog += "Observation " + std::to_string(i)
					+ " has a 0 density of probability." + eol;
		}
	}

	std::string warnLog;
	if (0 < tempWarnLog.size()) {
		warnLog = "Error in MixtureComposer::eStepObserved: " + eol
				+ tempWarnLog;
	}

//	std::cout << "MixtureComposer::eStepObservedInd, tik" << std::endl;
//	std::cout << tik_ << std::endl;

	return warnLog;
}

bool MixtureComposer::eStepObservedInd(Index i) {
	bool isIndividualObservable = true;

	RowVector<Real> lnComp(nClass_); // row vector, one index per class
	RowVector<Real> currVar(nClass_);

	for (Index k = 0; k < nClass_; k++) {
		lnComp(k) = std::log(prop_[k]);
	}

	for (Index j = 0; j < nVar_; ++j) {
		for (Index k = 0; k < nClass_; k++) {
			currVar(k) = observedProbabilityCache_(j)(i, k);
		}

		if (minInf < currVar.maxCoeff()
				|| !v_mixtures_[j]->sampleApproximationOfObservedProba()) {
			lnComp += currVar;
		}
	}

	if (lnComp.maxCoeff() == minInf) { // individual is not observable if its probability is 0 in every classes, in that case the run can not continue
		isIndividualObservable = false;
	}

	tik_.row(i).logToMulti(lnComp);

	return isIndividualObservable;
}

} /* namespace mixt */
