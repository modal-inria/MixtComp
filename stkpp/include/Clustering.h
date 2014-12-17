/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004  Serge Iovleff

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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                  */

/*
 * Project:  stkpp::Clustering
 * Purpose:  Main include file for the Clustering project.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file Clustering.h
 *  @brief This file include all the header files of the project Clustering.
 *
 *  @defgroup Clustering Clustering using generative models
 *  @brief The project Clustering proposes classes for modeling and estimating
 *  generative mixture model.
 *
 *  The aim of this project is to define Interface and specialized classes
 *  in order to manipulate and estimate the parameters of any kind of
 *  generative mixture model.
 *
 *  In statistics, a mixture model is a probabilistic model for representing
 *  the presence of subpopulations within an overall population, without
 *  requiring that an observed data-set should identify the sub-population to
 *  which an individual observation belongs. Formally a mixture model
 *  corresponds to the mixture distribution that represents the probability
 *  distribution of observations in the overall population. However, while
 *  problems associated with "mixture distributions" relate to deriving the
 *  properties of the overall population from those of the sub-populations,
 *  "mixture models" are used to make statistical inferences about the
 *  properties of the sub-populations given only observations on the pooled
 *  population, without sub-population-identity information.
 *
 *  Some ways of implementing mixture models involve steps that attribute
 *  postulated sub-population-identities to individual observations (or weights
 *  towards such sub-populations), in which case these can be regarded as types
 *  of unsupervised learning or clustering procedures. However not all inference
 *  procedures involve such steps.
 **/

/**  @ingroup Clustering
 *  @namespace STK::Clust
 *  @brief The namespace Clust enclose all the enum and utilities functions
 *  needed by the Clustering project.
 **/

#ifndef CLUSTERING_H
#define CLUSTERING_H


#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ajk_bjk.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ajk_bk.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ajk_bj.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ajk_b.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ak_bjk.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ak_bk.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ak_bj.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_ak_b.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_aj_bjk.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_aj_bk.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_a_bjk.h"
#include "../projects/Clustering/include/GammaMixtureModels/STK_Gamma_a_bk.h"

#include "../projects/Clustering/include/GaussianMixtureModels/STK_Gaussian_sjk.h"
#include "../projects/Clustering/include/GaussianMixtureModels/STK_Gaussian_sk.h"
#include "../projects/Clustering/include/GaussianMixtureModels/STK_Gaussian_sj.h"
#include "../projects/Clustering/include/GaussianMixtureModels/STK_Gaussian_s.h"

#include "../projects/Clustering/include/CategoricalMixtureModels/STK_Categorical_pjk.h"
#include "../projects/Clustering/include/CategoricalMixtureModels/STK_Categorical_pk.h"

#include "../projects/Clustering/include/STK_MixtureInit.h"
#include "../projects/Clustering/include/STK_MixtureAlgo.h"
#include "../projects/Clustering/include/STK_MixtureStrategy.h"
#include "../projects/Clustering/include/STK_MixtureComposer.h"
#include "../projects/Clustering/include/STK_MixtureCriterion.h"
#include "../projects/Clustering/include/STK_MixtureFacade.h"
#include "../projects/Clustering/include/STK_MixtureManager.h"

#endif // CLUSTERING_H
