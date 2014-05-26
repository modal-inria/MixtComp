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

/** @file STK_Gamma_ajk_bjkImpl.h
 *  @brief In this file we implement the maximization step and the initializations
 *  steps of the gamma_ajk_bjk mixture model.
 **/

#ifndef GAMMA_AJK_BJKIMPL_H
#define GAMMA_AJK_BJKIMPL_H

#include "../../../Analysis/include/STK_Algo_FindZero.h"
#include "../../../Analysis/include/STK_Funct_raw.h"

namespace STK
{

namespace hidden
{

/** @ingroup hidden
 *  Functor computing the derivative of the lnLikelihood of a gamma_ajk_bjk model */
class dlgamma_ajk_bjk : public IFunction<dlgamma_ajk_bjk >
{
  public:
    inline dlgamma_ajk_bjk( Real const& mean, Real const& meanLog)
                          : mean_(mean), meanLog_(meanLog)  {}
    /** @return the value of the function at a
     * @param a a positive real value
     **/
    inline Real fImpl(Real a) const
    { return (meanLog_ - std::log(mean_/a) - Funct::psi_raw(a));}
    /** @return the minimal value of the function at x */
    inline Real xminImpl() const { return 0;}
  private:
    Real mean_;
    Real meanLog_;
};

} // namespace hidden

/** @ingroup Clustering
 *  Implementation of initializeStep, mStep and randomInit methods for
 *  gamma_ajk_bjk models
 **/
template<class Array>
struct MixtureModelImpl< Array, Gamma_ajk_bjk_Parameters >
{
  typedef Gamma_ajk_bjk_Parameters Parameters;
  typedef MixtureComponent<Array, Parameters> Component;
  typedef typename Array::Col ColVector;

  /** run mStep */
  static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik, Array const* p_data)
  {
    // estimate a and b
    for (int k= baseIdx; k <= p_tik->lastIdxCols(); ++k)
    {
      Gamma_ajk_bjk_Parameters* paramk = components[k]->p_param();
      //Array const* p_data = components[k]->p_data();
      ColVector tik(p_tik->col(k), true); // create a reference

      for (int j=p_data->firstIdxCols(); j<=p_data->lastIdxCols(); ++j)
      {
        Real start1 = (paramk->mean_[j]*paramk->mean_[j]) / paramk->variance_[j]; // moment estimator
        if ((start1 <=0.) || (Arithmetic<Real>::isNA(start1))) throw Clust::mStepFail_;
        Real start2 = paramk->shape_[j];      // oldest value

        hidden::dlgamma_ajk_bjk funct(paramk->mean_[j], paramk->meanLog_[j]);
        Real a =  Algo::findZero(funct, start1, start2, 1e-08);
        if (!Arithmetic<Real>::isFinite(a))
        {
#ifdef STK_MIXTURE_DEBUG
  stk_cout << "ML estimation failed in MixtureModelImpl< Array, Gamma_ajk_bjk_Parameters >::mStep()\n";
  stk_cout << "start1 =" << start1 << _T("\n";);
  stk_cout << "f(start1) =" << funct(start1) << _T("\n";);
  stk_cout << "start2 =" << start2 << _T("\n";);
  stk_cout << "f(start2) =" << funct(start2) << _T("\n";);
#endif
          a = start1; // use moment estimator
        }

        // set values
        paramk->shape_[j] = a;
        paramk->scale_[j] = paramk->mean_[j]/a;
      }
    }
  }
};

}  // namespace STK

#endif /* GAMMA_AJK_BJKIMPL_H */
