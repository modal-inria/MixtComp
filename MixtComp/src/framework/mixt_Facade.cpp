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

/** @file mixt_Facade.cpp
 *  @brief In this file 
 **/

#include "mixt_Facade.h"

namespace mixt
{

Facade::Facade() :
    composerModel_(0),
    strategy_(0)
{}

bool Facade::createComposerModel(int nbClusters)
{
  composerModel_ = new CompositeMixtureModel(nbClusters);
  return true;
}

bool Facade::registerMixture(IMixture& mixture)
{
  composerModel_->v_mixtures_.push_back(&mixture);
  return true;
}

bool Facade::setAlgo(std::vector<s_algo> algoList)
{
  algoList_ = algoList;
  return true;
}

/** create a SimpleStrategy */
bool Facade::createSimpleStrategy( STK::Clust::initType init, int nbTrialInInit
                         , int nbTry)
{
  strategy_->setModel(composerModel_);
  strategy_->createSimpleStrategy( init, nbTrialInInit
                                 , algoList_.at(0).typeOfAlgo, algoList_.at(0).nbIter, algoList_.at(0).epsilon
                                 , nbTry
                                 , algoList_.at(1).typeOfAlgo, algoList_.at(1).nbIter, algoList_.at(1).epsilon);
  return true;
}

/** create a SemStrategy */
bool Facade::createSemStrategy( STK::Clust::initType init, int nbTrialInInit
                              , int nbBurnInIter
                              , int nbIter)
{
  strategy_->setModel(composerModel_);
  strategy_->createSemStrategy( init, nbTrialInInit
                              , nbBurnInIter
                              , nbIter);
  return true;
}

bool Facade::run()
{
  strategy_->run();
  return true;
}

Facade::~Facade()
{
  delete strategy_;
  delete composerModel_;
}

} /* namespace mixt */
