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
 **/

/** @file SimulUtil.cpp
 *  @brief In this file we implement the mixture simulation utilities function.
 **/

#include "SimulUtil.h"
#include "STatistiK.h"

using namespace STK;
using namespace STK::Law;

/* Simulate a Categorical distribution */
void simulZiLaw( int K, Array2DVector<Real>& pk, bool fixed)
{
  pk.resize(K);
  if (fixed) { pk = 1./K;}
  else
  {
    pk.randUnif();
    pk /= pk.sum();
  }
}
/** Simulate zi */
void simulZi( int n, Array2DVector<Real> const& pk, Array2DVector<int>& zi)
{
  // get probabilites of the zi
  Categorical zilaw(pk);
  zi.resize(n);
  zilaw.randArray(zi);
}

/** Simulate a Categorical_pjk mixture model */
void simulCategorical_pkModel( int d, int K, int L, Array_pkProba& proba)
{
  // create arrays of porbabilities
  proba.resize(K);
  for (int k= baseIdx; k <= proba.lastIdx(); ++k)
  {
    proba[k].resize(L);
    proba[k].randUnif(); // generate numbers in ]0,1[
    proba[k] /= proba[k].sum(); // normalize to 1
  }
}

/** Simulate a Categorical_pjk mixture data set */
void simulCategorical_pkMixture( int n, int d, Array_pkProba const& proba, Array2DVector<Real> const& pk
                               , Array2D<int>& dataij, Array2DVector<int>& zi)
{
  simulZi(n, pk, zi);
  // create Categorical laws
  Array1D< Law::Categorical > law(proba.range());
  for (int k= baseIdx; k <= proba.lastIdx(); ++k)  { law[k].setProb(proba[k]);}
  // generate data
  dataij.resize(n, d);
  for (int i= baseIdx; i <= dataij.lastIdxRows(); ++i)
  {
    for (int j= baseIdx; j <= dataij.lastIdxCols(); ++j)
    { dataij(i,j) = law[zi[i]].rand();}
  }
}


/** Simulate a Categorical_pjk mixture model */
void simulGamma_bjModel( int d, int K, Array_bjParam& param)
{
  // create arrays of porbabilities
  param.resize(K);
  for (int k= baseIdx; k <= param.lastIdx(); ++k)
  {
    param[k][baseIdx]   = Law::Exponential::rand(10.);
    param[k][baseIdx+1] = Law::Exponential::rand(1.);
  }
}

/** Simulate a Categorical_pjk mixture data set */
void simulGamma_bjMixture( int n, int d, Array_bjParam const& param, Array2DVector<Real> const& pk
                         , Array2D<Real>& dataij, Array2DVector<int>& zi)
{
  simulZi(n, pk, zi);
  // create Categorical laws
  Array1D< Law::Gamma > law(param.range());
  for (int k= baseIdx; k <= param.lastIdx(); ++k)
  { law[k].setShape(param[k][baseIdx]);
    law[k].setScale(param[k][baseIdx+1]);
  }
  // generate data
  dataij.resize(n, d);
  for (int i= baseIdx; i <= dataij.lastIdxRows(); ++i)
  {
    for (int j= baseIdx; j <= dataij.lastIdxCols(); ++j)
    { dataij(i,j) = law[zi[i]].rand();}
  }
}

/** Simulate a Categorical_pjk mixture data set */
void simulGaussian_sModel( int d, int K, Array_bjParam& param)
{
  // create arrays of porbabilities
  param.resize(K);
  for (int k= baseIdx; k <= param.lastIdx(); ++k)
  {
    param[k][baseIdx]   = Law::Normal::rand(0.,10.);
    param[k][baseIdx+1] = Law::Exponential::rand(1.);
  }
}
/** Simulate a Categorical_pjk mixture data set */
void simulGaussian_sMixture( int n, int d, Array_sParam const& param, Array2DVector<Real> const& pk
                           , Array2D<Real>& dataij, Array2DVector<int>& zi)
{
  simulZi(n, pk, zi);
  // create Categorical laws
  Array1D< Law::Gaussian > law(param.range());
  for (int k= baseIdx; k <= param.lastIdx(); ++k)
  { law[k].setMu(param[k][baseIdx]);
    law[k].setSigma(param[k][baseIdx+1]);
  }
  // generate data
  dataij.resize(n, d);
  for (int i= baseIdx; i <= dataij.lastIdxRows(); ++i)
  {
    for (int j= baseIdx; j <= dataij.lastIdxCols(); ++j)
    { dataij(i,j) = law[zi[i]].rand();}
  }
}

