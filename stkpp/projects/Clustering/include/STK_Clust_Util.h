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
class IMixtureStrategy;
class IMixtureComposer;

namespace Clust
{

/** @ingroup Clustering
 *  initialization type
 **/
enum initType
{
  randomInit_ = 0,      // DEPRECATED
  randomParamInit_ = 0, // same as random randomInit_
  randomClassInit_ = 1,
  randomFuzzyInit_ = 2
};

/** @ingroup Clustering
 *  Convert a String to a initType. The recognized strings are
 * <table>
 * <tr> <th> Initialization   </th></tr>
 * <tr> <td> "randomInit"</td></tr>
 * <tr> <td> "randomParamInit"</td></tr>
 * <tr> <td> "randomClassInit"</td></tr>
 * <tr> <td> "randomFuzzyInit"</td></tr>
 * <tr> <td> "random"         </td></tr>
 * <tr> <td> "class"          </td></tr>
 * <tr> <td> "fuzzy"          </td></tr>
 * </table>
 *  @param type the type of initialization wanted
 *  @return the initType corresponding (default is randomClassInit)
 *  @note if the string is not found in the list above,the type Clust::randomClassInit_
 *  is returned.
 **/
initType stringToInit( std::string const& type);

/** @ingroup Clustering
 *  Algorithms of estimation
 **/
enum algoType
{
  emAlgo_ = 0,
  cemAlgo_ = 1,
  semAlgo_ = 2,
  semiSemAlgo_ = 3
};

/** @ingroup Clustering
 *  Convert a String to an algoType. The recognized strings are
 * <table>
 * <tr> <th> Algorithm     </th></tr>
 * <tr> <td> "emAlgo"      </td></tr>
 * <tr> <td> "cemAlgo"     </td></tr>
 * <tr> <td> "semAlgo"     </td></tr>
 * <tr> <td> "semiSemAlgo" </td></tr>
 * <tr> <td> "em"          </td></tr>
 * <tr> <td> "cem"         </td></tr>
 * <tr> <td> "sem"         </td></tr>
 * <tr> <td> "semiSem"         </td></tr>
 * </table>
 *  @param type the type of algorithm wanted
 *  @return the algoType corresponding (default is emAlgo)
 *  @note The capitalized letters have no effect and if the string is not found
 *  in the list above,the type Clust::emAlgo_ is returned.
 **/
algoType stringToAlgo( std::string const& type);

/** @ingroup Clustering
 *  strategy of estimation
 **/
enum strategyType
{
  simpleStrategy_ = 0,
  XemStrategy_    = 1, // not implemented
  SemStrategy_    = 2,
  FullStrategy_   = 3
};

/** @ingroup Clustering
 *  Specific exceptions allowing to handle the erroros that can occur in the
 *  estimation process.
 **/
enum exceptions
{
  randomInitFail_,
  randomParamInitFail_,
  randomClassInitFail_,
  randomFuzzyInitFail_,
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
  modelCreated_ =0,         ///< the model has been created but is not initialized
  modelInitialized_ =1,     ///< the model is initialized, but its parameters are not initialized
  modelParamInitialized_=2, ///< The parameters of the model have been initialized
  shortRun_,                ///< A short run has been done
  longRun_,                 ///< A long run has been done
  modelFinalized_           ///< the model is finalized
};

/** @ingroup Clustering
 *  list of the class of mixture implemented in stkpp
 **/
enum MixtureClass
{
  Gamma_,
  Gaussian_,
  Categorical_,
  unknown_mixture_class_
};

/** @ingroup Clustering
 * list of the mixtures that can be used by the composer
 **/
enum Mixture
{
  Gamma_ajk_bjk_,
  Gamma_ajk_bk_,
  Gamma_ajk_bj_,
  Gamma_ajk_b_,
  Gamma_ak_bjk_,
  Gamma_ak_bk_,
  Gamma_ak_bj_,
  Gamma_ak_b_,
  Gamma_aj_bjk_,
  Gamma_aj_bk_,
  Gamma_a_bjk_,
  Gamma_a_bk_,
  Gaussian_sjk_,
  Gaussian_sk_,
  Gaussian_sj_,
  Gaussian_s_,
  Categorical_pjk_,
  Categorical_pk_,
  unknown_mixture_
};

/** @ingroup Clustering
 *  convert a Mixture to a MixtureClass.
 *  @param type the type of Mixture
 *  @return the MixtureClass associated to this Mixture.
 **/
MixtureClass mixtureToMixtureClass( Mixture const& type);

/** @ingroup Clustering
 *  Convert a String to a Mixture. The recognized strings are
 * <table >
 * <tr> <th> Model             </th> </tr>
 * <tr> <td> "Gamma_ajk_bjk"   </td></tr>
 * <tr> <td> "Gamma_ajk_bk"    </td></tr>
 * <tr> <td> "Gamma_ajk_bj"    </td></tr>
 * <tr> <td> "Gamma_ajk_b"     </td></tr>
 * <tr> <td> "Gamma_ak_bjk"    </td></tr>
 * <tr> <td> "Gamma_ak_bk"     </td></tr>
 * <tr> <td> "Gamma_ak_bj"     </td></tr>
 * <tr> <td> "Gamma_ak_b"      </td></tr>
 * <tr> <td> "Gamma_aj_bjk"    </td></tr>
 * <tr> <td> "Gamma_aj_bk"     </td></tr>
 * <tr> <td> "Gamma_a_bjk"     </td></tr>
 * <tr> <td> "Gamma_a_bk"      </td></tr>
 * <tr> <td> "Gaussian_sjk"    </td></tr>
 * <tr> <td> "Gaussian_sk"     </td></tr>
 * <tr> <td> "Gaussian_sj"     </td></tr>
 * <tr> <td> "Gaussian_s"      </td></tr>
 * <tr> <td> "Categorical_pjk" </td></tr>
 * <tr> <td> "Categorical_pk"  </td></tr>
 * </table>
 *  @param type the String we want to convert
 *  @return the Mixture represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_mixture_ type is returned.
 **/
Mixture stringToMixture( std::string const& type);

/** @ingroup Clustering
 *  convert a string to a Mixture and specify if the model is with free proportions
 *  or fixed proportions. The recognized strings are
 * <table border >
 * <tr> <th> Free proportions     </th><th> Fixed Proportions   </th> </tr>
 * <tr> <td> "Gamma_pk_ajk_bjk"   </td><td> "Gamma_p_ajk_bjk"   </td> </tr>
 * <tr> <td> "Gamma_pk_ajk_bk"    </td><td> "Gamma_p_ajk_bk"    </td> </tr>
 * <tr> <td> "Gamma_pk_ajk_bj"    </td><td> "Gamma_p_ajk_bj"    </td> </tr>
 * <tr> <td> "Gamma_pk_ajk_b"     </td><td> "Gamma_p_ajk_b"     </td> </tr>
 * <tr> <td> "Gamma_pk_ak_bjk"    </td><td> "Gamma_p_ak_bjk"    </td> </tr>
 * <tr> <td> "Gamma_pk_ak_bk"     </td><td> "Gamma_p_ak_bk"     </td> </tr>
 * <tr> <td> "Gamma_pk_ak_bj"     </td><td> "Gamma_p_ak_bj"     </td> </tr>
 * <tr> <td> "Gamma_pk_ak_b"      </td><td> "Gamma_p_ak_b"      </td> </tr>
 * <tr> <td> "Gamma_pk_aj_bjk"    </td><td> "Gamma_p_aj_bjk"    </td> </tr>
 * <tr> <td> "Gamma_pk_aj_bk"     </td><td> "Gamma_p_aj_bk"     </td> </tr>
 * <tr> <td> "Gamma_pk_a_bjk"     </td><td> "Gamma_p_a_bjk"     </td> </tr>
 * <tr> <td> "Gamma_pk_a_bk"      </td><td> "Gamma_p_a_bk"      </td> </tr>
 * <tr> <td> "Gaussian_pk_sjk"    </td><td> "Gaussian_p_sjk"    </td> </tr>
 * <tr> <td> "Gaussian_pk_sk"     </td><td> "Gaussian_p_sk"     </td> </tr>
 * <tr> <td> "Gaussian_pk_sj"     </td><td> "Gaussian_p_sj"     </td> </tr>
 * <tr> <td> "Gaussian_pk_s"      </td><td> "Gaussian_p_s"      </td> </tr>
 * <tr> <td> "Categorical_pk_pjk" </td><td> "Categorical_p_pjk" </td> </tr>
 * <tr> <td> "Categorical_pk_pk"  </td><td> "Categorical_p_pk"  </td> </tr>
 * </table>
 *  @param type the String we want to convert
 *  @param[out] freeProp @c true if the model have free proportions, @c false otherwise.
 *  @return the Mixture represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_mixture_ type is returned.
 **/
Mixture stringToMixture( std::string const& type, bool& freeProp);

/** @ingroup Clustering
 *  convert a Mixture to a String.
 *  @param type the type of Mixture we want to convert
 *  @return the string associated to this type.
 **/
std::string mixtureToString( Mixture const& type);

/** @ingroup Clustering
 *  convert a Mixture to a string specifying if the model is with free
 *  proportions.
 *  @sa stringToMixture
 *  @param type the Mixture we want to convert
 *  @param freeProp @c true if the model have free proportions, @c false otherwise.
 *  @return the string represented by the Mixture @c type.
 **/
std::string mixtureToString(Mixture type, bool freeProp);

/** @ingroup Clustering
 * Default number of try in an estimation strategy */
const int defaultNbTry = 5;

/** @ingroup Clustering
 * Default number of trial in an initialization */
const int defaultNbTrialInInit = 5;
/** @ingroup Clustering
 * Default algorithm type in initialization */
const Clust::algoType defaultAlgoInInit = semAlgo_;
/** @ingroup Clustering
 * Default number of iteration in an initialization algorithm */
const int defaultNbIterMaxInInit = 20;
/**  @ingroup Clustering
 * Default epsilon in the short runs (used in strategy) */
const Real defaultEpsilonInInit = 1e-02;

/** @ingroup Clustering
 * Default algorithm type in short run */
const Clust::algoType algoShortRun = cemAlgo_;
/** @ingroup Clustering
 * Default number of iterations in the short runs (used in FullStrategy) */
const int maxIterShortRun = 200;
/** @ingroup Clustering
 *  Default epsilon in the short runs (used in strategy) */
const Real epsilonShortRun = 1e-04;

/** @ingroup Clustering
 * Default algorithm type in long run */
const Clust::algoType algoLongRun = emAlgo_;
/**  @ingroup Clustering
 * Default number of iterations in the long run (used in FullStrategy) */
const int maxIterLongRun = 1000;
/**  @ingroup Clustering
 * Default epsilon in the long run (used in strategy) */
const Real epsilonLongRun = 1e-08;

/** @ingroup Clustering
 *  utility function for creating an estimation algorithm.
 *  @param algo the algorithm to create
 *  @param nbIterMax the maximal number of iteration of the algorithm
 *  @param epsilon the tolerance of the algorithm
 **/
IMixtureAlgo* createAlgo( Clust::algoType algo, int nbIterMax, Real epsilon);

/** @ingroup Clustering
 *  utility function for creating a a short Run algorithm.
 *  @param algo the algorithm to create
 *  @param nbIterMax the maximal number of iteration of the algorithm
 *  @param epsilon the tolerance of the algorithm
 **/
inline IMixtureAlgo* createShortRunAlgo( Clust::algoType algo = algoShortRun
                                       , int nbIterMax = maxIterShortRun
                                       , Real epsilon = epsilonShortRun)
{ return createAlgo(algo, nbIterMax, epsilon);}

/** @ingroup Clustering
 *  utility function for creating a a short Run algorithm.
 *  @param algo the algorithm to create
 *  @param nbIterMax the maximal number of iteration of the algorithm
 *  @param epsilon the tolerance of the algorithm
 **/
inline IMixtureAlgo* createLongRunAlgo( Clust::algoType algo = algoLongRun
                                      , int nbIterMax = maxIterLongRun
                                      , Real epsilon = epsilonLongRun)
{ return createAlgo(algo, nbIterMax, epsilon);}

/** @ingroup Clustering
 *  Utility function for creating a model initializer.
 *  @param init the kind of initializer to create
 *  @param nbInits the number of initialization to try
 *  @param algo the kind of algorithm to add to the initializer
 *  @param nbIterMax the maximal number of iteration of the initialization algorithm
 *  @param epsilon the tolerance of the initialization algorithm
 **/
IMixtureInit* createInit( Clust::initType init
                        , int nbInits = Clust::defaultNbTrialInInit
                        , Clust::algoType algo = defaultAlgoInInit
                        , int nbIterMax = defaultNbIterMaxInInit
                        , Real epsilon = defaultEpsilonInInit);

/** @ingroup Clustering
 *  Utility function for creating a SimpleStrategy.
 *  @param p_composer the composer to which we want to apply a the strategy.
 *  @param nbTry the number of tries.
 *  @param p_init the initializer to use.
 *  @param algo the algorithm to use in the long run.
 *  @return an instance of the SimpleStrategy
 **/
IMixtureStrategy* createSimpleStrategy( IMixtureComposer*& p_composer
                                      , int nbTry
                                      , IMixtureInit* const& p_init
                                      , IMixtureAlgo* const& algo);

/** @ingroup Clustering
 *  Utility function for creating a FullStrategy.
 *  @param p_composer the composer to which we want to apply a the strategy.
 *  @param nbTry the maximal number of tries.
 *  @param nbInitRun the number of initialization to perform.
 *  @param p_init the initializer to use.
 *  @param nbShortRun the number of shortRun.
 *  @param shortRunAlgo the algorithm to use in the short run.
 *  @param longRunAlgo the algorithm to use in the long run.
 *  @return an instance of the FullStrategy
 **/
IMixtureStrategy* createFullStrategy( IMixtureComposer*& p_composer
                                    , int nbTry, int nbInitRun
                                    , IMixtureInit* const& p_init
                                    , int nbShortRun, IMixtureAlgo* const& shortRunAlgo
                                    , IMixtureAlgo* const& longRunAlgo);
}  // namespace Clust


}  // namespace STK

#endif /* STK_CLUST_UTIL_H */
