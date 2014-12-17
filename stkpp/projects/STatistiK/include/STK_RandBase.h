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
 * Project:  Laws
 * Purpose:  Main pseudo-random uniform, gaussian and exponentiel
 * generators.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_RandBase.h
 *  @brief Declaration of the RandBase class.
 *
 *  The RandBase class furnish the
 *  main pseudo random number generators (uniform, exponential, gauss).
 **/

#ifndef STK_RANDBASE_H
#define STK_RANDBASE_H

#include "STKernel/include/STK_Misc.h"

#include "Arrays/include/STK_ArrayBase.h"

// MersenneTwister header.
#include "MersenneTwister.h"

namespace STK
{
/** @ingroup Laws
 *  @brief class for the Base random generator.
 *
 * This class inherit from MTRand which should not be used directly.
 * Using RandBase, one get a Type safe generator for use in STK++
 * applications.
 *
 * This class furnish :
 * - A pseudo normalized uniform random variate generator
 * - A pseudo normalized Gaussian random variate generator
 * - A pseudo normalized exponential random variate generator
 *
 * @author George Marsaglia andWai Wan Tsang,
 *      "The Ziggurat Method for Generating Random Variables",
 *      Journal of Statistical Software,  Volume 5, 2000, Issue 8.
 *
 * @author Jurgen A Doornik,
 *      "An improved Ziggurat Method to generate Normal Random Sample"
 *      http://www.doornik.com/research.html (2005).
 *
 * For the exponential Law we remove the old method and use directly
 * the inverse pdf method.
 **/
class RandBase : protected MTRand
{
  public:
    /** Default constructor.
     * auto-initialize with /dev/urandom or time() and clock().
     * @param glimit maximal value of the boxes in the ziggourat method
     * @param gvol volume of each box in the ziggourat method
     * @param gsize number of boxes
     *
     **/
    RandBase( Real const& glimit = 3.442619855899
  , Real const& gvol   = 9.91256303526217e-3
  , int const& gsize  = 128
  );

    /** Initialize with a simple int seed.
     * @param oneSeed seed of the generator
     * @param glimit maximal value of the boxes in the ziggourat method
     * @param gvol volume of each box in the ziggourat method
     * @param gsize number of boxes
     **/
    RandBase( int const& oneSeed
  , Real const& glimit = 3.442619855899
  , Real const& gvol   = 9.91256303526217e-3
  , int const& gsize  = 128
  );

    /** Initialize with an int seed Array.
     * @param bigSeed seed of the generator
     * @param glimit maximal value of the boxes in the ziggourat method
     * @param gvol volume of each box in the ziggourat method
     * @param gsize number of boxes
     **/
    template< class TContainer1D>
    RandBase( const ArrayBase<TContainer1D> &bigSeed
  , Real const& glimit = 3.442619855899
  , Real const& gvol   = 9.91256303526217e-3
  , int const& gsize  = 128
  )
  : gsize_(gsize), glimit_(glimit), gvol_(gvol)
    {
      // dimension
      int first = bigSeed.begin(), size = bigSeed.size();
      uint32* arraySeed = new uint32[size];
      // cast int in uint32
      for (int i=first; i<=bigSeed.lastIdx(); i++)
      { arraySeed[i-first] = uint32(bigSeed[i]);}
      // Re-seeding functions with same behavior as initializers
      seed(arraySeed, size);
      delete[] arraySeed;
      // initialize zigourat method for gaussian random generator
      gaussInit();
    }

   /** destructor. */
    ~RandBase();

   /** Pseudo-random int uniform generator.
    *  \f[
    *  f(x) = 1/(n+1), \quad 0\leq x \leq n
    *  \f]
    *  Return a [0,n] uniform integer number for n < 2^32 using the
    *  Mersenne Twister method. This is a wrapper of the MTRand class.
    *  @sa STK::MTRand
    **/
    inline int randDiscreteUnif() { return int(randInt());}

    inline Real randDiscreteUnifReal()
    {
      return Real(randInt());
    }

    /** pseudo-random uniform generator.
     *  This is a wrapper of the MTRand class.
     *  \f[
     *   f(x) = 1, \qquad 0< x <1
     *  \f]
     *  @return a uniform number in (0,1) using the Mersenne Twister method.
     *  @sa STK::MTRand
    **/
    inline Real randUnif() { return Real(randDblExc());}

   /** @return same as randUnif().*/
   inline Real operator()() { return randUnif(); }

    /** Pseudo-random gaussian generator of the gaussian probability law:
     * \f[     f(x) = \frac{1}{\sqrt{2\pi}}
     *         \exp\left\{-\frac{x^2}{2}\right\}.
     * \f]
     * @param mu mean of the gaussian distribution
     * @param sigma standard deviation of the gaussian distribution
     * @return a real number from a normal (Gaussian) distribution.
    **/
    Real randGauss(Real const& mu = 0, Real const& sigma = 1);

    /** Pseudo-random exponential generator.
     * \f[
     *    f(x) = \exp\left\{ -x \right\}.
     * \f]
     *  @return a real number from an exponential normalized
     *  distribution using the inverse pdf method.
    **/
    Real randExp();

    /** Pseudo-random int uniform generator for a uni-dimensional
     *  container of int .
     *  @param A container to fill
     **/
    template< class TContainer1D>
    inline void randDiscreteUnif(ArrayBase<TContainer1D>& A)
    { for (int i=A.beginRows(); i<=A.lastIdxRows(); i++)
        for (int j=A.beginCols(); j<=A.lastIdxCols(); j++)
          A(i,j) = randDiscreteUnif();
    }

    /** Pseudo-random Real uniform generator for a uni-dimensional
     *  container of Real.
     *  @param A container to fill
     **/
    template< class TContainer1D>
    inline void randUnif(ArrayBase<TContainer1D>& A)
    { for (int i=A.beginRows(); i<=A.lastIdxRows(); i++)
        for (int j=A.beginCols(); j<=A.lastIdxCols(); j++)
          A(i,j) = randUnif();
    }

    /** Pseudo-random Real gaussian generator for a uni-dimensional
     *  container of Real.
     *  @param A container to fill
     **/
    template< class TContainer1D>
    inline void randGauss(ArrayBase< TContainer1D>& A)
    { for (int i=A.beginRows(); i<=A.lastIdxRows(); i++)
        for (int j=A.beginCols(); j<=A.lastIdxCols(); j++)
          A(i,j) = randGauss();
    }

    /** Pseudo-random Real exponential generator for a uni-dimensional
     *  container of Real.
     *  @param A container to fill
     **/
    template< class TContainer1D>
    inline void randExp(ArrayBase< TContainer1D>& A)
    { for (int i=A.beginRows(); i<=A.lastIdxRows(); i++)
        for (int j=A.beginCols(); j<=A.lastIdxCols(); j++)
          A(i,j) = randExp();
    }

  private:
    /*  Gauss parameters. */
    /** Number of box for the gaussian ziggourat method.
     * [0] is bottom box and [size_-1] is top box.
     **/
    const int gsize_;
    /** limit of the bottom box.    */
    Real const   glimit_;
    /** volume of each box and of the remaining tail. */
    Real const   gvol_;
    /** kn holds coordinates, such that each rectangle has same area.
     *  wn holds kn[i+1]/kn[i].
     *  fn holds exp(-0.5 * kn[i] * kn[i]).
     **/
    Real *kn, *wn, *fn;
    /** Initialization of the Zigourrat method. */
    void gaussInit();
};

} // namespace STK

#endif //STK_RANDBASE_H

