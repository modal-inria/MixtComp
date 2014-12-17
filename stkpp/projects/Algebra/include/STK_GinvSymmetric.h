/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * Project:  stkpp::Algebra
 * created on: 2 nov. 2010
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_GinvSymmetric.h
 *  @brief In this file we define and implement the functor class GinvSymmetric.
 **/

#ifndef STK_GINVSYMMETRIC_H
#define STK_GINVSYMMETRIC_H

#include "STK_SymEigen.h"

namespace STK
{

/** @brief The GinvSymmetric class is a functor allowing to compute the generalized
 *  inverse of a symmetric matrix.
 */
class GinvSymmetric
{
  public:
    /** Constructor */
    inline GinvSymmetric() { ;}
    /** Destructor */
    inline virtual ~GinvSymmetric() { ;}
    /** compute the generalized inverse of the symmetric matrix x. x is
     *  overwritten by the result.
     *  @param x the matrix to inverse.
     **/
    template<class ArraySquare>
    inline void operator()(ArraySquare*& x)
    {
      SymEigen* decomp = new SymEigen(*x);
      decomp->run();
      decomp->ginv(*x);
      delete decomp;
    }
    /** Compute the generalized inverse of the symmetric matrix x. x is
     *  overwritten by the result.
     *  @param x the matrix to inverse.
     **/
    template<class ArraySquare>
    inline void operator()(ArraySquare& x)
    {
      SymEigen* decomp = new SymEigen(x);
      decomp->run();
      decomp->ginv(x);
      delete decomp;
    }
};

} // namespace STK

#endif /* STK_GINVSYMMETRIC_H */
