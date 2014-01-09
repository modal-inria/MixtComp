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
 * Project: stkpp::Clustering
 * created on: 5 sept. 2013
 * Author:  iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_GaussianUtil.h
 *  @brief In this file we implement some functionalities used in all Gaussian models.
 **/

#ifndef STK_GAUSSIANUTIL_H
#define STK_GAUSSIANUTIL_H

#include "../../../STatistiK/include/STK_Law_Uniform.h"
#include "../../../STatistiK/include/STK_Stat_Functors.h"

namespace STK
{
/** @ingroup Clustering
 *  enclosing class for the methods common to all the Gaussian models
 **/
template<class Component>
struct GaussianUtil
{
  typedef typename Component::Array Array;
  typedef typename Component::Parameters Parameters;
  typedef typename Array::Col ColVector;
  typedef typename Array::Row RowVector;

  static void randomMean(Array1D< Component* >& components)
  {
    if (components.size() <= 0) return;
    for (int k= components.firstIdx(); k <= components.lastIdx(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      Array const* p_data = components[k]->p_data();
      // random number in [0, size[
      int i = p_data->firstIdxRows() + std::floor(Law::Uniform::rand(0.,1.)*p_data->sizeRows());
      paramk->mean_.copy(p_data->row(i));
    }
  }

  /** compute the weighted mean of a Gaussian mixture.
   *  @param components the components with the parameters to initialize
   *  @param p_tik the tik
   **/
  static void updateMean(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {
    if (components.size() <= 0) return;
    for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      Array const* p_data = components[k]->p_data();
      ColVector tik(p_tik->col(k), true); // create a reference

      paramk->mean_ = Stat::mean(*p_data, tik);
      if (paramk->mean_.nbValues() != paramk->mean_.size()) throw Clust::mStepFail_;
    }
  }
  /** compute the initial weighted mean of a Gaussian mixture.
   *  @param components the components with the parameters to initialize
   *  @param p_tik the tik
   **/
  static void initialMean(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {
    if (components.size() <= 0) return;
    for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      Array const* p_data = components[k]->p_data();
      ColVector tik(p_tik->col(k), true); // create a reference

      paramk->mean_ = Stat::meanSafe(*p_data, tik);
      if (paramk->mean_.nbValues() != paramk->mean_.size()) throw Clust::mStepFail_;
    }
  }
};

}  // namespace STK

#endif /* STK_GAUSSIANUTIL_H */
