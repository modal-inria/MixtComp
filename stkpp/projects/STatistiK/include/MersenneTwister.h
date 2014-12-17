// MersenneTwister.h
// Mersenne Twister random number generator -- a C++ class MTRand
// Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus
// Richard J. Wagner  v1.0  15 May 2003  rjwagner@writeme.com

// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
// Copyright (C) 2000 - 2003, Richard J. Wagner
// Copyright (C) 2005 - 2008, Serge Iovleff
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in
//      the  documentation and/or other materials provided with the
//      distribution.
//
//   3. The names of its contributors may not be used to endorse or
//      promote products derived from this software without specific prior
//      written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// The original code included the following notice:
//
//     When you use this, send an email to: matumoto@math.keio.ac.jp
//     with an appropriate reference to your work.
//
// It would be nice to CC: rjwagner@writeme.com
//                     and Cokus@math.washington.edu
// when you write.
//
// Also it would be nice to cc to S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
// if you are using this code.
//
#ifndef MERSENNETWISTER_H
#define MERSENNETWISTER_H

#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "Analysis/include/STK_Const_Math.h" // for _2PI_

/** @brief Generate unsigner 32 bits random number using the
 *  Merdsenne Twister Algorithm.
 *
 * The Mersenne Twister is an algorithm for generating random numbers.
 * It was designed with consideration of the flaws in various other
 * generators. The period, 2^19937-1, and the order of equidistribution,
 * 623 dimensions, are far greater.  The generator is also fast; it
 * avoids multiplication and division, and it benefits from caches and
 * pipelines.  For more information see the inventors' web page at
 * http://www.math.keio.ac.jp/~matumoto/emt.html
 *
 * Reference
 * M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally
 * Equidistributed Uniform Pseudo-Random Number Generator", ACM
 * Transactions on  Modeling and Computer Simulation, Vol. 8, No. 1,
 * January 1998, pp 3-30.
 *
 * Do NOT use for CRYPTOGRAPHY without securely hashing several
 * returned values together, otherwise the generator state can be
 * learned after reading 624 consecutive values.
 * Access to 32-bit random numbers
 *
 * Not thread safe (unless auto-initialization is avoided and each thread
 * has its own MTRand object)
**/
class MTRand
{
  // Data
  public:
    /** unsigned integer type, at least 32 bits */
    typedef unsigned long uint32;

    enum { N = 624 };       ///< length of state vector
    enum { SAVE = N + 1 };  ///< length of array for save()

  protected:
    enum { M = 397 };  ///< period parameter

    uint32 state[N];   ///< internal state
    uint32 *pNext;     ///< Next value to get from state
    int left;          ///< number of values left before reload needed

  //Methods
  public:
    /** Initialize with a simple uint32 */
    inline MTRand( const uint32& oneSeed )
    { seed(oneSeed); }

    /** Initialize with a an array */
    inline MTRand( uint32 *const bigSeed, uint32 const seedLength = N )
    { seed(bigSeed,seedLength); }

    /** auto-initialize with /dev/urandom or time() and clock() */
    inline MTRand()
    { seed(); }

    /** real number in [0,1] */
    inline double rand()
    { return double(randInt()) * (1.0/4294967295.0); }

    /** real number in [0,n] */
    inline double rand( const double& n )
    { return rand() * n; }

    /** real number in [0,1) */
    inline double randExc()
    { return double(randInt()) * (1.0/4294967296.0); }

    /** real number in [0,n) */
    inline double randExc( const double& n )
    { return randExc() * n; }

    /** real number in (0,1) */
    inline double randDblExc()
    { return ( double(randInt()) + 0.5 ) * (1.0/4294967296.0); }

    /** real number in (0,n) */
    inline double randDblExc( const double& n )
    { return randDblExc() * n; }

    /** integer in [0,2^32-1] */
    inline uint32 randInt()
    {
      // Pull a 32-bit integer from the generator state
      // Every other access function simply transforms the numbers
      // extracted here
      if( left == 0 ) reload();
      --left;

      uint32 s1;
      s1  = *pNext++;
      s1 ^= (s1 >> 11);
      s1 ^= (s1 <<  7) & 0x9d2c5680UL;
      s1 ^= (s1 << 15) & 0xefc60000UL;
      return ( s1 ^ (s1 >> 18) );
    }
    /** same as rand(). */
    inline double operator()() { return rand(); }

    /** integer in [0,n] for n < 2^32 */
    inline uint32 randInt( const uint32& n )
    {
      // Find which bits are used in n
      // Optimized by Magnus Jonsson (magnus@smartelectronix.com)
      uint32 used = n;
      used |= used >> 1;
      used |= used >> 2;
      used |= used >> 4;
      used |= used >> 8;
      used |= used >> 16;

      // Draw numbers until one is found in [0,n]
      uint32 i;
      do
          i = randInt() & used;  // toss unused bits to shorten search
      while( i > n );
      return i;
    }

    /** Access to 53-bit random numbers
     * (capacity of IEEE double precision).
     *  real number in [0,1)
     **/
    inline double rand53()
    {
      uint32 a = randInt() >> 5, b = randInt() >> 6;
      // by Isaku Wada
      return ( a * 67108864.0 + b ) * (1.0/9007199254740992.0);
    }

    /** Access to nonuniform random number distributions.
     *  Return a real number from a normal (Gaussian) distribution with
     *  given mean and variance by Box-Muller method. */
    inline double randNorm( const double& mean, const double& variance )
    {
      double r = sqrt( -2.0 * log( 1.0-randDblExc()) ) * variance;
      double phi = STK::Const::_2PI_ * randExc();
      return mean + r * cos(phi);
    }

    /** Re-seeding functions with same behavior as initializers.
     *  Seed the generator with a simple uint32  */
    inline void seed( const uint32 oneSeed )
    {
      initialize(oneSeed);
      reload();
    }

    /** Re-seeding functions with same behavior as initializers.
     *  Seed the generator with an array of uint32's
     *  There are 2^19937-1 possible initial states.  This function
     *  allows all of those to be accessed by providing at least
     *  19937 bits (with a default seed length of N = 624 uint32's).
     *  Any bits above the lower 32 in each element are discarded.
     *  Just call seed() if you want to get array from /dev/urandom
    **/
    inline void seed( uint32 *const bigSeed, const uint32 seedLength )
    {
      initialize(19650218UL);
      int i = 1;
      uint32 j = 0;
      int k = ( N > seedLength ? N : seedLength );
      for( ; k; --k )
      {
        state[i] =
            state[i] ^ ( (state[i-1] ^ (state[i-1] >> 30)) * 1664525UL );
        state[i] += ( bigSeed[j] & 0xffffffffUL ) + j;
        state[i] &= 0xffffffffUL;
        ++i;  ++j;
        if( i >= N ) { state[0] = state[N-1];  i = 1; }
        if( j >= seedLength ) j = 0;
      }
      for( k = N - 1; k; --k )
      {
        state[i] =
          state[i] ^ ( (state[i-1] ^ (state[i-1] >> 30)) * 1566083941UL );
        state[i] -= i;
        state[i] &= 0xffffffffUL;
        ++i;
        if( i >= N ) { state[0] = state[N-1];  i = 1; }
      }
      state[0] = 0x80000000UL;// MSB is 1, assuring non-zero initial array
      reload();
    }

    /** Re-seeding functions with same behavior as initializers.
     *  Seed the generator with an array from /dev/urandom if available
     *  Otherwise use a hash of time() and clock() values
     **/
    inline void seed()
    {
      // First try getting an array from /dev/urandom
      FILE* urandom = fopen( "/dev/urandom", "rb" );
      if( urandom )
      {
          uint32 bigSeed[N];
          uint32 *s = bigSeed;
          int i = N;
          bool success = true;
          while( success && i-- )
              success = fread( s++, sizeof(uint32), 1, urandom );
          fclose(urandom);
          if( success ) { seed( bigSeed, N );  return; }
      }

      // Was not successful, so use time() and clock() instead
      seed( hash( time(NULL), clock() ) );
    }

    /** Saving and loading generator state.
     * to array of size SAVE
     **/
    inline void save( uint32* saveArray ) const
    {
      uint32 *sa = saveArray;
      const uint32 *s = state;
      int i = N;
      for( ; i--; *sa++ = *s++ ) {}
      *sa = left;
    }

    /** Saving and loading generator state.
     *  from such array
     **/
    inline void load( uint32 *const loadArray )
    {
      uint32 *s = state;
      uint32 *la = loadArray;
      int i = N;
      for( ; i--; *s++ = *la++ ) {}
      left = *la;
      pNext = &state[N-left];
    }

  protected:
    /** Initialize generator state with seed
     *  See Knuth TAOCP Vol 2, 3rd Ed, p.106 for multiplier.
     *  In previous versions, most significant bits (MSBs) of the seed
     *  affect only MSBs of the state array.
     *  Modified 9 Jan 2002 by Makoto Matsumoto.
     **/
    inline void initialize( const uint32 seed )
    {
      uint32 *s = state;
      uint32 *r = state;
      int i = 1;
      *s++ = seed & 0xffffffffUL;
      for( ; i < N; ++i )
      {
        *s++ = ( 1812433253UL * ( *r ^ (*r >> 30) ) + i ) & 0xffffffffUL;
        r++;
      }
    }

    /** Generate N new values in state
     *  Made disposeer and faster by Matthew Bellew
     *  (matthew.bellew@home.com)
     **/
    inline void reload()
    {
      uint32 *p = state;
      int i;
      for( i = N - M; i--; ++p )
        *p = twist( p[M], p[0], p[1] );
      for( i = M; --i; ++p )
        *p = twist( p[M-N], p[0], p[1] );
      *p = twist( p[M-N], p[0], state[0] );

      left = N, pNext = state;
    }
    /** High bit.*/
    inline uint32 hiBit( const uint32& u ) const
    { return u & 0x80000000UL; }

    /** low bit.*/
    inline uint32 loBit( const uint32& u ) const
    { return u & 0x00000001UL; }

    /** low bits.*/
    inline uint32 loBits( const uint32& u ) const
    { return u & 0x7fffffffUL; }

    /** mixed bits.*/
    inline uint32 mixBits( const uint32& u, const uint32& v ) const
    { return hiBit(u) | loBits(v); }

    /** twisted values.*/
    inline uint32 twist( const uint32& m, const uint32& s0, const uint32& s1 ) const
    { return m ^ (mixBits(s0,s1)>>1) ^ (-loBit(s1) & 0x9908b0dfUL); }

    /** Get a uint32 from t and c
     *  Better than uint32(x) in case x is floating point in [0,1]
     *  Based on code by Lawrence Kirby (fred@genesis.demon.co.uk)
     **/
    static inline uint32 hash( time_t t, clock_t c )
    {
      static uint32 differ = 0;  // guarantee time-based seeds will change

      uint32 h1 = 0;
      unsigned char *p = (unsigned char *) &t;
      for( size_t i = 0; i < sizeof(t); ++i )
      {
        h1 *= UCHAR_MAX + 2U;
        h1 += p[i];
      }
      uint32 h2 = 0;
      p = (unsigned char *) &c;
      for( size_t j = 0; j < sizeof(c); ++j )
      {
        h2 *= UCHAR_MAX + 2U;
        h2 += p[j];
      }
      return ( h1 + differ++ ) ^ h2;
    }
};

#endif  // MERSENNETWISTER_H
