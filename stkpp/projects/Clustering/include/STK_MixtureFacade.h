/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 4 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_MixtureFacade.h
 *  @brief In this file we define the StrategyFacade which construct properly a mixture model.
 **/


#ifndef STK_MIXTUREFACADE_H
#define STK_MIXTUREFACADE_H

#include "../../Sdk/include/STK_IRunner.h"
#include "STK_Clust_Util.h"

namespace STK
{

class IMixtureComposer;
class IMixtureStrategy;

/** facade design pattern.
 * The StrategyFacade allow to create the strategy for estimating a mixture model
 * with less effort
 **/
class StrategyFacade: public IRunnerBase
{
  public:
    /** constructor.
     * @param p_model a reference on the current model
     **/
    inline StrategyFacade( IMixtureComposer*& p_model)
                         : IRunnerBase(), p_model_(p_model), p_strategy_(0)
    {}
    /** destructor. */
    virtual ~StrategyFacade();
    /** set model in case we want to use the strategy again
     *  @param p_model the model to set
     **/
    inline void setModel(IMixtureComposer*& p_model) {p_model_ = p_model;};
    /** create a SimpleStrategy */
    void createSimpleStrategy( Clust::initType init, int nbTrialInInit, Clust::algoType initAlgo, int nbInitIter, Real initEpsilon
                             , int nbTry
                             , Clust::algoType algo, int nbIter, Real epsilon);
    /** create a FullStrategy */
    void createFullStrategy( Clust::initType init, int nbTryInInit, Clust::algoType initAlgo, int nbInitIter, Real initEpsilon
                           , int nbTry, int nbInitRun, int nbShortRun
                           , Clust::algoType shortAlgo, int nbShortIter, Real shortEpsilon
                           , Clust::algoType longAlgo, int nblongIter, Real longEpsilon);
    /** run the strategy */
   virtual bool run();

  protected:
    /** the mixture model to estimate */
    IMixtureComposer*& p_model_;
    /** the strategy to use in order to estimate the mixture model */
    IMixtureStrategy* p_strategy_;
};


}  // namespace STK

#endif /* STK_MIXTUREFACADE_H */
