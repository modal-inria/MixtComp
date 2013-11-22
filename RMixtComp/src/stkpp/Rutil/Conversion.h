/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  rtkpp
 * Purpose:  Define tools for converting R data to C data.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/


#ifndef CONVERSION_H
#define CONVERSION_H

#include <Rcpp.h>

namespace Conversion
{

/** Convert a Rcpp matrix to a stk++ Matrix.
 *  The package have to be build with the flag STKBASEARRAY=0.
 *  @param input the matrix to convert
 *  @param output the result
 */
template<class inp,class out>
inline void convertMatrix( inp const& input, out& output)
{
  int rows = input.rows();
  int cols = input.cols();

  output.resize(rows,cols);
   for(int i=0;i<rows;i++)
   { for(int j=0;j<cols;j++)
     { output(i,j) = input(i,j);}
   }
}

/** Convert a Rcpp vector to a stk++ vector.
 *  The package have to be build with the flag STKBASEARRAY=0.
 *  @param input the vector to convert
 *  @param output the result
 */
template<class inp,class out>
inline void convertvector( inp const& input,out& output)
{
    int len = input.size();
    output.resize(len);
    for (int i = 0; i < len; ++i)
    { output[i] = input[i];}
}

/** Convert a C matrix in a Rcpp Matrix
 *  @param nbRow number of rows of the matrix
 *  @param nbCol number of column of the matrix
 *  @param input the matrix to convert
 *  @return the Rcpp Matrix
 */
Rcpp::NumericMatrix CMatrixToRcppMatrix(int nbRow, int nbCol, double** input);

/** Convert a C integer matrix in a Rcpp Vector
 *  @param nbRow number of rows of the matrix
 *  @param nbCol number of column of the matrix
 *  @param input the C matrix of integer to convert
 *  @return the Rcpp Vector
 */
Rcpp::NumericMatrix CMatrixToRcppMatrix(int nbRow, int nbCol, int** input);

/** Convert a C vector in a Rcpp Vector
 *  @param dim the dimension of the vector
 *  @param input the C vector
 *  @return the Rcpp Vector
 */
Rcpp::NumericVector CVectorToRcppVector(int dim, double* input);

/** Convert a C integer vector in a Rcpp Vector
 *  @param dim the dimension of the input vector
 *  @param input the C integer vector
 *  @return A Rcpp matrix with the partition of in a binary form
 */
Rcpp::NumericVector CVectorToRcppVector(int dim, int* input);
}

#endif /* CONVERSION_H */
