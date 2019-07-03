/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

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
