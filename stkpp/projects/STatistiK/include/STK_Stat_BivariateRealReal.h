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
 * Project:  stkpp::StatistiK::StatDesc
 * Purpose:  Compute elementary statistics for two variables.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Stat_BivariateRealReal.h
 *  @brief This file contain the declaration of the class BivariateRealReal.
 **/

#ifndef STK_STAT_BIVARIATEREALREAL_H
#define STK_STAT_BIVARIATEREALREAL_H

#include "STK_Stat_UnivariateReal.h"

namespace STK
{
namespace Stat
{

/** @ingroup StatDesc
 *  Compute the variance of the variable V with fixed mean.
 *  \f[ \hat{\mu} = \frac{1}{n}
 *                  \sum_{i=1}^n (V(i) - \mu)^2.
 *  \f]
 *  @param X first variable
 *  @param Y second variable
 *  @param xMean the mean of X
 *  @param yMean the mean of Y
 *  @param unbiased @c true if we want an unbiased estimate of the variance,
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
    const int first = com.begin(), last = com.lastIdx();
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
 *
 *  @param X first variable
 *  @param Y second variable
 *  @param W weights
 *  @param xMean the mean of X
 *  @param yMean the mean of Y
 *  @param unbiased @c true if we want an unbiased estimate of the variance,
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
    int first = com.begin(), last = com.lastIdx();
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
 *  @param unbiased @c true if we want an unbiased estimate of the variance,
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
    Real xMean = mean(X), yMean = mean(Y);
    
    // get dimensions
    const int first = com.begin(), last = com.lastIdx();
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
 *  @param unbiased @c true if we want an unbiased estimate of the variance,
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
    int first = com.begin(), last = com.lastIdx();
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
