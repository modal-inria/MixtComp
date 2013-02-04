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
 * Project: stkpp::STatistiK::Statdesc
 * Purpose: Perform the usual transformation on data set.
 * Author:  Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Stat_Transform.h
 *  @brief In this file we implement the main transformation on data set.
 **/

#ifndef STK_STAT_TRANSFORM_H
#define STK_STAT_TRANSFORM_H

#include "../../STKernel/include/STK_Arithmetic.h"
#include "../../STKernel/include/STK_Misc.h"

#include "../../Arrays/include/STK_ITContainer.h"

#include "STK_Stat_UnivariateReal.h"

namespace STK
{
namespace Stat
{

/** Compute the mean of the variables in the container V and center V.
 *  @param V the container with the Data
 *  @param mean the Vector of the means
 **/
template < class Row, class TContainer2D >
void center( TContainer2D&  V, Row& mean)
{
  mean.resize(V.cols());
  for (int j=  V.firstIdxCols(); j<= V.lastIdxCols(); j++)
  {
    // compute mean
    Real mu = Stat::mean(V.col(j));
    // save current mean
    mean[j] = (mu = Arithmetic<Real>::isFinite(mu) ?  mu : 0.);
    // translate data
    for (int i= V.firstIdxRows(); i<= V.lastIdxRows(); i++)
    { V(i,j) -= mu;}
  }
}

/** Compute the mean of the variables in the container V and center V.
 *  @param V the container with the Data
 *  @param W the Vector of the weights
 *  @param mean the Vector of the means
 **/
template < class Row, class Column, class TContainer2D >
void center( TContainer2D& V, Column const& W, Row& mean)
{
#ifdef STK_DEBUG
  if (!V.rows().isIn(W.range()))
    throw runtime_error(_T("In center(V, W, mean) "
                           "V.range() not include in w.range()"));
#endif
  // create result
  mean.resize(V.cols());
  for (int j= V.firstIdxCols(); j<= V.lastIdxCols(); j++)
  {
    // compute mean
    Real mu = Stat::mean<Column>(V.col(j), W);
    // save current mean
    mean[j] = (mu = Arithmetic<Real>::isFinite(mu) ?  mu : 0.);
    // translate data
    for (int i= V.firstIdxRows(); i<= V.lastIdxRows(); i++)
    { V(i,j) -= mu;}
  }
}

/** Compute the mean and the variance of the variable V and standardize it.
 *  @param V the container with the Data
 *  @param mean the Vector of the means
 *  @param std the Vector of the standard deviation
 **/
template < class Row, class TContainer2D >
void standardize( TContainer2D& V, Row& mean, Row& std)
{
    // create result
    mean.resize(V.cols());
    std.resize(V.cols());
    // center
    center(V, mean);
    for (int j= V.firstIdxCols(); j<= V.lastIdxCols(); j++)
    {
      // compute standard deviation
      Real dev = Stat::variance(V.col(j));
      // take square root and save result
      std[j] = (dev = Arithmetic<Real>::isFinite(dev) ?  sqrt((double)dev) : 0.);
      // standardize data
      if (dev)
      {
        for (int i= V.firstIdxRows(); i<= V.lastIdxRows(); i++)
        { V(i,j) /= dev;}
      }
    }
}

/** Compute the mean and the variance of the variable V and standardize it.
 *  @param V the container with the Data
 *  @param W the Vector of the weights
 *  @param mean the Vector of the means
 *  @param std the Vector of the standard deviation
 **/
template < class Row, class Column, class TContainer2D >
void standardize( TContainer2D& V, Column const& W, Row& mean
                , Row& std
                )
{
#ifdef STK_DEBUG
  if (!V.rows().isIn(W.range()))
    throw runtime_error(_T("In standardize(V, W, mean) "
                           "V.range() not include in w.range()"));
#endif
  // create results
  mean.resize(V.cols());
  std.resize(V.cols());
  // center
  center(V, W, mean);
  for (int j= V.firstIdxCols(); j<= V.lastIdxCols(); j++)
  {
    // compute standard deviation
    Real dev = Stat::variance(V.col(j), W);
    // take square root and save result
    std[j] = (dev = Arithmetic<Real>::isFinite(dev) ?  sqrt((double)dev) : 0.);
    // standardize data
    if (dev)
    {
      for (int i= V.firstIdxRows(); i<= V.lastIdxRows(); i++)
      { V(i,j) /= dev;}
    }
  }
}

/** Add the mean of the centered variables in the container V.
 *  @param V the container with the data
 *  @param mean the Vector of the means
 **/
template < class Row, class TContainer2D >
void decenter( TContainer2D&  V, Row const& mean)
{
  if (V.cols() != mean.range())
    throw runtime_error(_T("Error in Stat::decenter(V, mean): "
                             "ranges are not the same."));
  // get dimensions
  const int  firstRow = V.firstIdxRows(), lastRow = V.lastIdxRows();
  const int  firstVar = V.firstIdxCols(), lastVar = V.lastIdxCols();
  for (int j= firstVar; j<= lastVar; j++)
  {
    // translate data
    Real mu = mean[j];
    for (int i= firstRow; i<= lastRow; i++)
    { V(i,j) += mu;}
  }
}

/** undo the standardization of the standardized variable V.
 *  @param V the container with the Data
 *  @param std the Vector of the standard deviation
 **/
template < class Row, class TContainer2D >
void destandardize( TContainer2D& V, Row const& std)
{
    if (V.cols() != std.range())
      throw runtime_error(_T("Error in Stat::decenter(V, std): "
                               "ranges are not the same."));
  // get dimensions
  const int  firstRow = V.firstIdxRows(), lastRow = V.lastIdxRows();
  const int  firstVar = V.firstIdxCols(), lastVar = V.lastIdxCols();
  for (int j= firstVar; j<= lastVar; j++)
  {
    // dilate
    if (std[j])
    {
      Real dev = std[j];
      // get dimensions
      for (int i= firstRow; i<= lastRow; i++)
      { V(i,j) *= dev;}
    }
  }
}

/** undo the standardization of the standardized variable V
 *  @param V the container with the Data
 *  @param mean the Vector of the means
 *  @param std the Vector of the standard deviation
 **/
template < class Row, class TContainer2D >
void destandardize( TContainer2D& V, Row const& mean, Row const& std)
{
  // dilate
  destandardize(V, std);
  // decenter
  decenter(V, mean);
}

}  // namespace Stat

}  // namespace STK

#endif /*STK_STAT_TRANSFORM_H*/
