/*--------------------------------------------------------------------*/
/*  Copyright (C) 2004-2007  Serge Iovleff
    
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
 * Project:  Analysis
 * Purpose:  Implementation of functions around the gamma function ratio
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Funct_gammaRatio.cpp
 *  @brief In this file we implement functions around the gamma
 *  function ratio.
 **/

#include "../include/STK_Funct_gammaRatio.h"

#include "../include/STK_ISerie.h"
#include "../include/STK_Algo.h"
#include "../include/STK_Funct_gamma.h"
#include "../include/STK_Funct_util.h"
#include "../include/STK_Funct_poisson_raw.h"
#include "../include/STK_Funct_raw.h"

#include "STKernel/include/STK_Exceptions.h"

namespace STK
{

namespace Funct
{
/** @ingroup Analysis
 *  @brief Compute the poisson density up to a factor.

 *  Compute the function:
 *  \f[
 *    R(a, b) = e^{-b} \frac{b^a}{\Gamma(a)}
 *            = a p(a, b)
 *  \f]
 * where p(a,b) denote the poisson density.
 * We use the partial deviance.
 * @see http://www.herine.net/stat/software/dbinom.html
 * b should be greater than zero.
 * @param a first parameter
 * @param b second parameter
 **/
static Real apois(Real const& a, Real const& b)
{
  // special value
  if (b==0.) return( 0. );
  // stirling approximation and deviance
  return( sqrt(a)*exp(-gammaLnStirlingError(a)-dev0(a, b))* (Const::_1_SQRT2PI_));
}

/** @ingroup Analysis
 * This Serie computes
 * \f[
 *    \frac{x^n}{(a+n)n!}
 * \f]
 * and is used in gammaRatio_dl.
 */
class Seriedl : public ISerie<Seriedl>
{ 
  private:
    Real const& x_;
    mutable int n_;
    mutable Real xn_factn_;// x^n/n!
    mutable Real aplusn_; // a+n
  
  public:
    /** constructor */
    inline Seriedl( Real const& a, Real const& x)
                  : x_(x), n_(1), xn_factn_(x), aplusn_(a+1.)
    {}
    /** compute the first term of the serie */
    inline Real firstImpl() const { return x_/aplusn_;}
    /** compute the next terms */
    inline Real nextImpl() const
    {
      // n and x^n/ n!
      xn_factn_ *= (x_/(++n_));
      // x^n/((a+n)n!)
      return xn_factn_/(++aplusn_);
    }
};

/** @ingroup Analysis
 *  @brief Compute the incomplete gamma function ratio Q(a,x)
 *  using the Taylor serie development representation
 *  \f[ Q(a, x) = 1- \frac{x^a}{\Gamma(a+1)} - \frac{x^a}{\Gamma(a)}
 *      \sum_{n=1}^\infty
 *      (-1)^n\frac{x^n}{(a+n)n!}
 *  \f]
 *  @see Abramovitz and Stegun [6.5.29]
 *  This representation is well behaved for x<=1.
 * 
 *  In order to avoid loss of significant digit near 1, we evaluate
 *  \f$ 1- \exp\{a\ln(x)-\ln(\Gamma(a+1))\} \f$ using the function
 *  @c expm1 if we want to compute the upper tail.
 *  @param a parameter of the gamma ratio function
 *  @param x value to evaluate the gamma ratio function
 *  @param lower_tail @c true if we want the lower tail, @c false otherwise
 **/
static Real gammaRatio_dl( Real const& a, Real const& x, bool lower_tail)
{
  Seriedl f(a, x);
  if (lower_tail)
     return (1.- a*sumAlternateSerie(f))*poisson_pdf_raw(a, x)*exp(x);
  else
    return apois(a, x) * sumAlternateSerie(f) * exp(x)
         - expm1(a*log(x)-gammaLn(a+1.));
}

/** @ingroup Analysis
 *  @brief Compute the incomplete gamma function ratio Q(a, x)
 *  using its continued fraction representation.
 *
 *  Compute the incomplete gamma function ratio Q(a, x)
 *  using the continued fraction representation
 *  \f[ Q(a, x) = \frac{e^{-x} x^a}{\Gamma(a)}
 *      \left(
 *      \frac{1}{x+1-a-} \frac{1.(1-a)}{x+3-a-} \frac{2.(2-a)}{x+5-a-}
 *      \ldots
 *      \right)
 *  \f]
 *  This cf is well behaved for almost all value of a and x.
 *  @param a parameter of the gamma ratio function
 *  @param x value to evaluate the gamma ratio function
 *  @param lower_tail @c true if we want the lower tail, @c false otherwise
 **/
static Real gammaRatio_cf( Real const& a, Real const& x, bool lower_tail)
{
  // initialize b_n
  Real bn = x + 1. -a; // b_1
  // initialize numerator
  Real Nold = 0, Ncur = 1.; // = a_1 = 1
  // initialize denominator
  Real Dold = 1, Dcur = bn; // = b_1

  // normalize if necessary by 2^32
  if (std::abs(Dcur) > 4294967296.0)
  {
    Ncur /= Dcur;
    Dold /= Dcur;
    Dcur = 1.;
  }

  // auxiliary variables
  Real aminusn = a;
  // result
  Real cf = Ncur/Dcur;
  // iterations
  for (int n=1; ; n++)
  {
    // compute a_n
    Real an = n*(--aminusn);
    // check trivial case
    // (note a_n =0 => b_n != 0 for x>0 and thus cf!=N_n/D_n exists)
    if (an == 0.) break;
    // update b_n
    bn += 2.;
    // compute numerator and denominator
    Real anew = bn * Ncur + an * Nold;
    Nold = Ncur;
    Ncur = anew;
    anew = bn * Dcur + an * Dold;
    Dold = Dcur;
    Dcur = anew;
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
    // test D_n not too small
    if (std::abs(Dcur) != 0)
    {
      Real cfold = cf;
      cf = Ncur/Dcur;
      // check cv: relative; absolute for small cf
      if (std::abs(cf - cfold) < std::abs(cf)*Arithmetic<Real>::epsilon())
        break;
    }
  }
  // cv
  return (lower_tail) ? (1 - apois(a, x)*cf)
                      : (apois(a, x)*cf);
}
 
/** @ingroup Analysis
 *  @brief Compute the incomplete gamma function ratio P(a,x)
 *  using the serie development representation.
 *
 *  Compute the incomplete gamma function ratio P(a,x)
 *  using its serie development representation
 *  \f[ P(a, x) = \frac{e^{-x} x^a}{\Gamma(a+1)}
 *      \sum_{n=0}^\infty
 *      \frac{1}{(a+1) \ldots (a+n)} x^n
 *  \f]
 *  @param a parameter of the gamma ratio function
 *  @param x value to evaluate the gamma ratio function
 *  @param lower_tail @c true if we want the lower tail, @c false otherwise
 **/
static Real gammaRatio_sr( Real const& a, Real const& x, bool lower_tail)
{
  Real y = a+1., term = x / (a+1.),  sum = term;
  /* sum =  1+\sum_{n=1}^\infty x^n / (a+1)*(a+2)*...*(a+n)) */
  do
  {
    y++;
    sum += (term *= x / y);
  }
  while (term > sum * Arithmetic<Real>::epsilon());
  sum+=1;

  if (lower_tail)
    return poisson_pdf_raw(a, x)*sum;
  else
    return 1.-poisson_pdf_raw(a, x)*sum;
}

/** @ingroup Analysis
 *  @brief Compute the incomplete gamma function ratio Q(a,x)
 *  using its asymptotic expansion.
 * 
 * Compute the incomplete gamma function ratio Q(a,x)
 * using its asymptotic expansion
 *  \f[ Q(a, x) =
 *      \frac{e^{-x} x^{a-1}}{\Gamma(a)}
 *      \left[ 1+
 *      \sum_{n=1}^{N-1}
 *      \frac{(a-1)\ldots (a-n)}{x^n}
 *      +\frac{(a+1) \ldots (a+N)\theta_N}{x^N}
 *      \right]
 *  \f]
 * with \f$ \theta_N < \frac{x}{x-(a_N)}\f$.
 * We must have a>1 and x>=a : no check is done.
 * @param a parameter of the gamma ratio function
 * @param x value to evaluate the gamma ratio function
 *  @param lower_tail @c true if we want the lower tail, @c false otherwise
 **/
static Real gammaRatio_ae( Real const& a, Real const& x, bool lower_tail)
{
  Real term = 1, sum = 0, b=a-1;
  // sum =  1+\sum_{n=1}^\infty (a-1)*...*(a - n) / x^n
  while ((b > 1.) && (term > sum * Arithmetic<Real>::epsilon()))
  {
    sum += (term *= b / x);
    b--;
  }
  sum+=1.;
  // achieved cv
  if (b>1.)
    return lower_tail ? (1.-poisson_pdf_raw(a-1, x)*sum)
                      : (   poisson_pdf_raw(a-1, x)*sum);
  // case (b>x) and (b <=1)
  if (b>x)
    return lower_tail ? (gammaRatio_sr(b, x, lower_tail)-poisson_pdf_raw(a-1, x)*sum)
                      : (gammaRatio_sr(b, x, lower_tail)+poisson_pdf_raw(a-1, x)*sum);
  // case (b <x) and (b <=1)
  return lower_tail ? gammaRatio_cf(b, x, lower_tail)-poisson_pdf_raw(a-1, x)*sum
                    : gammaRatio_cf(b, x, lower_tail)+poisson_pdf_raw(a-1, x)*sum;
}
 
/** @ingroup Analysis
 *  @brief Compute the incomplete gamma function ratio P(a,x)
 *  using the Poisson asymptotic expansion.
 * 
 * Compute the incomplete gamma function ratio P(a,x)
 * using the asymptotic expansion of the poisson distribution
 * \f[
 *  \Gamma(a+1, a+d)
 * \f]
 * @param a1 parameter (a-1) of the gamma ratio function
 * @param apd value to evaluate the gamma ratio function
 *  @param lower_tail @c true if we want the lower tail, @c false otherwise
 **/
static inline Real poisson_ae( Real const& a1, Real const& apd, bool lower_tail = true)
{
  // odd coefficients
  static const Real coefs_i[8] =
  {
    0., /* placeholder used for 1-indexing */
          2/3.,
         -4/135.,
          8/2835.,
         16/8505.,
      -8992/12629925.,
    -334144/492567075.,
     698752/1477701225.
  };
  // stirling coefficients
  static const Real coefs_s[8] =
  {
    0., /* placeholder used for 1-indexing */
             1/12.,
             1/288.,
          -139/51840.,
          -571/2488320.,
        163879/209018880.,
       5246819/75246796800.,
    -534703531/902961561600.
  };
  // compute D = -x(log(1+d/x) - d/x) = xlog(x/(x+d))+(x+d)-x 
  Real D   = dev0(a1, apd);
  // compute sqrt(2D)
  Real sqrt2D = sqrt (2. * D);
  // compute D/x
  Real D_x = D/a1;

  // treat negative difference
  if (apd - a1 < 0) sqrt2D = -sqrt2D;

  // variables for the numerator
  Real num = 0;
  Real sum1, num1_term, sum2, num2_term;
  num1_term = sum1 = sqrt(a1);
  num2_term = sum2 = sqrt2D;

  // variables for the denominator
  Real den = a1;
  Real den_term = 1;
  // computation
  for (int i = 1; i < 8; i++)
  {
    // compute the numerator
    num += num1_term * coefs_i[i];
    num += num2_term * coefs_s[i];
    // first sum
    sum1 *= (D_x / i) ;  // sqrt(x) * D^n/(n! x^n)
    num1_term = num1_term/a1 + sum1;
    // second sum
    sum2 *= (D_x / (i + 0.5)); // sqrt(2D) * D^n/((1+0.5)*...(n+0.5)*x^n)
    num2_term = num2_term/a1 + sum2;
    // compute the denominator
    den += den_term * coefs_s[i];
    den_term /= a1;
  }
  // result (P or Q ?)
   return lower_tail ? normal_cdf_raw( sqrt2D)
                     - (num / den) * normal_pdf_raw(sqrt2D)
                     : normal_cdf_raw(-sqrt2D)
                     + (num / den) * normal_pdf_raw(sqrt2D);
}
 
/** Compute the incomplete gamma function ratio Q(a,x)
 *  \f[ Q(a, x) = \frac{1}{\Gamma(a)}
 *      \int_x^\infty e^{-t} t^{a-1} dt
 *  \f]
 *  @param a parameter of the gamma ratio function
 *  @param x value to evaluate the gamma ratio function
 **/
Real gammaRatioQ(Real const& a, Real const& x)
{ return gammaRatio(a, x, false);}
 
/** Compute the incomplete gamma function ratio P(a,x)
 *  \f[ P(a, x) = \frac{1}{\Gamma(a)}
 *      \int_0^x e^{-t} t^{a-1} dt
 *  \f]
 *  @param a parameter of the gamma ratio function
 *  @param x value to evaluate the gamma ratio function
 **/
Real gammaRatioP(Real const& a, Real const& x)
{ return gammaRatio(a, x, true);}
 
/** Compute the incomplete gamma function ratio P(a,x)
 *  \f[ P(a, x) = \frac{1}{\Gamma(a)}
 *      \int_0^x e^{-t} t^{a-1} dt
 *  \f]
 *  @param a parameter of the gamma ratio function
 *  @param x value to evaluate the gamma ratio function
 *  @param lower_tail @c true if we want the lower tail, @c false otherwise
 **/
Real gammaRatio(Real const& a, Real const& x, bool lower_tail)
{
  // Check if a and x are available
  if (Arithmetic<Real>::isNA(a)||Arithmetic<Real>::isNA(x)) return a;
  // Negative parameter not allowed
  if (a<=0)
    throw domain_error("Funct::gammaRatio(a,x,lower_tail) "
                       "Negative parameter a");
  // trivial case
  if (x<=0) return lower_tail ? 0 : 1.;
  if (Arithmetic<Real>::isInfinite(x)) return lower_tail ? 1. : 0.;

  // small values of x
  if (x <= 1.) return gammaRatio_dl(a, x, lower_tail);

  // large a compared to x
  if (0.75*a> x) return gammaRatio_sr(a, x, lower_tail);

  // large x compared to a 
  if (0.75*x > a)
  { // if a>1 we can use the a.e. before computing
    if (a>1) return gammaRatio_ae(a, x, lower_tail);
    else     return gammaRatio_cf(a, x, lower_tail);
  }

  // (a<100) and (x ~ a)
  if (a<100.)
  {
    // a greater than x
    if (a>x) return gammaRatio_sr(a, x, lower_tail);
    else
    { // if a>1 we can use the a.e. before computing
      if (a>1) return gammaRatio_ae(a, x, lower_tail);
      else     return gammaRatio_cf(a, x, lower_tail);}
    }
  // (x ~ a) and (a>=100)
  return poisson_ae(a-1, x, lower_tail);
}
} // namespace Funct

} // namespace STK
