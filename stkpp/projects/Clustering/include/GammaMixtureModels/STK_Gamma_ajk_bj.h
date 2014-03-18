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

/** @file STK_Gamma_ajk_bj.h
 *  @brief In this file we define the Gamma_ajk_bj and Gamma_p_ajk_bj mixture classes.
 **/

#ifndef STK_GAMMA_AJK_BJ_H
#define STK_GAMMA_AJK_BJ_H

#include "STK_GammaBase.h"
#include "STK_GammaUtil.h"

#include "../../../STatistiK/include/STK_Law_Exponential.h"

#include "STK_Gamma_ajk_bjImpl.h"


namespace STK
{
template<class Array>class Gamma_ajk_bj;

namespace Clust
{
/** @ingroup Clustering
 *  Traits class for the Gamma_ajk_bj traits policy. */
template<class _Array>
struct MixtureModelTraits< Gamma_ajk_bj<_Array> >
{
  typedef _Array Array;
  typedef Gamma_ajk_bj_Parameters Parameters;
  typedef GammaComponent<_Array, Parameters> Component;
};

} // namespace Clust

/** @ingroup Clustering
 *  Gamma_ajk_bj is a mixture model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) = \sum_{k=1}^K p_k
 *     \prod_{j=1}^p\left(\frac{x_i^j}{b_{j}}\right)^{a_{jk}-1}
 *                   \frac{e^{-x_i^j/b_{j}}} {b_{j} \, \Gamma(a_{jk})},
 *      \quad x_i^j>0, \quad i=1,\ldots,n.
 * \f]
 **/
template<class Array>
class Gamma_ajk_bj : public GammaBase<Gamma_ajk_bj<Array> >
{
  public:
    typedef typename Clust::MixtureModelTraits< Gamma_ajk_bj<Array> >::Component Component;
    typedef typename Clust::MixtureModelTraits< Gamma_ajk_bj<Array> >::Parameters Parameters;
    typedef GammaBase<Gamma_ajk_bj<Array> > Base;
    typedef typename Array::Col ColVector;

    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline Gamma_ajk_bj( int nbCluster) : Base(nbCluster), scale_() {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Gamma_ajk_bj( Gamma_ajk_bj const& model) : Base(model), scale_(model.scale_) {}
    /** destructor */
    inline ~Gamma_ajk_bj() {}
    /** Initialize the component of the model.
     *  This function have to be called prior to any used of the class.
     *  In this interface, the @c initializeModel() method call the base
     *  class IMixtureModel::initializeModel() and for all the
     *  components create the parameters.
     **/
    void initializeModel()
    {
      Base::initializeModel();
      scale_.resize(this->nbVariable());
      scale_ = 1.;
      for (int k= components().firstIdx(); k <= components().lastIdx(); ++k)
      { p_param(k)->p_scale_ = &scale_;}
    }
    /** use the default static method initializeStep() for a first initialization
     *  of the parameters using tik values.
     **/
    void initializeStep();
    /** Initialize randomly the parameters of the Gaussian mixture. The centers
     *  will be selected randomly among the data set and the standard-deviation
     *  will be set to 1.
     */
    void randomInit();
    /** Compute the weighted mean and the common variance. */
    void mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*this->nbVariable()+ this->nbVariable();}

  protected:
    /** Array of the common scale */
    Array2DPoint<Real> scale_;
};

/* Initialize the parameters using mStep. */
template<class Array>
void Gamma_ajk_bj<Array>::initializeStep()
{
  try
  { GammaUtil<Component>::initialMoments(components(), p_tik());}
  catch (Clust::exceptions const & e)
  { throw Clust::initializeStepFail_;}
  // estimate ajk and bj
  for (int k= p_tik()->firstIdxCols(); k <= p_tik()->lastIdxCols(); ++k)
  {
    for (int j=p_data()->firstIdxCols(); j<=p_data()->lastIdxCols(); ++j)
    {
      // set a values
      Real a = p_param(k)->mean_[j]*p_param(k)->mean_[j]/p_param(k)->variance_[j];
      if ((a<=0)||Arithmetic<Real>::isNA(a)) throw Clust::initializeStepFail_;
      p_param(k)->shape_[j] = a;
    }
  }
  for (int j=scale_.firstIdx(); j<=scale_.lastIdx(); ++j)
  {
    Array2DPoint<Real> meank(p_tik()->cols()), ak(p_tik()->cols()), tk(p_tik()->cols());
    for (int k= p_tik()->firstIdxCols(); k <= p_tik()->lastIdxCols(); ++k)
    {
      tk[k]    = p_tik()->col(k).sum();
      ak[k]    = p_param(k)->shape_[j];
      meank[k] = p_param(k)->mean_[j];
    }
    Real b = tk.dot(meank)/tk.dot(ak);
    if ((b<=0)||Arithmetic<Real>::isNA(b)) throw Clust::initializeStepFail_;
    scale_[j] = b;
  }
}

/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Gamma_ajk_bj<Array>::randomInit()
{
  for (int j=p_data()->firstIdxCols(); j<=p_data()->lastIdxCols(); ++j)
  {
    Real mean = p_data()->col(j).mean();
    if ((mean <=0.) || (Arithmetic<Real>::isNA(mean))) throw Clust::randomInitFail_;
    Real variance = p_data()->col(j).variance();
    if ((variance <=0.) || (Arithmetic<Real>::isNA(variance))) throw Clust::randomInitFail_;
    // random shape for each cluster
    for (int k= components().firstIdx(); k <= components().lastIdx(); ++k)
    { p_param(k)->shape_[j] = STK::Law::Exponential::rand(mean*mean/variance);}
    // random scale
    scale_[j] = STK::Law::Exponential::rand(variance/mean);
  }
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Gamma_ajk_bj<Array>::randomInit done\n");
  for (int k= components.firstIdx(); k <= components.lastIdx(); ++k)
  {
    stk_cout << _T("Component no ") << k << _T("\n");
    stk_cout << p_param(k)->shape_;
    stk_cout << scale_;
  }
#endif
}

/* Compute the weighted mean and the common variance. */
template<class Array>
void Gamma_ajk_bj<Array>::mStep()
{
  try
  { GammaUtil<Component>::moments(components(), p_tik());}
  catch (Clust::exceptions const & e)
  { throw Clust::mStepFail_;}

  MixtureModelImpl<  Array, Gamma_ajk_bj_Parameters >::mStep(components(), p_tik());
}


}  // namespace STK

#endif /* STK_GAMMA_AJK_BJ_H */
