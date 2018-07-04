/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 18, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Function.h"

namespace mixt {

void IDClass(mixt::MixtureComposer& mc, Rcpp::NumericMatrix& idc) {
	idc = Rcpp::NumericMatrix(mc.nbClass(), // no resize in Rcpp::NumericMatrix, hence the call to the constructor
			mc.nbVar());

	Matrix < Real > IDClass;
	mc.IDClass(IDClass);

	Rcpp::CharacterVector row(mc.nbClass()); // names of the parameters
	Rcpp::CharacterVector col(mc.nbVar()); // names for expectation and confidence interval values

	std::vector < std::string > rowName = mc.paramName();
	std::vector < std::string > colName = mc.mixtureName();

	for (Index j = 0; j < mc.nbVar(); ++j) {
		for (Index k = 0; k < mc.nbClass(); ++k) {
			idc(k, j) = IDClass(k, j);
		}
	}

	for (Index k = 0; k < mc.nbClass(); ++k) {
		row(k) = rowName[k];
	}

	for (Index j = 0; j < mc.nbVar(); ++j) {
		col(j) = colName[j];
	}

	Rcpp::List dimnms = Rcpp::List::create(row, col);
	idc.attr("dimnames") = dimnms;
}

void lnProbaGivenClass(mixt::MixtureComposer& mc, Rcpp::NumericMatrix& pGCR) {
	pGCR = Rcpp::NumericMatrix(mc.nbInd(), // no resize in Rcpp::NumericMatrix, hence the call to the constructor
			mc.nbClass());

	Matrix < Real > pGCCPP;
	mc.lnProbaGivenClass(pGCCPP);

	for (Index i = 0; i < mc.nbInd(); ++i) {
		for (Index k = 0; k < mc.nbClass(); ++k) {
			pGCR(i, k) = pGCCPP(i, k);
		}
	}
}

void completedProbaLog(mixt::MixtureComposer& mc, Rcpp::NumericVector& completedProbabilityLogBurnIn, Rcpp::NumericVector& completedProbabilityLogRun) {
	completedProbabilityLogBurnIn = Rcpp::NumericVector(mc.completedProbabilityLogBurnIn().size());
	completedProbabilityLogRun = Rcpp::NumericVector(mc.completedProbabilityLogRun().size());

	for (Index i = 0; i < mc.completedProbabilityLogBurnIn().size(); ++i) {
		completedProbabilityLogBurnIn(i) = mc.completedProbabilityLogBurnIn()(i);
	}
	for (Index i = 0; i < mc.completedProbabilityLogRun().size(); ++i) {
		completedProbabilityLogRun(i) = mc.completedProbabilityLogRun()(i);
	}
}

void observedTik(mixt::MixtureComposer& mc, Rcpp::NumericVector& oikR) {
	oikR = Rcpp::NumericVector(mc.nbInd()); // no resize in Rcpp::NumericMatrix, hence the call to the constructor

	Vector < Real > oikCPP;
	mc.observedTik(oikCPP);

	for (Index i = 0; i < mc.nbInd(); ++i) {
		for (Index k = 0; k < mc.nbClass(); ++k) {
			oikR(i) = oikCPP(i);
		}
	}
}

void matDelta(mixt::MixtureComposer& mc, Rcpp::NumericMatrix& delta) {
	delta = Rcpp::NumericMatrix(mc.nbVar(), mc.nbVar());

	Matrix < Real > DELTA;
	mc.Delta(DELTA);
	for (Index j = 0; j < mc.nbVar(); ++j) {
		for (Index h = 0; h < mc.nbVar(); ++h) {
			delta(h, j) = DELTA(h, j);
		}
	}
}

void paramRToCpp(const Rcpp::List& RParam, StrategyParam& CppParam) {
	if (RParam.containsElementNamed("nbBurnInIter")) {
		CppParam.nbBurnInIter_ = RParam["nbBurnInIter"];
	} else {
		std::cout << "Parameter nbBurnInIter not found, a default value of " + std::to_string(CppParam.nbBurnInIter_) + " is used." << std::endl;
	}

	if (RParam.containsElementNamed("nbIter")) {
		CppParam.nbIter_ = RParam["nbIter"];
	} else {
		std::cout << "Parameter nbIter not found, a default value of " + std::to_string(CppParam.nbIter_) + " is used." << std::endl;
	}

	if (RParam.containsElementNamed("nbGibbsBurnInIter")) {
		CppParam.nbGibbsBurnInIter_ = RParam["nbGibbsBurnInIter"];
	} else {
		std::cout << "Parameter nbGibbsBurnInIter not found, a default value of " + std::to_string(CppParam.nbGibbsBurnInIter_) + " is used." << std::endl;
	}

	if (RParam.containsElementNamed("nbGibbsIter")) {
		CppParam.nbGibbsIter_ = RParam["nbGibbsIter"];
	} else {
		std::cout << "Parameter nbGibbsIter not found, a default value of " + std::to_string(CppParam.nbGibbsIter_) + " is used." << std::endl;
	}

	if (RParam.containsElementNamed("nInitPerClass")) {
		Index nInitPerClass = RParam["nInitPerClass"];
		if (nInitPerClass < 0) {
			std::cout << "nInitPerClass must be positive, a default value of " +  std::to_string(CppParam.nInitPerClass_) + " is used." << std::endl;
		} else {
			CppParam.nInitPerClass_ = nInitPerClass;
		}
	} else {
		std::cout << "Parameter nInitPerClass not found, a default value of " +  std::to_string(CppParam.nInitPerClass_) + " is used." << std::endl;
	}

	if (RParam.containsElementNamed("nSemTry")) {
		Index nSemTry = RParam["nSemTry"];
		if (nSemTry < 1) {
			std::cout << "nSemTry must be striclty positive, a default value of " +  std::to_string(CppParam.nSemTry_) + " is used." << std::endl;
		} else {
			CppParam.nSemTry_ = nSemTry;
		}
	} else {
		std::cout << "Parameter nSemTry not found, a default value of " +  std::to_string(CppParam.nSemTry_) + " is used." << std::endl;
	}

}

} // namespace mixt
