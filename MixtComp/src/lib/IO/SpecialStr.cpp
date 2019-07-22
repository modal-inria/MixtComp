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
 *  Created on: August 25, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#include <IO/SpecialStr.h>

namespace mixt {

const std::string strNumber = "((?:(?:-|\\+)?(?:\\d+(?:\\.\\d*)?)|(?:\\.\\d+))(?:(?:e|E)-\\d+)?)"; // note the multiple non-capturing groups, as only the overall group is capturing
const std::string strPositiveInteger = "(\\d+)";
const std::string strQMark = "(\\?)";
const std::string strBlank = " *";
const std::string strLeftPar = " *\\[ *";
const std::string strRightPar = " *\\] *";
const std::string strCentralColon = " *: *";
const std::string strMinusInf = "-inf";
const std::string strPlusInf = "\\+inf";

} // namespace mixt
