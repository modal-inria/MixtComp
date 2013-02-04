/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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
 * Project:  stkpp::Arrays
 * created on: 17 févr. 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Arrays_Util.h
 *  @brief In this file we define utilities for the Array classes.
 *
 *  @namespace Arrays the namespace Arrays contain all the internal stuff needed
 *  by The Arrays* classes.
 *
 **/


#ifndef STK_ARRAY_UTIL_H
#define STK_ARRAY_UTIL_H

#include "../../STKernel/include/STK_Range.h"

namespace STK
{

namespace Arrays
{

/** Return n+m, where n is the first number such that m < 2^n.
  *  @param m the size of the container
 **/
int evalCapacity(int const& m);

/** Return n+m, where n is the first number such that m < 2^n.
  *  @param I the range of the container
 **/
Range evalCapacity(Range const& I);


/** Intrinsic dimension of the container : 1D, 2D, 3D or 4D. Higher
 *  dimensions are not allowed. */
enum Dimension
{
  _0D_ = 0, ///< a single scalar have no dimension
  _1D_ = 1,
  _2D_ = 2,
  _3D_ = 3,
  _4D_ = 4
};

/** Dimension of the operand in a BinaryOperator. The
 *  mixed cases are for the diagonal matrices which are like
 *  1D containers but acts as 2D containers. */
enum BinaryDim
{
  bin0D_ = 0,
  bin1D_ = 1,
  bin2D_ = 2,
  bin1D2D_,
  bin2D1D_
};

/** Define the Storage Orientation of the container */
enum Orientation
{
  by_row_ =0,  ///< by_row_  storage by row
  by_col_ =1   ///< by_col_  storage by column
};

/** Define the different type of Array that can be handle */
enum Storage
{
  dense_,      ///< dense_  dense array
  sparse_      ///< sparse_  sparse array
};

/** Define the different structure of Arrays that can be handle */
enum Structure
{
  general_ =0 ,       ///< general_ a general matrix
  square_,            ///< square_ a square matrix
  diagonal_,          ///< diagonal_ a diagonal matrix
  lower_triangular_,  ///< lower_triangular_ a lower triangular matrix
  upper_triangular_,  ///< upper_triangular_ a upper triangular matrix
  vector_,            ///< vector_  one column array
  point_,             ///< point_   one row array
  number_,            ///< number_  1x1 array (like a number)
  expression_         ///< An expression that will be evaluated further
};

} // namespace Arrays

} // namespace STK



#endif /* STK_ARRAY_UTIL_H */
