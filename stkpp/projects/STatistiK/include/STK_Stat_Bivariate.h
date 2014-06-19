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
 * Project:  stkpp::StatDesc
 * Purpose:  Compute elementary statistics for two variables.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Stat_Bivariate.h
 *  @brief This file contain the declaration of the class Bivariate.
 **/

#ifndef STK_STAT_BIVARIATE_H
#define STK_STAT_BIVARIATE_H

#include "../include/STK_Stat_UnivariateReal.h"

namespace STK
{
namespace Stat
{

/** @ingroup StatDesc
 *  @brief Computation of the bivariate Statistics of two Variables.
 * 
 *  The template parameter TContainer1D is the type of container
 *  used for storing the data : It should derive from
 *  ITContainer and contain elements of type TYPE.
 * 
 *  The template parameter TYPE is the Type of the data to analyze.
 **/
template < class XTYPE, class YTYPE, class TContainer1D >
class Bivariate
{
  protected:
    /** Statistics of the X variable */
    Univariate< TContainer1D>    xStat_;
    /** Statistics of the Y variable */
    Univariate< TContainer1D>    yStat_;

  public:
    /** Default constructor. Compute the univariate statistics of the two
     *  variables.
     *  @param X the first variable
     *  @param Y the second variable
     **/
    Bivariate( ITContainer< TContainer1D> const& X
             , ITContainer< TContainer1D> const& Y
             )
             : xStat_(X)
             , yStat_(Y)
    {}

    /** Copy constructor
     *  @param stat the statistics to copy
     **/
    Bivariate( const Bivariate& stat)
              : xStat_(stat.xStat_)
              , yStat_(stat.yStat_)
    { ;}
    
    /** virtual destructor. **/
    virtual ~Bivariate() { ;}
  
    /** Operator = : overwrite the Bivariate with stat.
     *  @param stat the statistics to copy
     *  @return this object
     **/
    Bivariate& operator=( const Bivariate& stat)
    {
      xStat_ = stat.xStat_;
      yStat_ = stat.yStat_;
      return *this;
    }

    /** set a new data sets
     *  @param X the first variable
     *  @param Y the second variable
     * */
    void setData( TContainer1D const& X,  TContainer1D const& Y)
    {
      xStat_.setData(X);
      yStat_.setData(Y);
    }

    /** get statistics of the first variable */
    inline Univariate<XTYPE, TContainer1D> xStat() const {return xStat_;}
    /** get statistics of the second variable */
    inline Univariate<YTYPE, TContainer1D> yStat() const {return yStat_;}
 };
  
  
}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_BIVARIATE_H*/
