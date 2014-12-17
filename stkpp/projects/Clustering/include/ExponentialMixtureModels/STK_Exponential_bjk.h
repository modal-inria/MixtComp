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

/** @file STK_Exponential_bjk.h
 *  @brief In this file we define the Exponential_pk_ajk_bjk and Exponential_p_ajk_bjk models.
 **/

#ifndef STK_EXPONENTIAL_BJK_H
#define STK_EXPONENTIAL_BJK_H

#include "STK_ExponentialBase.h"

#include "../../../STatistiK/include/STK_Law_Exponential.h"

namespace STK
{
template<class Array>class Exponential_bjk;

namespace Clust
{
/** @ingroup Clustering
 * Traits class for the Exponential_bjk traits policy
 **/
template<class _Array>
struct MixtureTraits< Exponential_bjk<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Exponential_bjk_Parameters        Parameters;
  typedef Array2D<Real>        Param;
};

} // namespace Clust

/** @ingroup Clustering
 *  Exponential_bjk is a mixture model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) = \sum_{k=1}^K p_k
 *     \prod_{j=1}^p \frac{e^{-x_i^j/b_{jk}}}{b_{jk}}
 *      \quad x_i^j\geq 0, \quad i=1,\ldots,n.
 * \f]
 **/
template<class Array>
class Exponential_bjk : public ExponentialBase< Exponential_bjk<Array> >
{
  public:
    typedef typename Clust::MixtureTraits< Exponential_bjk<Array> >::Parameters Parameters;
    typedef ExponentialBase< Exponential_bjk<Array> > Base;

     using Base::p_tik;using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline Exponential_bjk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Exponential_bjk( Exponential_bjk const& model) : Base(model) {}
    /** destructor */
    inline ~Exponential_bjk() {}
    /** initialize shape and scale parameters using weighted moment estimates.*/
    inline bool initializeStep() { return mStep();}
    /** Initialize randomly the parameters of the Exponential mixture. The shape
     *  will be selected randomly using an exponential of parameter mean^2/variance
     *  and the scale will be selected randomly using an exponential of parameter
     *  variance/mean.
     */
    void randomInit();
    /** Compute the mStep. */
    bool mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*this->nbVariable();}
    /** set the parameters of the model */
    void setParametersImpl();
};

/* set the parameters of the model
 *  @param params the parameters of the model
 **/
template<class Array>
void Exponential_bjk<Array>::setParametersImpl()
{
    for (int k = baseIdx; k < components().end(); ++k)
    {
      for (int j = p_data()->beginCols(); j < p_data()->endCols(); ++j)
      { p_param(k)->scale_[j]  = this->paramBuffer_(k, j);
    }
  }
}

/* Initialize randomly the parameters of the gamma mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Exponential_bjk<Array>::randomInit()
{
  // estimate b
  for (int k= baseIdx; k < components().end(); ++k)
  {
    for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {
       Real mean = p_data()->col(j).wmean(p_tik()->col(k));
       p_param(k)->scale_[j] = Law::Exponential::rand(mean);
    }
  }
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Exponential_bjk<Array>::randomInit done\n");
  this->writeParameters(stk_cout);
#endif
}

/* Compute the weighted mean and the common variance. */
template<class Array>
bool Exponential_bjk<Array>::mStep()
{
  // estimate b
  for (int k= baseIdx; k < components().end(); ++k)
  {
    for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
    { p_param(k)->scale_[j] = p_data()->col(j).wmean(p_tik()->col(k));}
  }
  return true;
}


}  // namespace STK

#endif /* STK_EXPONENTIAL_BJK_H */
