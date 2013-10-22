/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public
 License along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place,
 Suite 330,
 Boston, MA 02111-1307
 USA

 */

/*
 * Project:  MixtComp
 * created on: Oct 21, 2013
 * Author:   Vincent KUBICKI
 **/

/** @file mixt_Facade.h
 *  @brief In this file 
 **/

#ifndef MIXT_FACADE_H_
#define MIXT_FACADE_H_

#include "mixtureInterface/IMixture.h"
#include "stkpp/projects/Clustering/include/STK_Clust_Util.h"
#include "stkpp/projects/Clustering/include/STK_IMixtureModelBase.h"
#include "stkpp/projects/Clustering/include/STK_MixtureFacade.h"

class CompositeMixtureModel;
class STK::StrategyFacade;

#include <vector>

namespace mixt
{

struct s_algo
{
    STK::Clust::algoType typeOfAlgo;
    int nbIter;
    STK::Real epsilon;
};

class Facade
{
  public:
    Facade();
    virtual ~Facade();
    bool createComposerModel(int nbClusters);
    bool registerMixture(IMixture& mixture);
    bool setAlgo(std::vector<s_algo> algoList);
    bool createSimpleStrategy( STK::Clust::initType init, int nbTrialInInit
                             , int nbTry);
    bool createSemStrategy( STK::Clust::initType init, int nbTrialInInit
                          , int nbBurnInIter
                          , int nbIter);
    bool run();
  protected:
    CompositeMixtureModel* composerModel_;
    STK::StrategyFacade* strategy_;
    std::vector<s_algo> algoList_;
};

} /* namespace mixt */
#endif /* MIXT_FACADE_H_ */
