/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::STatistiK
 * Purpose:  Primary include file for STatistiK project.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STatistiK.h
 *  @brief This file include all the header files of the project STatistiK.
 * 
 * @defgroup STatistiK STatistiK (Statistical tools).
 * @brief The StatistiK project contains the main tools for computing the usual
 * statistics.
 *
 * It is divided in two sub-projects:
 * - @ref Laws
 * - @ref StatDesc
 **/

/** @ingroup STatistiK
 *  @defgroup Laws  The probabilities laws sub-project.
 *  In this sub-project, we compute and simulate the usual probabilities laws:
 *  normal law, binomial law, Cauchy law,...
 **/

/** @ingroup STatistiK
 *  @defgroup StatDesc The descriptive statistics sub-project.
 *  In this sub-project, we compute the usual descriptive statistics of variables.
 **/

/** @ingroup STatistiK
 *  @namespace STK::Law
 *  @brief This is the namespace enclosing the classes handling the usual
 *  probabilities Laws.
 **/

/** @ingroup STatistiK
 *  @namespace STK::Stat
 *  @brief this is the namespace for the statistical treatment.
 *  The namespace Stat enclose the methods and classes  for usual statistical
 *  treatment of the variable like mean, variance, covariance, ...
 **/

#ifndef STATISTIK_H
#define STATISTIK_H

// random number generators
#include "../projects/STatistiK/include/MersenneTwister.h"
#include "../projects/STatistiK/include/STK_RandBase.h"

// namespace Law
// probabilities laws
#include "../projects/STatistiK/include/STK_Law_Util.h"

#include "../projects/STatistiK/include/STK_Law_Bernoulli.h"
#include "../projects/STatistiK/include/STK_Law_Categorical.h"

#include "../projects/STatistiK/include/STK_Law_Beta.h"
#include "../projects/STatistiK/include/STK_Law_Cauchy.h"
#include "../projects/STatistiK/include/STK_Law_Exponential.h"
#include "../projects/STatistiK/include/STK_Law_Gamma.h"
#include "../projects/STatistiK/include/STK_Law_Normal.h"
#include "../projects/STatistiK/include/STK_Law_Uniform.h"

#include "../projects/STatistiK/include/STK_Law_JointBernoulli.h"
#include "../projects/STatistiK/include/STK_Law_JointCauchy.h"
#include "../projects/STatistiK/include/STK_Law_JointNormal.h"
#include "../projects/STatistiK/include/STK_Law_JointGamma.h"

#include "../projects/STatistiK/include/STK_Law_MultiNormal.h"

// namespace Stat
// Univariate Statistics
#include "../projects/STatistiK/include/STK_Stat_Functors.h"
#include "../projects/STatistiK/include/STK_Stat_UnivariateReal.h"

// bivariate Statistics
#include "../projects/STatistiK/include/STK_Stat_Bivariate.h"
#include "../projects/STatistiK/include/STK_Stat_BivariateRealReal.h"

// Multivariate Statistics
#include "../projects/STatistiK/include/STK_Stat_Multivariate.h"
#include "../projects/STatistiK/include/STK_Stat_MultivariateReal.h"

// perform the usual transformations on variables
#include "../projects/STatistiK/include/STK_Stat_Transform.h"

#endif /*STATISTIK_H*/

