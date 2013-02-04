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
 * Project:  stkpp::STatistiK::StatDesc
 * Purpose:  Compute elementary 1D statistics for all variables.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Stat_Univariate.h
 *  @brief This file contain the specialization of the class Univariate
 *  for the Real Type.
 **/

#ifndef STK_STAT_UNIVARIATEREAL_H
#define STK_STAT_UNIVARIATEREAL_H

#include "../../Arrays/include/STK_ITContainer.h"
#include "../../DManager/include/STK_HeapSort.h"

#include "STK_Stat_Univariate.h"

namespace STK
{
namespace Stat
{

/** @ingroup StatDesc
 *  @brief Computation of the univariate Statistics of a Real Variable.
 *
 *  This is a specialization of the class Univariate for the type @c Real.
 *
 *  The template parameter @c TContainer1D is the type of container
 *  used for storing the data : It should derive from @c ITContainer and
 *  contain elements of type Real.
 *
 *  This specialization propose also statics methods for computing
 *  the (weighted) mean and the (weighted) variance.
 **/
template < class TContainer1D >
class Univariate<Real, TContainer1D>
{
  public:
    /** Default constructor
     *  Copy locally the variable V and set dimensions. The statistics
     *  are then computed.
     *  @param V variable
     *  @param sorted @c true if the variable is sorted, @c false otherwise
     **/
    Univariate( TContainer1D const& V, bool sorted = false)
              : nbSamples_(V.size())
              , nbAvailable_(V.size())
              , nbMiss_(0)
              , V_(V.asDerived())
              , W_()
              , weighted_(false)
              , sorted_(sorted)
              , comporder_(false)
              , compstat_(false)
              , sumweights_(0.)
              , sum2weights_(0.)
              , mean_(Arithmetic<Real>::NA())
              , median_(Arithmetic<Real>::NA())
              , var_(Arithmetic<Real>::NA())
              , uvar_(Arithmetic<Real>::NA())
              , std_(Arithmetic<Real>::NA())
              , ustd_(Arithmetic<Real>::NA())
              , mad_(Arithmetic<Real>::NA())
              , kurtosis_(Arithmetic<Real>::NA())
              , skewness_(Arithmetic<Real>::NA())
              , quartiles_(3, Arithmetic<Real>::NA(), String(_T("Quartiles")))
              , deciles_(9, Arithmetic<Real>::NA(), String(_T("Deciles")))
              , viceciles_(19, Arithmetic<Real>::NA(), String(_T("Viceciles")))
              , percentiles_(99, Arithmetic<Real>::NA(), String(_T("Percentiles")))
    {
      initializeVariable();
      compOrderStatistics();
      compStatistics();
    }

    /** Default constructor for weighted variables.
     *  Copy locally the variable V, the weights W and set dimensions.
     *  No statistics except min_, max_ and amax_ are calculated at
     *  this stage.
     *  @param V the variable
     *  @param W the weights
     *  @param sorted @c true if the variable is sorted, @c false otherwise
     **/
    Univariate( TContainer1D const& V
              , TContainer1D const& W
              , bool sorted = false
              )
              : nbSamples_(V.size())
              , nbAvailable_(V.size())
              , nbMiss_(0)
              , V_(V.asDerived())
              , W_(W.asDerived())
              , weighted_(true)
              , sorted_(sorted)
              , comporder_(false)
              , compstat_(false)
              , sumweights_(0.0)
              , sum2weights_(0.0)
              , mean_(Arithmetic<Real>::NA())
              , median_(Arithmetic<Real>::NA())
              , var_(Arithmetic<Real>::NA())
              , uvar_(Arithmetic<Real>::NA())
              , std_(Arithmetic<Real>::NA())
              , ustd_(Arithmetic<Real>::NA())
              , mad_(Arithmetic<Real>::NA())
              , kurtosis_(Arithmetic<Real>::NA())
              , skewness_(Arithmetic<Real>::NA())
              , quartiles_(3)
              , deciles_(9)
              , viceciles_(19)
              , percentiles_(99)
    {
      // check weights size
      if ((V_.range() != W_.range()))
      {  throw runtime_error("Univariate<Real, TContainer1D>::Univariate(V, W, sorted) "
                                  "V and W have not the same range");
      }
      initializeVariableAndWeights();
      compOrderStatistics();
      compWeightedStatistics();
    }

    /** Copy constructor
     *  @param stat the univariate statistics to copy
     **/
    Univariate( const Univariate& stat)
              : nbSamples_(stat.nbSamples_)
              , nbAvailable_(stat.nbAvailable_)
              , nbMiss_(stat.nbMiss_)
              , V_(stat.V_)
              , W_(stat.W_)
              , weighted_(stat.weighted_)
              , sorted_(stat.sorted_)
              , comporder_(stat.comporder_)
              , compstat_(stat.compstat_)
              , sumweights_(stat.sumweights_)
              , sum2weights_(stat.sum2weights_)
              , min_(stat.min_)
              , max_(stat.max_)
              , amax_(stat.amax_)
              , mean_(stat.mean_)
              , median_(stat.median_)
              , var_(stat.var_)
              , uvar_(stat.uvar_)
              , std_(stat.std_)
              , ustd_(stat.ustd_)
              , mad_(stat.mad_)
              , kurtosis_(stat.kurtosis_)
              , skewness_(stat.skewness_)
              , quartiles_(stat.quartiles_)
              , deciles_(stat.deciles_)
              , viceciles_(stat.viceciles_)
              , percentiles_(stat.percentiles_)
    { ;}

    /** destructor. */
    ~Univariate() { ;}

    /** Operator = : overwrite the Univariate with stat. */
    Univariate& operator=( const Univariate& stat)
    {
      nbSamples_ = stat.nbSamples_;
      nbAvailable_ = stat.nbAvailable_;
      nbMiss_ = stat.nbMiss_;
      V_           = stat.V_;
      W_           = stat.W_;
      weighted_    = stat.weighted_;
      sorted_      = stat.sorted_;
      comporder_   = stat.comporder_;
      compstat_    = stat.compstat_;
      sumweights_  = stat.sumweights_;
      sum2weights_ = stat.sum2weights_;
      min_         = stat.min_;
      max_         = stat.max_;
      amax_        = stat.amax_;
      mean_        = stat.mean_;
      median_      = stat.median_;
      var_         = stat.var_;
      uvar_        = stat.uvar_;
      std_         = stat.std_;
      ustd_        = stat.ustd_;
      mad_         = stat.mad_;
      kurtosis_    = stat.kurtosis_;
      skewness_    = stat.skewness_;
      quartiles_   = stat.quartiles_;
      deciles_     = stat.deciles_;
      viceciles_   = stat.viceciles_;
      percentiles_ = stat.percentiles_;

      return *this;
    }

    /** Compute the statistics of a new Variable.
     *  @param V the variable to set
     *  @param sorted true if the variable is sorted
     **/
    void setData( TContainer1D const& V, bool sorted = false)
    {
      // initialize dimensions
      nbSamples_    = V.size();
      nbAvailable_  = V.size();
      nbMiss_ = 0;
      // set variable and weights
      V_ = V.asDerived();
      W_.clear();  // no weights
      // initialize data
      weighted_    = false;
      sorted_      = sorted;
      comporder_   = false;
      compstat_    = false;
      sumweights_     = 0.0;
      sum2weights_    = 0.0;
      mean_        = Arithmetic<Real>::NA();
      median_      = Arithmetic<Real>::NA();
      var_         = Arithmetic<Real>::NA();
      uvar_        = Arithmetic<Real>::NA();
      std_         = Arithmetic<Real>::NA();
      ustd_        = Arithmetic<Real>::NA();
      mad_         = Arithmetic<Real>::NA();
      kurtosis_    = Arithmetic<Real>::NA();
      skewness_    = Arithmetic<Real>::NA();
      quartiles_   = Arithmetic<Real>::NA();
      deciles_     = Arithmetic<Real>::NA();
      viceciles_   = Arithmetic<Real>::NA();
      percentiles_ = Arithmetic<Real>::NA();

      initializeVariable();
      compOrderStatistics();
      compStatistics();
    }

    /** Compute the statistics of a new weighted Variable.
     *  @param V the variable to set
     *  @param W the weights of the variable
     *  @param sorted true if the variable is sorted
     **/
    void setData( ITContainer< TContainer1D> const& V
                , ITContainer< TContainer1D> const& W
                , bool sorted = false
                )
    {
      // check weights size
      if ((V.range() != W.range()))
      {  throw runtime_error("Univariate<Real>::setData(V, W, sorted) "
                                  "V and W have not the same range");
      }
      // initialize dimensions
      nbSamples_     = V.size();
      nbAvailable_  = V.size();
      nbMiss_ = 0;
      // set variable and weights
      V_ = V.asDerived();
      W_ = W.asDerived();
      // initialize data
      weighted_    = true;
      sorted_      = sorted;
      comporder_   = false;
      compstat_    = false;
      sumweights_  = 0.0;
      sum2weights_ = 0.0;
      mean_        = Arithmetic<Real>::NA();
      median_      = Arithmetic<Real>::NA();
      var_         = Arithmetic<Real>::NA();
      uvar_        = Arithmetic<Real>::NA();
      std_         = Arithmetic<Real>::NA();
      ustd_        = Arithmetic<Real>::NA();
      mad_         = Arithmetic<Real>::NA();
      kurtosis_    = Arithmetic<Real>::NA();
      skewness_    = Arithmetic<Real>::NA();
      quartiles_   = Arithmetic<Real>::NA();
      deciles_     = Arithmetic<Real>::NA();
      viceciles_   = Arithmetic<Real>::NA();
      percentiles_ = Arithmetic<Real>::NA();

      initializeVariableAndWeights();
      compOrderStatistics();
      compWeightedStatistics();
    }

    /** get the number of samples */
    inline const int nbSamples() const {return nbSamples_;}
    /** get the number of available samples (not missing) */
    inline const int nbAvailableSamples() const {return nbAvailable_;}
    /** get the number of missing samples */
    inline const int nbMissingSamples() const {return nbMiss_;}

    /** get the min of the variable */
    inline const Real min() const {return min_;}
    /** get the max of the variable */
    inline const Real max() const {return max_;}
    /** get the absolute maximal value */
    inline const Real aMax() const {return amax_;}

    /** get the mean of the variable */
    inline const Real mean() const {return mean_;}
    /** get the median of the variable */
    inline const Real median() const {return median_;}

    /** get the variance of the variable (division by n) */
    inline const Real variance() const {return var_;}
    /** get the unbiased Variance of the variable (division by n-1) */
    inline const Real unbiasedVariance() const {return uvar_;}
    /** get the standard deviation of the variable */
    inline const Real std() const {return std_;}
    /** get the sample standard deviation of the variable */
    inline const Real unbiasedStd() const {return ustd_;}
    /** get the median absolute deviation of the variable */
    inline const Real mad() const { return( mad_);}

    /** get the kurtosis of the variable */
    inline const Real kurtosis() const {return kurtosis_;}
    /** get the skewness of the variable */
    inline const Real skewness() const {return skewness_;}

    /** get the quartiles of the variable (25%) */
    inline TContainer1D const& quartiles() const {return quartiles_;}
    /** get the deciles of the varibales (10%) */
    inline TContainer1D const& deciles() const {return deciles_;}
    /** get the viceciles of the variable (5%) */
    inline TContainer1D const& viceciles() const { return viceciles_;}
    /** get the percentiles of the variable (1%) */
    inline TContainer1D const& percentiles() const { return percentiles_;}

  private:
    /** Initialize the container in order to discard the missing
     *  elements in the non weighted case.
     **/
    void initializeVariable()
    {
      // initialize the extreme values
      min_ =  Arithmetic<Real>::max();
      max_ = -Arithmetic<Real>::max();
      // discard missing values
      for (int i=V_.lastIdx(); i>=V_.firstIdx(); i--)
      {
        if ( Arithmetic<Real>::isNA(V_[i]) )
        {
          // if NA
          nbAvailable_--;         // decrease nbAvailable_
          nbMiss_++;        // increase nbMiss_
          V_.erase(i);    // Delete the current row
        }
        else
        {
          min_ = std::min(V_[i], min_);    // update min_
          max_ = std::max(V_[i], max_);    // update max_
        }
      }
      // all weights are 1/n.
      sumweights_ = 1.0;
      sum2weights_= 1./(Real)nbAvailable_;

      // no samples
      if ((nbAvailable_ == 0))
      {
        min_  = Arithmetic<Real>::NA();
        max_  = Arithmetic<Real>::NA();
        amax_ = Arithmetic<Real>::NA();
      }
      else // one or more samples : compute amax
        amax_ = std::max(std::abs(min_), std::abs(max_));
    }

    /** Initialize the containers in order to discard the missing
     *  elements in the weighted case.
     **/
    void initializeVariableAndWeights()
    {
      // initialize the extreme values
      min_ =  Arithmetic<Real>::max();
      max_ = -Arithmetic<Real>::max();

      // discard missing values or values without weights
      for (int i=V_.lastIdx(); i>=V_.firstIdx(); i--)
      {
        if ( Arithmetic<Real>::isNA(V_[i])||Arithmetic<Real>::isNA(W_[i]))
        {
          V_.erase(i); // Delete the current row
          W_.erase(i); // for the weights too
          nbAvailable_--;         // decrease number of samples
          nbMiss_++;        // increase number of missing values
        }
        else
        {
          min_       = std::min(V_[i], min_);    // update min_
          max_       = std::max(V_[i], max_);    // update max_
          sumweights_  += (W_[i] = std::abs((Real)W_[i]));// sum absolute weights
          sum2weights_ += W_[i] * W_[i];     // sum squared weights
        }
      }
      // no samples
      if ((nbAvailable_ == 0))
      {
        min_  = Arithmetic<Real>::NA();
        max_  = Arithmetic<Real>::NA();
        amax_ = Arithmetic<Real>::NA();
      }
      else // one or more samples : compute amax
        amax_ = std::max(std::abs(min_), std::abs(max_));
    }

    /** Compute the usual statistics of the variable:
     *  - mean the empirical mean of the variable
     *  - var_ the variance of of the variable (division by n)
     *  - uvar_ the sample variance of the variable (division by n-1)
     *  - std_ the standard deviation of the variable (n)
     *  - ustd_ the sample standard deviation of the variable (n-1)
     *  - mad_ the absolute deviation from the mean of the variable
     *   */
    void compStatistics()
    {
      // If there is no samples
      if (nbAvailable_ == 0)
      {
        mean_     = Arithmetic<Real>::NA();
        var_      = Arithmetic<Real>::NA();
        uvar_     = Arithmetic<Real>::NA();
        std_      = Arithmetic<Real>::NA();
        ustd_     = Arithmetic<Real>::NA();
        mad_      = Arithmetic<Real>::NA();
        kurtosis_ = Arithmetic<Real>::NA();
        skewness_ = Arithmetic<Real>::NA();
        return;
      }

      // One observation
      if (nbAvailable_ == 1)
      {
        mean_     = V_.front();
        var_      = Arithmetic<Real>::NA();
        std_      = 0.0;
        ustd_     = Arithmetic<Real>::NA();
        mad_      = 0.0;
        kurtosis_ = 0.0;
        skewness_ = 0.0;
        return;
      }
      // get indexes
      // get nbAvailable observation in Real
      const Real n = (Real)nbAvailable_;
      // first pass : compute the mean
      // scale the samples for preventing overflows
      mean_ = 0.;
      {
        if (amax_) // if the absolute maximal value is greater than 0
        {
          // sum samples with scaling
          for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
            mean_ += V_[i]/amax_;
          // divide by the number of available observation
          mean_ /= n;
          // unscale
          mean_ *= amax_;
        }
      }
      // second pass : compute the variance, skewness and kurtosis
      // initialize values
      Real sum = 0.0, dev1, dev2;
      var_  = 0.0;
      mad_  = 0.0;
      kurtosis_ = 0.0;
      skewness_ = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        sum       += (dev1 = V_[i]-mean_); // deviation from the mean
        var_      += (dev2 = dev1*dev1);   // squared deviation
        mad_      += std::abs( dev1 );          // absolute deviation
        skewness_ += dev2 * dev1;          // cubic deviation
        kurtosis_ += dev2 * dev2;          // squared squared deviation
      }
      mad_ *= sum2weights_;
      uvar_ = (var_ - sum*sum/n)/(n - 1.);
      var_  = (var_ - sum*sum/n)/n;
      std_  = sqrt((double)var_);
      ustd_ = sqrt((double)uvar_);
     // if there is variance
      if (var_)
      {
        skewness_ /= n;
        kurtosis_ /= n;

        skewness_ /= (var_ * std_);
        kurtosis_ /= (var_ * var_);
        kurtosis_ -= 3.0;
      }
      else
      {
        skewness_ = 0.0;
        kurtosis_ = 0.0;
      }
      // Statistics are computed
      compstat_ = true;
    }

    /** Compute the usual weighted statistics of the variable */
    void compWeightedStatistics()
    {
      // If there is no samples
      if (nbAvailable_ == 0)
      {
        mean_     = Arithmetic<Real>::NA();
        var_      = Arithmetic<Real>::NA();
        uvar_     = Arithmetic<Real>::NA();
        std_      = Arithmetic<Real>::NA();
        ustd_     = Arithmetic<Real>::NA();
        mad_      = Arithmetic<Real>::NA();
        kurtosis_ = Arithmetic<Real>::NA();
        skewness_ = Arithmetic<Real>::NA();
        return;
      }

      // One observation or pathological weights
      if ((nbAvailable_ == 1)||(sumweights_*sumweights_ <= sum2weights_))
      {
        mean_     = V_.front();
        var_      = 0.;
        uvar_     = Arithmetic<Real>::NA();
        std_      = 0.;
        ustd_     = Arithmetic<Real>::NA();
        mad_      = 0.;
        kurtosis_ = 0.;
        skewness_ = 0.;
        return;
      }
      // first pass : get the mean
      // scale the samples for preventing overflows
      mean_ = 0.;
      if (amax_) // if the maximal value is greater than 0
       {
        // sum samples
        for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
          mean_ += (W_[i]*V_[i])/amax_; // compute the mean with scaling
        mean_ /= sumweights_;           // weight the sum
        mean_ *= amax_;                 // and unscale
      }
      // second pass : compute the variance, skewness and kurtosis
      // initialize some values
      Real sum = 0.0, dev1, dev2;
      var_  = 0.0;
      mad_  = 0.0;
      kurtosis_ = 0.0;
      skewness_ = 0.0;
      for (int i=V_.firstIdx(); i<=V_.lastIdx(); i++)
      {
        Real weight = W_[i];
        sum       += weight * (dev1 = V_[i]-mean_); // deviation from the mean
        var_      += weight * (dev2 = dev1*dev1);   // squared value
        mad_      += weight * std::abs(dev1);       // absolute deviation from mean
        skewness_ += weight * dev2 * dev1;
        kurtosis_ += weight * dev2 * dev2;
      }
      mad_ /= sumweights_;
      uvar_ = (var_ - sum*sum/sumweights_)/(sumweights_ - sum2weights_/sumweights_);
      var_  = (var_ - sum*sum)/(sumweights_);
      std_  = sqrt((double)var_);
      ustd_ = sqrt((double)uvar_);
      // if there is variance
      if (var_)
      {
        skewness_ /= (sumweights_ * var_ * std_);
        kurtosis_ /= (sumweights_ * var_ * var_);
        kurtosis_ -= 3.0;
      }
      else
      {
        skewness_ = 0.0;
        kurtosis_ = 0.0;
      }
      // Stat are computed
      compstat_ = true;
    }

    /** Compute the Order Statistics of the variable */
    void compOrderStatistics()
    {
      // no samples
      if (nbAvailable_ == 0)
      {
        median_      = Arithmetic<Real>::NA();
        quartiles_   = Arithmetic<Real>::NA();
        deciles_     = Arithmetic<Real>::NA();
        viceciles_   = Arithmetic<Real>::NA();
        percentiles_ = Arithmetic<Real>::NA();
        return;
      }

      // One observation
      if (nbAvailable_ == 1)
      {
        median_      = max_;
        quartiles_   = max_;
        deciles_     = max_;
        viceciles_   = max_;
        percentiles_ = max_;
        return;
      }
      // sort values
      if (!sorted_)
      {
        // if the Variable is not weighted, we can sort it directly
        if (!weighted_) heapSort(V_);
        else // otherwise we have to sort V_and W_ using indirection
        {
          Array1D<int> I; // auxiliary Array for indirection
          heapSort(I, V_);
          applySort1D(V_, I);
          applySort1D(W_, I);
        }
        sorted_ = true;
      }
      // Find the Quantiles of the distribution
      compQuantiles(quartiles_);
      compQuantiles(deciles_);
      compQuantiles(viceciles_);
      compQuantiles(percentiles_);
      // get the median
      median_ = quartiles_[2];
    }

    /** Compute the quantiles of the sorted variable V_ and store the
     *  result in the array T. The number of quantiles is given by the
     *  size of T. For exemple, if T.lastIdx() == 9, compQuantiles will
     *  return the deciles.
     *  @param T TContainer1Ds of the *tiles
     **/
    void compQuantiles(TContainer1D& T)
    {
       if (!sorted_)
      {  STKRUNTIME_ERROR_NO_ARG(Univariate<Real>::comptiles(T),V_ is not sorted);}
      // in case of
      T.shift(1);
      // number of quantiles
      int  nt = T.size(), shift = V_.firstIdx()-1;
      Real n1 = Real(nbAvailable_+1), nt1 = Real(nt+1);

      for (int j=T.firstIdx(), k=1; j<=T.lastIdx(); j++, k++)
      {
        // find index of the k-th quantile
        Real    find  = Real(k)*n1/nt1;  // compute the index in Real
        int tind  = std::max(int(1), int(find));  // in int
        int tind1 = std::min(nbAvailable_, tind+1);       // next
        Real    aux   = find - Real(tind); // lower ponderation

        // nbAvailable_+1 not perfectly divisible ? weighting...
        aux ? T[j] = aux * V_[shift+tind] + (1.0-aux)*V_[shift+tind1]
            : T[j] = V_[shift+tind];
      }
    }

  protected:
    // dimensions
    int    nbSamples_;    ///< Number of samples
    int    nbAvailable_; ///< Number of Available samples
    int    nbMiss_;      ///< Number of missing samples

    // containers
    TContainer1D V_;      ///< local copy of the variable
    TContainer1D W_;            ///< local copy of the weights

    // Some flag about the internal state of the object
    bool weighted_;       ///< Samples are weighted ?
    bool sorted_;         ///< Samples are sorted ?
    bool comporder_;      ///< Orders Statistics are computed ?
    bool compstat_;       ///< Usuals Statistics are computed ?

    // statistics
    Real sumweights_;        ///< Sum of the weights
    Real sum2weights_;       ///< Sum of the square of the weights
    Real min_;            ///< Min of the variable
    Real max_;            ///< Max of the variable
    Real amax_;           ///< Absolute maximal value

    Real mean_;           ///< mean of the variable
    Real median_;         ///< median of the variable
    Real var_;            ///< Variance of the variable (division by n)
    Real uvar_;           ///< Unbiased Variance of the variable (division by n-1)
    Real std_;            ///< Standard deviation of the variable (n)
    Real ustd_;           ///< Sample standard deviation of the variable (n-1)
    Real mad_;            ///< absolute deviation of the variable
    Real kurtosis_;       ///< kurtosis of the variable
    Real skewness_;       ///< Skewness of the variable

    TContainer1D quartiles_;    ///< Quartiles (25%)
    TContainer1D deciles_;      ///< Deciles (10%)
    TContainer1D viceciles_;    ///< viceciles (5%)
    TContainer1D percentiles_;  ///< percentiles (1%)
};

/**  @ingroup StatDesc
 *  Compute the mean of the variable V.
 *  \f[ \hat{\mu} = \frac{1}{n}
 *                  \sum_{i=1}^n V(i)
 *  \f]
 *  @param V the variable
 **/
template<class TContainer1D>
Real mean( TContainer1D const&  V)
{
  // no samples
  if (V.empty())
  { return Arithmetic<Real>::NA();}

  // get dimensions
  int nobs = V.size();
  // sum the samples
  Real sum  = 0.0;
  for (int i=V.firstIdx(); i<=V.lastIdx(); i++)
  { if (Arithmetic<Real>::isFinite(V[i]))
      sum += V[i];
    else
      nobs--;
  }
  // compute the mean
  if (nobs) return sum /= (Real)nobs;
  else      return Arithmetic<Real>::NA();
}

/**  @ingroup StatDesc
 *  Compute the minimal value of the variable V.
 *  @param V the variable
 *  @return the minimal value of the variable V
 **/
template<class TContainer1D>
Real min( TContainer1D const&  V)
{
  // no samples
  if (V.empty())
  { return Arithmetic<Real>::NA();}

  // get dimensions
  int first = V.firstIdx(), last = V.lastIdx();
  // compute the min
  Real min  = Arithmetic<Real>::infinity();
  for (int i=first; i<=last; i++)
  {
    if (Arithmetic<Real>::isFinite(V[i]))
      min = std::min(min, V[i]);
  }
  // return the result
  return min;
}

/**  @ingroup StatDesc
 *  Compute the maximal value of the variable V.
 *  @param V the variable
 *  @return the maximal value of the variable V
 **/
template<class TContainer1D>
Real max( TContainer1D const&  V)
{
  // no samples
  if (V.empty())
  { return Arithmetic<Real>::NA();}

  // get dimensions
  int first = V.firstIdx(), last = V.lastIdx();
  // compute the min
  Real max  = - Arithmetic<Real>::infinity();
  for (int i=first; i<=last; i++)
  {
    if (Arithmetic<Real>::isFinite(V[i]))
      max = std::max(max, V[i]);
  }
  // return the result
  return max;
}

/** @ingroup StatDesc
 *  Compute the (weighted) mean of the variable V
 *  \f[ \hat{\mu} = \frac{1}{\sum_{i=1}^n W(i)}
 *                  \sum_{i=1}^n W(i) V(i).
 *  \f]
 *  If the range of the weights does not match the range
 *  of the variable the method return the usual mean.
 *  @param V variable
 *  @param W weights
 **/
template<class TContainer1D>
Real mean( TContainer1D const&  V
         , TContainer1D const& W
         )
{
  // no samples
  if (V.empty()) { return Arithmetic<Real>::NA();}

  // get dimensions
  Real sum  = 0.0;
  // if the weight are not of the same size, ignore them
  if (V.range() != W.range()) return mean(V);

  // sum the weighted samples
  Real nweight = 0.0;
  for (int i=V.firstIdx(); i<=V.lastIdx(); i++)
  { if ( (Arithmetic<Real>::isFinite(V[i]))
       &&(Arithmetic<Real>::isFinite(W[i]))
       )
    {
      Real weight  = std::abs(W[i]);
      nweight  += weight;
      sum      += weight * V[i];
    }
  }
  // compute the mean
  return (nweight) ? sum /= nweight : 0.;
}

/** @ingroup StatDesc
 *  Compute the variance of the variable V with fixed mean.
 *  \f[ \hat{\mu} = \frac{1}{n}
 *                  \sum_{i=1}^n (V(i) - \mu)^2.
 *  \f]
 *  @param V variable
 *  @param mu the mean
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template<class TContainer1D>
Real varianceWithFixedMean( TContainer1D const& V
                          , Real const& mu
                          , bool unbiased = false
                          )
{
  // no samples
  if (V.empty()) { return Arithmetic<Real>::NA();}

  // get dimensions
  const int  first = V.firstIdx(), last = V.lastIdx();
  int nobs = V.size();
  // sum
  Real sum  = 0.0, var  = 0.0, dev;
  for (int i=first; i<=last; i++)
  {
    if (Arithmetic<Real>::isFinite(V[i]))
    {
      sum += (dev = V[i] - mu); // deviation from the mean
      var += (dev*dev);         // squared value
    }
    else nobs--;
  }
  // compute the variance
  if (unbiased)
  {
    return (nobs > 1) ? (var - (sum*sum)/(Real)nobs)/(Real)(nobs -1)
                      : Arithmetic<Real>::NA();

  }
  // variance
  return (nobs > 0) ? (var - (sum*sum)/(Real)nobs)/(Real)(nobs)
                    : Arithmetic<Real>::NA();
}

/** Compute the weighted variance of the variable V with fixed mean.
 *  \f[ \hat{\mu} = \frac{1}{\sum_{i=1}^n W(i)}
 *                  \sum_{i=1}^n W(i) (V(i) - \mu)^2
 *  \f]
 *  @param V variable
 *  @param W weights
 *  @param mu the mean
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template<class TContainer1D>
Real varianceWithFixedMean( TContainer1D const& V
                          , TContainer1D const& W
                          , Real const& mu
                          , bool unbiased = false
                          )
{
  // no samples
  if (V.empty()) { return Arithmetic<Real>::NA();}

  // if the weight are not of the same size, ignore them
  if (V.range() != W.range()) return varianceWithFixedMean(V, mu);

  // get dimensions
  const int  first = V.firstIdx(), last = V.lastIdx();
  // sum
  Real dev, sum = 0.0, var = 0.0, nweight = 0.0, nweight2 = 0.0;
  for (int i=first; i<=last; i++)
  { if ( Arithmetic<Real>::isFinite(V[i]) && Arithmetic<Real>::isFinite(W[i]) )
    {
      Real weight = std::abs(W[i]);
      nweight    += weight;
      nweight2   += weight * weight;
      sum        += weight*(dev = V[i]-mu); // deviation from the mean
      var        += weight*(dev*dev);       // squared value
    }
  }
  // compute the variance
  if (unbiased)
  {
    return (nweight*nweight - nweight2 > 0.) ? (var - sum*sum/nweight)/(nweight - nweight2/nweight)
                                             : Arithmetic<Real>::NA();

  }
  return (nweight) ? (var - sum*sum)/(nweight) : Arithmetic<Real>::NA();
}

/** @ingroup StatDesc
 *  Compute the variance of the variable V.
 *  \f[ \hat{\sigma}^2 = \frac{1}{n}
 *                       \sum_{i=1}^n (V(i)-\hat{\mu})^2.
 *  \f]
 *  @param V variable
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template<class TContainer1D>
Real variance( TContainer1D const& V, bool unbiased = false)
{
    // no samples
    if (V.empty()) { return Arithmetic<Real>::NA();}
    
    // Compute the mean
    Real mu = mean<TContainer1D>(V);
    
    // get dimensions
    const int  first = V.firstIdx(), last = V.lastIdx();
    int nobs = V.size();
    // sum
    Real sum  = 0.0, var  = 0.0, dev;
    for (int i=first; i<=last; i++)
    {
        if (Arithmetic<Real>::isFinite(V[i]))
        {
            sum += (dev = V[i] - mu); // deviation from the mean
            var += (dev*dev);         // squared value
        }
        else nobs--;
    }
    // compute the variance
    if (unbiased)
    {
        return (nobs > 1) ? (var - (sum*sum)/(Real)nobs)/(Real)(nobs -1)
        : Arithmetic<Real>::NA();
        
    }
    return (nobs > 0) ? (var - (sum*sum)/(Real)nobs)/(Real)(nobs)
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
 *  @param V variable
 *  @param W weights
 *  @param unbiased @c true if we want an unbiased estimator of the variance,
 *  @c false otherwise (default is @c false)
 **/
template<class TContainer1D>
Real variance( TContainer1D const& V
             , TContainer1D const& W
             , bool unbiased = false
             )
{
    // no samples
    if (V.empty()) { return Arithmetic<Real>::NA();}
    
    // Compute the mean if necessary
    Real mu = mean(V, W);
    
    // if the weight are not of the same size, ignore them
    if (V.range() != W.range()) return varianceWithFixedMean(V, mu);
    
    // get dimensions
    const int  first = V.firstIdx(), last = V.lastIdx();
    // sum
    Real dev, sum = 0.0, var = 0.0, nweight = 0.0, nweight2 = 0.0;
    for (int i=first; i<=last; i++)
    { if ( Arithmetic<Real>::isFinite(V[i]) && Arithmetic<Real>::isFinite(W[i]) )
    {
        Real weight = std::abs(W[i]);
        nweight    += weight;
        nweight2   += weight * weight;
        sum        += weight*(dev = V[i]-mu); // deviation from the mean
        var        += weight*(dev*dev);       // squared value
    }
    }
    // compute the variance
    if (unbiased)
    {
        return (nweight*nweight - nweight2 > 0.) ? (var - sum*sum/nweight)/(nweight - nweight2/nweight)
        : Arithmetic<Real>::NA();
        
    }
    return (nweight) ? (var - sum*sum)/(nweight) : Arithmetic<Real>::NA();
}
    

}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_UNIVARIATEREAL_H*/
