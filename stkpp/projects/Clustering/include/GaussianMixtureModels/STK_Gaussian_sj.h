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
struct MixtureModelTraits< Gaussian_sj<_Array> >
{
  typedef _Array Array;
  typedef Gaussian_sj_Parameters Parameters;
  typedef MixtureComponent<_Array, Parameters> Component;
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
    typedef typename Clust::MixtureModelTraits< Gaussian_sj<Array> >::Component Component;
    typedef typename Clust::MixtureModelTraits< Gaussian_sj<Array> >::Parameters Parameters;

    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

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
     *  This function have to be called prior to any used of the class.
     *  In this interface, the @c initializeModel() method call the base
     *  class IMixtureModel::initializeModel() and for all the
     *  components initialize the shared parameter sigma_.
     **/
    void initializeModel()
    {
      Base::initializeModel();
      sigma_.resize(this->nbVariable());
      sigma_ = 1.;
      for (int k= baseIdx; k <= components().lastIdx(); ++k)
      { p_param(k)->p_sigma_ = &sigma_;}
    }
    /** Compute the inital weighted mean and the initial common variance. */
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
    { return this->nbCluster()*this->nbVariable()+this->nbVariable();}

  protected:
    Array2DPoint<Real> sigma_;
};

/* Initialize the parameters using mStep. */
template<class Array>
void Gaussian_sj<Array>::initializeStep()
{ this->initialMean();
  sigma_ = 1.;
}

/* Initialize randomly the parameters of the Gaussian mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Gaussian_sj<Array>::randomInit()
{
    this->randomMean();
  sigma_ = 1.;
}

/* Compute the weighted mean and the common variance. */
template<class Array>
void Gaussian_sj<Array>::mStep()
{
  // compute the means
  this->updateMean();
  Array2DPoint<Real> variance(p_data()->cols(), 0.);
  for (int k= baseIdx; k <= components().lastIdx(); ++k)
  {
    variance += p_tik()->col(k).transpose()
               *(*p_data() - (Const::Vector<Real>(p_data()->rows()) * p_param(k)->mean_)
                ).square()
                ;
  }
  if (variance.nbAvailableValues() != this->nbVariable()) throw Clust::mStepFail_;
  if ((variance > 0.).template cast<int>().sum() != this->nbVariable()) throw Clust::mStepFail_;
  // compute the standard deviation
  sigma_ = (variance /= this->nbSample()).sqrt();
}

} // namespace STK

#endif /* STK_GAUSSIAN_SJ_H */
