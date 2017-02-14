/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016
*/

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

void paramJsonToCpp(const nlohmann::json& RParam,
                 StrategyParam& CppParam);

} // namespace mixt

#endif // MIXT_FUNCTION_H
