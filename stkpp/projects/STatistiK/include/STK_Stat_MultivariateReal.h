/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Project:  StatDesc
 * Purpose:  Compute multivariate elementary statistics for
 * a 2D container.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Stat_MultivariateReal.h
 *  @brief In this file we specialize the class Multivariate to real type.
 **/

#ifndef STK_STAT_MULTIVARIATEREAL_H
#define STK_STAT_MULTIVARIATEREAL_H

#include "../../STKernel/include/STK_Arithmetic.h"
#include "../../STKernel/include/STK_Misc.h"

#include "../../Arrays/include/STK_Array2DSquare.h"

#include "STK_Stat_UnivariateReal.h"
#include "STK_Stat_BivariateRealReal.h"
#include "STK_Stat_Multivariate.h"
#include "STK_Stat_Functors.h"

namespace STK
{
namespace Stat
{

typedef Multivariate<Matrix, Real> MultivariateMatrix;

/** @ingroup StatDesc
 *  @brief Computation of the Multivariate Statistics of a 2D Container
 *  of Real.
 *
 *  The class @c Multivariate is just a factory class for computing the mean,
 *  the variance and the covariance square matrix of a p_data set stored in a
 *  @c Array with n rows (the samples) and p columns (the variables).
 **/
template <class Array>
class Multivariate<Array, Real> : public IRunnerUnsupervised< Array, typename Array::Col>
{
  typedef typename Array::Row RowVector;
  typedef typename Array::Col ColVector;
  /** type of runner */
  typedef IRunnerUnsupervised< Array, ColVector> Runner;

  public:
    /** Constructor.
     *  Compute the Multivariate statistics of the Matrix data set.
     *  @param data the data set
     **/
    Multivariate( Array const& data)
                : Runner(&data)
                , nbSamples_(0)
                , nbVar_(0)
                , min_()
                , max_()
                , mean_()
                , var_()
                , cov_()
    { }

    /** Constructor.
     *  Compute the Multivariate statistics of the Matrix p_data set.
     *  @param p_data a pointer on the data set
     **/
    Multivariate( Array const* p_data)
                : Runner(p_data)
                , nbSamples_(0)
                , nbVar_(0)
                , min_()
                , max_()
                , mean_()
                , var_()
                , cov_()
    {}

    /** copy constructor.
     *  @param stat the statistics to copy
     **/
    Multivariate( Multivariate const& stat)
                : Runner(stat)
                , nbSamples_(stat.nbSamples_)
                , nbVar_(stat.nbVar_)
                , min_(stat.min_)
                , max_(stat.max_)
                , mean_(stat.mean_)
                , var_(stat.var_)
                , cov_(stat.cov_)
    {}
    /** virtual destructor.*/
    virtual ~Multivariate() { }

    /** clone pattern */
    inline virtual Multivariate* clone() const { return new Multivariate(*this);}

    /** @return the number of variables in the p_data_ set (the number of columns) */
    inline int const& nbVar() const {return nbVar_;}
    /** @return the number of samples in the p_data_ set (the number of rows) */
    inline int const& nbSamples() const {return nbSamples_;}
    /** @return the minimal values of the variables in a RowVector */
    inline RowVector const& min() const { return min_;}
    /** @return the maximal values of the variables in a RowVector  */
    inline RowVector const& max() const { return max_;}
    /** @return the mean of the variables in a RowVector */
    inline RowVector const& mean() const { return mean_;}
    /**@return the variance of the variables in a RowVector */
    inline RowVector const& variance() const { return var_;}
    /** @return the covariance of the variables in a square matrix */
    inline MatrixSquare const& covariance() const { return cov_;}

    /** run the estimation of the Multivariate statistics. **/
    virtual bool run()
    {
      if (!this->p_data_)
      { this->msg_error_ = STKERROR_NO_ARG(MultivariateMatrix::run(),data have not be set);
        return false;
      }
      try
      {
        resize(this->p_data_->cols());
        // for each variables
        for (int j=this->p_data_->firstIdxCols(); j<=this->p_data_->lastIdxCols(); j++)
        {
          mean_[j] = Stat::mean(this->p_data_->col(j));
          min_[j] = Stat::min(this->p_data_->col(j));
          max_[j] = Stat::max(this->p_data_->col(j));
          var_[j]  = varianceWithFixedMean(this->p_data_->col(j), mean_[j]);
          cov_(j, j) = var_[j];
          for (int i=this->p_data_->firstIdxCols(); i<j; i++)
          {
            cov_(i, j) = covarianceWithFixedMean(this->p_data_->col(i), this->p_data_->col(j), mean_[i], mean_[j]);
            cov_(j, i) = cov_(i, j);
          }
        }
      }
      catch (Exception const& error)
      {
        this->msg_error_ += _T("Error in MultivariateMatrix::run():\nWhat: ");
        this->msg_error_ += error.error();
        return false;
      }
      // no error
      return true;
    }
    /** run the estimation of the weighted multivariate statistics.
     * @param weights the weights of the samples
     **/
    virtual bool run( ColVector const& weights)
    {
      if (!this->p_data_)
      { this->msg_error_ = STKERROR_NO_ARG(MultivariateMatrix::run(weights),data have not be set);
        return false;
      }
      if (this->p_data_->rows() != weights.rows())
      { this->msg_error_ = STKERROR_NO_ARG(MultivariateMatrix::run(weights),data and weights does not have not the same size);
        return false;
      }
      try
      {
        resize(this->p_data_->cols());
        // for each variables
        for (int j= this->p_data_->firstIdxCols(); j<= this->p_data_->lastIdxCols(); j++)
        {
          mean_[j] = Stat::mean(this->p_data_->col(j), weights);
          var_[j]  = varianceWithFixedMean(this->p_data_->col(j), weights, mean_[j]);
          cov_(j, j) = var_[j];
          // compute the covariances
          for (int i= this->p_data_->firstIdxCols(); i<j; i++)
          {
            cov_(i, j) = covarianceWithFixedMean(this->p_data_->col(i), this->p_data_->col(j), weights, mean_[i], mean_[j]);
            cov_(j, i) = cov_(i, j);
          }
        }
      }
      catch (Exception const& error)
      {
        this->msg_error_  = _T("Error in MultivariateMatrix::run(weights): ");
        this->msg_error_ += error.error();
        return false;
      }
      // no error
      return true;
    }

  protected:
    /** number of samples */
    int nbSamples_;
    /** Number of variables */
    int nbVar_;

    /** Vector of the mean of the Variables */
    RowVector min_;
    /** Vector of the mean of the Variables */
    RowVector max_;
    /** Vector of the mean of the Variables */
    RowVector mean_;
    /** Vector of the variance of the variables */
    RowVector var_;
    /** Matrix of the covariance of the variables */
    MatrixSquare cov_;

    /** udpating method in case we set a new data set */
    virtual void update()
    {
      // if there is no data there is nothing to update
      if (this->p_data_)
      {
        nbSamples_ = this->p_data_->sizeRows();
        nbVar_     = this->p_data_->sizeCols();
        resize(Range());
      }
    }

  private:
    /** @param J range of the statistics  */
    void resize(Range const& J)
    {
      min_.resize(J);
      max_.resize(J);
      mean_.resize(J);
      var_.resize(J);
      cov_.resize(J);
    }
};

/**  @ingroup StatDesc
 *  Compute the covariance of the data set V.
 *  \f[ \hat{\sigma}^2 = \frac{1}{n}
 *                       \sum_{i=1}^n (V_i-\hat{\mu}) (V_i-\hat{\mu})'.
 *  \f]
 *  @param V variable
 *  @param cov the computed covariance
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template < class Array >
void covariance( Array const& V, MatrixSquare & cov, bool unbiased = false)
{
  typename Array::Row mean;
  // compute the mean
  mean = Stat::mean(V);
  // get dimensions
  const int firstVar = V.firstIdxCols(), lastVar = V.lastIdxCols();
  cov.resize(V.cols());
  for (int j= firstVar; j<= lastVar; j++)
  {
    cov(j, j) = varianceWithFixedMean(V.col(j), mean[j], unbiased);
    for (int i= firstVar; i<j; i++)
    { cov(j,i) = ( cov(i, j) = covarianceWithFixedMean(V.col(i), V.col(j), mean[i], mean[j], unbiased));}
  }
}

/**  @ingroup StatDesc
 *  Compute the weighted covariance of the data set V.
 *  \f[ \hat{\sigma}^2 = \frac{1}{n}
 *                       \sum_{i=1}^n  w_{i} w_j (V_i-\hat{\mu}) (V_i-\hat{\mu})'.
 *  \f]
 *  @param V the variable
 *  @param W the weights
 *  @param cov the computed covariance
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template <class Array, class WColVector >
void covariance( Array const& V, WColVector const& W, MatrixSquare & cov, bool unbiased = false)
{
  typedef typename Array::Row RowVector;
  typedef typename Array::Col ColVector;
  RowVector mean;
  // compute the
  mean = Stat::mean(V, W);
  // get dimensions
  const int firstVar = V.firstIdxCols(), lastVar = V.lastIdxCols();
  cov.resize(V.cols());
  for (int j= firstVar; j<= lastVar; j++)
  {
    cov(j, j) = varianceWithFixedMean<ColVector, WColVector>(V.col(j), W, mean[j], unbiased);
    for (int i= firstVar; i<j; i++)
    { cov(j,i) = ( cov(i, j) = covarianceWithFixedMean(V.col(i), V.col(j), W, mean[i], mean[j], unbiased));}
  }
}


}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_MULTIVARIATEREAL_H*/
