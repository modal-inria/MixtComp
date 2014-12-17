/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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
 * Project:  stkpp::Arrays
 * created on: 25 déc. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ProductImpl.h
 *  @brief In this file we implement the matrix-matrix product static methods.
 **/

#ifndef STK_PRODUCTIMPL_H
#define STK_PRODUCTIMPL_H

namespace STK
{
/* size of the block and panels used in the product algorithm */
const int blockSize = 4;
const int panelSize = 64;
const int vectorSize = 256;
}

#include "STK_ArrayByVectorProduct.h"
#include "STK_ArrayByArrayProduct.h"

namespace STK
{

namespace hidden
{
/** In the general case, i.e. when some of the structures are triangular
 *  we use the usual matrix multiplication formula.
 **/
template < class Lhs, class Rhs, class Result
         , int lhsStructure_ = hidden::Traits<Lhs>::structure_
         , int RhsStructure_ = hidden::Traits<Rhs>::structure_ >
struct ProductImpl
{
  typedef typename Result::Type Type;
  typedef MultImpl<Type> Cmult;
  typedef MultCoefImpl<Lhs, Rhs, Result> MultCoeff;

  static void runbp(Lhs const& lhs, Rhs const& rhs, Result& res )
  {
    for (int j=res.beginCols(); j< res.endCols(); j++)
    {
      Integer const end = res.rangeRowsInCol(j).end();
      for (int i=res.rangeRowsInCol(j).begin(); i< end; i++)
      { MultCoeff::dot(lhs, rhs, res, i, j);}
    }
  }
  static void runpb(Lhs const& lhs, Rhs const& rhs, Result& res )
  {
    for (int i=res.beginRows(); i< res.endRows(); i++)
    {
      Integer const end = res.rangeColsInRow(i).end();
      for (int j=res.rangeColsInRow(i).begin(); j< end; j++)
      { MultCoeff::dot(lhs, rhs, res, i, j);}
    }
  }
};

template <class Lhs, class Rhs, class Result>
struct ProductImpl<Lhs, Rhs, Result, Arrays::array2D_, Arrays::array2D_>
{
  enum
  {
    orientLhs_ = Lhs::orient_,
    orientRhs_ = Rhs::orient_
  };
  static inline void runbp(Lhs const& lhs, Rhs const& rhs, Result& res )
  { bp<Lhs,Rhs,Result>::run(lhs, rhs, res);}
  static inline void runpb(Lhs const& lhs, Rhs const& rhs, Result& res )
  { pb<Lhs,Rhs,Result>::run(lhs, rhs, res);}
};

template <class Lhs, class Rhs, class Result>
struct ProductImpl<Lhs, Rhs, Result, Arrays::array2D_, Arrays::square_>
{
  enum
  {
    orientLhs_ = Lhs::orient_,
    orientRhs_ = Rhs::orient_
  };
  static inline void runbp(Lhs const& lhs, Rhs const& rhs, Result& res )
  { bp<Lhs,Rhs,Result>::run(lhs, rhs, res);}
  static inline void runpb(Lhs const& lhs, Rhs const& rhs, Result& res )
  { pb<Lhs,Rhs,Result>::run(lhs, rhs, res);}
};

template <class Lhs, class Rhs, class Result>
struct ProductImpl<Lhs, Rhs, Result, Arrays::square_, Arrays::square_>
{
  enum
  {
    orientLhs_ = Lhs::orient_,
    orientRhs_ = Rhs::orient_
  };
  static inline void runbp(Lhs const& lhs, Rhs const& rhs, Result& res )
  { bp<Lhs,Rhs,Result>::run(lhs, rhs, res);}
  static inline void runpb(Lhs const& lhs, Rhs const& rhs, Result& res )
  { pb<Lhs,Rhs,Result>::run(lhs, rhs, res);}
};

template <class Lhs, class Rhs, class Result>
struct ProductImpl<Lhs, Rhs, Result, Arrays::square_, Arrays::array2D_>
{
  enum
  {
    orientLhs_ = Lhs::orient_,
    orientRhs_ = Rhs::orient_
  };
  static inline void runbp(Lhs const& lhs, Rhs const& rhs, Result& res )
  { bp<Lhs,Rhs,Result>::run(lhs, rhs, res);}
  static inline void runpb(Lhs const& lhs, Rhs const& rhs, Result& res )
  { pb<Lhs,Rhs,Result>::run(lhs, rhs, res);}
};

template <class Lhs, class Rhs, class Result>
struct ProductImpl<Lhs, Rhs, Result, Arrays::array2D_, Arrays::vector_>
{
  static inline void run(Lhs const& lhs, Rhs const& rhs, Result& res )
  { bv<Lhs,Rhs,Result>::run(lhs, rhs, res);}
};

template <class Lhs, class Rhs, class Result>
struct ProductImpl<Lhs, Rhs, Result, Arrays::square_, Arrays::vector_>
{
  static inline void run(Lhs const& lhs, Rhs const& rhs, Result& res )
  { bv<Lhs,Rhs,Result>::run(lhs, rhs, res);}
};

template <class Lhs, class Rhs, class Result, int lhsStructure_>
struct ProductImpl<Lhs, Rhs, Result, lhsStructure_, Arrays::vector_>
{
  typedef MultCoefImpl<Lhs, Rhs, Result> MultCoeff;
  static void run(Lhs const& lhs, Rhs const& rhs, Result& res )
  {
    for (int i=lhs.beginRows(); i< lhs.endRows(); i++)
    { MultCoeff::dot(lhs, rhs, res, i);}
  }
};


template <class Lhs, class Rhs, class Result, int RhsStructure_>
struct ProductImpl<Lhs, Rhs, Result, Arrays::point_, RhsStructure_>
{
  typedef MultCoefImpl<Lhs, Rhs, Result> MultCoeff;
  static void run(Lhs const& lhs, Rhs const& rhs, Result& res )
  {
    for (int j=rhs.beginCols(); j< rhs.endCols(); j++)
    { MultCoeff::dot(lhs, rhs, res, j);}
  }
};

template <class Lhs, class Rhs, class Result>
struct ProductImpl<Lhs, Rhs, Result, Arrays::point_, Arrays::array2D_>
{
  static inline void run(Lhs const& lhs, Rhs const& rhs, Result& res )
  { vb<Lhs,Rhs,Result>::run(lhs, rhs, res);}
};

template <class Lhs, class Rhs, class Result>
struct ProductImpl<Lhs, Rhs, Result, Arrays::point_, Arrays::square_>
{
  static inline void run(Lhs const& lhs, Rhs const& rhs, Result& res )
  { vb<Lhs,Rhs,Result>::run(lhs, rhs, res);}
};

} // namespace hidden

} // namespace STK

#endif /* STK_PRODUCTIMPL_H */
