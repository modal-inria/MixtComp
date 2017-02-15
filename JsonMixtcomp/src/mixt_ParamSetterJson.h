/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016
*/

/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/


#ifndef MIXT_PARAMSETTERR_H
#define MIXT_PARAMSETTERR_H

#include "json.hpp"
#include "../../MixtComp/src/LinAlg/mixt_LinAlg.h"
#include "../../MixtComp/src/Mixture/Rank/mixt_RankVal.h"

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
