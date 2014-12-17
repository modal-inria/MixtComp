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
 * Project:  stkpp::Model
 * Purpose:  Main include file for the Model project.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file StatModels.h
 *  @brief This file include all the header files of the project Model.
 *
 *  @defgroup StatModels Statistical Models
 *  @brief The project Model proposes classes for modeling and estimating
 *  Statistical Models.
 *
 *  The aim of this project is to define Interface and specialized classes
 *  in order to manipulate and estimate the parameters of any kind of
 *  statistical model.
 *
 *  A @e statistical @e model is a formalization of relationships between
 *  variables in the form of mathematical equations. A statistical model
 *  describes how one or more random variables are related to one or more random
 *  variables. The model is statistical as the variables are not
 *  deterministically but stochastically related. In mathematical terms, a
 *  statistical model is frequently thought of as a pair \f$ (Y, P) \f$ where
 *  \f$ Y \f$ is the set of possible observations and \f$ P \f$ the set of
 *  possible probability distributions on \f$ Y \f$. It is assumed that there is
 *  a distinct element of \f$ P \f$ which generates the observed data.
 *  Statistical inference enables us to make statements about which element(s)
 *  of this set are likely to be the true one.
 **/

/**  @ingroup StatModels
 *  @namespace Model namespace enclosing the enum and utilities functions of the
 *  StatModel project.
 **/

#ifndef STATMODELS_H
#define STATMODELS_H

#include "../projects/StatModels/include/STK_Model_Util.h"
#include "../projects/StatModels/include/STK_IStatModelBase.h"

#include "../projects/StatModels/include/STK_BernoulliModel.h"
#include "../projects/StatModels/include/STK_JointBernoulliModel.h"
#include "../projects/StatModels/include/STK_JointGaussianModel.h"
#include "../projects/StatModels/include/STK_JointGammaModel.h"

#include "../projects/StatModels/include/STK_GaussianModel.h"
#include "../projects/StatModels/include/STK_Gaussian2BlocksModel.h"

#include "../projects/StatModels/include/STK_PenCriterion.h"

#endif // STATMODELS_H
