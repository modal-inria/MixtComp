/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_MIXTURE_RANK_RANKPARSER_H
#define LIB_MIXTURE_RANK_RANKPARSER_H

#include <LinAlg/LinAlg.h>
#include <Mixture/Rank/RankIndividual.h>

namespace mixt {

std::string parseRankStr(const std::vector<std::string>& vecStr, int minMod, int& nbMod, Vector<RankIndividual>& vecInd);

} // namespace mixt

#endif // RANKPARSER_H
