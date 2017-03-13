/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 18, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Function.h"

namespace mixt
{

void IDClass(mixt::MixtureComposer& mc,
             Rcpp::NumericMatrix& idc) {
  idc = Rcpp::NumericMatrix(mc.nbClass(), // no resize in Rcpp::NumericMatrix, hence the call to the constructor
                            mc.nbVar());

  Matrix<Real> IDClass;
  mc.IDClass(IDClass);

  Rcpp::CharacterVector row(mc.nbClass()); // names of the parameters
  Rcpp::CharacterVector col(mc.nbVar()); // names for expectation and confidence interval values

  std::vector<std::string> rowName = mc.paramName();
  std::vector<std::string> colName = mc.mixtureName();

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

  Rcpp::List dimnms = Rcpp::List::create(row,
                                         col);
  idc.attr("dimnames") = dimnms;
}


void matDelta(mixt::MixtureComposer& mc,
              Rcpp::NumericMatrix& delta){
  delta = Rcpp::NumericMatrix(mc.nbVar(), mc.nbVar());
  
  Matrix<Real> DELTA;
  mc.Delta(DELTA);
  for (Index j = 0; j < mc.nbVar(); ++j) {
    for (Index h = 0; h < mc.nbVar(); ++h) {
      delta(h, j) = DELTA(h, j);
    }
  }
}


void paramRToCpp(const Rcpp::List& RParam,
                 StrategyParam& CppParam) {
  if(RParam.containsElementNamed("nbBurnInIter")){
    CppParam.nbBurnInIter_ = RParam["nbBurnInIter"];
  }else{
	  std::cout<<"Parameter nbBurnInIter not found, a value of 100 is used."<<std::endl;
  }
  if(RParam.containsElementNamed("nbIter")){
    CppParam.nbIter_ = RParam["nbIter"];
  }else{
	  std::cout<<"Parameter nbIter not found, a value of 100 is used."<<std::endl;
  }
  if(RParam.containsElementNamed("nbGibbsBurnInIter")){
    CppParam.nbGibbsBurnInIter_ = RParam["nbGibbsBurnInIter"];
  }else{
	  std::cout<<"Parameter nbGibbsBurnInIter not found, a value of 100 is used."<<std::endl;
  }
  if(RParam.containsElementNamed("nbGibbsIter")){
    CppParam.nbGibbsIter_ = RParam["nbGibbsIter"];
  }else{
	  std::cout<<"Parameter nbGibbsIter not found, a value of 100 is used."<<std::endl;
  }
  if(RParam.containsElementNamed("parameterEdgeAuthorized")){
    bool bias = RParam["parameterEdgeAuthorized"];
    if (bias == true) {
      CppParam.bias_ = biased_;
    }
  }else{
	  std::cout<<"Parameter parameterEdgeAuthorized not found, a value of FALSE is used."<<std::endl;
  }
}

} // namespace mixt
