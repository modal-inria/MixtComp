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
 * Project:  stkpp::StatistiK::StatDesc
 * Purpose:  Compute multivariate elementary statistics for a 2D container.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Stat_Multivariate.h
 *  @brief This file contain the declaration of the base class Multivariate.
 **/

#ifndef STK_STAT_MULTIVARIATE_H
#define STK_STAT_MULTIVARIATE_H

#include "Arrays/include/STK_Array2DVector.h"
#include "Sdk/include/STK_IRunner.h"

namespace STK
{
namespace Stat
{
/** @ingroup StatDesc
 *  @brief Computation of the multivariate statistics of a Variable.
 *
 *  The template parameter @c Array is the type of container
 *  used for storing the data : It should derive from
 *  ITContainer and contain elements of type Type.
 **/
template < class Array, class WColVector, typename Type =  typename Array::Type >
class Multivariate : public IRunnerUnsupervised< Array, WColVector>
{
  public:
    typedef IRunnerUnsupervised< Array, WColVector> Runner2D;
    /** Constructor.
     *  Data set are initialized but no computation is done
     *  in this constructor. Statistics of the number of missing data
     *  and available data are delegated to derived classes.
     *  @param p_data the data set
     **/
    Multivariate( Array const* p_data)
                : Runner2D(p_data)
                , nbSamples_(0)
                , nbVar_(0)
                , nbMiss_()
    {}

    /** Constructor.
     *  Data set are initialized but no computation is done
     *  in this constructor. Statistics of the number of missing data
     *  and available data are delegated to derived classes.
     *  @param data the data set
     **/
    Multivariate( Array const& data)
                : Runner2D(data)
                , nbSamples_()
                , nbVar_()
                , nbMiss_()
    {}
    /** virtual destructor. */
    virtual ~Multivariate() {}
    /** @return the number of samples in the data set (the number of rows) */
    inline int nbSamples() const {return nbSamples_;}
    /** @return the number of variables in the data set (the number of columns) */
    inline int nbVariable() const {return nbVar_;}
    /** Number of missing values
     *  @return An array with the number of missing values for each variables
     *  of the data set
     **/
    inline Array1D<int> const& nbMissingSamples() const {return nbMiss_;}
    /** @return An array with the number of observed values for each variables
     *  of the data set (not missing).
     **/
    inline Array1D<int> const& nbAvailableSamples() const {return nbAvailable_;}

    /** run the estimation of the Multivariate statistics. **/
    virtual bool run()
    {
      if (!this->p_data_)
      { this->msg_error_ = STKERROR_NO_ARG(Multivariate<Array>::run(),data are not set);
        return false;
      }
      nbSamples_ = this->p_data_->sizeRows();
      nbVar_     = this->p_data_->sizeCols();
      nbMiss_.resize(this->p_data_->cols());
      nbAvailable_.resize(this->p_data_->cols());
      // for each variables
      for (int j= this->p_data_->beginCols(); j<= this->p_data_->lastIdxCols(); j++)
      {
        // number of observations
        int nobs = nbSamples_;
        // compute the mean
        for (int i= this->p_data_->beginRows(); i<= this->p_data_->lastIdxRows(); i++)
        {  if (!Arithmetic<Type>::isFinite((*this->p_data_)(i,j))) nobs--;}
        nbAvailable_[j] = nobs;
        nbMiss_[j] = nbSamples_ - nobs;
      }
      return true;
    }
    /** run the estimation of the weighted multivariate statistics.
     *  @param weights the weights of the samples
     **/
    virtual bool run( WColVector const& weights)
    {
      if (!this->p_data_)
      { this->msg_error_ = STKERROR_NO_ARG(Multivariate<Array>::run(),data are not set);
        return false;
      }
      return run();
    }

  protected:
    /** number of samples */
    int nbSamples_;
    /** Number of variables */
    int nbVar_;
    /** number of missing data of each variables */
    Array1D<int> nbMiss_;
    /** number of observed data of each variables */
    Array1D<int> nbAvailable_;
};

}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_MULTIVARIATE_H*/
