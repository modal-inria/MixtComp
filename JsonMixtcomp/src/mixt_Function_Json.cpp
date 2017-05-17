/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/


#include "mixt_Function_Json.h"

namespace mixt
{

void IDClass(mixt::MixtureComposer& mc,
             nlohmann::json& idc) {

  nlohmann::json idc_row;
  nlohmann::json idc_;


  Matrix<Real> IDClass;
  mc.IDClass(IDClass);

  nlohmann::json row; // names of the parameters
  nlohmann::json col; // names for expectation and confidence interval values

  std::vector<std::string> rowName = mc.paramName();
  std::vector<std::string> colName = mc.mixtureName();



  for (Index j = 0; j < mc.nbVar(); ++j) {
    for (Index k = 0; k < mc.nbClass(); ++k) {
      idc_row[rowName[k]] = IDClass(k, j);
    }
    idc_[j] = idc_row;
  }
  idc["idc"] = idc_ ;
  idc["colName"] = colName;

}

void lnProbaGivenClass(mixt::MixtureComposer& mc,
                       nlohmann::json& pGCR) {

  nlohmann::json pGCR_row;
  nlohmann::json pGCR_;

  Matrix<Real> pGCCPP;
  mc.lnProbaGivenClass(pGCCPP);

  for (Index i = 0; i < mc.nbInd(); ++i) {
    for (Index k = 0; k < mc.nbClass(); ++k) {
      pGCR_row[k] = pGCCPP(i, k);
    }
    pGCR_[i] = pGCR_row;
  }
  pGCR = pGCR_;
}

void matDelta(mixt::MixtureComposer& mc,
              nlohmann::json& delta){
  nlohmann::json delta_row;
  nlohmann::json delta_;

  Matrix<Real> DELTA;
  mc.Delta(DELTA);
  for (Index j = 0; j < mc.nbVar(); ++j) {
    for (Index h = 0; h < mc.nbVar(); ++h) {
      delta_row[h] = DELTA(h, j);
    }
    delta_[j] = delta_row;
  }
  delta=delta_;
}


void paramJsonToCpp(const nlohmann::json& JsonParam,
                 StrategyParam& CppParam) {

  if (JsonParam.find("nbBurnInIter") != JsonParam.end()) {
    CppParam.nbBurnInIter_ = JsonParam["nbBurnInIter"];
  }
  if (JsonParam.find("nbIter") != JsonParam.end()) {
      CppParam.nbIter_ = JsonParam["nbIter"];
    }
  if (JsonParam.find("nbGibbsBurnInIter") != JsonParam.end()) {
      CppParam.nbGibbsBurnInIter_ = JsonParam["nbGibbsBurnInIter"];
    }
  if (JsonParam.find("nbGibbsIter") != JsonParam.end()) {
      CppParam.nbGibbsIter_ = JsonParam["nbGibbsIter"];
    }
  if(JsonParam.find("parameterEdgeAuthorized") != JsonParam.end()){
    bool bias = JsonParam["parameterEdgeAuthorized"];
    if (bias == true) {
      CppParam.bias_ = biased_;
    }
  }
}

} // namespace mixt
