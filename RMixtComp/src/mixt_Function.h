/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 18, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTION_H
#define MIXT_FUNCTION_H

#include "Rcpp.h"
#include "MixtComp/src/Composer/mixt_MixtureComposer.h"
#include "MixtComp/src/Strategy/mixt_StrategyParam.h"

namespace mixt
{

void IDClass(mixt::MixtureComposer& mc,
             Rcpp::NumericMatrix& idc);

void lnProbaGivenClass(mixt::MixtureComposer& mc,
                       Rcpp::NumericMatrix& pGCR);

void matDelta(mixt::MixtureComposer& mc,
              Rcpp::NumericMatrix& delta);

void paramRToCpp(const Rcpp::List& RParam,
                 StrategyParam& CppParam);

} // namespace mixt

#endif // MIXT_FUNCTION_H
