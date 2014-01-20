/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

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

*/

/*
 * Project:  stkpp:Clustering
 * created on: Oct 24, 2013
 * Author:   Serge Iovleff
 **/

/** @file STK_Gaussian_sjImpl.h
 *  @brief In this file we implement the methods needed by the Gaussian_sj models.
 **/

#ifndef STK_GAUSSIAN_SJIMPL_H
#define STK_GAUSSIAN_SJIMPL_H

#include "STK_DiagGaussianComponent.h"
#include "STK_GaussianUtil.h"

namespace STK
{

/** @ingroup Clustering
 *  Implementation of the methods needed by the Gaussian_sj models
 **/
template<class Array>
struct MixtureModelImpl< Array, Gaussian_sj_Parameters >
{
  typedef DiagGaussianComponent<Array, Gaussian_sj_Parameters> Component;
  typedef Gaussian_sj_Parameters Parameters;
  typedef typename Array::Col ColVector;
  /** Initialize randomly the parameters of the Gaussian mixture. The centers will
   *  be selected randomly among the data set and the standard-deviation will
   *  be set to 1.
   *  @param components the vector of the components of the mixture.
   */
  static void randomInit(Array1D< Component* >& components)
  {}
  /** Compute the weighted mean and the common variance.
   *  @param components the vector of the components of the mixture
   *  @param p_tik the matrix of the weights
   */
  static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {}
};

} // namespace STK

#endif /* STK_GAUSSIAN_SJIMPL_H */
