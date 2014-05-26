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
struct MixtureModelTraits< Gaussian_sk<_Array> >
{
  typedef _Array Array;
  typedef Gaussian_sk_Parameters Parameters;
  typedef MixtureComponent<_Array, Parameters> Component;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Gaussian mixture model @c Gaussian_sk is
 *  the most general diagonal Gaussian model and have a density function of the
 *  form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma^j_{k}} \exp\left\{-\frac{(x^j-\mu^j_k)^2}{2(\sigma^j_{k})^2}\right\}.
 * \f]
 **/
template<class Array>
class Gaussian_sk : public DiagGaussianBase<Gaussian_sk<Array> >
{
  public:
    typedef DiagGaussianBase<Gaussian_sk<Array> > Base;
    typedef typename Clust::MixtureModelTraits< Gaussian_sk<Array> >::Component Component;
    typedef typename Clust::MixtureModelTraits< Gaussian_sk<Array> >::Parameters Parameters;
    typedef typename Array::Col ColVector;

    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

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
    /** Compute the inital weighted mean and the initial common variances. */
    void initializeStep();
    /** Initialize randomly the parameters of the Gaussian mixture. The centers
     *  will be selected randomly among the data set and the standard-deviations
     *  will be set to 1.
     */
    void randomInit();
    /** Compute the weighted mean and the common variance. */
    void mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*this->nbVariable() + this->nbCluster();}
};

/** Initialize the parameters using mStep. */
template<class Array>
void Gaussian_sk<Array>::initializeStep()
{ this->initialMean();
  for (int k= baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    p_param(k)->sigma_
    = std::sqrt( (p_tik()->col(k).transpose()
                *(*p_data() - (Const::Vector<Real>(p_data()->rows()) * p_param(k)->mean_)).square()).sum()
        /(this->nbVariable()*p_tik()->col(k).sum())
       );
    if (p_param(k)->sigma_ <= 0.) throw Clust::initializeStepFail_;
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
  for (int k= baseIdx; k <= components().lastIdx(); ++k)
  { p_param(k)->sigma_ = 1.;}
}

/* Compute the weighted mean and the common variance. */
template<class Array>
void Gaussian_sk<Array>::mStep()
{
  // compute the means
  this->updateMean();
  for (int k= baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    p_param(k)->sigma_
    = sqrt( ( p_tik()->col(k).transpose()
             *(*p_data() - (Const::Vector<Real>(p_data()->rows()) * p_param(k)->mean_)
              ).square()
            ).sum()
           /(p_data()->sizeCols()*p_tik()->col(k).sum())
          );
    if (p_param(k)->sigma_ <= 0.) throw Clust::mStepFail_;
  }
}

} // namespace STK

#endif /* STK_GAUSSIAN_SJK_H */
