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
 * Project:  stkpp::Clustering
 * created on: Oct 24, 2013
 * Author:   Serge Iovleff
 **/

/** @file STK_Gaussian_skImpl.h
 *  @brief In this file we implement the randomInit and mStep methods for the Gaussian_sk models.
 **/

#ifndef STK_GAUSSIAN_SKIMPL_H
#define STK_GAUSSIAN_SKIMPL_H

#include "STK_DiagGaussianComponent.h"
#include "STK_GaussianUtil.h"

#include "../../../STatistiK/include/STK_Stat_Functors.h"

namespace STK
{

/** @ingroup Clustering
 *  Implementation of initializeStep, mStep and randomInit methods for
 *  Gaussian_sk models
 **/
template<class Array>
struct MixtureModelImpl< Array, Gaussian_sk_Parameters >
{
  typedef DiagGaussianComponent<Array, Gaussian_sk_Parameters> Component;
  typedef Gaussian_sk_Parameters Parameters;
  typedef typename Array::Col ColVector;

  /** Initialize the parameters using mStep.
   *  @param components the components with the parameters to initialize
   *  @param p_tik the tik
   **/
  static void initializeStep(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  { GaussianUtil<Component>::initialMean(components, p_tik);
    for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      Array const* p_data = components[k]->p_data();
      ColVector tik(p_tik->col(k), true); // create a reference

      paramk->sigma_ = Stat::varianceWithFixedMeanSafe(*p_data, tik, paramk->mean_, false).sqrt();
      if (paramk->sigma_ <= 0.) throw Clust::mStepFail_;
    }
  }
  /** Initialize randomly the parameters of the Gaussian mixture. The centers will
   *  be selected randomly among the data set and the standard-deviations will
   *  be set to 1.
   *  @param components the vector of the components of the mixture.
   */
  static void randomInit(Array1D< Component* >& components)
  {
    if (components.size() <= 0) return;
    GaussianUtil<Component>::randomMean(components);
    for (int k= components.firstIdx(); k <= components.lastIdx(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      paramk->sigma_ = 1.;
    }
  }
  /** Compute the weighted means and the weighted variances.
   *  @param components the vector of the components of the mixture
   *  @param p_tik the matrix of the weights
   */
  static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {
    if (components.size() <= 0) return;
    // compute the means
    GaussianUtil<Component>::updateMean(components, p_tik);
    for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      Array const* p_data = components[k]->p_data();
      ColVector tik(p_tik->col(k), true); // create a reference

      paramk->sigma_ = Stat::varianceWithFixedMean(*p_data, tik, paramk->mean_, false).sqrt();
      if (paramk->sigma_ <= 0.) throw Clust::mStepFail_;
    }
  }

};

} // namespace STK

#endif /* STK_GAUSSIAN_SKIMPL_H */
