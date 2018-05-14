/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */
/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/


#ifndef MIXT_PARAMSETTERR_H
#define MIXT_PARAMSETTERR_H

#include "json.hpp"
#include "LinAlg/mixt_LinAlg.h"
#include "Mixture/Rank/mixt_RankVal.h"

namespace mixt {

class ParamSetterJson {
  public:
    ParamSetterJson(const nlohmann::json param);

    ~ParamSetterJson();

    void getParam(const std::string& idName,
                  const std::string& paramName,
                  Vector<Real>& param,
                  std::string& paramStr) const;

    void getParam(const std::string& idName,
                  const std::string& paramName,
                  Vector<RankVal>& param,
                  std::string& paramStr) const;

  private:
    const nlohmann::json param_;
};

} // namespace mixt

#endif // MIXT_PARAMSETTERR_H
