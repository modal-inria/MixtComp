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

/** @file STK_Gamma_ajk_bjImpl.h
 *  @brief In this file we implement the specific steps of the gamma_ajk_bj model.
 **/

#ifndef STK_GAMMA_AJK_BJIMPL_H
#define STK_GAMMA_AJK_BJIMPL_H

#include "STK_GammaComponent.h"
#include "STK_GammaUtil.h"

#include "../../../Arrays/include/STK_Array2DPoint.h"

#include "../../../STatistiK/include/STK_Law_Exponential.h"
#include "../../../STatistiK/include/STK_Stat_UnivariateReal.h"
#include "../../../Analysis/include/STK_Algo_FindZero.h"
#include "../../../Analysis/include/STK_Funct_raw.h"

#define MAXITER 400

namespace STK
{

namespace hidden
{

/** @ingroup hidden
 *  Functor computing the lnLikelihood of a gamma_ajk_bj model
 **/
class dlgamma_ajk_bj : public IFunction<dlgamma_ajk_bj >
{
  public:
    /** initialize delta_ */
    inline dlgamma_ajk_bj( Real b, Real meanLog): delta_(meanLog - std::log(b)) {}
    /** @return the value of the function at a
     * @param a a positive real value
     **/
    inline Real fImpl(Real a) const { return (delta_ - Funct::psi_raw(a));}
    /** @return the minimal value of the function at x */
    inline Real xminImpl() const { return 0;}
  private:
    Real delta_;
};

} // namespace hidden

/** @ingroup Clustering
 *  Implementation of initializeStep, mStep and randomInit methods for
 *  gamma_ajk_bj models
 **/
template<class Array>
struct MixtureModelImpl<  Array, Gamma_ajk_bj_Parameters >
{
  typedef GammaComponent<Array, Gamma_ajk_bj_Parameters> Component;
  typedef Gamma_ajk_bj_Parameters Parameters;
  typedef typename Array::Col ColVector;

  // run mStep
  static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik)
  {
    // start estimations of the ajk and bj
    Real delta;
    int iter = 0;
    do
    {
      delta = 0.; iter++;
      // compute ajk
      for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
      {
        Gamma_ajk_bj_Parameters* paramk = components[k]->p_param();
        Array const* p_data = components[k]->p_data();
        for (int j=p_data->firstIdxCols(); j<=p_data->lastIdxCols(); ++j)
        {
          Real start1 = (paramk->mean_[j]*paramk->mean_[j]) / paramk->variance_[j]; // moment estimator
          if ((start1 <=0.) || (Arithmetic<Real>::isNA(start1))) throw Clust::mStepFail_;
          Real start2 = paramk->shape_[j];      // oldest value

          hidden::dlgamma_ajk_bj funct(paramk->scale(j), paramk->meanLog_[j]);
          Real a =  Algo::findZero(funct, start1, start2, 1e-08);
          if (!Arithmetic<Real>::isFinite(a))
          {
#ifdef STK_MIXTURE_DEBUG
stk_cout << "ML estimation failed in MixtureModelImpl< Array, Gamma_ajk_bj_Parameters >::mStep()\n";
stk_cout << "start1 =" << start1 << _T("\n";);
stk_cout << "f(start1) =" << funct(start1) << _T("\n";);
stk_cout << "start2 =" << start2 << _T("\n";);
stk_cout << "f(start2) =" << funct(start2) << _T("\n";);
#endif
              paramk->shape_[j] = start1; // use moment estimator
          }
          else { paramk->shape_[j] = a;}
        }
      }
      // compute bj
      Array2DPoint<Real> tk(p_tik->cols());
      for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
      { tk[k]    = p_tik->col(k).sum();}
      Array const* p_data = components.front()->p_data();
      Gamma_ajk_bj_Parameters* paramk = components.front()->p_param();
      for (int j=p_data->firstIdxCols(); j<=p_data->lastIdxCols(); ++j)
      {
        Array2DPoint<Real> meank(p_tik->cols()), ak(p_tik->cols());
        for (int k= p_tik->firstIdxCols(); k <= p_tik->lastIdxCols(); ++k)
        {
          ak[k]    = components[k]->p_param()->shape_[j];
          meank[k] = components[k]->p_param()->mean_[j];
        }
        Real b = tk.dot(meank)/tk.dot(ak);
        if (!Arithmetic<Real>::isFinite(b)) { throw Clust::mStepFail_;}
        delta = std::max(delta, std::abs(b - paramk->p_scale_->elt(j)));
        paramk->p_scale_->elt(j) = b;
      }
    }
    while((delta > 1.e-08) && (iter < MAXITER));
#ifdef STK_MIXTURE_VERBOSE
    if (iter == MAXITER)
    {
      stk_cout << _T("In MixtureModelImpl< Array, Gamma_ajk_bj_Component<Array> >::mStep()\n")
               << _T("mStep did not converge\n");
      stk_cout << _T("delta =") << delta << _T("\n");
    }
#endif
  }

  /** random initialization of the parameters. */
  static void randomInit(Array1D< Component* >& components)
  {
  }

};

}  // namespace STK

#undef MAXITER

#endif /* STK_GAMMA_AJK_BJIMPL_H */
