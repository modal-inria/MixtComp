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
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef FUNCTIONCSPARSER
#define FUNCTIONCSPARSER

#include <LinAlg/LinAlg.h>
#include <Mixture/Functional/FunctionCS.h>

namespace mixt {

std::string parseFunctionalStr(Index nSub, Index nCoeff, const std::vector<std::string>& dataStr, Vector<FunctionCS>& vecInd);

} // namespace mixt

#endif // FUNCTIONCSPARSER
