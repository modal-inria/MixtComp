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

/** @file STK_Gaussian_sj.h
 *  @brief In this file we define and implement the Gaussian_sj class
 **/

#ifndef STK_GAUSSIAN_SJ_H
#define STK_GAUSSIAN_SJ_H

#include "STK_DiagGaussianBase.h"

namespace STK
{

//forward declaration, to allow for recursive template
template<class Array>class Gaussian_sj;

namespace Clust
{
/** @ingroup hidden
 *  Traits class for the Gaussian_s traits policy. */
template<class _Array>
struct MixtureTraits< Gaussian_sj<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Gaussian_sj_Parameters Parameters;
  typedef Array2D<Real>        Param;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Gaussian mixture model Gaussian_sj have a density function of the
 *  form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma_j} \exp\left\{-\frac{(x^j-\mu^j_k)^2}{2\sigma_j^2}\right\}.
 * \f]
 **/
template<class Array>
class Gaussian_sj : public DiagGaussianBase<Gaussian_sj<Array> >
{
  public:
    typedef DiagGaussianBase<Gaussian_sj<Array> > Base;
    typedef typename Clust::MixtureTraits< Gaussian_sj<Array> >::Parameters Parameters;

    using Base::p_tik;
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline Gaussian_sj( int nbCluster) : Base(nbCluster), sigma_() {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Gaussian_sj( Gaussian_sj const& model): Base(model), sigma_(model.sigma_) {}
    /** destructor */
    inline ~Gaussian_sj() {}
    /** Initialize the component of the model.
     *  This function initialize the shared parameter sigma_  for all the
     *  components.
     **/
    void initializeModelImpl()
    {
      sigma_.resize(this->nbVariable());
      sigma_ = 1.;
      for (int k= baseIdx; k < components().end(); ++k)
      { p_param(k)->p_sigma_ = &sigma_;}
      paramBuffer_.resize(2*this->nbCluster(), p_data()->cols());
      paramBuffer_ = 0.;
    }
    /** Compute the inital weighted mean and the initial common standard deviation. */
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
    { return this->nbCluster()*this->nbVariable()+this->nbVariable();}
    /** set the parameters of the model*/
    void setParametersImpl();

  protected:
    /** Common standard deviation */
    Array2DPoint<Real> sigma_;
};

/* set the parameters of the model */
template<class Array>
void Gaussian_sj<Array>::setParametersImpl()
{
  for (int j= p_data()->beginCols(); j < p_data()->endCols(); ++j)
  { sigma_[j] = paramBuffer_(baseIdx+1, j);}
  for (int k= 0; k < this->nbCluster(); ++k)
  {
    for (int j= p_data()->beginCols(); j < p_data()->endCols(); ++j)
    { p_param(baseIdx+k)->mean_[j] = paramBuffer_(baseIdx+2*k, j);}
  }
}

/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Gaussian_sj<Array>::randomInit()
{
  // compute the initial mean
  this->randomMean();
  // compute the standard deviation
  Array2DPoint<Real> variance(p_data()->cols(), 0.);
  for (int k= baseIdx; k < components().end(); ++k)
  {
    variance += p_tik()->col(k).transpose()
               *(*p_data() - (Const::Vector<Real>(p_data()->rows()) * p_param(k)->mean_)
                ).square()
                ;
  }
  // compute the standard deviation
  sigma_ = (variance /= this->nbSample()).sqrt();
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Gaussian_sj<Array>::randomInit() done\n");
#endif
}

/* Compute the weighted mean and the common standard deviation. */
template<class Array>
bool Gaussian_sj<Array>::mStep()
{
  // compute the means
  if (!this->updateMean()) return false;
  // compute the standard deviation
  Array2DPoint<Real> variance(p_data()->cols(), 0.);
  for (int k= baseIdx; k < components().end(); ++k)
  {
    variance += p_tik()->col(k).transpose()
               *(*p_data() - (Const::Vector<Real>(p_data()->rows()) * p_param(k)->mean_)
                ).square()
                ;
  }
  if (variance.nbAvailableValues() != this->nbVariable()) return false;
  if ((variance > 0.).template cast<int>().sum() != this->nbVariable()) return false;
  // compute the standard deviation
  sigma_ = (variance /= this->nbSample()).sqrt();
  return true;
}

} // namespace STK

#endif /* STK_GAUSSIAN_SJ_H */
