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
#include "MixtComp/src/Various/mixt_IO.h"
#include "Arrays/include/STK_Display.h"

namespace mixt
{

ParamExtractorR::ParamExtractorR()
{}

ParamExtractorR::~ParamExtractorR()
{}

void ParamExtractorR::exportParam(std::string idName,
                                  const STK::Array2D<Real>* p_params,
                                  const STK::Array2D<Real>* p_paramsLogs,
                                  const std::vector<std::string>& paramNames,
                                  const Real confidenceLevel)
{
#ifdef MC_DEBUG_NEW
  std::cout << "ParamExtractorR::exportParam" << std::endl;
  std::cout << "idName: " << idName << std::endl;
  std::cout << "p_params->sizeRows(): " << p_params->sizeRows() << std::endl;
  std::cout << "p_params->sizeCols(): " << p_params->sizeCols() << std::endl;
#endif

  Rcpp::CharacterVector rows(p_params->sizeRows()); // names of the parameters
  Rcpp::CharacterVector cols; // names for expectation and confidence interval values

  Real alpha = (1. - confidenceLevel) / 2.;

  Rcpp::NumericMatrix paramR(p_params->sizeRows(),
                             p_params->sizeCols());

  // values setting
  for (int i = 0; i < p_params->sizeRows(); ++i)
  {
    for (int j = 0; j < p_params->sizeCols(); ++j)
    {
      paramR(i, j) = p_params->elt(i, j);
    }
  }

  // names setting for rows
  for (int i = 0; i < p_params->sizeRows(); ++i)
  {
    rows[i] = paramNames[i];
  }

  // names setting for cols
  if (p_params->sizeCols() == 1)
  {
    cols.push_back("value");
  }
  else if (p_params->sizeCols() == 3)
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

  if (p_paramsLogs->sizeRows() > 0 && p_paramsLogs->sizeCols()) // only if log has taken place, for example not during predict
  {
#ifdef MC_DEBUG_NEW
    std::cout << "(*p_paramsLogs)" << (*p_paramsLogs) << std::endl;
#endif
    // copy of the log data
    paramLogR = Rcpp::NumericMatrix(p_paramsLogs->sizeRows(),
                                    p_paramsLogs->sizeCols());
    for (int i = 0; i < p_paramsLogs->sizeRows(); ++i)
    {
      for (int j = 0; j < p_paramsLogs->sizeCols(); ++j)
      {
        paramLogR(i, j) = p_paramsLogs->elt(i, j);
      }
    }
    Rcpp::CharacterVector colsLog(p_paramsLogs->sizeCols());
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
