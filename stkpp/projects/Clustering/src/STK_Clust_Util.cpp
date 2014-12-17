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
 * Author:   iovleff, s...DOTi...ATstkppDOTorg
 **/

/** @file STK_Clust_Util.cpp
 *  @brief In this file we implement the utilities functions of the Clustering project.
 **/
#include "../include/STK_IMixtureComposer.h"
#include "../include/STK_MixtureAlgo.h"
#include "../include/STK_MixtureInit.h"
#include "../include/STK_MixtureStrategy.h"

namespace STK
{

namespace Clust
{

/* @ingroup Clustering
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
initType stringToInit( std::string const& type)
{
  if (toUpperString(type) == toUpperString(_T("randomInit"))) return randomInit_;
  if (toUpperString(type) == toUpperString(_T("randomParamInit"))) return randomParamInit_;
  if (toUpperString(type) == toUpperString(_T("randomClassInit"))) return randomClassInit_;
  if (toUpperString(type) == toUpperString(_T("randomFuzzyInit"))) return randomFuzzyInit_;
  if (toUpperString(type) == toUpperString(_T("random"))) return randomParamInit_;
  if (toUpperString(type) == toUpperString(_T("class"))) return randomClassInit_;
  if (toUpperString(type) == toUpperString(_T("fuzzy"))) return randomFuzzyInit_;
  return randomClassInit_;
}

/* @ingroup Clustering
 *  Convert a String to an algoType. The recognized strings are
 * <table>
 * <tr> <th> Algorithm   </th></tr>
 * <tr> <td> "emAlgo"</td></tr>
 * <tr> <td> "cemAlgo"</td></tr>
 * <tr> <td> "semAlgo"</td></tr>
 * <tr> <td> "em"</td></tr>
 * <tr> <td> "cem"         </td></tr>
 * <tr> <td> "sem"          </td></tr>
 * </table>
 *  @param type the type of algorithm wanted
 *  @return the algoType corresponding (default is emAlgo)
 *  @note if the string is not found in the list above,the type Clust::emAlgo_
 *  is returned.
 **/
algoType stringToAlgo( std::string const& type)
{
  if (toUpperString(type) == toUpperString(_T("emAlgo"))) return emAlgo_;
  if (toUpperString(type) == toUpperString(_T("cemAlgo"))) return cemAlgo_;
  if (toUpperString(type) == toUpperString(_T("semAlgo"))) return semAlgo_;
  if (toUpperString(type) == toUpperString(_T("semiSemAlgo"))) return semiSemAlgo_;
  if (toUpperString(type) == toUpperString(_T("em"))) return emAlgo_;
  if (toUpperString(type) == toUpperString(_T("cem"))) return cemAlgo_;
  if (toUpperString(type) == toUpperString(_T("sem"))) return semAlgo_;
  if (toUpperString(type) == toUpperString(_T("semiSem"))) return semiSemAlgo_;
  return emAlgo_;
}


/* @ingroup Clustering
 *  convert a TypeReduction to a String.
 *  @param type the type of exception that occur
 *  @return the string associated to this type.
 **/
String exceptionToString( exceptions const& type)
{
  if (type == randomInitFail_)      return String(_T("RandomInit fail"));
  if (type == randomClassInitFail_)     return String(_T("RandomClassInit fail"));
  if (type == randomFuzzyInitFail_) return String(_T("RandomFuzzyInit fail"));
  if (type == randomParamInitFail_) return String(_T("RandomParamInit fail"));
  if (type == initializeStepFail_)  return String(_T("initializeStep fail"));
  if (type == estimFail_) return String(_T("Estimation fail"));
  if (type == mStepFail_) return String(_T("mStep fail"));
  if (type == eStepFail_) return String(_T("eStep fail"));
  if (type == cStepFail_) return String(_T("cStep fail"));
  if (type == sStepFail_) return String(_T("sStep fail"));
  return String(_T("unknown exception"));
}

/* @ingroup Clustering
 *  convert a Mixture to a MixtureClass.
 *  @param type the type of Mixture
 *  @return the MixtureClass associated to this Mixture.
 **/
MixtureClass mixtureToMixtureClass( Mixture const& type)
{
  if (type == Gamma_ajk_bjk_) return Gamma_;
  if (type == Gamma_ajk_bk_) return Gamma_;
  if (type == Gamma_ajk_bj_) return Gamma_;
  if (type == Gamma_ajk_b_) return Gamma_;
  if (type == Gamma_ak_bjk_) return Gamma_;
  if (type == Gamma_ak_bk_) return Gamma_;
  if (type == Gamma_ak_bj_) return Gamma_;
  if (type == Gamma_ak_b_) return Gamma_;
  if (type == Gamma_aj_bjk_) return Gamma_;
  if (type == Gamma_aj_bk_) return Gamma_;
  if (type == Gamma_a_bjk_) return Gamma_;
  if (type == Gamma_a_bk_) return Gamma_;
  if (type == Gaussian_sjk_) return Gaussian_;
  if (type == Gaussian_sk_) return Gaussian_;
  if (type == Gaussian_sj_) return Gaussian_;
  if (type == Gaussian_s_) return Gaussian_;
  if (type == Categorical_pjk_) return Categorical_;
  if (type == Categorical_pk_) return Categorical_;
  return unknown_mixture_class_;
}

/* @ingroup Clustering
 *  convert a String to an Mixture.
 *  @param type the String we want to convert
 *  @return the Mixture represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_mixture_ type is returned.
 **/
Mixture stringToMixture( std::string const& type)
{
  if (toUpperString(type) == toUpperString(_T("Gamma_ajk_bjk"))) return Gamma_ajk_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_ajk_bk"))) return Gamma_ajk_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_ajk_bj"))) return Gamma_ajk_bj_;
  if (toUpperString(type) == toUpperString(_T("Gamma_ajk_b"))) return Gamma_ajk_b_;
  if (toUpperString(type) == toUpperString(_T("Gamma_ak_bjk"))) return Gamma_ak_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_ak_bk"))) return Gamma_ak_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_ak_bj"))) return Gamma_ak_bj_;
  if (toUpperString(type) == toUpperString(_T("Gamma_ak_b"))) return Gamma_ak_b_;
  if (toUpperString(type) == toUpperString(_T("Gamma_aj_bjk"))) return Gamma_aj_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_aj_bk"))) return Gamma_aj_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_a_bjk"))) return Gamma_a_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_a_bk"))) return Gamma_a_bk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_sjk"))) return Gaussian_sjk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_sk"))) return Gaussian_sk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_sj"))) return Gaussian_sj_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_s"))) return Gaussian_s_;
  if (toUpperString(type) == toUpperString(_T("Categorical_pjk"))) return Categorical_pjk_;
  if (toUpperString(type) == toUpperString(_T("Categorical_pk"))) return Categorical_pk_;
  return unknown_mixture_;
}
/* @ingroup Clustering
 *  convert a String to an Mixture.
 *  @param type the String we want to convert
 *  @return the Mixture represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_mixture_ type is returned.
 **/
Mixture stringToMixture( std::string const& type, bool& freeProp)
{
  freeProp = false;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_ajk_bjk"))) return Gamma_ajk_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_ajk_bk"))) return Gamma_ajk_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_ajk_bj"))) return Gamma_ajk_bj_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_ajk_b"))) return Gamma_ajk_b_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_ak_bjk"))) return Gamma_ak_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_ak_bk"))) return Gamma_ak_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_ak_bj"))) return Gamma_ak_bj_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_ak_b"))) return Gamma_ak_b_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_aj_bjk"))) return Gamma_aj_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_aj_bk"))) return Gamma_aj_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_a_bjk"))) return Gamma_a_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_p_a_bk"))) return Gamma_a_bk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_p_sjk"))) return Gaussian_sjk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_p_sk"))) return Gaussian_sk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_p_sj"))) return Gaussian_sj_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_p_s"))) return Gaussian_s_;
  if (toUpperString(type) == toUpperString(_T("Categorical_p_pjk"))) return Categorical_pjk_;
  if (toUpperString(type) == toUpperString(_T("Categorical_p_pk"))) return Categorical_pk_;
  freeProp = true;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_ajk_bjk"))) return Gamma_ajk_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_ajk_bk"))) return Gamma_ajk_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_ajk_bj"))) return Gamma_ajk_bj_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_ajk_b"))) return Gamma_ajk_b_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_ak_bjk"))) return Gamma_ak_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_ak_bk"))) return Gamma_ak_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_ak_bj"))) return Gamma_ak_bj_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_ak_b"))) return Gamma_ak_b_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_aj_bjk"))) return Gamma_aj_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_aj_bk"))) return Gamma_aj_bk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_a_bjk"))) return Gamma_a_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_pk_a_bk"))) return Gamma_a_bk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_pk_sjk"))) return Gaussian_sjk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_pk_sk"))) return Gaussian_sk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_pk_sj"))) return Gaussian_sj_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_pk_s"))) return Gaussian_s_;
  if (toUpperString(type) == toUpperString(_T("Categorical_pk_pjk"))) return Categorical_pjk_;
  if (toUpperString(type) == toUpperString(_T("Categorical_pk_pk"))) return Categorical_pk_;
  return unknown_mixture_;
}

/* @ingroup Clustering
 *  convert a Mixture to a String.
 *  @param type the type of Mixture we want to convert
 *  @return the string associated to this type.
 **/
std::string mixtureToString( Mixture const& type)
{
  if (type == Gamma_ajk_bjk_) return String(_T("Gamma_ajk_bjk"));
  if (type == Gamma_ajk_bk_) return String(_T("Gamma_ajk_bk"));
  if (type == Gamma_ajk_bj_) return String(_T("Gamma_ajk_bj"));
  if (type == Gamma_ajk_b_) return String(_T("Gamma_ajk_b"));
  if (type == Gamma_ak_bjk_) return String(_T("Gamma_ak_bjk"));
  if (type == Gamma_ak_bk_) return String(_T("Gamma_ak_bk"));
  if (type == Gamma_ak_bj_) return String(_T("Gamma_ak_bj"));
  if (type == Gamma_ak_b_) return String(_T("Gamma_ak_b"));
  if (type == Gamma_aj_bjk_) return String(_T("Gamma_aj_bjk"));
  if (type == Gamma_aj_bk_) return String(_T("Gamma_aj_bk"));
  if (type == Gamma_a_bjk_) return String(_T("Gamma_a_bjk"));
  if (type == Gamma_a_bk_) return String(_T("Gamma_a_bk"));
  if (type == Gaussian_sjk_) return String(_T("Gaussian_sjk"));
  if (type == Gaussian_sk_) return String(_T("Gaussian_sk"));
  if (type == Gaussian_sj_) return String(_T("Gaussian_sj"));
  if (type == Gaussian_s_) return String(_T("Gaussian_s"));
  if (type == Categorical_pjk_) return String(_T("Categorical_pjk"));
  if (type == Categorical_pk_) return String(_T("Categorical_pk"));
  return String(_T("unknown"));
}

/* @ingroup Clustering
 *  convert a Mixture to a string specifying if the model is with free
 *  proportions.
 *  @sa stringToMixture
 *  @param type the Mixture we want to convert
 *  @param freeProp @c true if the model have free proportions, @c false otherwise.
 *  @return the string represented by the Mixture @c type.
 **/
std::string mixtureToString(Mixture type, bool freeProp)
{
  if (freeProp == false)
  {
    if (type == Gamma_ajk_bjk_) return String(_T("Gamma_p_ajk_bjk"));
    if (type == Gamma_ajk_bk_) return String(_T("Gamma_p_ajk_bk"));
    if (type == Gamma_ajk_bj_) return String(_T("Gamma_p_ajk_bj"));
    if (type == Gamma_ajk_b_) return String(_T("Gamma_p_ajk_b"));
    if (type == Gamma_ak_bjk_) return String(_T("Gamma_p_ak_bjk"));
    if (type == Gamma_ak_bk_) return String(_T("Gamma_p_ak_bk"));
    if (type == Gamma_ak_bj_) return String(_T("Gamma_p_ak_bj"));
    if (type == Gamma_ak_b_) return String(_T("Gamma_p_ak_b"));
    if (type == Gamma_aj_bjk_) return String(_T("Gamma_p_aj_bjk"));
    if (type == Gamma_aj_bk_) return String(_T("Gamma_p_aj_bk"));
    if (type == Gamma_a_bjk_) return String(_T("Gamma_p_a_bjk"));
    if (type == Gamma_a_bk_) return String(_T("Gamma_p_a_bk"));
    if (type == Gaussian_sjk_) return String(_T("Gaussian_p_sjk"));
    if (type == Gaussian_sk_) return String(_T("Gaussian_p_sk"));
    if (type == Gaussian_sj_) return String(_T("Gaussian_p_sj"));
    if (type == Gaussian_s_) return String(_T("Gaussian_p_s"));
    if (type == Categorical_pjk_) return String(_T("Categorical_p_pjk"));
    if (type == Categorical_pk_) return String(_T("Categorical_p_pk"));
  }
  else
  {
    if (type == Gamma_ajk_bjk_) return String(_T("Gamma_pk_ajk_bjk"));
    if (type == Gamma_ajk_bk_) return String(_T("Gamma_pk_ajk_bk"));
    if (type == Gamma_ajk_bj_) return String(_T("Gamma_pk_ajk_bj"));
    if (type == Gamma_ajk_b_) return String(_T("Gamma_pk_ajk_b"));
    if (type == Gamma_ak_bjk_) return String(_T("Gamma_pk_ak_bjk"));
    if (type == Gamma_ak_bk_) return String(_T("Gamma_pk_ak_bk"));
    if (type == Gamma_ak_bj_) return String(_T("Gamma_pk_ak_bj"));
    if (type == Gamma_ak_b_) return String(_T("Gamma_pk_ak_b"));
    if (type == Gamma_aj_bjk_) return String(_T("Gamma_pk_aj_bjk"));
    if (type == Gamma_aj_bk_) return String(_T("Gamma_pk_aj_bk"));
    if (type == Gamma_a_bjk_) return String(_T("Gamma_pk_a_bjk"));
    if (type == Gamma_a_bk_) return String(_T("Gamma_pk_a_bk"));
    if (type == Gaussian_sjk_) return String(_T("Gaussian_pk_sjk"));
    if (type == Gaussian_sk_) return String(_T("Gaussian_pk_sk"));
    if (type == Gaussian_sj_) return String(_T("Gaussian_pk_sj"));
    if (type == Gaussian_s_) return String(_T("Gaussian_pk_s"));
    if (type == Categorical_pjk_) return String(_T("Categorical_pk_pjk"));
    if (type == Categorical_pk_) return String(_T("Categorical_pk_pk"));
  }
  return String(_T("unknown"));
}

/* utility function for creating an estimation algorithm
 *  @param algo the algorithm to create
 *  @param nbIterMax the maximal number of iteration of the algorithm
 *  @param epsilon the tolerance of the algorithm
 **/
IMixtureAlgo* createAlgo(Clust::algoType algo, int nbIterMax, Real epsilon)
{
  IMixtureAlgo* p_algo = 0;
  switch (algo)
  {
  case emAlgo_:
    p_algo = new EMAlgo();
    break;
  case cemAlgo_:
    p_algo = new CEMAlgo();
    break;
  case semAlgo_:
    p_algo = new SEMAlgo();
    break;
  case semiSemAlgo_:
    p_algo = new SemiSEMAlgo();
    break;
  default:
    break;
  }
  if (p_algo)
  {
    p_algo->setNbIterMax(nbIterMax);
    p_algo->setEpsilon(epsilon);
  }
  return p_algo;
}

/* utility function for creating a model initializer
 *  @param init the kind of initializer to create
 *  @param algo the kind of algorithm to add to the initializer
 *  @param nbIterMax the maximal number of iteration of the algorithm
 *  @param epsilon the tolerance of the algorithm
 **/
IMixtureInit* createInit(Clust::initType init,  int nbInits, Clust::algoType algo, int nbIterMax, Real epsilon)
{
  IMixtureInit* p_init = 0;
  switch (init)
  {
    case Clust::randomParamInit_:
      p_init = new RandomInit();
      break;
    case Clust::randomClassInit_:
      p_init = new ClassInit();
      break;
    case Clust::randomFuzzyInit_:
      p_init = new FuzzyInit();
      break;
    default:
      break;
  }
  if (p_init)
  {
    p_init->setNbTry(nbInits);
    p_init->setInitAlgo(Clust::createAlgo(algo, nbIterMax, epsilon));
  }
  return p_init;
}

/* @ingroup Clustering
 *  Utility function for creating a SimpleStrategy.
 *  @param nbTry the number of tries.
 *  @param p_init the initializer to use.
 *  @param algo the algorithm to use in the long run.
 *  @return an instance of the SimpleStrategy
 **/
IMixtureStrategy* createSimpleStrategy( IMixtureComposer*& p_composer
                                      , int nbTry
                                      , IMixtureInit* const& p_init
                                      , IMixtureAlgo* const& algo)
{
  SimpleStrategyParam* p_strategyParam = new SimpleStrategyParam();
  p_strategyParam->p_algo_ = algo;
  SimpleStrategy* p_strategy = new SimpleStrategy(p_composer);
  p_strategy->setNbTry(nbTry);
  p_strategy->setMixtureInit(p_init);
  p_strategy->setParam(p_strategyParam);
  return p_strategy;
}


/* @ingroup Clustering
 *  Utility function for creating a FullStrategy.
 *  @param nbTry the number of tries.
 *  @param p_init the initializer to use.
 *  @param nbShortRun the number of shortRun.
 *  @param shortRunAlgo the algorithm to use in the short run.
 *  @param longRunAlgo the algorithm to use in the long run.
 *  @return an instance of the SimpleStrategy
 **/
IMixtureStrategy* createFullStrategy( IMixtureComposer*& p_composer
                                    , int nbTry, int nbInitRun
                                    , IMixtureInit* const& p_init
                                    , int nbShortRun, IMixtureAlgo* const& shortRunAlgo
                                    , IMixtureAlgo* const& longRunAlgo)
{
  FullStrategyParam* p_strategyParam = new FullStrategyParam();
  p_strategyParam->nbInitRun_  = nbInitRun;
  p_strategyParam->nbShortRun_  = nbShortRun;
  p_strategyParam->p_shortAlgo_ = shortRunAlgo;
  p_strategyParam->p_longAlgo_  = longRunAlgo;
  FullStrategy* p_strategy = new FullStrategy(p_composer);
  p_strategy->setNbTry(nbTry);
  p_strategy->setMixtureInit(p_init);
  p_strategy->setParam(p_strategyParam);
  return p_strategy;
}

} // namespace Clust

} // namespace STK


