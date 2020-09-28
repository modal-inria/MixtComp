/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: Apr 14, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXTCOMP_H
#define MIXTCOMP_H

#include <algorithm>
#include <iterator>
#include <regex>


#include <IO/IO.h>
#include <IO/MisValParser.h>
#include <LinAlg/Maths.h>
#include <LinAlg/names.h>
#include <LinAlg/Typedef.h>
#include <Mixture/Functional/FunctionCS.h>
#include <Mixture/Functional/FuncCSComputation.h>
#include <Mixture/Functional/FunctionCSParser.h>
#include <Mixture/Simple/SimpleMixture.h>
#include <Mixture/Simple/Weibull/Weibull.h>
#include <Mixture/Simple/NegativeBinomial/NegativeBinomial.h>
#include <Manager/createAllMixtures.h>
#include <Mixture/Functional/FuncCSClass.h>
#include <Mixture/Functional/FuncCSProblem.h>
#include <Mixture/Rank/RankISRParser.h>
#include <Run/Learn.h>
#include <Run/Predict.h>
#include <Statistic/Statistic.h>
#include <Strategy/SEMStrategy.h>
#include <Strategy/GibbsStrategy.h>
#include <Various/Timer.h>

#endif /* MIXTCOMP_H */
