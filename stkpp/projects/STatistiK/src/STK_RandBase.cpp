/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004  Serge Iovleff

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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                   */

/*
 * Project:  Analysis
 * Purpose:  implementation of the class RandBase
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_RandBase.cpp
 *  @brief Implementation of the RandBase class
 *  (uniform, exponential and gauss random generators).
 **/

#include "../include/STK_RandBase.h"

namespace STK
{

/* Initialize with a simple int seed.                             */
RandBase::RandBase( int const& oneSeed
                  , Real const&    glimit
                  , Real const&    gvol
                  , int const& gsize
                  )
                  : MTRand( (uint32)oneSeed )
                  , gsize_(gsize)
                  , glimit_(glimit)
                  , gvol_(gvol)
{
  gaussInit();
}

/* auto-initialize with /dev/urandom or time() and clock() */
RandBase::RandBase( Real const&    glimit
                  , Real const&    gvol
                  , int const& gsize
                  )
                  : MTRand()
                  , gsize_(gsize)
                  , glimit_(glimit)
                  , gvol_(gvol)
{
  gaussInit();
}

/* destructor.                                                            */
RandBase::~RandBase()
{
  delete [] kn;
  delete [] wn;
  delete [] fn;
}

/* Pseudo-random gaussian generator.
 * \f[     f(x) = \frac{1}{\sqrt{2\pi}}
 *         \exp\left\{-\frac{x^2}{2}\right\}
 * \f]
 *  Return a real number from a normal (Gaussian) normalized
 *  distribution.
**/
Real RandBase::randGauss(Real const& mu, Real const& sigma)
{
  while(1)
  {
    // uniforms number
    Real u = 2.0 * randUnif() - 1.0;
    // random box
    uint32 i = randInt() & 0x7F; //
    // squeeze step : try the rectangular boxes
    if (std::abs(u) < wn[i]) return mu + sigma * u * kn[i];
    // bottom box: we have to sample from the tail
    if (i == 0)
    {
      // result
      Real x;
      // loop
      do { x = randExp() / glimit_;}
      while ( 2.0 * randExp() < x * x);
      // result sign(u) * (x+dr)
      return mu + sigma * sign(u, x+glimit_);
    }
    else // other box
    { // x : result, f1 : intermediary result
      Real x = u * kn[i], f1 = fn[i+1];
      // reject step : is this a sample from the wedges ?
      if ( f1 + randUnif()*( fn[i]*exp(x*x/2.)-f1) < 1.0)
      { return mu + sigma * x;}
    }
  }
  // avoid warning at compilation
  return 0.;
}

/* Pseudo-random exponential generator.
 * \f[ 
 *      f(x) = \exp\left\{ -x \right\}
 * \f]
 *  Return a real number from an exponential normalized
 *  distribution.
**/
Real RandBase::randExp()
{ return -Real(std::log(randUnif()));}

/* Initialization (Zigourrat method).  */
void RandBase::gaussInit()
{
  kn = new Real[gsize_+1];
  wn = new Real[gsize_];
  fn = new Real[gsize_+1];

  Real f = exp(-0.5 * glimit_ * glimit_);
  kn[0]  = gvol_ / f; // [0] is bottom block: gvol/f(gbound)

  kn[gsize_] = 0;
  fn[gsize_] = 1;

  kn[1]  = glimit_;
  fn[1]  = f;

  for (int i = 2; i < gsize_; ++i)
  {
    kn[i] = sqrt(-2 * log(gvol_ / kn[i-1] + f));
    fn[i] = (f = exp(-0.5 * kn[i] * kn[i]));
  }
  for (int i = 0; i < gsize_; ++i)
    wn[i] = kn[i + 1] / kn[i];
}

} // namespace STK
