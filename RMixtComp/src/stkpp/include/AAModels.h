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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                  */

/*
 * Project:  stkpp::AAModels
 * Purpose:  Main include file for the AAModels project.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file AAModels.h
 *  @brief This file include all the header files of the project AAModels.
 *
 *  @defgroup AAModels Auto-Associative Models.
 *  @brief The project AAM provides classes and tools for unsupervised learning
 *  and data analysis using Auto-Associative models.
 *
 * A function \f$ g \f$ is an auto-associative function
 * of dimension d if it is a map from \f$ \mathbb{R}^p \f$ to
 * \f$ \mathbb{R}^p \f$ that can be written \f$ g=r\circ p \f$ where
 * p (the ``Reduction'') is a map from \f$ \mathbb{R}^p \f$ to
 * \f$ \mathbb{R}^d \f$ (generally d<p) and r (the ``Regression'') is
 * a map from \f$ \mathbb{R}^d \f$ to \f$ \mathbb{R}^p \f$ .
 *
 * The Auto-Associative models project provide an Interface class allowing
 * to handle the projection and regression objects in an abstract setting.
 * It propose also an implementation when the data set is assumed gaussian
 * with a non-linear part, and a specialization when the regression part
 * is assumed linear.
 **/

#ifndef AAMODELS_H
#define AAMODELS_H

#include "../projects/AAModels/include/STK_IAAModel.h"
#include "../projects/AAModels/include/STK_GaussianAAModel.h"
#include "../projects/AAModels/include/STK_LinearAAModel.h"

#endif // AAMODELS_H
