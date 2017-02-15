/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016
*/

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
