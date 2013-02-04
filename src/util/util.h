#ifndef UTIL_H_
#define UTIL_H_
/**@file UTIL.h
* @brief 
*/
#define STKPPCONTAINERS

#include "stkpp/include/Arrays.h"

#ifdef STKPPCONTAINERS
/**
 * General Matrix containers
 */
typedef  STK::CArrayXX MatrixReal;
typedef STK::CArray<STK::Integer,STK::Arrays::general_> MatrixInteger;
typedef STK::CArray<bool,STK::Arrays::general_> MatrixBinary;

/**
 * General Row Vector containers
 */

typedef STK::CVectorX RowVectorReal;
typedef STK::CArray<STK::Integer, STK::Arrays::vector_, STK::UnknownSize, 1, STK::Arrays::by_col_> RowVectorInteger;
typedef STK::CArray<bool, STK::Arrays::vector_, STK::UnknownSize, 1, STK::Arrays::by_col_> RowVectorBinary;

/**
 * General column Vector containers
 */

typedef STK::CPointX ColVectorReal;
typedef STK::CArray<STK::Integer, STK::Arrays::vector_, 1, STK::UnknownSize, STK::Arrays::by_col_> ColVectorInteger;
typedef STK::CArray<bool, STK::Arrays::vector_, 1, STK::UnknownSize,STK::Arrays::by_col_> ColVectorBinary;
#endif

#endif /* UTIL_H_ */
