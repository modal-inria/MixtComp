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
 * Project:  stkpp::StatistiK::StatDesc
 * Purpose:  Compute elementary statistics for two variables.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Stat_BivariateRealReal.h
 *  @brief This file contain the declaration of the class BivariateRealReal.
 **/

#ifndef STK_STAT_BIVARIATEREALREAL_H
#define STK_STAT_BIVARIATEREALREAL_H

#include "STK_Stat_Bivariate.h"
#include "STK_Stat_UnivariateReal.h"

namespace STK
{
namespace Stat
{

/** @ingroup StatDesc
 *  @brief Computation of the bivariate Statistics of two Variables.
 * 
 *  The template parameter TContainer1D is the type of container
 *  used for storing the data : It should derive from
 *  ITContainer1D and contain elements of type Real.
 *
 *  This specialized class compute the Real univariate descriptive statistics
 *  of X and Y and the covariance between X and Y.
 *
 *  The covariance between X and Y will be computed using the formula
 *  @see Mark Galassi, Jim Davies, James Theiler,
 *  Brian Gough, Gerard Jungman, Michael Booth, and Fabrice Rossi.
 *  http://www.gnu.org/software/gsl/manual GNU Scientific Library -
 *   Reference manual, Version 1.15, 2011.
 *  http://www.gnu.org/software/gsl/manual/html_node/Weighted-Samples.html
 *  Sec. 21.7 Weighted Samples
 *
 *  \f$ cov(X,Y)=\frac{\sum_{i=1}^{N}w_i}{\left(\sum_{i=1}^{N}w_i\right)^2-\sum_{i=1}^{N}w_i^2}
 *  \sum_{i=1}^N w_i \left(  x_{i}-\bar{x} \right)  \left( y_{i}-\bar{y} \right). \f$
 **/
template < class TContainer1D >
class Bivariate<Real, Real, TContainer1D>
{
  public:
    /** Default constructor. Copy locally the vectors X and Y and compute
     *  the univariate and bivariate statistics.
     *  @param X the first variable
     *  @param Y the second variable
     **/
    Bivariate( ITContainer<TContainer1D> const& X
             , ITContainer<TContainer1D> const& Y
             )
             : X_(X.asDerived())
             , Y_(Y.asDerived())
             , W_()
             , xStat_(X.asDerived())
             , yStat_(Y.asDerived())
             , cov_(0.)
             , ucov_(0.)
             , cor_(0.)
             , ucor_(0.)
             , sumWeights_(0.0)
             , sum2Weights_(0.0)
    {
      compCovariance();
    }

    /** Default constructor. Compute the univariate statistics of the two
     *  variables. The
     *  @param X the first variable
     *  @param Y the second variable
     *  @param W the weights
     **/
    Bivariate( ITContainer<TContainer1D> const& X
             , ITContainer<TContainer1D> const& Y
             , ITContainer<TContainer1D> const& W
             )
             : X_(X.asDerived())
             , Y_(Y.asDerived())
             , W_(W)
             , xStat_(X, W)
             , yStat_(Y, W)
             , cov_(0.)
             , ucov_(0.)
             , cor_(0.)
             , ucor_(0.)
             , sumWeights_(0.0)
             , sum2Weights_(0.0)
    {
      compWeightedCovariance();
    }
    /** Copy constructor
     *  @param stat the statistics to copy
     **/
    Bivariate( Bivariate<Real, Real, TContainer1D> const& stat)
              : xStat_(stat.xStat_)
              , yStat_(stat.yStat_)
              , cov_(stat.cov_)
              , ucov_(stat.ucov_)
              , cor_(stat.cor_)
              , ucor_(stat.ucor_)
              , sumWeights_(stat.sumWeights_)
              , sum2Weights_(stat.sum2Weights_)
    { }
    
    /** virtual destructor. **/
    virtual ~Bivariate() { ;}
  
    /** Operator = : overwrite the BivariateRealReal with stat.
     *  @param stat the statistics to copy
     *  @return this object
     **/
    Bivariate& operator=( Bivariate<Real, Real, TContainer1D> const& stat)
    {
      xStat_ = stat.xStat_;
      yStat_ = stat.yStat_;
      W_     = stat.W_;
      cov_   = stat.cov_;
      ucov_  = stat.ucov_;
      cor_   = stat.cor_;
      ucor_  = stat.ucor_;
      return *this;
    }

    /** set a new data sets
     *  @param X the first variable
     *  @param Y the second variable
     * */
    void setData( ITContainer<TContainer1D> const& X
                , ITContainer<TContainer1D> const& Y
                )
    {
      X_ = X.asDerived();
      Y_ = Y.asDerived();
      W_.clear();
      xStat_.setData(X);
      yStat_.setData(Y);
      compCovariance();
    }

    /** set a new data sets
     *  @param X the first variable
     *  @param Y the second variable
     *  @param W the weights to apply
     * */
    void setData( ITContainer<TContainer1D> const& X
                , ITContainer<TContainer1D> const& Y
                , ITContainer<TContainer1D> const& W
                )
    {
      X_ = X.asDerived();
      Y_ = Y.asDerived();
      W_ = W.asDerived();
      xStat_.setData(X, W);
      yStat_.setData(Y, W);
      compWeightedCovariance();
    }

    /** get statistics of the first variable
     *  @return the statistics of X
     **/
    inline Univariate<Real, TContainer1D> const& xStat() const {return xStat_;}
    /** get statistics of the first variable
     *  @return the statistics of Y
     **/
    inline Univariate<Real, TContainer1D> const& yStat() const {return yStat_;}
    /** get covariance between the variables
     * @return the covariance between X and Y
     **/
    inline Real const& covariance() const { return cov_;}
    /** get correlation between the variables
     * @return the correlation between X and Y
     **/
    inline Real const& correlation() const { return cor_;}
    /** get the unbiased covariance between the variables
     * @return the unbiased covariance between X and Y
     **/
    inline Real const& unbiasedCovariance() const { return ucov_;}
    /** get the unbiased correlation between the variables
     * @return the unbiased correlation between X and Y
     **/
    inline Real const& unbiasedCorrelation() const { return ucor_;}

  protected:
    /** reference on the X data set. */
    TContainer1D X_;
    /** reference on the Y data set. */
    TContainer1D Y_;
    /** local copy of the weights */
    TContainer1D W_;
    /** Statistics of the X variable */
    Univariate<Real, TContainer1D>    xStat_;
    /** Statistics of the Y variable */
    Univariate<Real, TContainer1D>    yStat_;
    /** covariance between X and Y */
    Real cov_;
    /** unbiased covariance between X and Y */
    Real ucov_;
    /** correlation between X and Y */
    Real cor_;
    /** unbiased correlation between X and Y */
    Real ucor_;
    /** Sum of the weights */
    Real sumWeights_;
    /** Sum of the square of the weights */
    Real sum2Weights_;

    /** compute the covariance */
    void compCovariance()
    {
      // get dimensions
      Range com = STK::Range::inf(X_.range(),Y_.range());
      const int first = com.firstIdx(), last = com.lastIdx();
      int nobs = com.size();
      // get means
      Real xMean = xStat_.mean(), yMean= yStat_.mean();
      // compute covariance
      Real xsum  = 0.0, ysum = 0.0, xdev, ydev;
      cov_ = 0.;
      for (int i=first; i<=last; i++)
      {
        if (Arithmetic<Real>::isFinite(X_[i]) && Arithmetic<Real>::isFinite(Y_[i]))
        {
          xsum += (xdev = X_[i] - xMean); // deviation from the mean
          ysum += (ydev = Y_[i] - yMean); // deviation from the mean
          cov_ += (xdev*ydev);         // squared value
        }
        else nobs--;
      }
      if (nobs > 1)
      {
        ucov_ = (cov_ - (xsum*ysum)/(Real)nobs)/(Real)(nobs -1);
        ucor_ = ucov_/(xStat_.unbiasedStd()*yStat_.unbiasedStd());
        cov_  = (cov_ - (xsum*ysum)/(Real)nobs)/(Real)(nobs);
        cor_  = cov_/(xStat_.std()*yStat_.std());
      }
      else
      {
        if (nobs == 1)
        {
          cov_ = 0.;
          cor_ = 0.;
          ucov_ = Arithmetic<Real>::NA();
          ucor_ = Arithmetic<Real>::NA();
        }
        // no observations
        cov_ = Arithmetic<Real>::NA();
        cor_ = Arithmetic<Real>::NA();
        ucov_ = Arithmetic<Real>::NA();
        ucor_ = Arithmetic<Real>::NA();
      }
    }

    /** compute the weighted covariance */
    void compWeightedCovariance()
    {
      // get dimensions
      Range com = STK::Range::inf(X_.range(),Y_.range());
      int first = com.firstIdx(), last = com.lastIdx();
      // get means
      Real xMean = xStat_.mean(), yMean= yStat_.mean();
      // compute covariance
      Real xsum  = 0.0, ysum = 0.0, xdev, ydev;
      sumWeights_ = 0.0;
      sum2Weights_ = 0.0;
      // sum
      cov_ = 0.0;
      for (int i=first; i<=last; i++)
      {
        if (Arithmetic<Real>::isFinite(X_[i]) && Arithmetic<Real>::isFinite(Y_[i]))
        {
          xsum += (xdev = X_[i] - xMean); // deviation from the mean
          ysum += (ydev = Y_[i] - yMean); // deviation from the mean
          Real Wi = std::abs((Real)W_[i]);
          cov_ += Wi * (xdev*ydev);         // cross product
          sumWeights_  += Wi;           // sum absolute weights
          sum2Weights_ += Wi * Wi;     // sum squared weights
       }
     }
     // not pathological weights
     if (sumWeights_*sumWeights_ > sum2Weights_)
     {
       ucov_ = (cov_ - xsum*ysum/sumWeights_)/(sumWeights_ - sum2Weights_/sumWeights_);
       ucor_ = ucov_/(xStat_.unbiasedStd()*yStat_.unbiasedStd());
       cov_ = (cov_ - xsum*ysum)/(sumWeights_);
       cor_ = cov_/(xStat_.std()*yStat_.std());
     }
     else
     {
       if (sumWeights_) // if sum of the weights is not 0
       {
         cov_ = 0.;
         cor_ = 0.;
         ucov_ = Arithmetic<Real>::NA();
         ucor_ = Arithmetic<Real>::NA();
       }
       // no weigths
       cov_ = Arithmetic<Real>::NA();
       cor_ = Arithmetic<Real>::NA();
       ucov_ = Arithmetic<Real>::NA();
       ucor_ = Arithmetic<Real>::NA();
     }
   }
};
  
/** @ingroup StatDesc
 *  Compute the variance of the variable V with fixed mean.
 *  \f[ \hat{\mu} = \frac{1}{n}
 *                  \sum_{i=1}^n (V(i) - \mu)^2.
 *  \f]
 *  @param X first variable
 *  @param Y second variable
 *  @param xMean the mean of X
 *  @param yMean the mean of Y
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template<class TContainer1D>
Real covarianceWithFixedMean( TContainer1D const& X
                            , TContainer1D const& Y
                            , Real const& xMean
                            , Real const& yMean
                            , bool unbiased = false
                            )
{
    // get dimensions
    Range com = STK::Range::inf(X.range(),Y.range());
    // no samples
    if (com.empty()) { return Arithmetic<Real>::NA();}

    // get dimensions
    const int first = com.firstIdx(), last = com.lastIdx();
    int nobs = com.size();
    // compute covariance
    Real xsum  = 0.0, ysum = 0.0, cov  = 0.0, xdev, ydev;
    for (int i=first; i<=last; i++)
    {
      if (Arithmetic<Real>::isFinite(X[i]) && Arithmetic<Real>::isFinite(Y[i]))
      {
        xsum += (xdev = X[i] - xMean); // deviation from the mean
        ysum += (ydev = Y[i] - yMean); // deviation from the mean
        cov += (xdev*ydev);         // squared value
      }
      else nobs--;
    }
    // compute the variance
    if (unbiased)
    {
      return (nobs > 1) ? ((cov - (xsum*ysum)/(Real)nobs)/(Real)(nobs -1))
                        : Arithmetic<Real>::NA();

    }
    return (nobs > 0) ? (cov - (xsum*ysum)/(Real)nobs)/(Real)(nobs)
                      : Arithmetic<Real>::NA();
}

/** @ingroup StatDesc
 *  Compute the weighted variance of the variable V with fixed mean.
 *  \f[ \hat{\mu} = \frac{1}{\sum_{i=1}^n W(i)}
 *                  \sum_{i=1}^n W(i) (V(i) - \mu)^2
 *  \f]
 *  @param X first variable
 *  @param Y second variable
 *  @param W weights
 *  @param xMean the mean of X
 *  @param yMean the mean of Y
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template<class TContainer1D>
Real covarianceWithFixedMean( TContainer1D const& X
                            , TContainer1D const& Y
                            , TContainer1D const& W
                            , Real const& xMean
                            , Real const& yMean
                            , bool unbiased = false
                          )
{
    // get dimensions
    Range com = STK::Range::inf(X.range(),Y.range());
    // no samples
    if (com.empty()) { return Arithmetic<Real>::NA();}

    // if the weight are not of the same size, ignore them
    if (!com.isIn(W.range()))
      return covarianceWithFixedMean(X, Y, xMean, yMean, unbiased);

    // get dimensions
    int first = com.firstIdx(), last = com.lastIdx();
    // compute covariance
    Real xsum  = 0.0, ysum = 0.0, xdev, ydev, sumWeights = 0.0, sum2Weights = 0.0, cov = 0.0;
    for (int i=first; i<=last; i++)
    {
      if (Arithmetic<Real>::isFinite(X[i]) && Arithmetic<Real>::isFinite(Y[i]))
      {
        xsum += (xdev = X[i] - xMean); // deviation from the mean
        ysum += (ydev = Y[i] - yMean); // deviation from the mean
        Real Wi = std::abs((Real)W[i]);
        cov += Wi * (xdev*ydev);         // cross product
        sumWeights  += Wi;           // sum absolute weights
        sum2Weights += Wi * Wi;     // sum squared weights
      }
    }
    // compute the variance
    if (unbiased)
    {
      if (sumWeights*sumWeights > sum2Weights)
      {
        return (cov - xsum*ysum/sumWeights)/(sumWeights - sum2Weights/sumWeights);
      }
      else
      {
        if (sumWeights) return 0.;
      }
      return Arithmetic<Real>::NA();
    }
    return (sumWeights) ? (cov - xsum*ysum)/(sumWeights) : Arithmetic<Real>::NA();
}

/** @ingroup StatDesc
 *  Compute the covariance between the variable X and Y.
 *  \f[ \hat{\sigma}^2 = \frac{1}{n}
 *                       \sum_{i=1}^n (V(i)-\hat{\mu})^2.
 *  \f]
 *  @param X first variable
 *  @param Y second variable
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template<class TContainer1D>
Real covariance( TContainer1D const& X
                , TContainer1D const& Y
                , bool unbiased = false
                )
{
    // get dimensions
    Range com = STK::Range::inf(X.range(),Y.range());
    // no samples
    if (com.empty()) { return Arithmetic<Real>::NA();}
    
    // Compute the mean
    Real xMean = mean<TContainer1D>(X), yMean = mean<TContainer1D>(Y);
    
    // get dimensions
    const int first = com.firstIdx(), last = com.lastIdx();
    int nobs = com.size();
    // compute covariance
    Real xsum  = 0.0, ysum = 0.0, cov  = 0.0, xdev, ydev;
    for (int i=first; i<=last; i++)
    {
        if (Arithmetic<Real>::isFinite(X[i]) && Arithmetic<Real>::isFinite(Y[i]))
        {
            xsum += (xdev = X[i] - xMean); // deviation from the mean
            ysum += (ydev = Y[i] - yMean); // deviation from the mean
            cov += (xdev*ydev);         // squared value
        }
        else nobs--;
    }
    // compute the variance
    if (unbiased)
    {
        return (nobs > 1) ? ((cov - (xsum*ysum)/(Real)nobs)/(Real)(nobs -1))
        : Arithmetic<Real>::NA();
        
    }
    return (nobs > 0) ? ((cov - (xsum*ysum)/(Real)nobs)/(Real)(nobs))
    : Arithmetic<Real>::NA();
}

/** @ingroup StatDesc
 *  Compute the weighted variance of the variable V.
 *  \f[ \hat{\sigma}^2
 *  = \frac{\sum_{i=1}^n W(i)}{\left( \sum_{i=1}^n W(i))\right)^2-\sum_{i=1}^n W(i)^2}
 *    \sum_{i=1}^n W(i) (V(i)-\hat{\mu})^2.
 *  \f]
 * If there is no weights, this definition reduces to the usual
 * definition of the variance with factor 1/(n-1). If the range of the weights
 * is not equal to the range of the varaible, the usual varaince is computed.
 *  @param X first variable
 *  @param Y second variable
 *  @param W weights
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template<class TContainer1D>
Real covariance( TContainer1D const& X
               , TContainer1D const& Y
               , TContainer1D const& W
               , bool unbiased = false
               )
{
    // get dimensions
    Range com = STK::Range::inf(X.range(),Y.range());
    // no samples
    if (com.empty()) { return Arithmetic<Real>::NA();}
    
    // Compute the mean
    Real xMean = mean(X, W), yMean= mean(Y, W);
    
    // if the weight are not of the same size, ignore them
    if (!com.isIn(W.range()))
        return covarianceWithFixedMean(X, Y, xMean, yMean, unbiased);
    
    // get dimensions
    int first = com.firstIdx(), last = com.lastIdx();
    // compute covariance
    Real xsum  = 0.0, ysum = 0.0, xdev, ydev, sumWeights = 0.0, sum2Weights = 0.0, cov = 0.0;
    for (int i=first; i<=last; i++)
    {
        if (Arithmetic<Real>::isFinite(X[i]) && Arithmetic<Real>::isFinite(Y[i]))
        {
            xsum += (xdev = X[i] - xMean); // deviation from the mean
            ysum += (ydev = Y[i] - yMean); // deviation from the mean
            Real Wi = std::abs((Real)W[i]);
            cov += Wi * (xdev*ydev);         // cross product
            sumWeights  += Wi;           // sum absolute weights
            sum2Weights += Wi * Wi;     // sum squared weights
        }
    }
    // compute the variance
    if (unbiased)
    {
        if (sumWeights*sumWeights > sum2Weights)
        {
            return (cov - xsum*ysum/sumWeights)/(sumWeights - sum2Weights/sumWeights);
        }
        else
        {
            if (sumWeights) return 0.;
        }
        return Arithmetic<Real>::NA();
    }
    return (sumWeights) ? (cov - xsum*ysum)/(sumWeights) : Arithmetic<Real>::NA();
}


}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_BIVARIATEREALREAL_H*/
