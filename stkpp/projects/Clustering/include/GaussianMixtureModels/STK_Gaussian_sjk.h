/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

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
 * Project:  stkpp::Clustering
 * created on: Oct 24, 2013
 * Author:   Serge Iovleff
 **/

/** @file STK_Gaussian_sjk.h
 *  @brief In this file we implement the Gaussian_sjk class
 **/

#ifndef STK_GAUSSIAN_SJK_H
#define STK_GAUSSIAN_SJK_H

#include "STK_DiagGaussianBase.h"

namespace STK
{

//forward declaration, to allow for recursive template
template<class Array>class Gaussian_sjk;

namespace Clust
{
/** @ingroup Clustering
 *  Traits class for the Gaussian_sjk traits policy. */
template<class _Array>
struct MixtureTraits< Gaussian_sjk<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Gaussian_sjk_Parameters        Parameters;
  typedef Array2D<Real>        Param;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Gaussian mixture model @c Gaussian_sjk is
 *  the most general diagonal Gaussian model and have a density function of the
 *  form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma^j_{k}} \exp\left\{-\frac{(x^j-\mu^j_k)^2}{2(\sigma^j_{k})^2}\right\}.
 * \f]
 **/
template<class Array>
class Gaussian_sjk : public DiagGaussianBase<Gaussian_sjk<Array> >
{
  public:
    typedef DiagGaussianBase<Gaussian_sjk<Array> > Base;

    typedef typename Clust::MixtureTraits< Gaussian_sjk<Array> >::Parameters Parameters;

    using Base::p_tik;
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    Gaussian_sjk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    Gaussian_sjk( Gaussian_sjk const& model) : Base(model) {}
    /** destructor */
    ~Gaussian_sjk() {}
    /** Initialize the component of the model. */
    void initializeModelImpl()
    {
      paramBuffer_.resize(2*this->nbCluster(), p_data()->cols());
      paramBuffer_ = 0.;
    }
    /** Compute the initial weighted means and the initial weighted standard deviations
     *  of the mixture */
    inline bool initializeStep() { return mStep();}
    /** Initialize randomly the parameters of the Gaussian mixture. The centers
     *  will be selected randomly among the data set and the standard-deviation
     *  will be set to 1.
     */
    void randomInit();
    /** Compute the weighted mean and the common standard deviation. */
    bool mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return 2*this->nbCluster()*this->nbVariable();}
    /** set the parameters of the model */
    void setParametersImpl();
};

/* set the parameters of the model */
template<class Array>
void Gaussian_sjk<Array>::setParametersImpl()
{
  for (int k= 0; k < this->nbCluster(); ++k)
  {
    for (int j= p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {
      p_param(baseIdx+k)->mean_[j]  = paramBuffer_(baseIdx+2*k  , j);
      p_param(baseIdx+k)->sigma_[j] = paramBuffer_(baseIdx+2*k+1, j);
    }
  }
}

/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Gaussian_sjk<Array>::randomInit()
{
  this->randomMean();
  // compute the standard deviation
  for (int k= baseIdx; k < components().end(); ++k)
  {
    p_param(k)->sigma_ = Stat::varianceWithFixedMean(*p_data(), p_tik()->col(k), p_param(k)->mean_, false).sqrt();
  }
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Gaussian_sjk<Array>::randomInit() done\n");
#endif
}

/* Compute the weighted means and the weighted standard deviations. */
template<class Array>
bool Gaussian_sjk<Array>::mStep()
{
  // compute the means
  if (!this->updateMean()) return false;
  // compute the standard deviation
  for (int k= baseIdx; k < components().end(); ++k)
  {
    p_param(k)->sigma_ = Stat::varianceWithFixedMean(*p_data(), p_tik()->col(k), p_param(k)->mean_, false).sqrt();
    if (p_param(k)->sigma_.nbAvailableValues() != p_param(k)->sigma_.size()) return false;
  }
  return true;
}

} // namespace STK

#endif /* STK_GAUSSIAN_SJK_H */
