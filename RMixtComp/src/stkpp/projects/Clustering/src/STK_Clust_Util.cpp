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
 **/

/** @file STK_Clust_Util.cpp
 *  @brief In this file we implement the utilties functions of the Clustering project.
 **/

#include "../../STKernel/include/STK_String_Util.h"

#include "../include/STK_Clust_Util.h"
#include "../include/STK_MixtureAlgo.h"
#include "../include/STK_MixtureInit.h"
#include "../include/ingredients/STK_Ingredients.h"

namespace STK
{

namespace Clust
{

/* @ingroup Clustering
 *  convert a TypeReduction to a String.
 *  @param type the type of exception that occur
 *  @return the string associated to this type.
 **/
String exceptionToString( exceptions const& type)
{
  if (type == initializeStepFail_)  return String(_T("initializeStep fail"));
  if (type == randomInitFail_) return String(_T("RandomInit fail"));
  if (type == estimFail_) return String(_T("Estimation fail"));
  if (type == mStepFail_)  return String(_T("mStep fail"));
  if (type == eStepFail_)  return String(_T("eStep fail"));
  if (type == cStepFail_)  return String(_T("cStep fail"));
  if (type == sStepFail_)  return String(_T("sStep fail"));
  return String(_T("unknown exception"));
}

/* @ingroup Clustering
 *  convert a String to an Ingredient.
 *  @param type the String we want to convert
 *  @return the Ingredient represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_mixture_ type is returned.
 **/
Ingredient stringToIngredient( std::string const& type)
{
  if (toUpperString(type) == toUpperString(_T("Gamma_ajk_bjk"))) return Gamma_ajk_bjk_;
  if (toUpperString(type) == toUpperString(_T("Gamma_ajk_bj"))) return Gamma_ajk_bj_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_sjk"))) return Gaussian_sjk_;
  if (toUpperString(type) == toUpperString(_T("Gaussian_s"))) return Gaussian_s_;
  return unknown_mixture_;
}

/* @ingroup Clustering
 *  convert a Ingredient to a String.
 *  @param type the type of Ingredient we want to convert
 *  @return the string associated to this type.
 **/
std::string ingredientToString( Ingredient const& type)
{
  if (type == Gamma_ajk_bjk_) return String(_T("Gamma_ajk_bjk"));
  if (type == Gamma_ajk_bj_) return String(_T("Gamma_ajk_bj"));
  if (type == Gaussian_sjk_) return String(_T("Gaussian_sjk"));
  if (type == Gaussian_s_) return String(_T("Gaussian_s"));
  return String(_T("unknown"));
}

/** @ingroup Clustering
 *  utility function for creating an Ingredient.
 *  @param model ingredient to create
 **/
IMixture* createIngredient(Ingredient model, std::string const& id, int nbCluster)
{
  switch (model)
  {
    case Gamma_ajk_bjk_:
      return new IngredientGamma_ajk_bjk(id, nbCluster);
      break;
    case Gamma_ajk_bj_:
      return new IngredientGamma_ajk_bj(id, nbCluster);
      break;
    case Gaussian_sjk_:
      return new IngredientGaussian_sjk(id, nbCluster);
      break;
    case Gaussian_s_:
      return new IngredientGaussian_s(id, nbCluster);
      break;
    default:
      return 0;
      break;
  }
  // avoid warning
  return 0;
}

/* utility function for creating an estimation algorithm
 *  @param algo the algorithm to create
 *  @param nbIterMax the maximal number of iteration of the algorithm
 *  @param epsilon the tolerance of the algorithm
 **/
IMixtureAlgo* createAlgo(Clust::algoType algo, int nbIterMax, Real epsilon)
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Entering createAlgo() with:\n")
           << _T("nbIterMax = ") << nbIterMax << _T("\n")
           << _T("epsilon = ") << epsilon << _T("\n");
#endif
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
  default:
    break;
  }
  p_algo->setNbIterMax(nbIterMax);
  p_algo->setEpsilon(epsilon);
  return p_algo;
}

/* utility function for creating a model initializer
 *  @param init the kind of initializer to create
 *  @param algo the kind of algorithm to add to the initializer
 *  @param nbIterMax the maximal number of iteration of the algorithm
 *  @param epsilon the tolerance of the algorithm
 **/
IMixtureInit* createInit(Clust::initType init,  int nbTrials, Clust::algoType algo, int nbIterMax, Real epsilon)
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Entering createInit() with:\n")
           << _T("nbTrials = ") << nbTrials << _T("\n")
           << _T("nbIterMax = ") << nbIterMax << _T("\n")
           << _T("epsilon = ") << epsilon << _T("\n");
#endif

  IMixtureInit* p_init = 0;
  switch (init)
  {
    case Clust::randomInit_:
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
  p_init->setNbTrials(nbTrials);
  p_init->setInitAlgo(Clust::createAlgo(algo, nbIterMax, epsilon));
  return p_init;
}

} // namespace Clust

} // namespace STK


