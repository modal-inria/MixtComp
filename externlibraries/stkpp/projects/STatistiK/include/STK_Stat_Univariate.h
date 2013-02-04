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
 * Project:  stkpp::StatDesc
 * Purpose:  Compute elementary 1D statistics for all variables.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Stat_Univariate.h
 *  @brief This file contain the declaration of the class Univariate.
 **/

#ifndef STK_STAT_UNIVARIATE_H
#define STK_STAT_UNIVARIATE_H

#include "../../STKernel/include/STK_Integer.h"
#include "../../STKernel/include/STK_Real.h"

namespace STK
{
namespace Stat
{

/** @ingroup StatDesc
 *  @brief Computation of the univariate Statistics of a Variable.
 * 
 *  This is a base class that should be specialized or sub-classed.
 *  The template parameter @c TContainer1D is the type of container
 *  used for storing the data : It should derive from
 *  ITContainer1D and contain elements of type TYPE.
 * 
 *  The template parameter TYPE is the Type of the data to analyze.
 **/
template < class TYPE, class TContainer1D >
class Univariate
{
  public:
    /** Default constructor
     *  Compute the number of available observations and of
     *  missing observations of the variable V.
     *  @param V variable to parse
     **/
    Univariate( ITContainer1D<TContainer1D> const& V )
              : nbSamples_(V.size())
              , nobs_(V.size())
              , nbMiss_(0)
    {
      // loop over all observations 
      for (int i=V.lastIdx(); i>=V.firstIdx(); --i)
        // not finite ?
        if (!Arithmetic<TYPE>::isFinite(V[i]))
        {
          nobs_--;         // decrease nbAvailableObs_
          nbMiss_++;        // increase nbMissingObs_
        }
     }

    /** Copy constructor
     *  @param stat the statistics to copy
     **/
    Univariate( Univariate const& stat)
              : nbSamples_(stat.nbSamples_)
              , nobs_(stat.nobs_)
              , nbMiss_(stat.nbMiss_)
    { ;}
    
    /** virtual destructor. */
    virtual ~Univariate() { ;}
  
    /** Operator = : overwrite the Univariate with stat.
     *  @param stat the Univariate object to copy
     *  @return this object
     **/
    Univariate& operator=( Univariate const& stat)
    {
      nbSamples_     = stat.nbSamples_;
      nobs_  = stat.nobs_;
      nbMiss_ = stat.nbMiss_;
      return *this;
    }

    /** set a new Variable.
     *  @param V variable to parse
     **/
    void setData( TContainer1D const& V)
    {
      nbSamples_     = V.size();
      nobs_  = V.size();
      nbMiss_ = 0;
      // loop over all observations 
      for (int i=V.lastIdx(); i>=V.firstIdx(); i--)
        if (!Arithmetic<TYPE>::isFinite(V[i])) // not finite ?
        {
          nobs_--;         // decrease nbAvailableObs_
          nbMiss_++;        // increase nbMissingObs_
        }
    }

    /** get the number of observations
     *  @return the number of samples
     **/
    inline int const& nbSamples() const {return nbSamples_;}
    /** get the number of available samples (not missing)
     *  @return the number of available samples
     **/
    inline int const& nbAvailableObs() const {return nobs_;}
    /** get the number of missing samples
     *  @return the number of missing values
     **/
    inline int const& nbMissingSamples() const {return nbMiss_;}

  protected:
   int    nbSamples_;     ///< Number of observations
   int    nobs_;         ///< Number of Available observations
   int    nbMiss_;       ///< Number of missing observations
};
  
  
}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_UNIVARIATE_H*/
