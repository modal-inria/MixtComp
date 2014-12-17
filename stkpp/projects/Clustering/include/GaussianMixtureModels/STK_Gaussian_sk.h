/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

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

/** @file STK_Gaussian_sk.h
 *  @brief In this file we define the Gaussian_sk model
 **/

#ifndef STK_GAUSSIAN_SK_H
#define STK_GAUSSIAN_SK_H

#include "STK_DiagGaussianBase.h"

namespace STK
{

//forward declaration, to allow for recursive template
template<class Array>class Gaussian_sk;

namespace Clust
{
/** @ingroup Clustering
 *  Traits class for the Gaussian_sk traits policy. */
template<class _Array>
struct MixtureTraits< Gaussian_sk<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Gaussian_sk_Parameters Parameters;
  typedef Array2D<Real>        Param;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Gaussian mixture model @c Gaussian_sk is
 *  the most general diagonal Gaussian model and have a density function of the
 *  form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma_{k}} \exp\left\{-\frac{(x^j-\mu^j_k)^2}{2(\sigma_{k})^2}\right\}.
 * \f]
 **/
template<class Array>
class Gaussian_sk : public DiagGaussianBase<Gaussian_sk<Array> >
{
  public:
    typedef DiagGaussianBase<Gaussian_sk<Array> > Base;
    typedef typename Clust::MixtureTraits< Gaussian_sk<Array> >::Parameters Parameters;

    using Base::p_tik;
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline Gaussian_sk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Gaussian_sk( Gaussian_sk const& model) : Base(model) {}
    /** destructor */
    inline ~Gaussian_sk() {}
    /** Initialize the component of the model. */
    void initializeModelImpl()
    {
      paramBuffer_.resize(2*this->nbCluster(), p_data()->cols());
      paramBuffer_ = 0.;
    }
    /** Compute the inital weighted mean and the initial common standard deviations. */
    inline bool initializeStep() { return mStep();}
    /** Initialize randomly the parameters of the Gaussian mixture. The centers
     *  will be selected randomly among the data set and the standard-deviations
     *  will be set to 1.
     */
    void randomInit();
    /** Compute the weighted mean and the common standard deviation. */
    bool mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*this->nbVariable() + this->nbCluster();}
    /** set the parameters of the model */
    void setParametersImpl();
};

/* set the parameters of the model */
template<class Array>
void Gaussian_sk<Array>::setParametersImpl()
{
  for (int k= baseIdx; k < this->nbCluster(); ++k)
  {
    for (int j= p_data()->beginCols(); j < p_data()->endCols(); ++j)
    { p_param(baseIdx+k)->mean_[j] = paramBuffer_(baseIdx+2*k, j);}
    p_param(baseIdx+k)->sigma_ = paramBuffer_(baseIdx+2*k+1, p_data()->beginCols());
  }
}
/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Gaussian_sk<Array>::randomInit()
{
  this->randomMean();
  // compute the standard deviation
  Real variance;
  for (int k= baseIdx; k < components().end(); ++k)
  {
    variance = sqrt( ( p_tik()->col(k).transpose()
                     *(*p_data() - (Const::Vector<Real>(p_data()->rows()) * p_param(k)->mean_)
                      ).square()
                     ).sum() / (p_data()->sizeCols()*p_tik()->col(k).sum())
                   );
    p_param(k)->sigma_ = ((variance<=0) || !Arithmetic<Real>::isFinite(variance))
                       ? 1.
                       : std::sqrt(variance/(this->nbSample()*this->nbVariable()));
  }
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Gaussian_sk<Array>::randomInit() done\n");
#endif
}

/* Compute the weighted mean and the common standard deviation. */
template<class Array>
bool Gaussian_sk<Array>::mStep()
{
  // compute the means
  if (!this->updateMean()) return false;
  // compute the standard deviation
  for (int k= baseIdx; k < components().end(); ++k)
  {
    p_param(k)->sigma_
    = sqrt( ( p_tik()->col(k).transpose()
             *(*p_data() - (Const::Vector<Real>(p_data()->rows()) * p_param(k)->mean_)
              ).square()
            ).sum()
           /(p_data()->sizeCols()*p_tik()->col(k).sum())
          );
    if (p_param(k)->sigma_ <= 0.) return false;
  }
  return true;
}

} // namespace STK

#endif /* STK_GAUSSIAN_SJK_H */
