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

/** @file STK_Gamma_ak_bj.h
 *  @brief In this file we define the Gamma_pk_ak_bj and Gamma_p_ak_bj mixture models.
 **/

#ifndef STK_GAMMA_AK_BJ_H
#define STK_GAMMA_AK_BJ_H

#include "STK_GammaBase.h"

#include "../../../STatistiK/include/STK_Law_Exponential.h"

#define MAXITER 400
#define TOL 1e-8

namespace STK
{
template<class Array>class Gamma_ak_bj;

namespace Clust
{
/** @ingroup Clustering
 *  Traits class for the Gamma_ak_bj traits policy. */
template<class _Array>
struct MixtureTraits< Gamma_ak_bj<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Gamma_ak_bj_Parameters Parameters;
  typedef Array2D<Real>        Param;
};

} // namespace Clust

/** @ingroup Clustering
 *  Gamma_ak_bj is a mixture model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) = \sum_{k=1}^K p_k
 *     \prod_{j=1}^p\left(\frac{x_i^j}{b_{j}}\right)^{a_{k}-1}
 *                   \frac{e^{-x_i^j/b_{j}}} {b_{j} \, \Gamma(a_{k})},
 *      \quad x_i^j>0, \quad i=1,\ldots,n.
 * \f]
 **/
template<class Array>
class Gamma_ak_bj : public GammaBase<Gamma_ak_bj<Array> >
{
  public:
    typedef typename Clust::MixtureTraits< Gamma_ak_bj<Array> >::Parameters Parameters;
    typedef GammaBase<Gamma_ak_bj<Array> > Base;

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
    inline Gamma_ak_bj( int nbCluster) : Base(nbCluster), scale_() {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Gamma_ak_bj( Gamma_ak_bj const& model) : Base(model), scale_(model.scale_) {}
    /** destructor */
    inline ~Gamma_ak_bj() {}
    /** Initialize the component of the model.
     *  In this interface, the scale_ parameter is shared between all the
     *  components.
     **/
    void initializeModelImpl()
    {
      scale_.resize(this->nbVariable());
      scale_ = 1.;
      for (int k= baseIdx; k < components().end(); ++k)
      { p_param(k)->p_scale_ = &scale_;}
      paramBuffer_.resize(2*this->nbCluster(), p_data()->cols());
      paramBuffer_ = 0.;
    }
    /** use the default static method initializeStep() for a first initialization
     *  of the parameters using tik values.
     **/
    inline bool initializeStep() { return mStep();}
    /** Initialize randomly the parameters of the Gaussian mixture. The centers
     *  will be selected randomly among the data set and the standard-deviation
     *  will be set to 1.
     */
    void randomInit();
    /** Compute the weighted mean and the common variance. */
    bool mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()+ this->nbVariable();}
    /** set the parameters of the model*/
    void setParametersImpl();

  protected:
    /** Array of the common scale */
    Array2DPoint<Real> scale_;
};


/* set the parameters of the model */
template<class Array>
void Gamma_ak_bj<Array>::setParametersImpl()
{
  for (int j= p_data()->beginCols(); j < p_data()->endCols(); ++j)
  { scale_[j] = paramBuffer_(baseIdx+1, j);}
  for (int k= 0; k < this->nbCluster(); ++k)
  { p_param(baseIdx+k)->shape_ = paramBuffer_(baseIdx+2*k, p_data()->beginCols());}
}


/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Gamma_ak_bj<Array>::randomInit()
{
    // compute moments
    this->moments();
  // simulates ak
  for (int k= baseIdx; k < components().end(); ++k)
  {
    Real value= 0.;
    for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {
      Real mean = meanjk(j,k), variance = variancejk(j,k);
      value += mean*mean/variance;
    }
    p_param(k)->shape_ = Law::Exponential::rand(value/(this->nbVariable()));
  }
  // simulate bj
  for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
  {
    Real value= 0.;
    for (int k= baseIdx; k < components().end(); ++k)
    {
      Real mean = meanjk(j,k), variance = variancejk(j,k);
      value += p_param(k)->tk_ * variance/mean;
    }
    scale_[j] = Law::Exponential::rand(value/(this->nbSample()));
  }
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Gamma_ak_bj<Array>::randomInit done\n");
  this->writeParameters(stk_cout);
#endif
}

/* Compute the weighted mean and the common variance. */
template<class Array>
bool Gamma_ak_bj<Array>::mStep()
{
  if (!this->moments()) { return false;}
  // start estimations of the ajk and bj
  Real qvalue = this->qValue();
  int iter;
  for(iter=0; iter<MAXITER; ++iter)
  {
    // compute ak
    for (int k= baseIdx; k < components().end(); ++k)
    {
      // moment estimate and oldest value
      Real x0 = (p_param(k)->mean_.square()/p_param(k)->variance_).mean();
      Real x1 = p_param(k)->shape_;
      if ((x0 <=0.) || !Arithmetic<Real>::isFinite(x0)) return false;

      // compute shape
      hidden::invPsi f((p_param(k)->meanLog_ - scale_.log()).mean());
      Real a =  Algo::findZero(f, x0, x1, TOL);

      if (!Arithmetic<Real>::isFinite(a))
      {
        p_param(k)->shape_ = x0; // use moment estimate
#ifdef STK_MIXTURE_DEBUG
        stk_cout << _T("ML estimation failed in Gamma_ak_bj::mStep()\n");
        stk_cout << "x0 =" << x0 << _T("\n";);
        stk_cout << "f(x0) =" << f(x0) << _T("\n";);
        stk_cout << "x1 =" << x1 << _T("\n";);
        stk_cout << "f(x1) =" << f(x1) << _T("\n";);
#endif
      }
      else { p_param(k)->shape_ = a;}
    }
    // update all the b^j
    for (int j=p_data()->beginCols(); j<p_data()->endCols(); ++j)
    {
      Real num = 0., den = 0.;
      for (int k= baseIdx; k < components().end(); ++k)
      {
        num += p_param(k)->mean_[j] * p_param(k)->tk_;
        den += p_param(k)->shape_   * p_param(k)->tk_;
      }
      // compute b_j
      Real b = num/den;
      // divergence
      if (!Arithmetic<Real>::isFinite(b)) { return false;}
      scale_[j] = b;
    }
    // check convergence
    Real value = this->qValue();
#ifdef STK_MIXTURE_DEBUG
    if (value < qvalue)
    {
      stk_cout << _T("In Gamma_ak_bj::mStep(): mStep diverge\n");
      stk_cout << _T("New value =") << value << _T(", qvalue =") << qvalue << _T("\n");
    }
#endif
    if ((value - qvalue) < TOL) break;
    qvalue = value;
  }
#ifdef STK_MIXTURE_DEBUG
  if (iter == MAXITER)
  {
    stk_cout << _T("In Gamma_ak_bj::mStep(): mStep did not converge\n");
    stk_cout << _T("qvalue =") << qvalue << _T("\n");
  }
#endif
  return true;
}


}  // namespace STK

#undef MAXITER
#undef TOL

#endif /* STK_GAMMA_AK_BJ_H */
