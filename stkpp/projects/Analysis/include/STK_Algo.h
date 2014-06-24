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
 * Project:  stkpp::STatistiK
 * Purpose:  Method implementing Algorithms.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Algo.h
 *  @brief In this file we give some templated generic algorithms.
 **/

#ifndef STK_ALGO_H
#define STK_ALGO_H

#include "STKernel/include/STK_Misc.h"
#include "STK_Const_Math.h"
#include "STK_ISerie.h"
#include "STK_IFunction.h"

namespace STK
{

/** @ingroup Analysis
 * @brief Sum an alternating series using the Chebichev polynomials.
 * 
 * Compute
 *  \f[
 *  S = \sum_{k=0}^{+\infty} (-1)^k a_k
 *  \f]
 * where the coefficients of the series are given or computed using the
 * parameter @c ISerie.
 *
 * The number of iterations is the first number such that
 *  \f[
 *      \frac{2}{(3+\sqrt(8))^n} < \epsilon,
 *  \f]
 * where \f$ \epsilon \f$ is the precision of the type Real.
 * 
 * @param f the ISerie giving the terms of the serie
 * @param n the number of iterations
**/
template <class Serie>
Real sumAlternateSerie(const ISerie<Serie>& f, int const& n = 0)
{
  // Compute the rate of convergence
  Real rate = 3.0 + 2.0 * Const::_SQRT2_;
  // compute the number of iterations if needed
  int niter = n;
  if (niter==0)
  {
    niter = (int )( log(Arithmetic<Real>::epsilon())
                  / (Const::_LN2_-log(rate))
                  ) + 1;
  }
  // initialization
  rate   = pow(rate, niter);
  Real b = -2.0/(rate+1.0/rate), c = -1.0, sum = 0.0;
  // first iteration (k = 0)
  sum += (c = b-c) * f.first();
  b   *= (- 2.*niter*niter);
  // iterations
  for (int k = 1; k<niter; k++)
  {
    sum += (c = b-c) * f.next();
    Real aux = k+1.;
    b   *=   ((k + niter)/(k + aux)) * ((k - niter)/aux) * 2.;
  }
  return sum;
}

/** @ingroup Analysis
 * @brief Sum a serie using the epsilon acceleration process.
 * 
 * Compute
 * \f[
 *  S = \sum_{k=0}^{+\infty} a_k
 *  \f]
 * where the coefficients of the series are given or computed
 * by the templated parameter Serie.
 *
 * The series should be monotone in absolute value.
 * We use the epsilon algorithm acceleration process with 6 epsilon.
 * 
 * @param f the functor giving the terms of the serie
 * @param iter_max the number of iterations
**/
template <class Serie>
Real sumSerie(const ISerie<Serie>& f, int const& iter_max = 10)
{
  Real e0[7]; // original serie
  Real e1[6];
  Real e2[5];
  Real e3[4];
  Real e4[3];
  Real e5[2];
  Real delta, sum; // e6

  e0[0] = f.first();        //f[0];
  e0[1] = e0[0] + f.next();
  e0[2] = e0[1] + f.next();
  e0[3] = e0[2] + f.next();
  e0[4] = e0[3] + f.next();
  e0[5] = e0[4] + f.next();
  e0[6] = e0[5] + f.next();
  
  // first epsilon e_{-1}[n] = 0
  e1[0] = 1/(e0[1] - e0[0]);
  e1[1] = 1/(e0[2] - e0[1]);
  e1[2] = 1/(e0[3] - e0[2]);
  e1[3] = 1/(e0[4] - e0[3]);
  e1[4] = 1/(e0[5] - e0[4]);
  e1[5] = 1/(e0[6] - e0[5]);
  
  // second epsilon
  e2[0] = e0[0] + 1/(e1[1] - e1[0]);
  e2[1] = e0[1] + 1/(e1[2] - e1[1]);
  e2[2] = e0[2] + 1/(e1[3] - e1[2]);
  e2[3] = e0[3] + 1/(e1[4] - e1[3]);
  e2[4] = e0[4] + 1/(e1[5] - e1[4]);

  // third epsilon
  e3[0] = e1[0] + 1/(e2[1] - e2[0]);
  e3[1] = e1[1] + 1/(e2[2] - e2[1]);
  e3[2] = e1[2] + 1/(e2[3] - e2[2]);
  e3[3] = e1[3] + 1/(e2[4] - e2[3]);

  // fourth epsilon
  e4[0] = e2[0] + 1/(e3[1] - e3[0]);
  e4[1] = e2[1] + 1/(e3[2] - e3[1]);
  e4[2] = e2[2] + 1/(e3[3] - e3[2]);

  // fifth epsilon
  e5[0] = e3[0] + 1/(e4[1] - e4[0]);
  e5[1] = e3[1] + 1/(e4[2] - e4[1]);

  // sum e6[0]
  sum = e4[0] + (delta = 1/(e5[1] - e5[0]));

  // main loop
  for (int n=1; n<=iter_max; n++)
  { // roll s0
    e0[4] = e0[5];
    e0[5] = e0[6];
    e0[6] += f.next();
  
    // roll first epsilon
    e1[3] = e1[4];
    e1[4] = e1[5];
    if ( (delta = (e0[6] - e0[5])) == 0) break;
    e1[5] = 1./delta;
  
    // roll second epsilon
    e2[2] = e2[3];
    e2[3] = e2[4];
    if ( (delta = (e1[5] - e1[4])) == 0) break;
    e2[4] = e0[4] + 1./delta;

    // roll third epsilon
    e3[1] = e3[2];
    e3[2] = e3[3];
    if ( (delta = (e2[4] - e2[3])) == 0) break;
    e3[3] = e1[3] + 1./delta;

    // roll fourth epsilon
    e4[0] = e4[1];
    e4[1] = e4[2];
    if ( (delta = (e3[3] - e3[2])) == 0) break;
    e4[2] = e2[2] + 1./delta;

    // roll fifth epsilon
    e5[0] = e5[1];
    if ( (delta = (e4[2] - e4[1])) == 0) break;
    e5[1] = e3[1] + 1./delta;

    // roll sixth epsilon and compute sum
    if ( (delta = (e5[1] - e5[0])) == 0) break;
    if (!Arithmetic<Real>::isFinite(delta = 1./delta)) break;
    // sum
    Real sum1 = e4[0] + delta;
    if (sum1 == sum) break;
    sum = sum1;
  }
  
  return sum;
}
/** @ingroup Analysis
 *  @brief Evaluate a continued fraction.
 * 
 * Compute
 * \f[
 *  S = \frac{a_1}{b_1+}\frac{a_2}{b_2+}\frac{a_3}{b_3+}\frac{a_4}{b_4+}
 *  \ldots
 *  \f]
 * where the coefficients of the series are given or computed
 * by the templated parameter Serie.
 * @param iter_max the number of iterations
 * @param a Denominator serie
 * @param b Numerator serie
**/
template <class Seriea, class Serieb>
Real continuedFraction( const ISerie<Seriea>& a
                      , const ISerie<Serieb>& b
                      , int const& iter_max = 100
                      )
{
  // initialize a_n
  Real an = a.first(); // a_1
  // note a_1 =0 => cf = 0
  if (an == 0.) return 0.;
  // initialize b_n
  Real bn = b.first(); // b_1
  // initialize numerator
  Real Nold = 0, Ncur = an; // = a_1
  // initialize denominator
  Real Dold = 1, Dcur = bn; // = b_1
  // result
  Real cf = 0.;
  // iterations
  for (int n=1; n<=iter_max; n++)
  {
    // compute a_n
    an = a.next();
    // check trivial case
    // (a_n =0) and (D_n = 0) => cf = +\infty
    if (an == 0.) return Ncur/Dcur;
    // update b_n
    bn = b.next();
    // compute numerator and denominator
    Real Nnew = bn * Ncur + an * Nold;
    Real Dnew = bn * Dcur + an * Dold;
    // update old numerator and denominator
    Nold = Ncur;
    Dold = Dcur;
    // update current numerator and denominator
    Ncur = Nnew;
    Dcur = Dnew;
    // normalize if necessary with 2^32
    if (std::abs(Dcur) > 4294967296.0)
    {
      Ncur /= Dcur;
      Nold /= Dcur;
      Dold /= Dcur;
      Dcur = 1.;
    }
    // normalize if necessary with 2^32
    if (std::abs(Ncur) > 4294967296.0)
    {
      Ncur /= 4294967296.0;
      Nold /= 4294967296.0;
      Dold /= 4294967296.0;
      Dcur /= 4294967296.0;
    }
    // if D_n not to small check cv
    if (std::abs(Dcur) != 0)
    {
      Real cfnew = Ncur/Dcur;
      // check cv
      if (std::abs(cf - cfnew) < std::abs(cfnew)*Arithmetic<Real>::epsilon())
        return cfnew;
      else
        cf = cfnew;
    }
  }
  // avoid warning at compilation
  return cf;
}

} // namespace STK

#endif /*STK_ALGO_H*/
