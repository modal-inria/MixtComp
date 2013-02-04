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

 Contact : Serge.Iovleff@stkpp.org
 */

/*
 * Project:  stkpp::STatistiK::Law
 * created on: 29 juil. 2011
 * Purpose:  define the templated MultiNormal law.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_Law_MultiNormal.h
 *  @brief In this file we define the templated MultiNormal law.
 **/

#ifndef STK_MULTINORMAL_H
#define STK_MULTINORMAL_H

#include "STK_Law_IMultiLaw.h"
#include "STK_Law_Normal.h"

#include "../include/STK_Const_Math.h"

#include "../../Arrays/include/STK_Array2D.h"
#include "../../Arrays/include/STK_Array2DSquare.h"
#include "../../Arrays/include/STK_Array2DLowerTriangular.h"
#include "../../Arrays/include/STK_Array2DUpperTriangular.h"
#include "../../Arrays/include/STK_Array2DFunctors.h"

#include "../../Algebra/include/STK_EigenvaluesSymmetric.h"
#include "../../Algebra/include/STK_LinAlgebra1D.h"
#include "../../Algebra/include/STK_LinAlgebra2D.h"

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
template <class Container1D>
class MultiNormal: public IMultiLaw<Container1D>
{
  public:
    /** Constructor.
     *  @param mu mean of the Normal distribution
     *  @param sigma covariance matrix of the Normal distribution
     **/
    MultiNormal( Container1D const& mu, MatrixSquare const& sigma)
               : IMultiLaw<Container1D>(_T("MultiNormal"))
               , mu_()
               , sigma_()
               , decomp_(0)
    { setParameters(mu, sigma);}

    /** destructor. */
    virtual ~MultiNormal() {}
    /** @@return the location parameter */
    inline Container1D const& mu() const { return mu_;}
    /** @@return the variance-covariance matrix */
    inline MatrixSquare const& sigma() const { return sigma_;}
    /** @@return the square root of the variance-covariance matrix */
    inline MatrixSquare const& squareroot() const { return squareroot_;}
    /** @@return the eigenvalue decomposition */
    inline EigenvaluesSymmetric const& decomp() const { return decomp_;}
    /** update the parameters specific to the law. */
    void setParameters( Container1D const& mu, MatrixSquare const& sigma)
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
      int rank = decomp_.rank(), last = mu_.firstIdx() + rank -1;
      for (int j=mu_.firstIdx(); j<= last; j++)
      { invEigenvalues_[j] = 1./decomp_.eigenvalues()[j];}
      for (int j=last+1; j<= mu_.lastIdx(); j++)
      { invEigenvalues_[j] = 0.;}

      for (int j=mu_.firstIdx(); j<= mu_.lastIdx(); ++j)
      { squareroot_.row(j) = decomp_.rotation().col(j) * decomp_.eigenvalues().sqrt()[j]; }
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
    virtual Real pdf( Container1D const& x) const
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
    Real lpdf( Container1D const& x) const
    {
      // check ranges
      if (x.range() != mu_.range() )
      { STKRUNTIME_ERROR_NO_ARG(MultiNormal::lpdf(x),x.range() != mu_.range());}
      // compute x - mu
      Container1D xbar; //= x - mu_;
      xbar = x - mu_;
      // compute P'(x-mu)P
      Container1D xrot;
      xrot.move(mult(xbar, decomp_.rotation()));
      // compute pdf using (x-mu)'PD^{-1}P'(x-mu)
      Real res = 0.5 * weightedNormTwo2(xrot, invEigenvalues_)
               + invEigenvalues_.size() * Const::_LNSQRT2PI_
               + 0.5 * log((double)decomp_.det());
      return -res;
    }

    /** @brief compute the log likehood of a data set.
     *  sum the values of the log-pdf at the points stored in x.
     *  @param data the multivariate values to compute the lpdf.
     *  @return the value of the log-pdf
     **/
    template < class TContainer2D>
    Real lnLikelihood( TContainer2D const& data) const
    {
      // check ranges
      if (data.cols() != mu_.range() )
      { STKRUNTIME_ERROR_NO_ARG(MultiNormal::logLikelihood(x),data.cols() != mu_.range());}
      // compute x - mu
      Point xres(mu_.range()), xrot(mu_.range());
      // get dimensions of the samples and sum over all ln-likelihood values
      const int first = data.firstIdxRows(), last = data.lastIdxRows();
      Real sum = 0.0;
      for (int i=first; i<= last; i++)
      {
        // compute residual
        xres = data.row(i) - mu_;
        // compute xrot <- (xre-mu)*P and store result in xrot
        xrot.move(mult(xres, decomp_.rotation()));
        // compute lpdf using (x-mu)'P D^{-1} P'(x-mu)
        Real res = 0.5 * weightedNormTwo2(xrot, invEigenvalues_);
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
    virtual void rand( Container1D& x) const
    {
      // fill it with iid N(0,1) variates
      Normal(0., 1.).randArray(x);
      // rotate with squareroot_
      x.move(mult(x, squareroot_));
      // translate with mu_
      x += mu_;
    }
    /** @brief simulate a realization of the Multivariate Law and store the
     *  result in x (using a reference vector). The class Container1D have
     *  to derive from IContainerRef.
     *  @param[out] x the simulated value.
     **/
    template < class TContainer2D>
    void rand( ArrayBase<TContainer2D>& x) const
    {
      // fill it with iid N(0,1) variates
      Normal(0., 1.).randArray(x.asDerived());
      // rotate with squareroot_
      x.asDerived().move(mult(x.asDerived(), squareroot_));
      // translate with mu_
      for (int i= x.firstIdxRows(); i<= x.lastIdxRows(); i++)
        x.asDerived().row(i) += mu_;
    }

  protected:
    /** The position parameter. **/
    Container1D mu_;
    /** The covariance parameter. **/
    MatrixSquare sigma_;
    /** the decomposition in eigenvalues of the covariance matrix*/
    EigenvaluesSymmetric decomp_;
    /** inverse of the eigenvalues of sigma_ */
    MatrixDiagonal invEigenvalues_;
    /** The square root of the matrix @c Sigma_. **/
    MatrixSquare squareroot_;
};

} // namespace Law

} // namespace STK

#endif /* STK_MULTINORMAL_H */
