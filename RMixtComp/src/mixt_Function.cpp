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

  for (int j = 0; j < mc.nbVar(); ++j) {
    for (int k = 0; k < mc.nbClass(); ++k) {
      idc(k, j) = IDClass(k, j);
    }
  }

  for (int k = 0; k < mc.nbClass(); ++k) {
    row(k) = rowName[k];
  }

  for (int j = 0; j < mc.nbVar(); ++j) {
    col(j) = colName[j];
  }

  Rcpp::List dimnms = Rcpp::List::create(row,
                                         col);
  idc.attr("dimnames") = dimnms;
}

void paramRToCpp(const Rcpp::List& RParam,
                 StrategyParam& CppParam) {
  if(RParam.containsElementNamed("nbBurnInIter")){
    CppParam.nbBurnInIter_ = RParam["nbBurnInIter"];
  }
  if(RParam.containsElementNamed("nbIter")){
    CppParam.nbIter_ = RParam["nbIter"];
  }
  if(RParam.containsElementNamed("nbGibbsBurnInIter")){
    CppParam.nbGibbsBurnInIter_ = RParam["nbGibbsBurnInIter"];
  }
  if(RParam.containsElementNamed("nbGibbsBurnInIter")){
    CppParam.nbGibbsIter_ = RParam["nbGibbsIter"];
  }
}

} // namespace mixt
