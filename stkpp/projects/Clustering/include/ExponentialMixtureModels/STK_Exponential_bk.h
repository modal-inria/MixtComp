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

/** @file STK_Exponential_bk.h
 *  @brief In this file we define the Exponential_pk_ajk_bk and Exponential_p_ajk_bk models.
 **/

#ifndef STK_EXPONENTIAL_BK_H
#define STK_EXPONENTIAL_BK_H


#include "STK_ExponentialBase.h"

#include "../../../STatistiK/include/STK_Law_Exponential.h"

namespace STK
{
template<class Array>class Exponential_bk;

namespace Clust
{
/** @ingroup Clustering
 * Traits class for the Exponential_bk traits policy
 **/
template<class _Array>
struct MixtureTraits< Exponential_bk<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Exponential_bk_Parameters  Parameters;
  typedef Array2D<Real>            Param;
};

} // namespace Clust

/** @ingroup Clustering
 *  Exponential_bk is a mixture model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) = \sum_{k=1}^K p_k
 *     \prod_{j=1}^p \left(\frac{x_i^j}{b_{k}}\right)^{a_{jk}-1}
 *                   \frac{e^{-x_i^j/b_{k}}}{b_{k}} \,
 *                    \quad x_i^j>0, \quad i=1,\ldots,n.
 * \f]
 **/
template<class Array>
class Exponential_bk : public ExponentialBase< Exponential_bk<Array> >
{
  public:
    typedef typename Clust::MixtureTraits< Exponential_bk<Array> >::Parameters Parameters;
    typedef ExponentialBase< Exponential_bk<Array> > Base;

     using Base::p_tik;using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline Exponential_bk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Exponential_bk( Exponential_bk const& model) : Base(model) {}
    /** destructor */
    inline ~Exponential_bk() {}
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
    inline int computeNbFreeParameters() const { return this->nbCluster();}
    /** set the parameters of the model */
    void setParametersImpl();
};

/* set the parameters of the model
 *  @param params the parameters of the model
 **/
template<class Array>
void Exponential_bk<Array>::setParametersImpl()
{
  for (int k = baseIdx; k < components().end(); ++k)
  { p_param(k)->scale_  = this->paramBuffer_(k, p_data()->beginCols());}
}


/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Exponential_bk<Array>::randomInit()
{
  // compute moments
  for (int k= baseIdx; k < components().end(); ++k)
  {
    Real value = 0.;
    for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {  value += p_data()->col(j).wmean(p_tik()->col(k));}
    p_param(k)->scale_ = Law::Exponential::rand(value/this->nbVariable());
  }
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Exponential_bk<Array>::randomInit done\n");
  this->writeParameters(stk_cout);
#endif
}

/* Compute the weighted mean and the common variance. */
template<class Array>
bool Exponential_bk<Array>::mStep()
{
    Real value = 0.;
    for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {  value += p_data()->col(j).wmean(p_tik()->col(k));}
    p_param(k)->scale_ = value;
}

}  // namespace STK

#endif /* STK_EXPONENTIAL_BK_H */
