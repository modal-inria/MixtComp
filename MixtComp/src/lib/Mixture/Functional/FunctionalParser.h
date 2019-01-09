/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef FUNCTIONALPARSER
#define FUNCTIONALPARSER

#include <LinAlg/LinAlg.h>
#include <Mixture/Functional/Function.h>

namespace mixt {

std::string parseFunctionalStr(Index nSub, Index nCoeff, const std::vector<std::string>& dataStr, Vector<Function>& vecInd);

} // namespace mixt

#endif // FUNCTIONALPARSER
