/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Apr 14, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_MIXTCOMP_H
#define MIXT_MIXTCOMP_H

#include <algorithm>
#include <iterator>

#include <regex>

#include <nlopt.h>

#include "IO/mixt_IO.h"
#include "IO/mixt_MisValParser.h"
#include "LinAlg/mixt_Math.h"
#include "LinAlg/mixt_Typedef.h"
#include <LinAlg/names.h>
#include <Mixture/Functional/mixt_Function.h>
#include <Mixture/Functional/mixt_FunctionalComputation.h>
#include <Mixture/Functional/mixt_FunctionalClass.h>
#include <Mixture/Functional/mixt_FunctionalParser.h>
#include <Mixture/Functional/FuncProblem.h>
#include <Mixture/Rank/mixt_RankParser.h>
#include <Mixture/Simple/SimpleMixture.h>
#include <Mixture/Simple/Weibull/Weibull.h>
#include <Mixture/Simple/NegativeBinomial/NegativeBinomial.h>
#include <Manager/createAllMixtures.h>
#include <Run/Learn.h>
#include <Run/Predict.h>
#include <Statistic/mixt_Statistic.h>
#include <Strategy/mixt_SEMStrategy.h>
#include <Strategy/mixt_GibbsStrategy.h>
#include <Various/mixt_Timer.h>

#endif /* MIXT_MIXTCOMP_H */
