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
 *  Created on: July 7, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_ParamExtractorR.h"
#include "MixtComp/src/IO/mixt_IO.h"
#include "MixtComp/src/LinAlg/mixt_LinAlg.h"

namespace mixt
{

ParamExtractorR::ParamExtractorR()
{}

ParamExtractorR::~ParamExtractorR()
{}

void ParamExtractorR::exportParam(std::string idName,
                                  const Matrix<Real>& statStorage,
                                  const Matrix<Real>& logStorage,
                                  const std::vector<std::string>& paramNames,
                                  const Real confidenceLevel)
{
#ifdef MC_DEBUG
  std::cout << "ParamExtractorR::exportParam" << std::endl;
  std::cout << "idName: " << idName << std::endl;
  std::cout << "params.rows(): " << statStorage.rows() << std::endl;
  std::cout << "params.cols(): " << statStorage.cols() << std::endl;
#endif

  Rcpp::CharacterVector rows(statStorage.rows()); // names of the parameters
  Rcpp::CharacterVector cols; // names for expectation and confidence interval values

  Real alpha = (1. - confidenceLevel) / 2.;

  Rcpp::NumericMatrix paramR(statStorage.rows(),
                             statStorage.cols());

  // values setting
  for (int i = 0; i < statStorage.rows(); ++i)
  {
    for (int j = 0; j < statStorage.cols(); ++j)
    {
      paramR(i, j) = statStorage(i, j);
    }
  }

  // names setting for rows
  for (int i = 0; i < statStorage.rows(); ++i)
  {
    rows[i] = paramNames[i];
  }

  // names setting for cols
  if (statStorage.cols() == 1)
  {
    cols.push_back("value");
  }
  else if (statStorage.cols() == 3)
  {
    cols.push_back("expectation");
    cols.push_back(  std::string("q ")
                   + type2str(alpha * 100.)
                   + std::string("%"));
    cols.push_back(  std::string("q ")
                   + type2str((1. - alpha) * 100.)
                   + std::string("%"));
  }

  Rcpp::List dimnms = Rcpp::List::create(rows, cols);
  paramR.attr("dimnames") = dimnms;

  Rcpp::NumericMatrix paramLogR;

  if (logStorage.rows() > 0 && logStorage.cols()) // only if log has taken place, for example not during predict
  {
#ifdef MC_DEBUG
    std::cout << "paramsLogs: " << logStorage << std::endl;
#endif
    // copy of the log data
    paramLogR = Rcpp::NumericMatrix(logStorage.rows(),
                                    logStorage.cols());
    for (int i = 0; i < logStorage.rows(); ++i)
    {
      for (int j = 0; j < logStorage.cols(); ++j)
      {
        paramLogR(i, j) = logStorage(i, j);
      }
    }
    Rcpp::CharacterVector colsLog(logStorage.cols());
    Rcpp::List dimnmsLog = Rcpp::List::create(rows, colsLog);
    paramLogR.attr("dimnames") = dimnmsLog;
  }
  
  Rcpp::List ls = Rcpp::List::create(Rcpp::Named("stat") = paramR   ,
                                     Rcpp::Named("log")  = paramLogR);

  param_[idName] = ls;

#ifdef MC_DEBUG
  std::cout << "ParamExtractorR::exportParam, param_.size():  " << param_.size() << std::endl;
#endif
}

Rcpp::List ParamExtractorR::rcppReturnParam() const
{
  return param_;
}

} // namespace mixt
