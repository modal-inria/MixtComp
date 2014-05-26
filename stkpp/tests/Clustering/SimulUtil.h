/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014  Serge Iovleff

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
 * Project:  stkpp::tests
 * created on: 6 mai 2014
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file SimulUtil.h
 *  @brief Common functions used for simulating mixture models
 **/

#ifndef SIMULUTIL_H
#define SIMULUTIL_H

#include "../../include/Arrays.h"

typedef STK::Array1D< STK::Array2DVector<STK::Real> > Array_pkProba; // Categorical
typedef STK::Array1D< STK::CArrayVector<STK::Real, 2> > Array_sParam; // Gaussian
typedef STK::Array1D< STK::CArrayVector<STK::Real, 2> > Array_bjParam; // Gamma

/** Simulate a Categorical_pjk mixture model */
void simulZiLaw( int K, STK::Array2DVector<STK::Real>& pk, bool fixed = false);
/** Simulate zi */
void simulZi( int n, STK::Array2DVector<STK::Real> const& pk, STK::Array2DVector<int>& zi);

/** Simulate a Categorical_pjk mixture model */
void simulCategorical_pkModel( int d, int K, int L, Array_pkProba& proba);
/** Simulate a Categorical_pjk mixture data set */
void simulCategorical_pkMixture( int n, int d, Array_pkProba const& proba, STK::Array2DVector<STK::Real> const& pk
                               , STK::Array2D<int>& dataij, STK::Array2DVector<int>& zi);


/** Simulate a Gamma_ajk_bj mixture model */
void simulGamma_bjModel( int d, int K, Array_bjParam& param);
/** Simulate a Gamma_ajk_bj mixture data set */
void simulGamma_bjMixture( int n, int d, Array_bjParam const& param, STK::Array2DVector<STK::Real> const& pk
                         , STK::Array2D<STK::Real>& dataij, STK::Array2DVector<int>& zi);

/** Simulate a Gaussian_s mixture model */
void simulGaussian_sModel( int d, int K, Array_bjParam& param);
/** Simulate a Gaussian_s  mixture data set */
void simulGaussian_sMixture( int n, int d, Array_sParam const& param, STK::Array2DVector<STK::Real> const& pk
                           , STK::Array2D<STK::Real>& dataij, STK::Array2DVector<int>& zi);

/** Add randomly missing values to the array dataij */
template<typename Type>
void simulMissing(STK::Real proba, STK::Array2D<Type>& dataij)
{
  STK::RandBase generator;
  for (int j= dataij.firstIdxCols(); j <= dataij.lastIdxCols(); ++j)
  {
    for (int i= dataij.firstIdxRows(); i <= dataij.lastIdxRows(); ++i)
    {
      if(generator.randUnif()<proba)
      { dataij(i,j) = STK::Arithmetic<Type>::NA();}
    }
  }
}
#endif /* SIMULUTIL_H */
