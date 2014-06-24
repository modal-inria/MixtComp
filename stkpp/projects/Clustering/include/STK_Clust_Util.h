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
 * Project:  stkpp::
 * created on: 2 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_Clust_Util.h
 *  @brief In this file we define the enum, constants and utilities functions
 *  of the Clustering project.
 **/


#ifndef STK_CLUST_UTIL_H
#define STK_CLUST_UTIL_H

#include "STKernel/include/STK_Real.h"


namespace STK
{

// forward declaration
class IMixtureAlgo;
class IMixtureInit;
class IMixture;

namespace Clust
{

/** @ingroup Clustering
 *  initialization type
 **/
enum initType
{
  randomInit_ = 0,
  randomClassInit_ = 1,
  randomFuzzyInit_ = 2
};


/** @ingroup Clustering
 *  Algorithms of estimation
 **/
enum algoType
{
  emAlgo_ = 0,
  cemAlgo_ = 1,
  semAlgo_ = 2
};

/** @ingroup Clustering
 *  strategy of estimation
 **/
enum strategyType
{
  simpleStrategy_ = 0,
  XemStrategy_  = 1,
  SemStrategy_ = 2
};

/** @ingroup Clustering
 *  Specific exceptions allowing to handle the erroros that can occur in the
 *  estimation process.
 **/
enum exceptions
{
  randomInitFail_,
  estimFail_,
  initializeStepFail_,
  mStepFail_,
  eStepFail_,
  cStepFail_,
  sStepFail_
};

/** @ingroup Clustering
 *  convert a Clust::exceptions to a String.
 *  @param type the type of exception that occur
 *  @return the string associated to this exception.
 **/
String exceptionToString( exceptions const& type);


/** @ingroup Clustering
 *  Give the state of the model.
 **/
enum modelState
{
  modelCreated_, ///< the model has been created but is not initialized
  modelInitialized_, ///< the model is initialized, but its parameters are not initialized
  initialization_, ///< The parameters of the model have been initialized
  shortRun_,      ///< A short run has been done
  longRun_,   ///< a long run has been done
  finalized_ ///< the model is finalized
};

/** @ingroup Clustering
 * list of the mixtures that can be used by the composer
 **/
enum Mixture
{
  Gamma_ajk_bjk_,
  Gamma_ajk_bj_,
  Gaussian_sjk_,
  Gaussian_sk_,
  Gaussian_sj_,
  Gaussian_s_,
  Categorical_pjk_,
  Categorical_pk_,
  unknown_mixture_
};

/** @ingroup Clustering
 *  convert a String to an Mixture.
 *  @param type the String we want to convert
 *  @return the Mixture represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_mixture_ type is returned.
 **/
Mixture stringToMixture( std::string const& type);

/** @ingroup Clustering
 *  convert a Mixture to a String.
 *  @param type the type of Mixture we want to convert
 *  @return the string associated to this type.
 **/
std::string mixtureToString( Mixture const& type);

/** @ingroup Clustering
 * Default number of trial in an initialization */
const int defaultNbTrialInInit = 5;
/** @ingroup Clustering
 * Default number of iteration in an algorithm */
const int defaultNbIterMaxInInit = 200;
/**  @ingroup Clustering
 * Default epsilon in the short runs (used in strategy) */
const Real defaultEpsilonInInit = 1e-04;

/** @ingroup Clustering
 *  Default epsilon in algorithm */
const int defaultnbIterInAlgo = 200;
/** @ingroup Clustering
 *  Default epsilon in algorithm */
const Real defaultEpsilonInAlgo = 1e-08;

/** @ingroup Clustering
 * Default number of try in an estimation strategy */
const int defaultNbTry = 5;

/** @ingroup Clustering
 * Default number of iterations in the short runs (used in XemStrategy) */
const int maxIterShortRun = 200;
/**  @ingroup Clustering
 * Default number of iterations in the long run (used in XemStrategy) */
const int maxIterLongRun = 1000;

/**  @ingroup Clustering
 * Default epsilon in the short runs (used in strategy) */
const Real epsilonShortRun = 1e-04;
/**  @ingroup Clustering
 * Default epsilon in the long run (used in strategy) */
const Real epsilonLongRun = Arithmetic<Real>::epsilon();


/** @ingroup Clustering
 *  utility function for creating an estimation algorithm.
 *  @param algo the algorithm to create
 *  @param nbIterMax the maximal number of iteration of the algorithm
 *  @param epsilon the tolerance of the algorithm
 **/
IMixtureAlgo* createAlgo(Clust::algoType algo, int nbIterMax = defaultnbIterInAlgo, Real epsilon = defaultEpsilonInAlgo);

/** @ingroup Clustering
 *  Utility function for creating a model initializer.
 *  @param init the kind of initializer to create
 *  @param nbTrials the number of trial in the initailization
 *  @param algo the kind of algorithm to add to the initializer
 *  @param nbIterMax the maximal number of iteration of the initialization algorithm
 *  @param epsilon the tolerance of the initialization algorithm
 **/
IMixtureInit* createInit( Clust::initType init
                        , int nbTrials = Clust::defaultNbTrialInInit
                        , Clust::algoType algo = semAlgo_
                        , int nbIterMax = defaultNbIterMaxInInit
                        , Real epsilon = defaultEpsilonInInit);

}  // namespace Clust


}  // namespace STK

#endif /* STK_CLUST_UTIL_H */
