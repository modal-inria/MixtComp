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

MixtureComposer::MixtureComposer(
    Index nbInd,
		Index nbClass,
		Real confidenceLevel) :
		    idName_("z_class"),
		    nbClass_(nbClass),
		    nbInd_(nbInd),
		    nbVar_(0),
		    prop_(nbClass),
		    tik_(nbInd, nbClass),
		    sampler_(
		        *this,
		        zClassInd_,
		        tik_,
		        nbClass),
		    paramStat_(
		        prop_,
		        confidenceLevel),
		        dataStat_(zClassInd_),
		        confidenceLevel_(confidenceLevel),
			completedProbabilityCache_(nbInd_) {
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

Real MixtureComposer::lnObservedProbability(int i, int k) const {
	Real sum = std::log(prop_[k]);

	for (Index j = 0; j < nbVar_; ++j) { // use the cache
	  sum += observedProbabilityCache_(j)(i, k);
	}

	return sum;
}

void MixtureComposer::printTik() const {
	std::cout << "Sampled t_ik" << std::endl;
	std::cout << tik_ << std::endl;
}

void MixtureComposer::observedTik(Vector<Real>& oZMode) const {
	oZMode.resize(nbInd_);
	Matrix<Real> observedTikMat(nbInd_, nbClass_);

	Matrix<Real> lnComp(nbInd_, nbClass_);

	for (Index k = 0; k < nbClass_; ++k) {
		for (Index i = 0; i < nbInd_; ++i) {
			lnComp(i, k) = lnObservedProbability(i, k);
		}
	}

	Index mode;
	for (Index i = 0; i < nbInd_; ++i) { // sum is inside a log, hence the numerous steps for the computation
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
	Matrix<Real> lnComp(nbInd_, nbClass_);

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

	for (Index i = 0; i < nbInd_; ++i) { // completion is only on the latent class, latent data in models is marginalized over
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

void MixtureComposer::mStep() {
	mStepPi(); // computation of z_ik frequencies, which correspond to ML estimator of proportions
	for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it) {
		(*it)->mStep(); // call mStep on each variable
	}
}

void MixtureComposer::sampleZ() {
#pragma omp parallel for
	for (Index i = 0; i < nbInd_; ++i) {
		sampleZ(i);
	}
}

void MixtureComposer::sampleZ(int i) {
	sampler_.sStepNoCheck(i);
}

void MixtureComposer::eStepCompleted() {
#pragma omp parallel for
	for (Index i = 0; i < nbInd_; ++i) {
		eStepCompletedInd(i);
	}

//	std::cout << "MixtureComposer::eStep, tik" << std::endl;
//	std::cout << tik_ << std::endl;
}

void MixtureComposer::eStepCompletedInd(int i) {
	RowVector<Real> lnComp(nbClass_);
	for (Index k = 0; k < nbClass_; k++) {
		lnComp(k) = lnCompletedProbability(i, k);
	}

	completedProbabilityCache_(i) = tik_.row(i).logToMulti(lnComp);
}

void MixtureComposer::mStepPi() {
	prop_ = 0.;
	for (Index i = 0; i < zClassInd_.zi().data_.rows(); ++i) {
		prop_(zClassInd_.zi().data_(i)) += 1.;
	}
	prop_ = prop_ / prop_.sum();
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

void MixtureComposer::sampleUnobservedAndLatent() {
#pragma omp parallel for
	for (Index i = 0; i < nbInd_; ++i) {
		sampleUnobservedAndLatent(i);
	}
}

void MixtureComposer::sampleUnobservedAndLatent(int i) {
	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->sampleUnobservedAndLatent(i);
	}
}

std::string MixtureComposer::checkSampleCondition() const {
	std::string warnLog = checkNbIndPerClass();
	for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		warnLog += (*it)->checkSampleCondition();
	}

	return warnLog;
}

std::string MixtureComposer::checkNbIndPerClass() const {
	for (Index k = 0; k < nbClass_; ++k) {
		if (0 < zClassInd_.classInd()(k).size()) {
			continue;
		}
		else {
			std::stringstream sstm;
			sstm << "MixtureComposer::checkNbIndPerClass: at least one class is empty. Maybe you provided more individuals "
				<< "that the number of classes ?" << std::endl;
			return sstm.str();
		}
	}

	return "";
}

void MixtureComposer::storeSEMRun(
		int iteration,
		int iterationMax,
		RunType runType) {
	if (runType == burnIn_) {
		if (iteration == 0) {
			completedProbabilityLogBurnIn_.resize(iterationMax + 1);
		}
		completedProbabilityLogBurnIn_(iteration) = completedProbabilityCache_.sum();
	}

	if (runType == run_) {
		if (iteration == 0) {
			completedProbabilityLogRun_.resize(iterationMax + 1);
		}
		completedProbabilityLogRun_(iteration) = completedProbabilityCache_.sum();
	}

	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->storeSEMRun(iteration, iterationMax);
	}

	paramStat_.sampleParam(iteration, iterationMax);

	if (iteration == iterationMax){
		paramStat_.normalizeParam(paramStr_); // enforce that estimated proportions sum to 1, but only if paramStr is of the form "nModality: x"
		paramStat_.setExpectationParam(); // replace pi by the median values
	}
}

void MixtureComposer::setObservedProbaCache() {
  observedProbabilityCache_.resize(nbVar_);

  for (Index j = 0; j < nbVar_; ++j) {
    observedProbabilityCache_(j) = Matrix<Real>(nbInd_, nbClass_);
    observedProbabilityCache_(j) = 0.;
  }

  for (Index j = 0; j < nbVar_; ++j) {
    for (Index i = 0; i < nbInd_; ++i) {
      for (Index k = 0; k < nbClass_; ++k) {
        observedProbabilityCache_(j)(i, k) = v_mixtures_[j]->lnObservedProbability(i, k);
      }
    }
  }
}

void MixtureComposer::storeGibbsRun(
		int ind,
		int iteration,
		int iterationMax) {
	dataStat_.sampleVals(
			ind,
			iteration,
			iterationMax);

	if (iteration == iterationMax) {
		dataStat_.imputeData(ind, tik_); // impute the missing values using empirical mean or mode, depending of the model. Latest completed tik are replaced by observed tik
	}

	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
		(*it)->storeGibbsRun(
				ind,
				iteration,
				iterationMax);
	}
}

void MixtureComposer::registerMixture(IMixture* p_mixture) {
	v_mixtures_.push_back(p_mixture);
	++nbVar_;
}

void MixtureComposer::gibbsSampling(
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
		myTimer.iteration(i, nbInd_ - 1);
		writeProgress(
		    group,
				groupMax,
				i,
				nbInd_ - 1);

		for (int iterGibbs = 0; iterGibbs < nbGibbsIter; ++iterGibbs) {
			eStepCompletedInd(i);

			sampleZ(i);
			sampleUnobservedAndLatent(i);

			if (sample == sampleData_) {
				storeGibbsRun(
					i,
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
	tik_ = 1. / nbClass_;
	sampleZ(); // since tik are uniform, this sStep corresponds to an uniform initialization of z

	for(MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
#pragma omp parallel for
		for (Index i = 0; i < nbInd_; ++i) {
			(*it)->initData(i);
		}
	}
}

std::string MixtureComposer::initParam() {
  prop_ = 1. / nbClass_; // this is roughly equivalent to an estimation by maximization of likelihood, considering that proportions in all t_ik are equal

	Vector<Index> initObs(nbClass_); // observations used to initialize individuals
	for (Index k = 0; k < nbClass_; ++k) {
	  MultinomialStatistic multi;
	  Index sampleInd = multi.sampleInt(0, zClassInd_.classInd()(k).size() - 1);
	  std::set<Index>::const_iterator it = zClassInd_.classInd()(k).begin();
	  for (Index i = 0; i < sampleInd; ++i) {
	    ++it;
	  }

	  initObs(k) = *it;
	}

	std::string warnLog;
	for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
	  std::string varLog;
	  varLog += (*it)->initParam(initObs);
	  if (0 < varLog.size()) {
	    std::stringstream sstm;
	    sstm << "Error(s) in variable: " << (*it)->idName() << ": " << std::endl << varLog << std::endl;
	    warnLog += sstm.str();
	  }
	}

	if (0 < warnLog.size()) {
	  std::stringstream sstm;
	  sstm << "Errors in MixtureComposer::initParam: " << std::endl << warnLog;
	  warnLog = sstm.str();
	}

	return warnLog;
}

void MixtureComposer::E_kj(Matrix<Real>& ekj) const {
	ekj.resize(nbClass_, nbVar_);
	ekj = 0.;

	for (Index i = 0; i < nbInd_; ++i) {
		for (Index j = 0; j < nbVar_; ++j) {
			Vector<Real> lnP(nbClass_); // ln(p(z_i = k, x_i^j))
			Vector<Real> t_ik_j(nbClass_); // p(z_i = k / x_i^j)
			for (Index k = 0; k < nbClass_; ++k) {
			  lnP(k) = std::log(prop_(k)) + observedProbabilityCache_(j)(i, k);
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
				lnP(k) = std::log(prop_(k)) + observedProbabilityCache_(j)(i, k);
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

void MixtureComposer::computeObservedProba() {
  for (Index j = 0; j < nbVar_; ++j) {
    v_mixtures_[j]->computeObservedProba();
  }
}

std::string MixtureComposer::initializeLatent() {
  std::string warnLog;

  computeObservedProba(); // whether the Gibbs comes after a SEM or is used in prediction, parameters are known at that point
  setObservedProbaCache();
  eStepObserved();
  sampleZ();
  initializeMarkovChain();
  sampleUnobservedAndLatent();
  warnLog = checkSampleCondition();

  return warnLog;
}

void MixtureComposer::initializeMarkovChain() {
  for (Index j = 0; j < nbVar_; ++j) {
    v_mixtures_[j]->initializeMarkovChain();
  }
}

void MixtureComposer::eStepObserved() {
#pragma omp parallel for
  for (Index i = 0; i < nbInd_; ++i) {
    eStepObservedInd(i);
  }

//  std::cout << "MixtureComposer::eStepObservedInd, tik" << std::endl;
//  std::cout << tik_ << std::endl;
}

void MixtureComposer::eStepObservedInd(Index i) {
	RowVector<Real> lnComp(nbClass_);
	for (Index k = 0; k < nbClass_; k++) {
		lnComp(k) = lnObservedProbability(i, k); // main difference with eStepObservedInd
	}

	if (minInf < lnComp.maxCoeff()) {
		tik_.row(i).logToMulti(lnComp);
	}
	else {
		std::cout << "MixtureComposer::eStepObservedInd, i: " << i << ", " << itString(lnComp) << std::endl;
		std::cout << "!!! Individual i: " << i << " has a null observed probability !!!" << std::endl;
		tik_.row(i) = prop_;
	}
}

} /* namespace mixt */
