/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */
/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/

#ifndef MIXT_PARAMEXTRACTORJSON_H
#define MIXT_PARAMEXTRACTORJSON_H

#include <map>
#include "json.hpp"
#include "Mixture/Rank/mixt_RankStat.h"
#include "LinAlg/mixt_LinAlg.h"
#include "Mixture/Rank/mixt_RankVal.h"

namespace mixt {

class ParamExtractorJson {
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

    nlohmann::json jsonReturnParam() const;
  private:
    std::vector<std::string> mixtureName_;
    std::vector<std::map<std::string, nlohmann::json> > param_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORR_H
