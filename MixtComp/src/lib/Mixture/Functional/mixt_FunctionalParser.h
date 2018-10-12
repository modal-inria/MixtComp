/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTIONALPARSER
#define MIXT_FUNCTIONALPARSER

#include <boost/regex.hpp>

#include "LinAlg/mixt_LinAlg.h"
#include "mixt_Function.h"

namespace mixt {

std::string parseFunctionalStr(Index nSub, Index nCoeff, const std::vector<std::string>& dataStr, Vector<Function>& vecInd);

} // namespace mixt

#endif // MIXT_FUNCTIONALPARSER
