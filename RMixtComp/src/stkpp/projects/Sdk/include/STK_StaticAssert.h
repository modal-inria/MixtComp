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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::STKernel
 * created on: 9 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_StaticAssert.h
 *  @brief In this file we define a StaticAssert convenience structure
 *  for error checking at compile time.
 **/


#ifndef STK_STATICASSERT_H
#define STK_STATICASSERT_H

#if(__cplusplus >= 201100L)

// if native static_assert is enabled, let's use it
#include <type_traits>
#define STK_STATICASSERT(X,MSG) static_assert(X,#MSG);

#else // not C++11

#define STK_STATICASSERT(COND,MSG) \
if (STK::StaticAssert<bool(COND)>::MSG) {}

namespace STK
{

template<bool condition> struct StaticAssert;

// if the condition is false the compiler will complain, ,it does not find the message
template<> struct StaticAssert<false> {};
template<> struct StaticAssert<true>
{
  enum
  {
    YOU_TRIED_CALLING_A_VECTOR_METHOD_ON_A_MATRIX,
    YOU_TRIED_TO_USE_A_UNIDIMENSIONAL_METHOD_ON_A_MATRIX,
    YOU_TRIED_TO_CONSTRUCT_A_REFERENCE_WITH_A_DIFFERENT_ORIENTATION,
    YOU_TRIED_TO_CONSTRUCT_A_SQUAREMATRIX_WITH_DIFFERENT_DIMENSIONS,
    YOU_TRIED_TO_CONSTRUCT_A_VECTOR_WITH_MORE_THAN_ONE_COLUM,
    YOU_TRIED_TO_CONSTRUCT_A_POINT_WITH_MORE_THAN_ONE_ROW,
    YOU_TRIED_TO_CONSTRUCT_A_SCALAR_WITH_MORE_THAN_ONE_ROW_OR_ONE_COLUMN,
    YOU_TRIED_TO_APPLY_A_BINARY_OPERATOR_WITH_WRONG_ROWS_SIZE,
    YOU_TRIED_TO_APPLY_A_BINARY_OPERATOR_WITH_WRONG_COLS_SIZE,
    YOU_TRIED_TO_APPLY_A_BINARY_OPERATOR_BETWEEN_NOT_COMPATIBLE_ARRAYS,
    YOU_TRIED_TO_ASSIGN_A_NOT_COMPATIBLE_ARRAY,
    YOU_TRIED_TO_APPLY_A_METHOD_BETWEEN_NOT_COMPATIBLE_ARRAYS,
    YOU_TRIED_TO_APPLY_A_PRODUCT_OPERATOR_BETWEEN_NOT_COMPATIBLE_ARRAYS,
    YOU_TRIED_TO_APPLY_A_BINARY_OPERATOR_WITH_WRONG_DIMENSIONS_SIZE,
    INVALID_VECTOR_VECTOR_PRODUCT,
    INVALID_POINT_POINT_PRODUCT
  };
};

} // namespace STK

#endif // not C++11

#define STK_STATICASSERT_SAME_ORIENTATION(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_CONSTRUCT_A_REFERENCE_WITH_A_DIFFERENT_ORIENTATION)

#define STK_STATICASSERT_DIMENSIONS_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_APPLY_A_METHOD_BETWEEN_NOT_COMPATIBLE_ARRAYS)

#define STK_STATICASSERT_SQUARE_SIZES_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_CONSTRUCT_A_SQUAREMATRIX_WITH_DIFFERENT_DIMENSIONS)

#define STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_CONSTRUCT_A_VECTOR_WITH_MORE_THAN_ONE_COLUM)

#define STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_CONSTRUCT_A_POINT_WITH_MORE_THAN_ONE_ROW)

#define STK_STATICASSERT_SCALAR_SIZE_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_CONSTRUCT_A_SCALAR_WITH_MORE_THAN_ONE_ROW_OR_ONE_COLUMN)

#define STK_STATICASSERT_ROWS_DIMENSIONS_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_APPLY_A_BINARY_OPERATOR_WITH_WRONG_ROWS_SIZE)

#define STK_STATICASSERT_COLS_DIMENSIONS_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_APPLY_A_BINARY_OPERATOR_WITH_WRONG_COLS_SIZE)

#define STK_STATICASSERT_BINARY_OPERATOR_DIMENSIONS_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_APPLY_A_BINARY_OPERATOR_WITH_WRONG_DIMENSIONS_SIZE)

#define STK_STATICASSERT_BINARY_OPERATOR_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_APPLY_A_BINARY_OPERATOR_BETWEEN_NOT_COMPATIBLE_ARRAYS)

#define STK_STATICASSERT_PRODUCT_OPERATOR_MISMATCH(COND) \
    STK_STATICASSERT(COND,YOU_TRIED_TO_APPLY_A_PRODUCT_OPERATOR_BETWEEN_NOT_COMPATIBLE_ARRAYS)

#define STK_STATICASSERT_VECTOR_ONLY(EXPR) \
STK_STATICASSERT((EXPR::structure_==(int)Arrays::vector_)||(EXPR::structure_==(int)Arrays::point_),YOU_TRIED_CALLING_A_VECTOR_METHOD_ON_A_MATRIX)

#define STK_STATICASSERT_ONE_DIMENSION_ONLY(EXPR) \
STK_STATICASSERT((EXPR::structure_==(int)Arrays::vector_)||(EXPR::structure_==(int)Arrays::point_)||(EXPR::structure_==(int)Arrays::diagonal_),YOU_TRIED_TO_USE_A_UNIDIMENSIONAL_METHOD_ON_A_MATRIX)


#endif /* STK_STATICASSERT_H */
