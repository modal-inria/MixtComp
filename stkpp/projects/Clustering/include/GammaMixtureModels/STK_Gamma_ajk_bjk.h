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

/** @file STK_Gamma_ajk_bjk.h
 *  @brief In this file we define the Gamma_ajk_bjk and Gamma_p_ajk_bjk models.
 **/

#ifndef STK_GAMMA_AJK_BJK_H
#define STK_GAMMA_AJK_BJK_H

#include "STK_GammaBase.h"

#include "../../../STatistiK/include/STK_Law_Exponential.h"

#include "STK_Gamma_ajk_bjkImpl.h"

namespace STK
{
template<class Array>class Gamma_ajk_bjk;

namespace Clust
{
/** @ingroup hidden
 * Traits class for the Gamma_ajk_bjk traits policy
 **/
template<class _Array>
struct MixtureModelTraits< Gamma_ajk_bjk<_Array> >
{
  typedef _Array Array;
  typedef MixtureComponent<_Array, Gamma_ajk_bjk_Parameters> Component;
  typedef Gamma_ajk_bjk_Parameters        Parameters;
};

} // namespace hidden

/** @ingroup Clustering
 *  Gamma_ajk_bjk is a mixture model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) = \sum_{k=1}^K p_k
 *     \prod_{j=1}^p\left(\frac{x_i^j}{b_{jk}}\right)^{a_{jk}-1}
 *                   \frac{e^{-x_i^j/b_{jk}}}{b_{jk} \, \Gamma(a_{jk})},
 *      \quad x_i^j>0, \quad i=1,\ldots,n.
 * \f]
 **/
template<class Array>
class Gamma_ajk_bjk : public GammaBase< Gamma_ajk_bjk<Array> >
{
  public:
    typedef typename Clust::MixtureModelTraits< Gamma_ajk_bjk<Array> >::Component Component;
    typedef typename Clust::MixtureModelTraits< Gamma_ajk_bjk<Array> >::Parameters Parameters;
    typedef GammaBase< Gamma_ajk_bjk<Array> > Base;

    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline Gamma_ajk_bjk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Gamma_ajk_bjk( Gamma_ajk_bjk const& model) : Base(model) {}
    /** destructor */
    inline ~Gamma_ajk_bjk() {}
    /** initialize shape and scale parameters using weighted moment estimators.*/
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
    { return 2*this->nbCluster()*this->nbVariable();}
};

/* Initialize the parameters using moment estimators. */
template<class Array>
void Gamma_ajk_bjk<Array>::initializeStep()
{
    try
    { this->initialMoments();}
    catch (Clust::exceptions const & e)
    { throw e;}
    // estimate a and b
    for (int k= baseIdx; k <= p_tik()->lastIdxCols(); ++k)
    {
      for (int j=p_data()->firstIdxCols(); j<=p_data()->lastIdxCols(); ++j)
      {
        Real a = p_param(k)->mean_[j]*p_param(k)->mean_[j]/p_param(k)->variance_[j];
        if ((a<=0)||Arithmetic<Real>::isNA(a)) throw Clust::initializeStepFail_;

        p_param(k)->shape_[j] = a;
        p_param(k)->scale_[j] = p_param(k)->mean_[j]/a;
      }
    }
}

/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Gamma_ajk_bjk<Array>::randomInit()
{
  for (int j=p_data()->firstIdxCols(); j<=p_data()->lastIdxCols(); ++j)
  {
    Real mean = p_data()->col(j).mean();
    Real variance = p_data()->col(j).variance();
    for (int k= baseIdx; k <= components().lastIdx(); ++k)
    {
      // generate values
      Real a = STK::Law::Exponential::rand(mean*mean/variance);
      Real b = STK::Law::Exponential::rand(variance/mean);
      p_param(k)->shape_[j] = a;
      p_param(k)->scale_[j] = b;
    }
  }
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Gamma_ajk_bjk<Array>::randomInit() done\n");
  for (int k= baseIdx; k <= components().lastIdx(); ++k)
  {
    stk_cout << _T("Component no ") << k << _T("\n");
    stk_cout << p_param(k)->shape_ << _T("\n");
    stk_cout << p_param(k)->scale_ << _T("\n");
  }
#endif
}

/* Compute the weighted mean and the common variance. */
template<class Array>
void Gamma_ajk_bjk<Array>::mStep()
{
    try
    { this->moments();}
    catch (Clust::exceptions const & e)
    { throw Clust::mStepFail_;}
    // call mStep implementation
    MixtureModelImpl<  Array, Gamma_ajk_bjk_Parameters >::mStep(components(), p_tik(), p_data());
}

}  // namespace STK

#endif /* STK_GAMMA_AJK_BJK_H */
