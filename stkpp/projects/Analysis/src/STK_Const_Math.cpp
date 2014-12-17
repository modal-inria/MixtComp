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
 * Project:  Analysis
 * Purpose:  implementation of the class Const
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Const_Math.cpp
 *  @brief In this file we implement functions computing Mathematical
 *  constants.
 **/

#include "STKernel/include/STK_Integer.h"
#include "STKernel/include/STK_Real.h"
#include "../include/STK_Const_Math.h"
#include "../include/STK_ISerie.h"
#include "../include/STK_Algo.h"

namespace STK
{

namespace Const
{

/** @ingroup Analysis
 *  Serie : return the kth element of the series of pi.
 * \f[
 * a_k = \left(\frac{1}{16}\right)^k
 *       \left( \frac{4}{8k+1}
 *            - \frac{2}{8k+4}
 *            - \frac{1}{8k+5}
 *            - \frac{1}{8k+6}
 *       \right)
 * \f]
 */
class SeriePi : public ISerie<SeriePi>
{
  private:
    mutable Real dig16_;
    mutable int k4_;

  public:
    /** Default ctor: initialize the variables
     **/
    inline SeriePi() : dig16_(1.), k4_(0)
    { ;}

    /** @return the first value (k=0) */
    inline Real firstImpl() const { return 47./15.;}
 
    /** @return the next value */
    inline Real nextImpl() const
    { 
      dig16_ /= 16.;
      k4_    += 4;
      return  dig16_
            * ( 2.0/(k4_+0.5)
              - 1.0/(k4_+2.0)
              - 0.5/(k4_+2.5)
              - 0.5/(k4_+3.0)
              );
    }
 
   /** return the kth value
    *  @param k index of the value we want to compute
    **/
    inline Real operator[](int k) const
    { int aux = 4*k;
      return  std::pow(2., -aux)
            * ( 2.0/(aux+0.5)
              - 1.0/(aux+2.0)
              - 0.5/(aux+2.5)
              - 0.5/(aux+3.0)
              );
  }
};

/** @ingroup Analysis
 *  Compute \f$ \pi \f$ using the Bailey et al.
 *  (Bailey et al. 1997, Adamchik and Wagon 1997) formula.
 * \f[
 * \pi = \sum_{k=0}^{+\infty} \left(\frac{1}{16}\right)^k
 *       \left( \frac{4}{8k+1}
 *            - \frac{2}{8k+4}
 *            - \frac{1}{8k+5}
 *            - \frac{1}{8k+6}
 *       \right)
 * \f]
**/
Real pi()
{
  SeriePi f;
  return sumSerie<SeriePi>(f);
}

/** @ingroup Analysis
 *  Serie : return the kth element of the series of euler constant.
 */
class SerieEuler : public ISerie<SerieEuler>
{
  public:
    /** Default ctor: initialize the variables */
    inline SerieEuler() : k_(2.) {}
    /** @return the first value (k=2) */
    inline Real firstImpl() const { return (Const::_LN2_/2.);}
    /** return the next value */
    inline Real nextImpl() const
    { 
      k_    += 1.;
      return  std::log(k_)/k_;
    }
    /** return the kth value
     *  @param k index of the value we want to compute
     **/
    inline Real operator[](int k) const { return std::log(k+2.)/(Real)(k+2);}

  private:
    mutable Real k_;

};

/** @ingroup Analysis
 *  Compute the euler constant \f$ \gamma \f$ using
 *  the formula
 * \f[
 * \gamma = \frac{\log(2)}{2}
 *        + \frac{1}{\log(2)}
 *          \sum_{k=2}^{+\infty} \frac{\log(k)}{k}
 * \f]
**/
Real euler()
{
  SerieEuler f;
  return Const::_LN2_/2.0 + sumAlternateSerie<SerieEuler>(f)/Const::_LN2_;
}

} // namespace CONST

} // namespace STK
