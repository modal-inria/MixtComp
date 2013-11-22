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
 * Project:  stkpp::Rtools
 * Purpose:  Define tools for converting R data to C data.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

#include "Conversion.h"

namespace Conversion
{


//To spin off double** in Rcpp::NumericMatrix
Rcpp::NumericMatrix CMatrixToRcppMatrix(int nbRow, int nbCol, double** input)
{
  //NumericMatrix for input
  Rcpp::NumericMatrix output(nbRow,nbCol);
  for(int i=0;i<nbRow;i++)
  {
    for(int j=0;j<nbCol;j++)
    {
      output(i,j) = input[i][j];
    }
  }
  return output;
}


//To spin off int** in Rcpp::NumericMatrix
Rcpp::NumericMatrix CMatrixToRcppMatrix(int nbRow, int nbCol, int** input)
{
    //NumericMatrix for input
    Rcpp::NumericMatrix output(nbRow,nbCol);
    for(int i=0;i<nbRow;i++)
    {
      for(int j=0;j<nbCol;j++)
      {
        output(i,j) = input[i][j];
      }
    }
    return output;
}

//To spin off double* in Rcpp::NumericVector
Rcpp::NumericVector CVectorToRcppVector(int dim, double* input)
{
  // NumericVector for input
  Rcpp::NumericVector output(dim);
  for(int i=0;i<dim;i++)
  {
      output(i) = input[i];
  }
  return output;
}

//To spin off int* in Rcpp::NumericVector
Rcpp::NumericVector CVectorToRcppVector(int dim, int* input)
{
  // NumericVector for input
  Rcpp::NumericVector output(dim);
  for(int i=0;i<dim;i++)
  {
      output(i) = input[i];
  }
  return output;
}

}


















