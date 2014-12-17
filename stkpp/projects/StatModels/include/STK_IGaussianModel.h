/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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

 Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/*
 * Project:  stkpp::Model
 * created on: 13 août 2011
 * Purpose:  Create a gaussian statistical model.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IGaussianModel.h
 *  @brief In this file we define the Interface class IGaussianModel.
 **/

#ifndef STK_IGAUSSIANMODEL_H
#define STK_IGAUSSIANMODEL_H

#include "STK_IStatModel.h"

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DSquare.h"
#include "Arrays/include/STK_Array2DVector.h"
#include "Arrays/include/STK_Array2DPoint.h"

#include "STatistiK/include/STK_Stat_Functors.h"


namespace STK
{
/**
 * Compute the gaussian log likehood of a one dimensionnal gaussian model.
 * @param data the data set
 * @param mu the mean of the gaussian law
 * @param sigma the variance of the gaussian law
 * @return
 */
Real univariateGaussianLnLikelihood(Vector const& data, Real const& mu, Real const& sigma);

/** Compute the gaussian log likelihood of a diagonal gaussian model.
 *  @param data the data set
 *  @param mu the mean of the gaussian law
 *  @param sigma the (diagonal) covairance matrix (only the diagonal is used)
 *  @return
 */
Real diagonalGaussianLnLikelihood(ArrayXX const& data, Point const& mu, ArraySquareX const& sigma);

/** @ingroup StatModels
 *  @brief Compute the the maximum likelihood estimates of a complete Gaussian
 *  statistical model.
 *
 *  A random vector \f$ X \in; \mathbb{R}^p \f$ is Gaussian if
 *  \f[
 *  X\ \sim\ \mathcal{N}(\mu,\ \Sigma).
 *  \f]
 *
 * The likelihood function of a gaussian sample is:
 * \f[
 *  L(\mu,\Sigma)=[\mbox{constant}]
 *               \prod_{i=1}^n \det(\Sigma)^{-1/2}
 *               \exp\left(-\frac{1}{2} (X_i-\mu)^T \Sigma^{-1} (X_i-\mu)\right)
 * \f]
 *
 * The maximum likelihood estimate can be performed via matrix calculus formulae
 * Re-write the likelihood in the log form using the trace trick:
 * \f[
 * \ln L(\mu,\Sigma) = \operatorname{const} -{n \over 2} \ln \det(\Sigma)
 * -\frac{1}{2} \operatorname{tr} \left[ \Sigma^{-1} \sum_{i=1}^n (X_i-\mu) (X_i-\mu)^T \right].
 * \f]
 *
 * The differential of this ln-likelihood is
 * \f[
 *  d \ln L(\mu,\Sigma) = -{n \over 2} \operatorname{tr} \left[ \Sigma^{-1} \left\{ d \Sigma \right\} \right]
 * -\frac{1}{2} \operatorname{tr} \left[ - \Sigma^{-1} \{ d \Sigma \} \Sigma^{-1}
 *  \sum_{i=1}^n (X_i-\mu)(X_i-\mu)^T - 2 \Sigma^{-1} \sum_{i=1}^n (X_i - \mu) \{ d \mu \}^T \right].
 * \f]
 * It naturally breaks down into the part related to the estimation of the mean,
 * and to the part related to the estimation of the variance. The first order
 * condition for maximum, \f$d \ln L(\mu,\Sigma)=0\f$, is satisfied when the
 * terms multiplying \f$ d \mu\f$ and \f$ d \Sigma\f$ are identically zero.
 * Assuming (the maximum likelihood estimate of) \f$\Sigma\f$ is non-singular,
 * the first order condition for the estimate of the mean vector is
 * \f[
 *  \sum_{i=1}^n (X_i - \mu) = 0,
 * \f]
 * which leads to the maximum likelihood estimate
 * \f[
 *  \hat{\mu} = \bar{X} = \frac{1}{n} \sum_{i=1}^n X_i.
 * \f]
 * This lets us simplify
 * \f$ \sum_{i=1}^n (X_i-\mu)(X_i-\mu)^T = \sum_{i=1}^n (X_i-\bar{X})(X_i-\bar{X})^T = S\f$.
 * Then the terms involving \f$ d \Sigma\f$ in \f$ d \ln L\f$
 *  can be combined as
 *  \f[
 *    -\frac{1}{2} \operatorname{tr} \left(
 *    \Sigma^{-1} \left\{ d \Sigma \right\} \left[ nI_p - \Sigma^{-1} S \right]
 *                                   \right).
 *  \f]
 *  The first order condition \f$ d \ln L(\mu,\Sigma)=0\f$ will hold when the
 *  term in the square bracket is (matrix-valued) zero. Pre-multiplying the
 *  latter by \f$\Sigma\f$ and dividing by \f$n\f$ gives
 *  \f$\hat{\Sigma} = \frac{1}{n} S,\f$
 *
 *  The IGaussianModel class is an interface class that can be derived in order
 *  to impose various constraint on the covariance matrix.
 **/
template <class Array>
class IGaussianModel : public IStatModel<Array>
{
  typedef IStatModel<Array> Base;
  typedef typename Array::Row RowVector;
  typedef typename Array::Col ColVector;

  protected:
    /** constructor.
     * @param p_data pointer on the data set
     **/
    IGaussianModel( Array const* p_data) : Base(p_data) {}
    /** constructor.
     * @param data reference on the data set
     **/
    IGaussianModel( Array const& data) : Base(data) {}
  public:
    /** destructor. */
    virtual ~IGaussianModel() {}
    /** @return the empirical mean */
    inline RowVector const& mean() const { return mean_;}

  protected:
    /** Vector of the empirical means */
    RowVector mean_;
    /** compute the empirical mean */
    inline void compMean() { mean_ = Stat::mean(*this->p_data_);}
    /** compute the empirical weighted mean
     *  @param weights the weights of the samples
     **/
    inline void compWeightedMean(ColVector const& weights)
    { mean_ = Stat::mean(*(this->p_data_), weights);}
    /** compute the empirical covariance matrix. */
    virtual void compCovariance() =0;
    /** compute the empirical weighted covariance matrix.
     *  @param weights the weights of the samples
     **/
    virtual void compWeightedCovariance(ColVector const& weights) =0;
};

} // namespace STK

#endif /* STK_IGAUSSIANMODEL_H */
