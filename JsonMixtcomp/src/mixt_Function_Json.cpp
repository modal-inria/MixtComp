/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: Aug 18, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Function_Json.h"

namespace mixt
{

void IDClass(mixt::MixtureComposer& mc,
             nlohmann::json& idc) {
  idc = nlohmann::json(mc.nbClass());
  nlohmann::json idc_row(mc.nbVar());

  Matrix<Real> IDClass;
  mc.IDClass(IDClass);

  nlohmann::json row(mc.nbClass()); // names of the parameters
  nlohmann::json col(mc.nbVar()); // names for expectation and confidence interval values

  std::vector<std::string> rowName = mc.paramName();
  std::vector<std::string> colName = mc.mixtureName();

  for (Index j = 0; j < mc.nbVar(); ++j) {
    for (Index k = 0; k < mc.nbClass(); ++k) {
      idc_row[rowName[k]] = IDClass(k, j);
    }
    idc[j] = idc_row;
  }

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
