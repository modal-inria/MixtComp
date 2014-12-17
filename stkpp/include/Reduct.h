/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * Project:  stkpp::reduct
 * created on: 16 June 2010
 * Purpose:  main header file of the Reduct project.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file Reduct.h
 *  @brief This file include all the header files of the project Reduct.
 **/

/** @defgroup Reduct Dimension Reduction.
 * @brief The project Reduct propose a set of classes for computing dimension
 * Reduction (or feature extraction) of a data set.
 *
 * Feature extraction transforms the data in the high-dimensional space to a
 * space of fewer dimensions. The data transformation may be linear, as in
 * principal component analysis (PCA), but many nonlinear dimensionality
 * reduction techniques also exist.
 *
 * The main linear technique for dimensionality reduction, principal component
 * analysis, performs a linear mapping of the data to a lower dimensional space
 * in such a way that the variance of the data in the low-dimensional
 * representation is maximized.
 *
 * If the data is non-linear it is possible to unfold the underlying support
 * of the data set using the local variance which try to preserve the
 * neighborhood variations of the original data set.
 *
 * An alternative approach to neighborhood preservation is through the
 * minimization of a cost function that measures differences between distances
 * in the input and output spaces. Important examples of such techniques include
 * classical multidimensional scaling (which is identical to PCA in somme cases),
 * Isomap (which uses geodesic distances in the data space), diffusion maps
 * (which uses diffusion distances in the data space), t-SNE (which minimizes
 * the divergence between distributions over pairs of points), and curvilinear
 * component analysis.
 **/

/**  @ingroup Reduct
 * @namespace STK::Reduct
 * @brief This namespace encloses all variables and constant specific to the
 * Reduct project.
 **/



#ifndef REDUCT_H
#define REDUCT_H

#include "../projects/Reduct/include/STK_Reduct_Util.h"
#include "../projects/Reduct/include/STK_IReduct.h"
#include "../projects/Reduct/include/STK_ILinearReduct.h"

#include "../projects/Reduct/include/STK_LocalVariance.h"
#include "../projects/Reduct/include/STK_LocalVariancePage.h"

#include "../projects/Reduct/include/STK_ProjectedVariance.h"

#endif /* REDUCT_H */
