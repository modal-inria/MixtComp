/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

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
