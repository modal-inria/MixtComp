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
 * Project:  stkpp::STatistiK::Law
 * created on: 29 juil. 2011
 * Purpose:  define the templated MultiNormal law.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Law_MultiNormal.h
 *  @brief In this file we define the templated MultiNormal law.
 **/

#ifndef STK_LAW_MULTINORMAL_H
#define STK_LAW_MULTINORMAL_H


#include "Arrays/include/STK_CArray.h"
#include "Arrays/include/STK_CArraySquare.h"
#include "Arrays/include/STK_CArrayPoint.h"
#include "Arrays/include/STK_CArrayVector.h"
#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DSquare.h"
#include "Arrays/include/STK_Array2DDiagonal.h"
#include "Arrays/include/STK_Array2DLowerTriangular.h"
#include "Arrays/include/STK_Array2DUpperTriangular.h"
#include "Arrays/include/STK_Array2D_Functors.h"
#include "Arrays/include/STK_Const_Arrays.h"

#include "Algebra/include/STK_SymEigen.h"

#include "STK_Law_IMultiLaw.h"
#include "STK_Law_Normal.h"

#include "Analysis/include/STK_Const_Math.h"

namespace STK
{

namespace Law
{

/** @ingroup Laws
 *  @brief  Class for the multivariate Normal distribution.
 *
 *  In probability theory and statistics, the "multivariate normal distribution"
 *  or "multivariate Gaussian distribution", is a generalization of the
 *  one-dimensional (univariate) @link Normal normal distribution @endlink to
 *  higher dimensions.  A random vector is said to be multivariate normally
 *  distributed if every linear combination of its components has a univariate
 *  normal distribution.  The multivariate normal distribution is often used to
 *  describe, at least approximately, any set of (possibly) correlated
 *  real-valued random variables each of which clusters around a mean value.
 *
 *  The multivariate normal distribution of a \f$ p\f$-dimensional random vector
 *  \f[
 *  \mathbf{X} = \left(
 *   X_1, X_2, \ldots, X_p
 *               \right)'
 *  \f]
 *  can be written in the following notation
 *  \f[
 *  \mathbf{X}\ \sim\ \mathcal{N}(\mu,\ \Sigma).
 *  \f]
 *  with \f$ p \f$-dimensional mean vector
 *  \f[
 *  \mu = \left(
 *        \mathrm{E}[X_1], \mathrm{E}[X_2], \ldots, \mathrm{E}[X_k]
 *        \right)'
 *  \f]
 *  and \f$ p \times p \f$ covariance matrix
 *  \f[
 *  \Sigma = [\mathrm{Cov}(X_i, X_j)]_{i=1,2,\ldots,p;\ j=1,2,\ldots,p}
 *  \f]
 */
template <class RowVector>
class MultiNormal: public IMultiLaw<RowVector>
{
  public:
    /** Constructor.
     *  @param mu mean of the Normal distribution
     *  @param sigma covariance matrix of the Normal distribution
     **/
    MultiNormal( RowVector const& mu, ArraySquareX const& sigma)
               : IMultiLaw<RowVector>(_T("MultiNormal"))
               , mu_()
               , sigma_()
               , decomp_(sigma)
    { setParameters(mu, sigma);}

    /** destructor. */
    virtual ~MultiNormal() {}
    /** @@return the location parameter */
    inline RowVector const& mu() const { return mu_;}
    /** @@return the variance-covariance matrix */
    inline ArraySquareX const& sigma() const { return sigma_;}
    /** @@return the square root of the variance-covariance matrix */
    inline ArraySquareX const& squareroot() const { return squareroot_;}
    /** @@return the eigenvalue decomposition */
    inline SymEigen const& decomp() const { return decomp_;}
    /** update the parameters specific to the law. */
    void setParameters( RowVector const& mu, ArraySquareX const& sigma)
    {
      // check dimensions
      if (mu.range() != sigma.range())
      { STKRUNTIME_ERROR_NO_ARG(MultiNormal::setParameters(mu, sigma),mu.range() != sigma.range());}
      mu_    = mu;
      sigma_ = sigma;
      // decomposition of the covariance matrix
      decomp_.setData(sigma_);
      if (!decomp_.run())
      { throw runtime_error(decomp_.error());}
      // compute the inverse of the eigenvalues of sigma_ and the squareroot_
      // matrix needed by rand
      invEigenvalues_.resize(mu_.range());
      squareroot_.resize(mu_.range());
      // get dimension
      int rank = decomp_.rank(), end = mu_.begin() + rank;
      for (int j=mu_.begin(); j< end; j++)
      { invEigenvalues_[j] = 1./decomp_.eigenValues()[j];}
      for (int j=end; j< mu_.end(); j++) { invEigenvalues_[j] = 0.;}

      squareroot_ = decomp_.rotation() * decomp_.eigenValues().sqrt().diag() * decomp_.rotation().transpose();
    }
    /** @brief compute the probability distribution function (density) of the
     * multivariate normal law
     * \f[
     *  f(x) = \frac{1}{ (2\pi)^{k/2}|\Sigma|^{1/2} }
     *           \exp\!\left( {-\tfrac{1}{2}}(x-\mu)'\Sigma^{-1}(x-\mu) \right),
     * \f]
     *  Give the value of the pdf at the point x.
     *  @param x the multivariate value to compute the pdf.
     *  @return the value of the pdf
     **/
    virtual Real pdf( RowVector const& x) const
    {
      // check determinant is not 0
      if (decomp_.det() == 0.)
      { STKRUNTIME_ERROR_NO_ARG(MultiNormal::pdf(x),|sigma| == 0.);}
      // check ranges
      if (x.range() != mu_.range() )
      { STKRUNTIME_ERROR_NO_ARG(MultiNormal::pdf(x),x.range() != mu_.range());}
      // compute pdf in log-space
      return std::exp((double)lpdf(x));
    }

    /** @brief compute the log probability distribution function.
     *  Give the value of the log-pdf at the point x.
     *  @param x the multivariate value to compute the lpdf.
     *  @return the value of the log-pdf
     **/
    Real lpdf( RowVector const& x) const
    {
      // check ranges
      if (x.range() != mu_.range() )
      { STKRUNTIME_ERROR_NO_ARG(MultiNormal::lpdf(x),x.range() != mu_.range());}
      // compute pdf using ||(x-mu)'P||_{D^{-1}}^2
      Real res = 0.5 * ((x - mu_) * decomp_.rotation()).wnorm2(invEigenvalues_)
               + invEigenvalues_.size() * Const::_LNSQRT2PI_
               + 0.5 * log((double)decomp_.det());
      return -res;
    }

    /** @brief compute the log likehood of a data set.
     *  sum the values of the log-pdf at the points stored in x.
     *  @param data the multivariate values to compute the lpdf.
     *  @return the value of the log-pdf
     **/
    template < class Array>
    Real lnLikelihood( Array const& data) const
    {
      // check ranges
      if (data.cols() != mu_.range() )
      { STKRUNTIME_ERROR_NO_ARG(MultiNormal::lnLikelihood(x),data.cols() != mu_.range());}
      // get dimensions of the samples and sum over all ln-likelihood values
      const int first = data.beginRows(), last = data.lastIdxRows();
      Real sum = 0.0;
      for (int i=first; i<= last; i++)
      {
        // compute lpdf using \sum_i ||(x_i-mu)'P||_{D^{-1}}^2
        Real res = 0.5 * ((data.row(i) - mu_) * decomp_.rotation()).wnorm2(invEigenvalues_) ;
        sum += res;
      }
      sum += data.sizeRows()*( invEigenvalues_.size() * Const::_LNSQRT2PI_
                             + 0.5 * log((double)decomp_.det())
                             );
      return -sum;
    }

    /** @brief simulate a realization of the Multivariate Law and
     *  store the result in x.
     *  @param[out] x the simulated value.
     **/
    virtual void rand( RowVector& x) const
    {
      // fill it with iid N(0,1) variates
      Normal(0., 1.).randArray(x);
      // rotate with squareroot_ and translate with mu_
      x = x * squareroot_ + mu_;
    }
    /** @brief simulate a realization of the Multivariate Law and store the
     *  result in x (using a reference vector). The class RowVector have
     *  to derive from IContainerRef.
     *  @param[out] x the simulated value.
     **/
    template < class Array>
    void rand( ArrayBase<Array>& x) const
    {
      // fill it with iid N(0,1) variates
      Normal(0., 1.).randArray(x.asDerived());
      // rotate with squareroot_ and translate with mu_
      x.asDerived() = x.asDerived() * squareroot_ + Const::Vector<Real>(x.rows()) * mu_;
    }

  protected:
    /** The position parameter. **/
    RowVector mu_;
    /** The covariance parameter. **/
    ArraySquareX sigma_;
    /** the decomposition in eigenvalues of the covariance matrix*/
    SymEigen decomp_;
    /** inverse of the eigenvalues of sigma_ */
    ArrayDiagonalX invEigenvalues_;
    /** The square root of the matrix @c Sigma_. **/
    ArraySquareX squareroot_;
};

} // namespace Law

} // namespace STK

#endif /* STK_LAW_MULTINORMAL_H */
