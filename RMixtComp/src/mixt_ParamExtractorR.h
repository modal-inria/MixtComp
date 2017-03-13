/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 07, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_PARAMEXTRACTORR_H
#define MIXT_PARAMEXTRACTORR_H

#include "MixtComp/src/Mixture/Rank/mixt_RankStat.h"
#include "MixtComp/src/LinAlg/mixt_LinAlg.h"
#include "MixtComp/src/Mixture/Rank/mixt_RankVal.h"
#include "Rcpp.h"

namespace mixt {

class ParamExtractorR {
  public:
    void setNbMixture(int nbMixture);

    /** Extractor for ConfIntParamStat */
    void exportParam(int indexMixture,
                     const std::string& idName,
                     const std::string& paramName,
                     const Matrix<Real>& paramStat,
                     const Matrix<Real>& paramsLog,
                     const std::vector<std::string>& paramNames,
                     Real confidenceLevel,
                     const std::string& paramStr);

    /** Extractor for Rank data type */
    void exportParam(int indexMixture,
                     const std::string& idName,
                     const std::string& paramName,
                     const std::vector<RankStat>& paramStat,
                     const std::vector<std::string>& paramNames,
                     Real confidenceLevel,
                     const std::string& paramStr);

    Rcpp::List rcppReturnParam() const;
  private:
    std::vector<std::string> mixtureName_;
    std::vector<std::map<std::string, Rcpp::List> > param_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORR_H
