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

#include "mixt_ParamSetterR.h"

namespace mixt
{

ParamSetterR::ParamSetterR(const Rcpp::List param) :
    param_(param)
{}

ParamSetterR::~ParamSetterR()
{}

void ParamSetterR::getParam(const std::string& idName,
                            const std::string& paramName,
                            Vector<Real>& param,
                            std::string& paramStr) const {
  Rcpp::List listCurrId = param_[idName];
  Rcpp::List listStatLog = listCurrId[paramName];
  Rcpp::NumericMatrix currParam = listStatLog["stat"];
  int nRows = currParam.nrow();
  param.resize(nRows);

  for (int i = 0; i < nRows; ++i) {
    param(i) = currParam(i, 0); // only the mode / expectation is used, quantile information is discarded
  }

  paramStr = Rcpp::as<std::string>(listStatLog("paramStr"));
}

void ParamSetterR::getParam(const std::string& idName,
                            const std::string& paramName,
                            Vector<RankVal>& param) const {
  Rcpp::List listParam = param_[idName];
  Rcpp::List listStatLog = listParam[paramName];
  Rcpp::List listClass = listStatLog["stat"];

  int nbClass = listClass.size();
  param.resize(nbClass); // listCurrParam contains one element per class

  for (int k = 0; k < nbClass; ++k) {
    Rcpp::List listPair = listClass(k);
    Rcpp::List modePair = listPair(0); // take the mode of mu
    Rcpp::NumericVector currVec = modePair(0); // extracting the vector value containing mu

    param(k).setNbPos(currVec.size()); // setting the storage size for mu
    param(k).setO(currVec); // setting the value of mu
  }
}

} // namespace mixt
