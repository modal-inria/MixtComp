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

MixtureComposer::MixtureComposer(Index nbInd,
		Index nbClass,
		Real confidenceLevel) :
						idName_("z_class"),
						nbClass_(nbClass),
						nbInd_(nbInd),
						nbVar_(0),
						prop_(nbClass),
						tik_(nbInd,
								nbClass),
								sampler_(*this,
										zClassInd_,
										tik_,
										nbClass),
										paramStat_(prop_,
												confidenceLevel),
												dataStat_(zClassInd_),
												confidenceLevel_(confidenceLevel) {
	std::cout << "MixtureComposer::MixtureComposer, nbInd: " << nbInd << ", nbClass: " << nbClass << std::endl;
	zClassInd_.setIndClass(nbInd, nbClass);

	std::stringstream sstm;
	sstm << "nbModality: " << nbClass;
	paramStr_ = sstm.str();
}

MixtureComposer::~MixtureComposer() {
	for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it) {
		delete (*it);
	}
}

void MixtureComposer::initializeTik() {
	tik_ = 1. / nbClass_;
}

Real MixtureComposer::lnObservedProbability(int i, int k) const {
	Real sum = std::log(prop_[k]);

	for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it) {
		Real logProba = (*it)->lnObservedProbability(i, k);
		sum += logProba;
	}

	return sum;
}

void MixtureComposer::printObservedTik() const {
	Matrix<Real> lnComp(nbInd_,
			nbClass_);

	for (Index k = 0; k < nbClass_; ++k) {
		for (Index i = 0; i < nbInd_; ++i) {
			lnComp(i, k) = lnObservedProbability(i, k);
		}
	}

	Matrix<Real> observedTik(nbInd_, nbClass_);
	for (Index i = 0; i < nbInd_; ++i) { // sum is inside a log, hence the numerous steps for the computation
		RowVector<Real> dummy;
		observedTik.row(i).logToMulti(lnComp.row(i));
	}
}

Real MixtureComposer::lnObservedLikelihood() {
	Real lnLikelihood = 0.;
	Matrix<Real> lnComp(nbInd_,
			nbClass_);

	for (Index k = 0; k < nbClass_; ++k) {
		for (Index i = 0; i < nbInd_; ++i) {
			lnComp(i, k) = lnObservedProbability(i, k);
		}
	}

	for (Index i = 0; i < nbInd_; ++i) { // sum is inside a log, hence the numerous steps for the computation
		RowVector<Real> dummy;
		lnLikelihood += dummy.logToMulti(lnComp.row(i));
	}

	return lnLikelihood;
}

Real MixtureComposer::lnCompletedLikelihood() {
	Real lnLikelihood = 0.;

	for (Index i = 0; i < nbInd_; ++i) { // Compute the completed likelihood for the complete mixture model, using the completed data
		lnLikelihood += lnObservedProbability(i, zClassInd_.zi().data_(i));
	}

	return lnLikelihood;
}

Real MixtureComposer::lnCompletedProbability(int i, int k) const {
	Real sum = std::log(prop_[k]);
	Real logProba;

	for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it) {
		logProba = (*it)->lnCompletedProbability(i, k);
		sum += logProba;
	}

	return sum;
}

void MixtureComposer::mStep(EstimatorType bias) {
	pStep(bias); // computation of z_ik frequencies, which correspond to ML estimator of proportions
	for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it) {
		(*it)->mStep(bias); // call mStep on each variable
	}
}

void MixtureComposer::sStepCheck() {
	for (Index i = 0; i < nbInd_; ++i) {
		sStepCheck(i);
	}
}

void MixtureComposer::sStepCheck(int i) {
	sampler_.sStepCheck(i);
}

void MixtureComposer::sStepNoCheck() {
#pragma omp parallel for
	for (Index i = 0; i < nbInd_; ++i) {
		sStepNoCheck(i);
	}
}

void MixtureComposer::sStepNoCheck(int i) {
	sampler_.sStepNoCheck(i);
}

void MixtureComposer::eStep() {
#pragma omp parallel for
	for (Index i = 0; i < nbInd_; ++i) {
		eStepInd(i);
	}

	std::cout << "MixtureComposer::eStep, tik" << std::endl;
	std::cout << tik_ << std::endl;
}

void MixtureComposer::eStepInd(int i) {
	RowVector<Real> lnComp(nbClass_);
	for (Index k = 0; k < nbClass_; k++) {
		lnComp(k) = lnCompletedProbability(i, k);
	}

	tik_.row(i).logToMulti(lnComp);
}

void MixtureComposer::pStep(EstimatorType bias) {
	prop_ = 0.;
	for (Index i = 0; i < zClassInd_.zi().data_.rows(); ++i) {
		prop_(zClassInd_.zi().data_(i)) += 1.;
	}
	prop_ = prop_ / prop_.sum();

	if (bias == biased_) {
		for (Index k = 0; k < nbClass_; ++k) {
			prop_(k) = std::max(prop_(k)    , epsilon );
			prop_(k) = std::min(1. - epsilon, prop_(k));
		}
	}
}

void MixtureComposer::writeParameters() const {
	std::cout << "Composer nbFreeParameter = " << nbFreeParameters() << std::endl;
	std::cout << "Composer proportions = " << itString(prop_) << std::endl;

	for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		std::cout << "Parameters of " << (*it)->idName() << "\n";
		(*it)->writeParameters();
	}
}

int MixtureComposer::nbFreeParameters() const {
	int sum = nbClass_ - 1; // proportions
	for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		sum+= (*it)->nbFreeParameter();
	}
	return sum;
}

void MixtureComposer::samplingStepCheck() {
	for (Index i = 0; i < nbInd_; ++i) {
		samplingStepCheck(i);
	}
}

void MixtureComposer::samplingStepCheck(int i) {
	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->samplingStepCheck(i);
	}
}

void MixtureComposer::samplingStepNoCheck(SamplerInitialization init) {
#pragma omp parallel for
	for (Index i = 0; i < nbInd_; ++i) {
		samplingStepNoCheck(init, i);
	}
}

void MixtureComposer::samplingStepNoCheck(SamplerInitialization init, int i) {
	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->samplingStepNoCheck(init, i);
	}
}

int MixtureComposer::checkSampleCondition(std::string* warnLog) const {
	if (warnLog == NULL) { // if no description of the error is expected, to speed the treatment
		if (checkNbIndPerClass() == 0) {
		  std::cout << "!!! Empty class" << std::endl;
			return 0;
		}
		for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
			if ((*it)->checkSampleCondition() == 0) {
			  std::cout << "!!! check sample fail in model" << std::endl;
				return 0; // no need for log generation -> faster evaluation of checkSampleCondition
			}
		}
	}
	else { // if error description is expected
		int probaCondition = 1; // proba of condition on data given the completed data
		std::string indLog;
		probaCondition *= checkNbIndPerClass(&indLog);
		*warnLog += indLog;
		for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
			std::string mixtLog;
			int currProba = (*it)->checkSampleCondition(&mixtLog); // the global warnLog is not passed directly to the mixture, to avoid accidental wiping
			probaCondition *= currProba;
			*warnLog += mixtLog;
		}

		return probaCondition;
	}

	return 1;
}

int MixtureComposer::checkNbIndPerClass(std::string* warnLog) const {
	for (Index k = 0; k < nbClass_; ++k) {
		if (zClassInd_.classInd()(k).size() > 0) {
			continue;
		}
		else {
			if (warnLog != NULL) {
				std::stringstream sstm;
				sstm << "MixtureComposer::checkNbIndPerClass, at least one class is empty. Did you provide more individuals "
						<< "that the number of classes ?" << std::endl;
				*warnLog += sstm.str();
			}

			return 0;
		}
	}

	return 1;
}

void MixtureComposer::storeSEMRun(int iteration,
		int iterationMax) {
	paramStat_.sampleParam(iteration,
			iterationMax);
	if (iteration == iterationMax){
		paramStat_.normalizeParam(paramStr_); // enforce that estimated proportions sum to 1, but only if paramStr is of the form "nModality: x"
		paramStat_.setExpectationParam(); // replace pi by the median values
	}
	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->storeSEMRun(iteration,
				iterationMax);
	}
}

void MixtureComposer::storeGibbsRun(int ind,
		int iteration,
		int iterationMax) {
	dataStat_.sampleVals(ind,
			iteration,
			iterationMax);

	if (iteration == iterationMax) {
		dataStat_.imputeData(ind, tik_); // impute the missing values using empirical mean or mode, depending of the model. Latest completed tik are replaced by observed tik
	}

	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->storeGibbsRun(ind,
				iteration,
				iterationMax);
	}
}

void MixtureComposer::registerMixture(IMixture* p_mixture) {
	v_mixtures_.push_back(p_mixture);
	++nbVar_;
}

void MixtureComposer::gibbsSampling(
    SamplerInitialization init,
		GibbsSampleData sample,
		int nbGibbsIter,
		int group,
		int groupMax) {
	Timer myTimer;
	if (sample == sampleData_) {
		myTimer.setName("Gibbs: run (individuals count as iterations)");
	}
	else {
		myTimer.setName("Gibbs: burn-in (individuals count as iterations)");
	}

#pragma omp parallel for
	for (Index i = 0; i < nbInd_; ++i) {
		if (init == callInitDataIfMarkovChain_) { // at this point, no latent variables are know, so no conditional information can be used to determine the class
		  tik_.row(i) = prop_; // the proportions are the marginal probability of the class over everything else, including the observed variables. Note that the observed probability could be used to compute observed tik to take the observed value into account, but its computation could be difficult, depending of the variable.
			sStepNoCheck(i);
			samplingStepNoCheck(callInitDataIfMarkovChain_, i); // since the class is known, the rest of the completion can be carried out
		}

		myTimer.iteration(i, nbInd_ - 1);
		writeProgress(group,
				groupMax,
				i,
				nbInd_ - 1);

		for (int iterGibbs = 0; iterGibbs < nbGibbsIter; ++iterGibbs) {
			eStepInd(i);

			sStepNoCheck(i);
			samplingStepNoCheck(doNotCallInitData_, i);

			if (sample == sampleData_) {
				storeGibbsRun(i,
						iterGibbs,
						nbGibbsIter - 1);
			}
		}
	}
}

std::vector<std::string> MixtureComposer::paramName() const {
	std::vector<std::string> names(nbClass_);
	for (Index k = 0; k < nbClass_; ++k) {
		std::stringstream sstm;
		sstm << "k: "
				<< k + minModality;
		names[k] = sstm.str();
	}

	return names;
}

std::vector<std::string> MixtureComposer::mixtureName() const {
	std::vector<std::string> names(nbVar_);
	for (Index j = 0; j < nbVar_; ++j) {
		names[j] = v_mixtures_[j]->idName();
	}

	return names;
}

void MixtureComposer::initData() {
	initializeTik();
	sStepNoCheck(); // uniform initialization of z

	for(MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
#pragma omp parallel for
		for (Index i = 0; i < nbInd_; ++i) {
			(*it)->initData(i);
		}
	}
}

void MixtureComposer::initParam() {
	UniformStatistic uni;
	for (Index k = 0; k < nbClass_; ++k) {
		prop_(k) = uni.sample(0., 1.);
	}
	prop_ = prop_ / prop_.sum();

	Vector<Index> allObs(nbInd_);
	for (Index i = 0; i < nbInd_; ++i) {
		allObs(i) = i;
	}
	MultinomialStatistic multi;
	multi.shuffle(allObs);

	Vector<Index> initObs(nbClass_); // observations used to initialize individuals
	for (Index i = 0; i < nbClass_; ++i) {
		initObs(i) = allObs(i);
	}

	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->initParam(initObs);
	}

	writeParameters();
}

void MixtureComposer::E_kj(Matrix<Real>& ekj) const {
	ekj.resize(nbClass_, nbVar_);
	ekj = 0.;

	for (Index i = 0; i < nbInd_; ++i) {
		for (Index j = 0; j < nbVar_; ++j) {
			Vector<Real> lnP(nbClass_); // ln(p(z_i = k, x_i^j))
			Vector<Real> t_ik_j(nbClass_); // p(z_i = k / x_i^j)
			for (Index k = 0; k < nbClass_; ++k) {
				lnP(k) = std::log(prop_(k)) + v_mixtures_[j]->lnObservedProbability(i, k);
			}
			t_ik_j.logToMulti(lnP); // "observed" t_ik, for the variable j
			Vector<Real> t_ink_j = 1. - t_ik_j; // The nj means: "all classes but k".

			for (Index k = 0; k < nbClass_; ++k) {
				Real p, nP;

				if (epsilon < t_ik_j(k)) {
					p = - t_ik_j(k) * std::log(t_ik_j (k));
				}
				else {
					p = 0.;
				}

				if (epsilon < t_ink_j(k)) {
					nP = - t_ink_j(k) * std::log(t_ink_j(k));
				}
				else {
					nP = 0.;
				}

				ekj(k, j) += p;
			}
		}
	}
}

// Added by Matthieu
void MixtureComposer::Delta(Matrix<Real>& delta) const {
	delta.resize(nbVar_, nbVar_);
	delta = 0.;
	for (Index i = 0; i < nbInd_; ++i){
		Matrix<Real> probacond(nbClass_, nbVar_); // P(Z_i=k|x_{ij}) k=1,...,K in row and j=1,...,d in column
		for (Index j = 0; j < nbVar_; ++j){
			Vector<Real> lnP(nbClass_); // ln(p(z_i = k, x_i^j))
			for (Index k = 0; k < nbClass_; ++k) {
				lnP(k) = std::log(prop_(k)) + v_mixtures_[j]->lnObservedProbability(i, k);
			}
			probacond.col(j).logToMulti(lnP); // "observed" t_ik, for the variable j
		}

		for (Index k = 0; k < nbClass_; ++k) {
			for(Index j = 0; j < nbVar_; ++j){
				for(Index h=j; h < nbVar_; ++h){
					// Contribution of observation i to the distance between variables j and h for component k
					delta(j,h) +=  (probacond(k,j) - probacond(k,h)) * (probacond(k,j) - probacond(k,h));
				}
			}
		}
	}
	for(Index j = 0; j < nbVar_; ++j){
		for(Index h=j; h < nbVar_; ++h){
			delta(j,h) = std::sqrt(delta(j,h) / nbInd_); // Normalisation
			delta(h,j) = delta(j,h); // symetric matrix
		}
	}
}


void MixtureComposer::IDClass(Matrix<Real>& idc) const {
	idc.resize(nbClass_, nbVar_);

	if (nbClass_ > 1) {
		Matrix<Real> ekj;
		E_kj(ekj);
		Real den = nbInd_ * std::log(nbClass_);

		for (Index k = 0; k < nbClass_; ++k) {
			//    Real min = ekj.row(k).minCoeff();
			//    Real max = ekj.row(k).maxCoeff();

			for(Index j = 0; j < nbVar_; ++j) {
				//      idc(k, j) = (max - ekj(k, j)) / (max - min);
				//      idc(k, j) = 1. - ekj(k, j) / ekj.row(k).sum();
				idc(k, j) = ekj(k, j) / den;
			}
		}
	}
	else {
		idc = 1.;
	}
}

void MixtureComposer::lnProbaGivenClass(Matrix<Real>& pGC) const {
  pGC.resize(nbInd_, nbClass_);

  for (Index i = 0; i < nbInd_; ++i) {
    for(Index k = 0; k < nbClass_; ++k) {
      pGC(i, k) = lnObservedProbability(i, k);
    }
  }
}

void MixtureComposer::printClassInd() const {
	zClassInd_.printState();
}

} /* namespace mixt */
