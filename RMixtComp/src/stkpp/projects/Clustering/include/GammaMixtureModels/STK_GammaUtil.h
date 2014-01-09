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

/** @file STK_GammaUtil.h
 *  @brief In this file we implement some functionalities used in all gamma models.
 **/

#ifndef STK_GAMMAUTIL_H
#define STK_GAMMAUTIL_H

#include "../../../STatistiK/include/STK_Stat_Functors.h"

namespace STK
{
/** @ingroup Clustering
 *  enclosing class for the methods common to all the gamma models
 **/
template<class Component>
struct GammaUtil
{
  typedef typename Component::Array Array;
  typedef typename Component::Parameters Parameters;
  typedef typename Array::Col ColVector;

  /** compute the weighted moments of a gamma mixture.
   *  @param components the components with the parameters to initialize
   *  @param p_tik the tik
   **/
  static void moments(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {
    if (components.size() <= 0) return;
    for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      Array const* p_data = components[k]->p_data();
      ColVector tik(p_tik->col(k), true); // create a reference

      paramk->mean_ = Stat::mean(*p_data, tik);
      if (  (paramk->mean_.nbValues() != paramk->mean_.size())
         || (paramk->mean_ <= 0).template cast<int>().sum() > 0)
      {
#ifdef STK_MIXTURE_DEBUG
        stk_cout << _T("In GammaUtil::moments. k=")<< k << _T("mean = ") << paramk->mean_;
#endif
        throw Clust::estimFail_;
      }
      paramk->meanLog_ = Stat::mean(p_data->log(), tik);
      if (paramk->meanLog_.nbValues() != paramk->meanLog_.size())
      {
#ifdef STK_MIXTURE_DEBUG
        stk_cout << _T("In GammaUtil::moments. k=")<< k << _T("meanLog = ") << paramk->meanLog_;
#endif
        throw Clust::estimFail_;
      }
      paramk->variance_ = Stat::varianceWithFixedMean(*p_data,tik, paramk->mean_, true);
      if (  (paramk->variance_.nbValues() != paramk->variance_.size())
         || (paramk->variance_ <= 0).template cast<int>().sum() > 0)
      {
#ifdef STK_MIXTURE_DEBUG
        stk_cout << _T("In GammaUtil::moments. k=")<< k << _T("meanLog = ") << paramk->meanLog_;
#endif
        throw Clust::estimFail_;
      }
//
//      for (int j=p_data->firstIdxCols(); j<=p_data->lastIdxCols(); ++j)
//      {
//        Real mean =  p_data->col(j).wmean(tik);
//        if ((mean<=0)||Arithmetic<Real>::isNA(mean)) { throw Clust::estimFail_;}
//        paramk->mean_[j] = mean;
//        Real meanLog =  p_data->col(j).log().wmean(tik);
//        if (Arithmetic<Real>::isNA(meanLog)) throw Clust::estimFail_;
//        paramk->meanLog_[j] = meanLog;
//        Real variance =  p_data->col(j).wvariance(tik);
//        if ((variance<=0)||Arithmetic<Real>::isNA(variance)) throw Clust::estimFail_;
//        paramk->variance_[j] = variance;
//      }
    }
  }
  /** compute safely the weighted moments of a gamma mixture.
   *  @param components the components with the parameters to initialize
   *  @param p_tik the tik
   **/
  static void initialMoments(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {
    if (components.size() <= 0) return;
    for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
    {
      Parameters* paramk = components[k]->p_param();
      Array const* p_data = components[k]->p_data();
      ColVector tik(p_tik->col(k), true); // create a reference
      for (int j=p_data->firstIdxCols(); j<=p_data->lastIdxCols(); ++j)
      {
        Real mean =  p_data->col(j).safe().wmean(tik);
        if ((mean<=0)||Arithmetic<Real>::isNA(mean))
        {
#ifdef STK_MIXTURE_DEBUG
          stk_cout << _T("In GammaUtil::initialMoments. k=")<< k << _T("mean = ") << mean;
#endif
          throw Clust::estimFail_;
        }
        paramk->mean_[j] = mean;
        Real meanLog =  (p_data->col(j).safe(1e-08)).log().wmean(tik);
        if (Arithmetic<Real>::isNA(meanLog))
        {
#ifdef STK_MIXTURE_DEBUG
          stk_cout << _T("In GammaUtil::initialMoments. k=")<< k << _T("meanLog = ") << meanLog;
#endif
          throw Clust::estimFail_;
        }
        paramk->meanLog_[j] = meanLog;
        Real variance =  p_data->col(j).safe().wvariance(tik);
        if ((variance<=0)||Arithmetic<Real>::isNA(variance))
        {
#ifdef STK_MIXTURE_DEBUG
          stk_cout << _T("In GammaUtil::initialMoments. k=")<< k << _T("variance = ") << variance;
#endif
          throw Clust::estimFail_;
        }
        paramk->variance_[j] = variance;
      }
    }
  }
};

}  // namespace STK

#endif /* STK_GAMMAUTIL_H */
