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
 * Project:  stkpp::STatistiK::StatDesc
 * Purpose:  Compute elementary 1D statistics for all variables.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Stat_Functors.h
 *  @brief This file contain the functors computings statistics.
 **/

#ifndef STK_STAT_FUNCTORS_H
#define STK_STAT_FUNCTORS_H

#include "../../Sdk/include/STK_MetaTemplate.h"
#include "../../Sdk/include/STK_StaticAssert.h"

#include "../../Arrays/include/STK_ExprBaseFunctors.h"


namespace STK
{


namespace Stat
{
/** @ingroup Stat
 *  Compute the minimal value of the variable V
 **/
template<class Derived>
struct MinOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline MinOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /**  @return the minimal value of the variable V
     *  \f[ \min_{i=1}^n v_i \f]
     **/
    Type const operator()() const
    {
      Type min  = Arithmetic<Type>::infinity();
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { min = std::min(min, V_[i]);}
      return min;
    }
    /** @return the minimal value of the variable V
     *  \f[ \min_{i=1}^n w_i v_i \f]
     *  @param w the weights
     **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const&  w) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      Type min  = Arithmetic<Type>::infinity();
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { min = std::min(min, w[i]*V_[i]);}
      return min;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute safely the minimal value of the variable V
 **/
template<class Derived>
struct MinSafeOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline MinSafeOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /**  @return the safely computed minimal value of the variable V */
    Type const operator()() const
    {
      Type min  = Arithmetic<Type>::infinity();
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if (!Arithmetic<Type>::isNA(V_[i])) min = std::min(min, V_[i]);}
      return min;
    }
    /** @return the safely computed weighted minimal value of the variable V
     *  @param w the weights
     **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const&  w) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      Type min  = Arithmetic<Type>::infinity();
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if (!Arithmetic<Type>::isNA(V_[i])) min = std::min(min, w[i]*V_[i]);}
      return min;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute the maximal value of the variable V
 **/
template<class Derived>
struct MaxOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline MaxOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the maximal value of the variable V
     *  \f[ \max_{i=1}^n v_i \f]
     **/
    Type const operator()() const
    {
      Type max  = -Arithmetic<Type>::infinity();
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { max = std::max(max, V_[i]);}
      return max;
    }
    /** @return the weighted maximal value of the variable V
     *  \f[ \max_{i=1}^n w_i v_i \f]
     *  @param w the weights
     **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const&  w) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      Type max  = -Arithmetic<Type>::infinity();
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { max = std::max(max, w[i]*V_[i]);}
      return max;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute safely the maximal value of the variable V
 **/
template<class Derived>
struct MaxSafeOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline MaxSafeOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the safely computed maximal value of the variable V */
    Type const operator()() const
    {
      Type max  = -Arithmetic<Type>::infinity();
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if (!Arithmetic<Type>::isNA(V_[i])) max = std::max(max, V_[i]);}
      return max;
    }
    /** @return the safely computed weighted maximal value of the variable V
     *  @param w the weigths
     **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const&  w) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      Type max  = -Arithmetic<Type>::infinity();
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if (!Arithmetic<Type>::isNA(V_[i])) max = std::max(max, w[i]*V_[i]);}
      return max;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute the sum of the variable V.
 **/
template<class Derived>
struct SumOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline SumOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the mean of the variable V
     *  \f[ \hat{\mu} = \frac{1}{n} \sum_{i=1}^n V(i) \f]
     **/
    Type const operator()() const
    {
      // sum the samples
      Type sum  = 0.;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++) { sum += V_[i];}
      // compute the mean
      return sum;
    }
    /** @return the weighted mean value of the variable V
     *  \f[ \hat{\mu} = \frac{1}{\sum_{i=1}^n w(i)} \sum_{i=1}^n w(i) V(i). \f]
     *  @param w the weigths
     **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const&  w) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      // sum the weighted samples
      Type sum  = 0.0, nweight = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { sum     += w[i] * V_[i];}
      // compute the weighted mean. If all weights are 0, we get 0
      return sum;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute safely the sum of the variable V.
 **/
template<class Derived>
struct SumSafeOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline SumSafeOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the safely computed mean of the variable V discarding all missing values. */
    Type const operator()() const
    {
      // sum the samples
      Type sum  = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if (!Arithmetic<Type>::isNA(V_[i])) sum += V_[i];}
      // compute the mean
      return sum;
    }
    /** @return the safely computed weighted mean of the variable V
     *  @param w the weigths
     **/
    template< class Weights>
    inline Type const operator()( ExprBase<Weights> const&  w) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      if (V_.range() != w.range())
        STKRUNTIME_ERROR_NO_ARG(wmeanSafe,V.range()!=w.range());

      // sum the weighted samples
      Type sum  = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if ( (!Arithmetic<Type>::isNA(V_[i])) && (!Arithmetic<Type>::isNA(w[i])))
        { sum += std::abs(w[i]) * V_[i];}
      }
      // compute the weighted mean. If all weights are 0, we get 0
      return sum;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute the mean of the variable V.
 **/
template<class Derived>
struct MeanOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline MeanOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the mean of the variable V
     *  \f[ \hat{\mu} = \frac{1}{n} \sum_{i=1}^n V(i) \f]
     **/
    Type const operator()() const
    {
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      // sum the samples
      Type sum  = 0.;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++) { sum += V_[i];}
      // compute the mean
      return sum /= (Type)(V_.size());
    }
    /** @return the weighted mean value of the variable V
     *  \f[ \hat{\mu} = \frac{1}{\sum_{i=1}^n w(i)} \sum_{i=1}^n w(i) V(i). \f]
     *  @param w the weigths
     **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const&  w) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      // sum the weighted samples
      Type sum  = 0.0, nweight = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        Type const weight = w[i];
        nweight += weight;
        sum     += weight * V_[i];
      }
      // compute the weighted mean. If all weights are 0, we get 0
      return (nweight) ? sum /= nweight : 0.;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute safely mean of the variable V.
 **/
template<class Derived>
struct MeanSafeOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline MeanSafeOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the safely computed mean of the variable V discarding all missing values. */
    Type const operator()() const
    {
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      // get dimensions
      int nobs = V_.size();
      // sum the samples
      Type sum  = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { (!Arithmetic<Type>::isNA(V_[i])) ? sum += V_[i] : nobs--;}
      // compute the mean
      return nobs ? sum /= Type(nobs) : Arithmetic<Type>::NA();
    }
    /** @return the safely computed weighted mean of the variable V
     *  @param w the weigths
     **/
    template< class Weights>
    inline Type const operator()( ExprBase<Weights> const&  w) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      if (V_.range() != w.range())
        STKRUNTIME_ERROR_NO_ARG(wmeanSafe,V.range()!=w.range());

      // sum the weighted samples
      Type sum  = 0.0, nweight = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if ( (!Arithmetic<Type>::isNA(V_[i])) && (!Arithmetic<Type>::isNA(w[i])))
        {
          Type weight  = std::abs(w[i]);
          nweight  += weight;
          sum      += weight * V_[i];
        }
      }
      // compute the weighted mean. If all weights are 0, we get 0
      return (nweight) ? sum /= nweight : 0.;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute the variance of the variable V.
 **/
template<class Derived>
struct VarianceOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline VarianceOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the variance of the variable V.
     *  \f[ \hat{\sigma}^2 = \frac{1}{n} \sum_{i=1}^n (V(i)-\hat{\mu})^2. \f]
     *  @param unbiased @c true if we want an unbiased estimator of the variance,
     *  @c false otherwise
     **/
    inline Type const operator()(bool unbiased) const
    {
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}

      int const nobs = V_.size();
      // Compute the mean and sum
      Type mu = MeanOp<Derived>(V_)();
      Type sum  = 0.0, var  = 0.0, dev;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        sum += (dev = V_[i] - mu); // deviation from the mean
        var += (dev*dev);         // squared value
      }
      // compute the variance
      if (unbiased)
      { return (nobs > 1) ? (var - (sum*sum)/Type(nobs))/Type(nobs -1)
                          : Arithmetic<Type>::NA();
      }
      // variance
      return (var - (sum*sum)/(Type)nobs)/(Type)(nobs);
    }
    /** @return the weighted variance of the variable V.
     *  \f[ \hat{\sigma}^2
     *  = \frac{\sum_{i=1}^n w(i)}{\left( \sum_{i=1}^n w(i))\right)^2-\sum_{i=1}^n w(i)^2}
     *    \sum_{i=1}^n w(i) (V(i)-\hat{\mu})^2.
     *  \f]
     * If there is no weights, this definition reduces to the usual
     * definition of the variance with factor 1/(n-1).
     *  @param w weights
     *  @param unbiased @c true if we want an unbiased estimator of the variance,
     *  @c false otherwise
     **/
    template< class Weights>
    inline Type const operator()( ExprBase<Weights> const&  w, bool unbiased) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      // Compute the mean
      Type mu = MeanOp<Derived>(V_)(w);
      // sum the weighted samples
      Type dev, sum = 0.0, var = 0.0, nweight = 0.0, nweight2 = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        Type const weight = w[i];
        nweight    += weight;
        nweight2   += weight * weight;
        sum        += weight*(dev = V_[i]-mu); // deviation from the mean
        var        += weight*(dev*dev);       // squared value
      }
      // compute the variance
      if (unbiased)
      {
        return (nweight*nweight - nweight2 > 0.) ? (var - sum*sum/nweight)/(nweight - nweight2/nweight)
                                                 : 0.;
      }
      return (nweight) ? (var - sum*sum)/(nweight) : 0.;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute safely the variance of the variable V.
 **/
template<class Derived>
struct VarianceSafeOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline VarianceSafeOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the variance of the variable V discarding all missing values.
     *  \f[ \hat{\sigma}^2 = \frac{1}{n} \sum_{i=1}^n (V(i)-\hat{\mu})^2. \f]
     *  @param unbiased @c true if we want an unbiased estimator of the variance,
     *  @c false otherwise
     **/
    Type const operator()(bool unbiased) const
    {
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      int nobs = V_.size();
      // Compute the mean
      Type mu = MeanSafeOp<Derived>(V_)();
      // sum
      Type sum  = 0.0, var  = 0.0, dev;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        if (!Arithmetic<Type>::isNA(V_[i]))
        {
          sum += (dev = V_[i] - mu); // deviation from the mean
          var += (dev*dev);         // squared value
        }
        else nobs--;
      }
      // compute the variance
      if (unbiased)
      {
        return (nobs > 1) ? (var - (sum*sum)/Type(nobs))/Type(nobs -1)
                          : Arithmetic<Type>::NA();
      }
      // variance
      return (nobs > 0) ? (var - (sum*sum)/(Type)nobs)/(Type)(nobs)
                        : Arithmetic<Type>::NA();
    }
    /** @return the safely computed weighted variance of the variable V.
     *  \f[ \hat{\sigma}^2
     *  = \frac{\sum_{i=1}^n w(i)}{\left( \sum_{i=1}^n w(i))\right)^2-\sum_{i=1}^n w(i)^2}
     *    \sum_{i=1}^n w(i) (V(i)-\hat{\mu})^2.
     *  \f]
     * If there is no weights, this definition reduces to the usual
     * definition of the variance with factor 1/(n-1).
     *  @param w weights
     *  @param unbiased @c true if we want an unbiased estimator of the variance,
     *  @c false otherwise
    **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const&  w, bool unbiased) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      // Compute the mean
      Type mu = MeanSafeOp<Derived>(V_)(w);
      if (V_.range() != w.range())
        STKRUNTIME_ERROR_NO_ARG(VarianceSafeOp,V.range()!=w.range());
      // sum the weighted samples
      Type dev, sum = 0.0, var = 0.0, nweight = 0.0, nweight2 = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if ( !Arithmetic<Type>::isNA(V_[i]) && !Arithmetic<Type>::isNA(w[i]) )
        {
          Type weight = std::abs(w[i]);
          nweight    += weight;
          nweight2   += weight * weight;
          sum        += weight*(dev = V_[i]-mu); // deviation from the mean
          var        += weight*(dev*dev);       // squared value
        }
      }
      // compute the variance
      if (unbiased)
      {
        return (nweight*nweight - nweight2 > 0.) ? (var - sum*sum/nweight)/(nweight - nweight2/nweight)
                                                 : 0.;
      }
      return (nweight) ? (var - sum*sum)/(nweight) : 0.;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute the variance of the variable V when the mean is given.
 **/
template<class Derived>
struct VarianceWithFixedMeanOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline VarianceWithFixedMeanOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
  /** @return the variance of the variable V with fixed mean.
   *  \f[ \hat{\sigma^2} = \frac{1}{n} \sum_{i=1}^n (V(i) - \mu)^2. \f]
   *  using a compensated algorithm.
   *  @note
   *  Chan, Tony F.; Golub, Gene H.; LeVeque, Randall J. (1983).
   *  Algorithms for Computing the Sample Variance: Analysis and Recommendations.
   *  The American Statistician 37, 242-247.
   *  @param mu the fixed mean
   *  @param unbiased @c true if we want an unbiased estimator of the variance,
   *  @c false otherwise
   **/
    Type const operator()( Type const& mu, bool unbiased) const
    {
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      int const nobs = V_.size();
      // sum
      Type sum = 0., var = 0., dev;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        sum += (dev = V_[i] - mu); // deviation from the mean
        var += (dev*dev);         // squared value
      }
      // unbiased variance
      if (unbiased)
      {
        return (nobs > 1) ? (var - (sum*sum)/(Type)(V_.size()))/(Type)(V_.size() -1)
                              : Arithmetic<Type>::NA();
      }
      // ML variance
      return (var - (sum*sum)/(Type)nobs)/(Type)(nobs);
    }
    /** @return the weighted variance of the variable V with fixed mean.
     *  \f[ \hat{\sigma^2} = \frac{1}{\sum_{i=1}^n w(i)}
     *                  \sum_{i=1}^n w(i) (V(i) - \mu)^2
     *  \f]
     *  @param w weights
     *  @param mu the mean
     *  @param unbiased @c true if we want an unbiased estimator of the variance,
     *  @c false otherwise
     **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const& w, Type const& mu, bool unbiased) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      // sum the weighted samples
      Type dev, sum = 0.0, var = 0.0, nweight = 0.0, nweight2 = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        Type weight = w[i];
        nweight    += weight;
        nweight2   += weight * weight;
        sum        += weight*(dev = V_[i]-mu); // deviation from the mean
        var        += weight*(dev*dev);       // squared value
      }
      // compute the variance
      if (unbiased)
      {
        return (nweight*nweight - nweight2 > 0.) ? (var - sum*sum/nweight)/(nweight - nweight2/nweight)
                                                 : 0.;

      }
      return (nweight) ? (var - sum*sum)/(nweight) : 0.;
    }
  protected:
    Derived const& V_;
};

/** @ingroup Stat
 *  Compute safely the variance of the variable V when the mean is given.
 **/
template<class Derived>
struct VarianceWithFixedMeanSafeOp
{
    typedef typename Derived::Type Type;
    /** constructor */
    inline VarianceWithFixedMeanSafeOp( ExprBase<Derived> const&  V) :  V_(V.asDerived())
    { STK_STATICASSERT_ONE_DIMENSION_ONLY(Derived);}
    /** @return the safely computed variance of the variable V with fixed mean.
     *  \f[ \hat{\sigma^2} = \frac{1}{n} \sum_{i=1}^n (V(i) - \mu)^2. \f]
     *  using a compensated algorithm and discarding the missing values.
     *  @note
     *  Chan, Tony F.; Golub, Gene H.; LeVeque, Randall J. (1983).
     *  Algorithms for Computing the Sample Variance: Analysis and Recommendations.
     *  The American Statistician 37, 242-247.
     *  @param mu the fixed mean
     *  @param unbiased @c true if we want an unbiased estimator of the variance,
     *  @c false otherwise
     **/
    Type const operator()( Type mu, bool unbiased) const
    {
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      int nobs = V_.size();
      // sum
      Type sum  = 0.0, var  = 0.0, dev;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        if (!Arithmetic<Type>::isNA(V_[i]))
        {
          sum += (dev = V_[i] - mu); // deviation from the mean
          var += (dev*dev);         // squared value
        }
        else nobs--;
      }
      // compute the variance
      if (unbiased)
      {
        return (nobs > 1) ? (var - (sum*sum)/Type(nobs))/Type(nobs -1)
                          : Arithmetic<Type>::NA();
      }
      // variance
      return (nobs > 0) ? (var - (sum*sum)/(Type)nobs)/(Type)(nobs)
                        : Arithmetic<Type>::NA();
    }

    /** @return the safely computed weighted variance of the variable V with fixed mean.
     *  \f[ \hat{\sigma^2} = \frac{1}{\sum_{i=1}^n w(i)}
     *                  \sum_{i=1}^n w(i) (V(i) - \mu)^2
     *  \f]
     *  @param w weights
     *  @param mu the mean
     *  @param unbiased @c true if we want an unbiased estimator of the variance,
     *  @c false otherwise
     **/
    template< class Weights>
    Type const operator()( ExprBase<Weights> const& w
                         , Type const& mu
                         , bool unbiased) const
    {
      STK_STATICASSERT_ONE_DIMENSION_ONLY(Weights);
      // no samples
      if (V_.empty()) { return Arithmetic<Type>::NA();}
      if (V_.range() != w.range())
        STKRUNTIME_ERROR_NO_ARG(wmeanSafe,V.range()!=w.range());
      // sum the weighted samples
      Type dev, sum = 0.0, var = 0.0, nweight = 0.0, nweight2 = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      { if ( !Arithmetic<Type>::isNA(V_[i]) && !Arithmetic<Type>::isNA(w[i]) )
        {
          Type weight = std::abs(w[i]);
          nweight    += weight;
          nweight2   += weight * weight;
          sum        += weight*(dev = V_[i]-mu); // deviation from the mean
          var        += weight*(dev*dev);       // squared value
        }
      }
      // compute the variance
      if (unbiased)
      {
        return (nweight*nweight - nweight2 > 0.) ? (var - sum*sum/nweight)/(nweight - nweight2/nweight)
                                                 : 0.;

      }
      return (nweight) ? (var - sum*sum)/(nweight) : 0.;
    }
  protected:
    Derived const& V_;
};

/** @ingroup StatDesc
 *  Compute the minimal(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual min of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the minimal values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the array
 *  @return the minimal value(s) of A or NA if there is no available value.
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MinOp >::resultByColType
min(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MinOp>::ColOp(A)();}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MinOp >::resultByRowType
minByRow(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MinOp>::RowOp(A)();}

/** @ingroup StatDesc
 *  Compute the weighted minimal(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min; STK::Stat::sum.
 *  @param A the array
 *  @param w the weights
 *  @return the minimal value(s) of A or NA if there is no available value.
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MinOp >::resultByColType
min(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MinOp>::ColOp(A)(w);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MinOp >::resultByRowType
minByRow(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MinOp>::RowOp(A)(w);}

/** @ingroup StatDesc
 *  Compute safely the minimal(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual min of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the minimal values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the array
 *  @return the minimal value(s) of A or NA if there is no available
 *  value.
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MinSafeOp >::resultByColType
minSafe(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MinSafeOp>::ColOp(A)();}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MinSafeOp >::resultByRowType
minSafeByRow(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MinSafeOp>::RowOp(A)();}

/** @ingroup StatDesc
 *  Compute safely the weighted minimal(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the array
 *  @param w the weights
 *  @return the minimal value(s) of A or NA if there is no available
 *  value.
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MinSafeOp >::resultByColType
minSafe(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MinSafeOp>::ColOp(A)(w);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MinSafeOp >::resultByRowType
minSafeByRow(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MinSafeOp>::RowOp(A)(w);}

/** @ingroup StatDesc
 *  Compute the maximal(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual max of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the maximal values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the array
 *  @return the maximal value(s) of A or NA if there is no available
 *  value.
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MaxOp >::resultByColType
max(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MaxOp>::ColOp(A)();}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MaxOp >::resultByRowType
maxByRow(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MaxOp>::RowOp(A)();}

/** @ingroup StatDesc
 *  Compute the weighted maximal(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the array
 *  @param w the weights
 *  @return the maximal value(s) of A or NA if there is no available
 *  value.
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MaxOp >::resultByColType
max(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MaxOp>::ColOp(A)(w);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MaxOp >::resultByRowType
maxByRow(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MaxOp>::RowOp(A)(w);}

/** @ingroup StatDesc
 *  Compute safely the maximal(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual max of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the maximal values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the array
 *  @return the maximal value(s) of A or NA if there is no available
 *  value.
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MaxSafeOp >::resultByColType
maxSafe(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MaxSafeOp>::ColOp(A)();}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MaxSafeOp >::resultByRowType
maxSafeByRow(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MaxSafeOp>::RowOp(A)();}

/** @ingroup StatDesc
 *  Compute the weighted maximal(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the array
 *  @param w the weights
 *  @return the maximal value(s) of A or NA if there is no available
 *  value.
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MaxSafeOp >::resultByColType
maxSafe(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MaxSafeOp>::ColOp(A)(w);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MaxSafeOp >::resultByRowType
maxSafeByRow(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MaxSafeOp>::RowOp(A)(w);}

/** @ingroup StatDesc
 *  Compute the sum of A. If A is a row-vector or a
 *  column-vector then the function will return the usual mean of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the mean values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @return the mean(s) or NA if there is no available value
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, SumOp >::resultByColType
sum(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, SumOp>::ColOp(A)();}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, SumOp >::resultByRowType
meanByRow(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, SumOp>::RowOp(A)();}

/** @ingroup StatDesc
 *  Compute the weighted mean(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param w the weights
 *  @return the weighted mean(s) or NA if there is no available value
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, SumOp >::resultByColType
sum(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, SumOp>::ColOp(A)(w);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, SumOp >::resultByRowType
sumByRow(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, SumOp>::RowOp(A)(w);}

/** @ingroup StatDesc
 *  Compute safely the mean(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual mean of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the mean values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @return the mean(s) or NA if there is no available value
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, SumSafeOp >::resultByColType
sumSafe(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, SumSafeOp>::ColOp(A)();}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, SumSafeOp >::resultByRowType
sumSafeByRow(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, SumSafeOp>::RowOp(A)();}

/** @ingroup StatDesc
 *  Compute the weighted mean(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param w the weights
 *  @return the weighted mean(s) or NA if there is no available value
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, SumSafeOp >::resultByColType
sumSafe(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, SumSafeOp>::ColOp(A)(w);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, SumSafeOp >::resultByRowType
sumSafeByRow(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, SumSafeOp>::RowOp(A)(w);}

/** @ingroup StatDesc
 *  Compute the mean(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual mean of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the mean values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @return the mean(s) or NA if there is no available value
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByColType
mean(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MeanOp>::ColOp(A)();}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByRowType
meanByRow(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MeanOp>::RowOp(A)();}

/** @ingroup StatDesc
 *  Compute the weighted mean(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param w the weights
 *  @return the weighted mean(s) or NA if there is no available value
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByColType
mean(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MeanOp>::ColOp(A)(w);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByRowType
meanByRow(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MeanOp>::RowOp(A)(w);}

/** @ingroup StatDesc
 *  Compute safely the mean(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual mean of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the mean values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @return the mean(s) or NA if there is no available value
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp >::resultByColType
meanSafe(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp>::ColOp(A)();}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp >::resultByRowType
meanSafeByRow(Derived const& A)
{ return typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp>::RowOp(A)();}

/** @ingroup StatDesc
 *  Compute safely the weighted mean(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param w the weights
 *  @return the weighted mean(s) or NA if there is no available value
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp >::resultByColType
meanSafe(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp>::ColOp(A)(w);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp >::resultByRowType
meanSafeByRow(Derived const& A, ExprBase<Weights> const& w)
{ return typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp>::RowOp(A)(w);}

/** @ingroup StatDesc
 *  Compute the variance(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual variance of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the variance values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param unbiased the unbiased variance(s) if @c true or the Maximum-likelihood
 *  variance otherwise (the default)
 *  @return the variance(s) or NA if there is no available value
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, VarianceOp >::resultByColType
variance(Derived const& A, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceOp>::ColOp(A)(unbiased);}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, VarianceOp >::resultByRowType
varianceByRow(Derived const& A, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceOp>::RowOp(A)(unbiased);}

/** @ingroup StatDesc
 *  Compute the weighted variance(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param w the weights
 *  @param unbiased the unbiased variance(s) if @c true or the Maximum-likelihood
 *  variance otherwise (the default)
 *  @return the variance(s) or NA if there is no available value
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, VarianceOp >::resultByColType
variance(Derived const& A, ExprBase<Weights> const& w, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceOp>::ColOp(A)(w, unbiased);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, VarianceOp >::resultByRowType
varianceByRow(Derived const& A, ExprBase<Weights> const& w, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceOp>::RowOp(A)(w, unbiased);}

/** @ingroup StatDesc
 *  Compute safely the variance(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual variance of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the variance values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param unbiased the unbiased variance(s) if @c true or the Maximum-likelihood
 *  variance otherwise (the default)
 *  @return the variance(s) or NA if there is no available value
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, VarianceSafeOp >::resultByColType
varianceSafe(Derived const& A, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceSafeOp>::ColOp(A)(unbiased);}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, VarianceSafeOp >::resultByRowType
varianceSafeByRow(Derived const& A, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceSafeOp>::RowOp(A)(unbiased);}

/** @ingroup StatDesc
 *  Compute safely the weighted variance(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param w the weights
 *  @param unbiased the unbiased variance(s) if @c true or the Maximum-likelihood
 *  variance otherwise (the default)
 *  @return the variance(s) or NA if there is no available value
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, VarianceSafeOp >::resultByColType
varianceSafe(Derived const& A, ExprBase<Weights> const& w, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceSafeOp>::ColOp(A)(w, unbiased);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, VarianceSafeOp >::resultByRowType
varianceSafeByRow(Derived const& A, ExprBase<Weights> const& w, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceSafeOp>::RowOp(A)(w, unbiased);}

/** @ingroup StatDesc
 *  Compute the VarianceWithFixedMean(s) value(s) of A. If A is a row-vector or a
 *  column-vector then the function will return the usual variance of the vector.
 *  If A is a two-dimensional array, the function will return (by value) an
 *  Array2DPoint with the variance values of each columns.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param mean The mean (s) to use
 *  @param unbiased the unbiased variance(s) if @c true or the Maximum-likelihood
 *  variance otherwise (the default)
 *  @return the variance(s) or NA if there is no available value
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp >::resultByColType
  varianceWithFixedMean(Derived const& A, typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByColType mean, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp>::ColOp(A)(mean, unbiased = false);}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp >::resultByRowType
varianceWithFixedMeanByRow(Derived const& A, typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByColType mean, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp>::RowOp(A)(mean, unbiased = false);}

/** @ingroup StatDesc
 *  Compute Safely the VarianceWithFixedMean(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param mean The mean (s) to use
 *  @param w the weights
 *  @param unbiased the unbiased variance(s) if @c true or the Maximum-likelihood
 *  variance otherwise (the default)
 *  @return the variance(s) or NA if there is no available value
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp >::resultByColType
  varianceWithFixedMean(Derived const& A, ExprBase<Weights> const& w, typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByColType mean, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp>::ColOp(A)(w, mean, unbiased = false);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp >::resultByRowType
varianceWithFixedMeanByRow(Derived const& A, ExprBase<Weights> const& w, typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByColType mean, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp>::RowOp(A)(w, mean, unbiased = false);}

/** @ingroup StatDesc
 *  Compute safely the VarianceWithFixedMean(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param mean The mean (s) to use
 *  @param unbiased the unbiased variance(s) if @c true or the Maximum-likelihood
 *  variance otherwise (the default)
 *  @return the variance(s) or NA if there is no available value
 **/
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanSafeOp >::resultByColType
varianceWithFixedMeanSafe(Derived const& A, typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp >::resultByColType mean, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanSafeOp>::ColOp(A)(mean, unbiased = false);}
template< class Derived>
typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanSafeOp >::resultByRowType
  varianceWithFixedMeanSafeByRow(Derived const& A, typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp >::resultByColType mean, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanSafeOp>::RowOp(A)(mean, unbiased = false);}

/** @ingroup StatDesc
 *  Compute safely the weighted VarianceWithFixedMean(s) value(s) of A.
 *  @sa STK::Stat::mean, STK::Stat::max, STK::Stat::variance, STK::Stat::varianceWithFixedMean, STK::Stat::min, STK::Stat::sum.
 *  @param A the data
 *  @param mean The mean (s) to use
 *  @param w the weights
 *  @param unbiased the unbiased variance(s) if @c true or the Maximum-likelihood
 *  variance otherwise (the default)
 *  @return the variance(s) or NA if there is no available value
 **/
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanSafeOp >::resultByColType
varianceWithFixedMeanSafe(Derived const& A, ExprBase<Weights> const& w, typename hidden::ApplyFunctorSelector<Derived, MeanSafeOp >::resultByColType mean, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanSafeOp>::ColOp(A)(w, mean, unbiased);}
template< class Derived, class Weights>
typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanSafeOp >::resultByRowType
  varianceWithFixedMeanSafeByRow(Derived const& A, ExprBase<Weights> const& w, typename hidden::ApplyFunctorSelector<Derived, MeanOp >::resultByColType mean, bool unbiased = false)
{ return typename hidden::ApplyFunctorSelector<Derived, VarianceWithFixedMeanOp>::RowOp(A)(w, mean, unbiased);}


}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_FUNCTORS_H*/
