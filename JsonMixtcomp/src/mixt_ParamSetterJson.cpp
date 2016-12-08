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
 *  Created on: Nov 19, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include "json.hpp"
#include "mixt_ParamSetterJson.h"

namespace mixt
{

ParamSetterJson::ParamSetterJson(const nlohmann::json param) :
    param_(param)
{}

ParamSetterJson::~ParamSetterJson()
{}

void ParamSetterJson::getParam(const std::string& idName,
                            const std::string& paramName,
                            Vector<Real>& param,
                            std::string& paramStr) const {

  nlohmann::json listStatLog = param_[idName][paramName];
  nlohmann::json currParam = listStatLog["stat"];
  int nRows = currParam["median"].size();
  param.resize(nRows);
  for (int i = 0; i < nRows; ++i) {
    param(i) = currParam["median"][i].get<double>(); // only the mode / expectation is used, quantile information is discarded
  }

  paramStr = listStatLog["paramStr"].get<std::string>();
}

void ParamSetterJson::getParam(const std::string& idName,
                            const std::string& paramName,
                            Vector<RankVal>& param,
                            std::string& paramStr) const {
  nlohmann::json listStatLog = param_[idName][paramName];
  nlohmann::json listClass = listStatLog["stat"];

  int nbClass = listClass.size();
  param.resize(nbClass); // listCurrParam contains one element per class

  for (int k = 0; k < nbClass; ++k) {
    nlohmann::json listPair = listClass[k];
    nlohmann::json modePair = listPair[0]; // take the mode of mu
    nlohmann::json currVec = modePair[0]; // extracting the vector value containing mu

    param(k).setNbPos(currVec.size()); // setting the storage size for mu
    param(k).setO(currVec); // setting the value of mu
  }

  paramStr = listStatLog["paramStr"].get<std::string>();
}

} // namespace mixt
