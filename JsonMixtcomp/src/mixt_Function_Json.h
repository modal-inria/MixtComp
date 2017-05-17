/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/


#ifndef MIXT_FUNCTION_H
#define MIXT_FUNCTION_H

#include "json.hpp"
#include "../../MixtComp/src/Composer/mixt_MixtureComposer.h"
#include "../../MixtComp/src/Strategy/mixt_StrategyParam.h"

namespace mixt
{

void IDClass(mixt::MixtureComposer& mc,
             nlohmann::json& idc);

void lnProbaGivenClass(mixt::MixtureComposer& mc,
                       nlohmann::json&  pGCR);

void matDelta(mixt::MixtureComposer& mc,
              nlohmann::json& delta);

void paramJsonToCpp(const nlohmann::json& RParam,
                 StrategyParam& CppParam);

} // namespace mixt

#endif // MIXT_FUNCTION_H
