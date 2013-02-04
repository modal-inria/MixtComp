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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  StatDesc
 * Purpose:  Compute multivariate elementary statistics for
 * a 2D container.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Stat_MultivariateMatrix.h
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

namespace STK
{
namespace Stat
{

typedef Multivariate<Matrix> MultivariateMatrix;

/** @ingroup StatDesc
 *  @brief Computation of the Multivariate Statistics of a 2D Container
 *  of Real.
 *
 *  This partial specialization if the class @c Multivariate is just a factory
 *  class for computing the mean, the variance and the covariance Matrix of a
 *  p_data set stored in a @c TContainer2D with n rows (the samples) and
 *  p columns (the variables).
 *
 *  The p_data set will be copied as a reference of the original p_data set.
 *
 *  The p_data set can be weighted with a vector of weight.
 **/
template <>
class Multivariate<Matrix> : public IRunnerConst< Matrix>
{
  typedef typename Matrix::Type Type;
  typedef typename Matrix::Row Row;
  typedef typename Matrix::Col Col;
  /** type of runner */
  typedef IRunnerConst< Matrix> Runner2D;

  public:
    /** Constructor.
     *  Compute the Multivariate statistics of the Matrix data set.
     *  @param data the data set
     **/
    Multivariate( Matrix const& data)
                : Runner2D(&data)
                , nbSamples_(0)
                , nbVar_(0)
                , min_()
                , max_()
                , mean_()
                , var_()
                , cov_()
    { update(); }

    /** Constructor.
     *  Compute the Multivariate statistics of the Matrix p_data set.
     *  @param p_data a pointer on the data set
     **/
    Multivariate( Matrix const* p_data)
                : Runner2D(p_data)
                , nbSamples_(0)
                , nbVar_(0)
                , min_()
                , max_()
                , mean_()
                , var_()
                , cov_()
    { update(); }

    /** virtual destructor.*/
    virtual ~Multivariate() { }

    /** get the number of variables
     * @return the number of variables in the p_data set (the number of columns)
     **/
    inline int const& nbVar() const {return nbVar_;}
    /** get the umber of samples
     *  @return the number of samples in the p_data set (the number of rows)
     **/
    inline int const& nbSamples() const {return nbSamples_;}
    /** get the Vector of the minimal values
     * @return the minimal values of the variables
     **/
    inline Row const& min() const { return min_;}
    /** get the Vector of the maximal values
     * @return the maximal values of the variables
     **/
    inline Row const& max() const { return max_;}
    /** get the Vector of the mean
     * @return the mean of the variables */
    inline Row const& mean() const { return mean_;}
    /** get the vector of the variance of the Variables
     * @return the variance of the variables
     **/
    inline Row const& variance() const { return var_;}
    /** Matrix of the covariance of the variables
     * @return the covariance of the variables
     * */
    inline MatrixSquare const& covariance() const { return cov_;}
    /** run the estimation of the Multivariate statistics. **/
    virtual bool run()
    {
      try
      {
        if (!this->p_data_)
        { STKRUNTIME_ERROR_NO_ARG(MultivariateMatrix::run(),data have not be set);}
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
    virtual bool run( Col const& weights)
    {
      try
      {
        if (!this->p_data_)
        { STKRUNTIME_ERROR_NO_ARG(Stat::MutivariateReal::run(weights),data have not be set);}
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
    Row min_;
    /** Vector of the mean of the Variables */
    Row max_;
    /** Vector of the mean of the Variables */
    Row mean_;
    /** Vector of the variance of the variables */
    Row var_;
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
        min_.resize(this->p_data_->cols());
        max_.resize(this->p_data_->cols());
        mean_.resize(this->p_data_->cols());
        var_.resize(this->p_data_->cols());
        cov_.resize(this->p_data_->cols());
      }
    }
};

/** @ingroup StatDesc
 *  Compute the mean of the container V and store the result in mean
 *  @param V the container with the Data
 *  @param mean the container with the result
 **/
template <class TContainer2D >
void mean( TContainer2D const&  V, typename TContainer2D::Row & mean)
{
  mean.resize(V.cols());
  for (int j=  V.firstIdxCols(); j<= V.lastIdxCols(); j++)
    mean[j] = Stat::mean(V.col(j));
}

/**  @ingroup StatDesc
 *  Compute the (weighted) mean of the variable V
 *  \f[ \hat{\mu} = \frac{1}{\sum_{i=1}^n W(i)}
 *                  \sum_{i=1}^n W(i) V(i)
 *  \f]
 * if there is weights.
 *  @param V the variable
 *  @param W the weights
 *  @param mean the computed weighted mean
 **/
template < class TContainer2D >
void mean( TContainer2D const& V, typename TContainer2D::Col const& W
         , typename TContainer2D::Row & mean
         )
{
  mean.resize(V.cols());
  for (int j= V.firstIdxCols(); j<= V.lastIdxCols(); j++)
    mean[j] = Stat::mean(V.col(j), W);
}

/**  @ingroup StatDesc
 *  Compute the variance of the variable V.
 *  \f[ \hat{\sigma}^2 = \frac{1}{n-1}
 *                       \sum_{i=1}^n (V(i)-\hat{\mu})^2.
 *  \f]
 *  @param V variable
 *  @param var the array with the computed variances
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template < class TContainer2D >
void variance( TContainer2D const& V, typename TContainer2D::Row& var
             , bool unbiased = false
             )
{
  var.resize(V.cols());
  for (int j= V.firstIdxCols(); j<= V.lastIdxCols(); j++)
  { var[j] = Stat::variance(V.col(j), unbiased);}
}

/** @ingroup StatDesc
 *  Compute the weighted variance of the variable V.
 *  \f[ \hat{\sigma}^2 = \frac{\sum_{i=1}^n W(i)}
 *                          {\left( \sum_{i=1}^n W(i))\right)^2
 *                                - \sum_{i=1}^n W(i)^2}
 *                     \sum_{i=1}^n W(i) (V(i)-\hat{\mu})^2.
 *  \f]
 * If there is no weights, this definition reduces to the usual
 * definition of the variance with factor 1/(n-1).
 *  @param V variable
 *  @param W weights
 *  @param var the array with the computed variances
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template < class TContainer2D >
void variance( TContainer2D const& V, typename TContainer2D::Col const& W
             , typename TContainer2D::Row& var
             , bool unbiased = false
             )
{
  var.resize(V.cols());
  // check if there exist a mean vector
  for (int j= V.firstIdxCols(); j<= V.lastIdxCols(); j++)
    var[j] = Stat::variance(V.col(j), W, unbiased);
}

/**  @ingroup StatDesc
 *  Compute the variance of the variable V with fixed mean.
 *  \f[ \hat{\mu} = \frac{1}{n}
 *                  \sum_{i=1}^n (V(i) - \mu)^2.
 *  \f]
 *  @param V variable
 *  @param mu mean of the variable V
 * @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template < class Row, class Col, class TContainer2D >
Row varianceWithFixedMean( TContainer2D const& V
                                  , Row const& mu
                                  , bool unbiased = false
                                  )
{
  if (mu.range() != V.cols())
    throw runtime_error("In varianceWithFixedMean(V, mu, unbiased) "
                             "mu.range() != V.cols()");
  // create result
  Row var(V.cols());
  // get dimensions
  const int  firstVar = V.firstIdxCols(), lastVar = V.lastIdxCols();
  for (int j= firstVar; j<= lastVar; j++)
    var[j] = Stat::varianceWithFixedMean(V.col(j), mu[j], unbiased);
  // return variance
  return var;
}

/**  @ingroup StatDesc
 *  Compute the weighted variance of the variables V with fixed mean.
 *  \f[ \hat{\mu} = \frac{1}{\sum_{i=1}^n W(i)}
 *                  \sum_{i=1}^n W(i) (V(i) - \mu)^2
 *  \f]
 *  @param V variable
 *  @param W weights
 *  @param mu weighted mean of V
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template < class Row, class Col, class TContainer2D >
Row varianceWithFixedMean(TContainer2D const& V
                         , Col const& W
                         , Row const& mu
                         , bool unbiased = false
                         )
{
  if (mu.range() != V.cols())
    STKRUNTIME_ERROR_NO_ARG(varianceWithFixedMean(V, W, mu),mu.range() != V.cols());
  // create result
  Row var(V.cols());
  // get dimensions
  const int  firstVar = V.firstIdxCols(), lastVar = V.lastIdxCols();
  for (int j= firstVar; j<= lastVar; j++)
    var[j] = Stat::varianceWithFixedMean<Col>(V[j], W, mu[j], unbiased);
  // return variance
  return var;
}

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
template < class TContainer2D >
void covariance( TContainer2D const& V
               , MatrixSquare & cov
               , bool unbiased = false
               )
{
  typename hidden::Traits<TContainer2D>::Row mean;
  // compute the
  Stat::mean(V, mean);
  // get dimensions
  const int firstVar = V.firstIdxCols(), lastVar = V.lastIdxCols();
  cov.resize(V.cols());
  for (int j= firstVar; j<= lastVar; j++)
  {
    cov(j, j) = Stat::varianceWithFixedMean(V.col(j), mean[j], unbiased);
    for (int i= firstVar; i<j; i++)
    {
      cov(j,i) = ( cov(i, j) = Stat::covarianceWithFixedMean(V.col(i), V.col(j), mean[i], mean[j], unbiased));
    }
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
template < class Col, class TContainer2D >
void covariance( TContainer2D const& V
               , Col const& W
               , MatrixSquare & cov
               , bool unbiased = false
               )
{
  typename hidden::Traits<TContainer2D>::Row mean;
  typedef typename hidden::Traits<TContainer2D>::Col ColArray;
  // compute the
  Stat::mean(V, W, mean);
  // get dimensions
  const int firstVar = V.firstIdxCols(), lastVar = V.lastIdxCols();
  cov.resize(V.cols());
  for (int j= firstVar; j<= lastVar; j++)
  {
    cov(j, j) = Stat::varianceWithFixedMean<ColArray>(V.col(j), W, unbiased);
    for (int i= firstVar; i<j; i++)
    {
      cov(j,i) = ( cov(i, j) = Stat::covarianceWithFixedMean(V.col(i), V.col(j), W, mean[i], mean[j], unbiased));
    }
  }
}


}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_MULTIVARIATEREAL_H*/
