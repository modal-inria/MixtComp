/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 7, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "FuncProblem.h"
#include "FunctionalClass.h"

namespace mixt {

FunctionalClass::FunctionalClass(Vector<Function>& data, Real confidenceLevel) :
		nSub_(0), nCoeff_(0), data_(data), alphaParamStat_(alpha_, confidenceLevel), betaParamStat_(beta_, confidenceLevel), sdParamStat_(sd_, confidenceLevel) {
}

void FunctionalClass::setSize(Index nSub, Index nCoeff) {
	nSub_ = nSub;
	nCoeff_ = nCoeff;

	alpha_.resize(nSub, 2); // remember that storage is linear for alpha, two coefficients per subregression
	beta_.resize(nSub, nCoeff);
	sd_.resize(nSub);

	alpha_ = 0.; // initialization is mandatory as the optimization is an iterative process
	beta_ = 0.;
	sd_ = 0.;
}

std::string FunctionalClass::mStep(const std::set<Index>& setInd) {
	std::string warnLog;

	mStepAlpha(setInd);
	warnLog += mStepBetaSd(setInd);

	return warnLog;
}

void FunctionalClass::mStepAlpha(const std::set<Index>& setInd) {
	Index nSub = alpha_.rows();
	Index nParam = 2 * nSub;
	Index nFreeParam = 2 * (nSub - 1);

	Vector<Real> alpha(nFreeParam); // linear version of alpha, to conform to optimization library format
	for (Index s = 0; s < nSub - 1; ++s) { // note that the two first coefficients stay at 0, they are not free parameters
		alpha[2 * s] = alpha_(s + 1, 0);
		alpha[2 * s + 1] = alpha_(s + 1, 1);
	}

	FuncProblem fp(nParam, data_, setInd);

	cppoptlib::BfgsSolver<FuncProblem> solver;
	cppoptlib::Criteria<Real> crit = cppoptlib::Criteria<Real>::defaults(); // Create a Criteria class to set the solver's stop conditions
	crit.iterations = maxIterationOptim;
	solver.setStopCriteria(crit);

	solver.minimize(fp, alpha);

	for (Index s = 0; s < nSub - 1; ++s) { // from linear to matrix format
		alpha_(s + 1, 0) = alpha[2 * s];
		alpha_(s + 1, 1) = alpha[2 * s + 1];
	}
}

std::string FunctionalClass::mStepBetaSd(const std::set<Index>& setInd) {
	std::string warnLog;
	Vector<Index> nTTotal(nSub_, 0);

	for (std::set<Index>::const_iterator itData = setInd.begin(), itDataE = setInd.end(); itData != itDataE; ++itData) { // to create the complete design matrix and y for the class, the total number of timesteps over the class must be determined
		for (Index s = 0; s < nSub_; ++s) {
			nTTotal(s) += data_(*itData).w()(s).size();
		}
	}

	Vector<Matrix<Real> > design(nSub_);
	Vector<Vector<Real> > y(nSub_);
	for (Index s = 0; s < nSub_; ++s) {
		design(s).resize(nTTotal(s), nCoeff_);
		y(s).resize(nTTotal(s));

		Index i = 0; // current row in the global design matrix
		for (std::set<Index>::const_iterator itData = setInd.begin(), itDataE = setInd.end(); itData != itDataE; ++itData) {
			for (std::set<Index>::const_iterator itTime = data_(*itData).w()(s).begin(), itTimeE = data_(*itData).w()(s).end(); itTime != itTimeE; ++itTime) {
				design(s).row(i) = data_(*itData).vandermonde().row(*itTime);
				y(s)(i) = data_(*itData).x()(*itTime);
				++i;
			}
		}
	}

	subRegression(design, y, beta_, sd_);

	if (sd_.minCoeff() < epsilon) { // at least one coefficient is too small
		warnLog += "At least one sub regression has a standard deviation less than the minimal accepted value: " + epsilonStr + eol;
	}

	return warnLog;
}

void FunctionalClass::initParam() {
	alpha_ = 0.;
}

void FunctionalClass::setParamStorage() {
	alphaParamStat_.setParamStorage();
	betaParamStat_.setParamStorage();
	sdParamStat_.setParamStorage();
}

void FunctionalClass::setExpectationParam() {
	alphaParamStat_.setExpectationParam();
	betaParamStat_.setExpectationParam();
	sdParamStat_.setExpectationParam();
}

void FunctionalClass::sampleParam(Index iteration, Index iterationMax) {
	alphaParamStat_.sampleParam(iteration, iterationMax);
	betaParamStat_.sampleParam(iteration, iterationMax);
	sdParamStat_.sampleParam(iteration, iterationMax);
}

std::string FunctionalClass::checkSampleCondition(const std::set<Index>& setInd) const {
	std::string warnLog;
	bool value = checkNbDifferentValue(setInd);

	if (!value) {
		warnLog += "Not enough different values for t. Data points should be separated by at least " + epsilonStr + eol;
	}

	return warnLog;
}

bool FunctionalClass::checkNbDifferentValue(const std::set<Index>& setInd) const {
	for (Index s = 0; s < nSub_; ++s) {
		std::list<Real> listT;

		for (std::set<Index>::const_iterator it = setInd.begin(), itE = setInd.end(); it != itE; ++it) { // only loop on individuals in the current class
			for (std::set<Index>::const_iterator itW = data_(*it).w()(s).begin(), itWE = data_(*it).w()(s).end(); itW != itWE; ++itW) { // only loop on timesteps in the current subregression
				listT.push_back(data_(*it).t()(*itW));
			}
		}

		if (!differentValue(listT, nCoeff_, epsilon)) {
			return false;
		}
	}

	return true;
}

void FunctionalClass::samplingStepNoCheck(Index i) {
	data_(i).sampleWNoCheck(alpha_, beta_, sd_);
}
} // namespace mixt
