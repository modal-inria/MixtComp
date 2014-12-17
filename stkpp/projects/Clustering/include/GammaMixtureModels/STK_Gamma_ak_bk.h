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

/** @file STK_Gamma_ak_bk.h
 *  @brief In this file we define the Gamma_pk_ak_bk and Gamma_p_ak_bk models.
 **/

#ifndef STK_GAMMA_AK_BK_H
#define STK_GAMMA_AK_BK_H

#include "STK_GammaBase.h"

#include "../../../STatistiK/include/STK_Law_Exponential.h"


namespace STK
{
template<class Array>class Gamma_ak_bk;

namespace Clust
{
/** @ingroup Clustering
 * Traits class for the Gamma_ak_bk traits policy
 **/
template<class _Array>
struct MixtureTraits< Gamma_ak_bk<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Gamma_ak_bk_Parameters  Parameters;
  typedef Array2D<Real>            Param;
};

} // namespace Clust

/** @ingroup Clustering
 *  Gamma_ak_bk is a mixture model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) = \sum_{k=1}^K p_k
 *     \prod_{j=1}^p \left(\frac{x_i^j}{b_{k}}\right)^{a_{k}-1}
 *                   \frac{e^{-x_i^j/b_{k}}}{b_{k} \, \Gamma(a_{k})},
 *      \quad x_i^j>0, \quad i=1,\ldots,n.
 * \f]
 **/
template<class Array>
class Gamma_ak_bk : public GammaBase< Gamma_ak_bk<Array> >
{
  public:
    typedef typename Clust::MixtureTraits< Gamma_ak_bk<Array> >::Parameters Parameters;
    typedef GammaBase< Gamma_ak_bk<Array> > Base;

    using Base::p_tik;
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;
    using Base::meanjk;
    using Base::variancejk;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline Gamma_ak_bk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Gamma_ak_bk( Gamma_ak_bk const& model) : Base(model) {}
    /** destructor */
    inline ~Gamma_ak_bk() {}
    /** Initialize the component of the model. */
    void initializeModelImpl()
    {
      paramBuffer_.resize(2*this->nbCluster(), p_data()->cols());
      paramBuffer_ = 0.;
    }
    /** initialize shape and scale parameters using weighted moment estimates.*/
    inline bool initializeStep() { return mStep();}
    /** Initialize randomly the parameters of the Gamma mixture. The shape
     *  will be selected randomly using an exponential of parameter mean^2/variance
     *  and the scale will be selected randomly using an exponential of parameter
     *  variance/mean.
     */
    void randomInit();
    /** Compute the mStep. */
    bool mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const { return 2*this->nbCluster();}
    /** set the parameters of the model*/
    void setParametersImpl();
};

/* set the parameters of the model */
template<class Array>
void Gamma_ak_bk<Array>::setParametersImpl()
{
  for (int k= 0; k < this->nbCluster(); ++k)
  {
    p_param(baseIdx+k)->shape_ = paramBuffer_(baseIdx+2*k  , p_data()->beginCols());
    p_param(baseIdx+k)->scale_ = paramBuffer_(baseIdx+2*k+1, p_data()->beginCols());
  }
}

/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Gamma_ak_bk<Array>::randomInit()
{
    // compute moments
    this->moments();
  for (int k= baseIdx; k < components().end(); ++k)
  {
    Real mean = this->meank(k), variance = this->variancek(k);
    p_param(k)->shape_ = Law::Exponential::rand((mean*mean/variance));
    p_param(k)->scale_ = Law::Exponential::rand((variance/mean));
  }
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Gamma_ak_bk<Array>::randomInit done\n");
  this->writeParameters(stk_cout);
#endif
}

/* Compute the weighted mean and the common variance. */
template<class Array>
bool Gamma_ak_bk<Array>::mStep()
{
  if (!this->moments()) { return false;}
  // estimate a and b
  for (int k= baseIdx; k < components().end(); ++k)
  {
    // moment estimate and oldest value
    Real x0 = this->meank(k)*this->meank(k)/this->variancek(k);
    Real x1 = p_param(k)->shape_;
    if ((x0 <=0.) || (Arithmetic<Real>::isNA(x0))) return false;

    // get shape
    hidden::invPsiMLog f( (p_param(k)->meanLog_-std::log( this->meank(k))).mean() );
    Real a = Algo::findZero(f, x0, x1, 1e-08);
    if (!Arithmetic<Real>::isFinite(a))
    {
#ifdef STK_MIXTURE_DEBUG
        stk_cout << "ML estimation failed in Gamma_ak_bjk::mStep()\n";
        stk_cout << "x0 =" << x0 << _T("\n";);
        stk_cout << "f(x0) =" << f(x0) << _T("\n";);
        stk_cout << "x1 =" << x1 << _T("\n";);
        stk_cout << "f(x1) =" << f(x1) << _T("\n";);
#endif
      a = x0; // use moment estimate
    }
    // set values
    p_param(k)->shape_ = a;
    p_param(k)->scale_ = this->meank(k)/a;
  }
  return true;
}


}  // namespace STK

#endif /* STK_GAMMA_AK_BK_H */
