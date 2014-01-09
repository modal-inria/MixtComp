/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

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
 * Author:   Vincent KUBICKI
 **/

/** @file STK_Gaussian_sImpl.h
 *  @brief In this file we implement the randomInit and mStep methods for the Gaussian_s models.
 **/

#ifndef STK_GAUSSIAN_SIMPL_H
#define STK_GAUSSIAN_SIMPL_H

#include "STK_DiagGaussianComponent.h"
#include "STK_GaussianUtil.h"

namespace STK
{

/** @ingroup Clustering
 *  Implementation of mStep and randomInit methods for Gaussian_s models
 **/
template<class Array>
struct MixtureModelImpl< Array, Gaussian_s_Parameters >
{
  typedef DiagGaussianComponent<Array, Gaussian_s_Parameters> Component;
  typedef Gaussian_s_Parameters Parameters;
  typedef typename Array::Col ColVector;
  /** Initialize the parameters using mStep.
   *  @param components the components with the parameters to initialize
   *  @param p_tik the tik
   **/
  static void initializeStep(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  { GaussianUtil<Component>::initialMean(components, p_tik);
    initialVariance(components, p_tik);
  }

  /** Initialize randomly the parameters of the Gaussian mixture. The centers will
   *  be selected randomly among the data set and the standard-deviation will
   *  be set to 1.
   *  @param components the vector of the components of the mixture.
   */
  static void randomInit(Array1D< Component* >& components)
  {
    if (components.size() <= 0) return;
    GaussianUtil<Component>::randomMean(components);
    Parameters* paramk = components.front()->p_param();
    *(paramk->p_sigma_) = 1.;
  }
  /** Compute the weighted mean and the common variance.
   *  @param components the vector of the components of the mixture
   *  @param p_tik the matrix of the weights
   */
  static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {
    if (components.size() <= 0) return;
    // compute the means
    GaussianUtil<Component>::updateMean(components, p_tik);
    // compute the standard deviation
    Array const* p_data = components.front()->p_data();
    Real variance = 0.0;
    for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      variance += ( p_tik->col(k).transpose()
                   * (*p_data - (Const::Vector<Real>(p_data->rows()) * paramk->mean_)).square()).sum();
    }
    if ((variance<=0)||Arithmetic<Real>::isNA(variance)) throw Clust::estimFail_;
    *(components.front()->p_param()->p_sigma_) = std::sqrt(variance/(p_data->sizeRows()*p_data->sizeCols()));
  }
  /** Compute the weighted mean and the common variance.
   *  @param components the vector of the components of the mixture
   *  @param p_tik the matrix of the weights
   */
  static void initialVariance(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {
    if (components.size() <= 0) return;
    // compute the standard deviation
    Array const* p_data = components.front()->p_data();
    Real variance = 0.0;
    for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      variance += ( p_tik->col(k).transpose()
                   * (p_data->safe() - (Const::Vector<Real>(p_data->rows()) * paramk->mean_)).square()).sum();
    }
    if ((variance<=0)||Arithmetic<Real>::isNA(variance)) throw Clust::estimFail_;
    *(components.front()->p_param()->p_sigma_) = std::sqrt(variance/(p_data->sizeRows()*p_data->sizeCols()));
  }
};

} // namespace STK

#endif /* STK_GAUSSIAN_SIMPL_H */
