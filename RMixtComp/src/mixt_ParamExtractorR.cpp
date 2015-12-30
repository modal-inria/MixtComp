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

void ParamExtractorR::exportParam(const std::string& idName,
                                  const std::string& paramName,
                                  const Matrix<Real>& statStorage,
                                  const Matrix<Real>& logStorage,
                                  const std::vector<std::string>& paramNames,
                                  Real confidenceLevel)
{
#ifdef MC_DEBUG
  std::cout << "ParamExtractorR::exportParam, idName: " << idName << ", paramName: " << paramName << std::endl;
#endif

  Rcpp::CharacterVector rows(statStorage.rows()); // names of the parameters
  Rcpp::CharacterVector cols; // names for expectation and confidence interval values

  Real alpha = (1. - confidenceLevel) / 2.;

  Rcpp::NumericMatrix statR(statStorage.rows(),
                             statStorage.cols());

  // values setting
  for (int i = 0; i < statStorage.rows(); ++i)
  {
    for (int j = 0; j < statStorage.cols(); ++j)
    {
      statR(i, j) = statStorage(i, j);
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
    cols.push_back("median");
    cols.push_back(  std::string("q ")
                   + type2str(alpha * 100.)
                   + std::string("%"));
    cols.push_back(  std::string("q ")
                   + type2str((1. - alpha) * 100.)
                   + std::string("%"));
  }

  Rcpp::List dimnms = Rcpp::List::create(rows, cols);
  statR.attr("dimnames") = dimnms;

  Rcpp::NumericMatrix logR;

  if (logStorage.rows() > 0 && logStorage.cols()) // only if log has taken place, for example not during predict
  {
#ifdef MC_DEBUG
    std::cout << "paramsLogs: " << logStorage << std::endl;
#endif
    // copy of the log data
    logR = Rcpp::NumericMatrix(logStorage.rows(),
                                    logStorage.cols());
    for (int i = 0; i < logStorage.rows(); ++i)
    {
      for (int j = 0; j < logStorage.cols(); ++j)
      {
        logR(i, j) = logStorage(i, j);
      }
    }
    Rcpp::CharacterVector colsLog(logStorage.cols());
    Rcpp::List dimnmsLog = Rcpp::List::create(rows, colsLog);
    logR.attr("dimnames") = dimnmsLog;
  }

#ifdef MC_DEBUG
  std::cout << "param_[idName].size(): " << param_[idName].size() << std::endl;
#endif

  param_[idName][paramName] = Rcpp::List::create(Rcpp::Named("stat") = statR,
                                                 Rcpp::Named("log")  = logR);

#ifdef MC_DEBUG
  std::cout << "param_[idName].size(): " << param_[idName].size() << std::endl;
#endif
}

/** Extractor for Rank data type */
void ParamExtractorR::exportParam(const std::string& idName,
                                  const std::string& paramName,
                                  const std::vector<RankParamStat>& paramStat,
                                  const std::vector<std::string>& paramNames,
                                  Real confidenceLevel)
{
#ifdef MC_DEBUG
  std::cout << "ParamExtractorR::exportParam, idName: " << idName << ", paramName: " << paramName << std::endl;
#endif

  Rcpp::CharacterVector rowsNames = Rcpp::wrap(paramNames); // names of the classes, used for both parameters values and logs

  int nbClass = paramStat.size();

  std::list<Rcpp::List> statCPP; // global list, of param descriptor, each element is the descriptor of a class

  for (int k = 0; k < nbClass; ++k) // loop to create the parameters descriptor
  {
    int nbPos = paramStat[k].nbPos(); // get number of positions in rank
    const std::list<std::pair<RankVal, Real> >& statStorageMu = paramStat[k].statStorageMu(); // helper reference to point to current statStorage
    std::list<Rcpp::List> classProba; // list of pairs {vector representing rank, proba} for the current class

    for (std::list<std::pair<RankVal, Real> >::const_iterator it = statStorageMu.begin(), ite = statStorageMu.end();
         it != ite;
         ++it)
    {
      const RankVal& rankCPP = it->first; // current rank in C++
      Rcpp::IntegerVector rankR(nbPos); // current rank in R
      for (int p = 0; p < nbPos; ++p)
      {
        rankR(p) = rankCPP.o()(p);
      }
      classProba.push_back(Rcpp::List::create(rankR,
                                              it->second));
    }
    statCPP.push_back(Rcpp::wrap(classProba));
  }

  Rcpp::List statR = Rcpp::wrap(statCPP);
  statR.attr("names") = rowsNames;

  std::list<Rcpp::List> logCPP; // global list, of param descriptor, each element is the descriptor of a class

  for (int k = 0; k < nbClass; ++k) // loop to create the parameters log
  {
    int nbPos = paramStat[k].nbPos(); // get number of positions in rank
    const Vector<RankVal>& logStorageMu = paramStat[k].logStorageMu(); // helper reference to point to current statStorage
    std::list<Rcpp::IntegerVector> classProba; // list of sampled mu for the current class

    for (int i = 0, ie = logStorageMu.size(); i < ie; ++i)
    {
      const RankVal& rankCPP = logStorageMu(i); // current rank in C++
      Rcpp::IntegerVector rankR(nbPos); // current rank in R
      for (int p = 0; p < nbPos; ++p)
      {
        rankR(p) = rankCPP.o()(p);
      }
      classProba.push_back(rankR);
    }
    logCPP.push_back(Rcpp::wrap(classProba));
  }

  Rcpp::List logR = Rcpp::wrap(logCPP);
  logR.attr("names") = rowsNames;

#ifdef MC_DEBUG
  std::cout << "param_[idName].size(): " << param_[idName].size() << std::endl;
#endif

  param_[idName][paramName] = Rcpp::List::create(Rcpp::Named("stat") = statR,
                                                 Rcpp::Named("log")  = logR);

#ifdef MC_DEBUG
  std::cout << "param_[idName].size(): " << param_[idName].size() << std::endl;
#endif
}

Rcpp::List ParamExtractorR::rcppReturnParam() const
{
#ifdef MC_DEBUG
  std::cout << "ParamExtractorR::rcppReturnParam(), param_.size(): " << param_.size() << std::endl;
#endif

  Rcpp::List res = Rcpp::wrap(param_);

#ifdef MC_DEBUG
  std::cout << "ParamExtractorR::rcppReturnParam(), res.size(): " << res.size() << std::endl;
#endif

  return res;
}

} // namespace mixt
