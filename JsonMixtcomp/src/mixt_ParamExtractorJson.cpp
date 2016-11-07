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

#include "mixt_ParamExtractorJson.h"
#include "../../MixtComp/src/IO/mixt_IO.h"
#include "../../MixtComp/src/LinAlg/mixt_LinAlg.h"

namespace mixt {

void ParamExtractorJson::setNbMixture(int nbMixture) {
  mixtureName_.resize(nbMixture);
  param_.resize(nbMixture);
}

void ParamExtractorJson::exportParam(int indexMixture,
                                  const std::string& idName,
                                  const std::string& paramName,
                                  const Matrix<Real>& statStorage,
                                  const Matrix<Real>& logStorage,
                                  const std::vector<std::string>& paramNames,
                                  Real confidenceLevel,
                                  const std::string& paramStr) {
  nlohmann::json rows(statStorage.rows()); // names of the parameters
  nlohmann::json cols; // names for expectation and confidence interval values


  Real alpha = (1. - confidenceLevel) / 2.;

  nlohmann::json statJson(statStorage.cols());
  nlohmann::json statJson_row;
  nlohmann::json colName;

  // names setting for cols
  if (statStorage.cols() == 1) {
      for (int i = 0; i < statStorage.rows(); ++i) {
        statJson_row[i] = statStorage(i, 0);
      }
      statJson["value"] = statJson_row;

   } else if (statStorage.cols() == 3) {
     std::vector<std::string> name_col_str = {"median",std::string("q ")+ type2str(alpha * 100.)+ std::string("%"),std::string("q ")+ type2str((1. - alpha) * 100.)+ std::string("%")};

     for (int j = 0; j < statStorage.cols(); ++j) {
       for (int i = 0; i < statStorage.rows(); ++i) {
        statJson_row[i] = statStorage(i, j);
       }
       statJson[name_col_str[j]]= statJson_row;
     }
  }

  // names setting for rows


  nlohmann::json logJson(logStorage.rows());

  if (logStorage.rows() > 0 && logStorage.cols()) {// only if log has taken place, for example not during predict
    // copy of the log data
    nlohmann::json logJson_row(logStorage.cols());

    for (int i = 0; i < logStorage.rows(); ++i) {
      for (int j = 0; j < logStorage.cols(); ++j) {
        logJson_row[j] = logStorage(i, j);
      }
      logJson[paramNames[i]] = logJson_row ;
    }
  }

  mixtureName_[indexMixture] = idName;
  param_[indexMixture][paramName]["stat"] = statJson;
  param_[indexMixture][paramName]["log"] = logJson;
  param_[indexMixture][paramName]["paramStr"] = paramStr;

}

/** Extractor for Rank data type */
void ParamExtractorJson::exportParam(int indexMixture,
                                  const std::string& idName,
                                  const std::string& paramName,
                                  const std::vector<RankStat>& paramStat,
                                  const std::vector<std::string>& paramNames,
                                  Real confidenceLevel,
                                  const std::string& paramStr) {
  int nbClass = paramStat.size();
  int nbPos = paramStat[0].nbPos(); // get number of positions in rank

  nlohmann::json statJson; // global list, of param descriptor, each element is the descriptor of a class
  for (int k = 0; k < nbClass; ++k)  { // loop to create the parameters descriptor
    const std::list<std::pair<RankVal, Real> >& statStorageMu = paramStat[k].statStorageMu(); // helper reference to point to current statStorage
    std::list<nlohmann::json> classProba; // list of pairs {vector representing rank, proba} for the current class
    for (std::list<std::pair<RankVal, Real> >::const_iterator it = statStorageMu.begin(), ite = statStorageMu.end();
         it != ite;
         ++it) {
      const RankVal& rankCPP = it->first; // current rank in C++
      nlohmann::json rankJson(nbPos); // current rank in R
      for (int p = 0; p < nbPos; ++p) {
        rankJson = rankCPP.o()(p);
      }

      nlohmann::json classProbaInd = rankJson;
      classProbaInd.push_back(it->second);
      classProba.push_back(classProbaInd);
    }
    statJson[paramNames[k]] = classProba;
  }


  nlohmann::json logJson; // global list, of param descriptor, each element is the descriptor of a class
  for (int k = 0; k < nbClass; ++k) { // loop to create the parameters log
    int nbPos = paramStat[k].nbPos(); // get number of positions in rank
    const Vector<RankVal>& logStorageMu = paramStat[k].logStorageMu(); // helper reference to point to current statStorage
    std::list<nlohmann::json> classProba; // list of sampled mu for the current class
    for (int i = 0, ie = logStorageMu.size(); i < ie; ++i) {
      const RankVal& rankCPP = logStorageMu(i); // current rank in C++
      nlohmann::json rankJson(nbPos); // current rank in R
      for (int p = 0; p < nbPos; ++p) {
        rankJson[p] = rankCPP.o()(p);
      }
      classProba.push_back(rankJson);
    }
    logJson[paramNames[k]] = classProba;
  }

  mixtureName_[indexMixture] = idName;
  param_[indexMixture][paramName]["stat"] = statJson;
  param_[indexMixture][paramName]["log"] = logJson;
  param_[indexMixture][paramName]["paramStr"] = paramStr;

}


nlohmann::json ParamExtractorJson::jsonCppReturnParam() const {
  nlohmann::json mixtureNameR = mixtureName_;
  nlohmann::json paramJson = param_;
//  for (int i = 0; i < mixtureName_.size(); ++i) {
//    paramJson[mixtureName_[i]] = param_[i];
//  }
  return paramJson;
}

} // namespace mixt
