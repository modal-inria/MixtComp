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
 * created on: 16 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_PtrCopycat.h
 *  @brief In this file we implement the copy methods used when copying an array
 *  in an other array.
 **/


#ifndef STK_PTRCOPYCAT_H
#define STK_PTRCOPYCAT_H

#include <algorithm>
#include "STK_Arrays_Util.h"

namespace STK
{
namespace hidden
{

/** @ingroup hidden
 * @breif allows to select the kind of Copŷcat to use at compile time.*/
template < typename Derived, typename Rhs, int TStructure_, int RhsStructure_>
struct BasedCopyCat;

/** @ingroup hidden
 * @breif allows to select the kind of BasedCopŷcat to use at compile time.*/
template < typename Derived, typename Rhs, int TOrient_>
struct BasedCopyCatSelector;

//---------------------GENERAL----------------------------------
// general <- general
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::array2D_, Arrays::array2D_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i)
      for (int j= STKBASEARRAYS; j <= rhs.lastIdxCols(); ++j)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    for (int j= STKBASEARRAYS; j <= rhs.lastIdxCols(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// general <- square
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::array2D_, Arrays::square_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i)
      for (int j= STKBASEARRAYS; j <= rhs.lastIdx(); ++j)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    for (int j= STKBASEARRAYS; j <= rhs.lastIdx(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// general <- diagonal
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::array2D_, Arrays::diagonal_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    lhs.setValue(Type(0));
    for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i,i) = rhs.elt(i);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    lhs.setValue(Type(0));
    for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i,i) = rhs.elt(i);}
  }
};

// general <- lower_triangular
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::array2D_,   Arrays::lower_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j= STKBASEARRAYS; j <= last; ++j)
    { int i= STKBASEARRAYS;
      for (; i < j; ++i)                 { lhs.elt(i,j) = Type(0);}
      for (; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int j= last+1; j <= rhs.lastIdxCols(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = Type(0);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int i= STKBASEARRAYS; i <= last; ++i)
    { int j= STKBASEARRAYS;
      for (; j <=i; ++j)                 { lhs.elt(i,j) = rhs.elt(i, j);}
      for (; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = Type(0);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
};

// general <- upper_triangular
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::array2D_, Arrays::upper_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j= STKBASEARRAYS; j <= last; ++j)
    { int i= STKBASEARRAYS;
      for (; i <= j; ++i)                { lhs.elt(i,j) = rhs.elt(i, j);}
      for (; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) =  Type(0);}
    }
    for (int j= last+1; j <= rhs.lastIdxCols(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i= STKBASEARRAYS; i <= last; ++i)
    { int j= STKBASEARRAYS;
      for (; j <i; ++j)                  { lhs.elt(i,j) = Type(0);}
      for (; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = Type(0);}
  }
};

//---------------------SQUARE----------------------------------
// square <- general
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::square_, Arrays::array2D_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i)
      for (int j= STKBASEARRAYS; j <= rhs.lastIdxCols(); ++j)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    for (int j= STKBASEARRAYS; j <= rhs.lastIdxCols(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// square <- square
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::square_, Arrays::square_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i)
      for (int j= STKBASEARRAYS; j <= rhs.lastIdx(); ++j)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    for (int j= STKBASEARRAYS; j <= rhs.lastIdx(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// square <- diagonal
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::square_, Arrays::diagonal_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    lhs.setValue(Type(0));
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i= STKBASEARRAYS; i <= last; ++i) { lhs.elt(i,i) = rhs.elt(i);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    lhs.setValue(Type(0));
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i= STKBASEARRAYS; i <= last; ++i) { lhs.elt(i,i) = rhs.elt(i);}
  }
};

// square_ <- lower_triangular
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::square_,   Arrays::lower_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j= STKBASEARRAYS; j <= last; ++j)
    { int i= STKBASEARRAYS;
      for (; i < j; ++i)                 { lhs.elt(i,j) = Type(0);}
      for (; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int j= last+1; j <= rhs.lastIdxCols(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = Type(0);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int i= STKBASEARRAYS; i <= last; ++i)
    { int j= STKBASEARRAYS;
      for (; j <=i; ++j)                 { lhs.elt(i,j) = rhs.elt(i, j);}
      for (; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = Type(0);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
};

// square_ <- upper triangular
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::square_, Arrays::upper_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j= STKBASEARRAYS; j <= last; ++j)
    { int i= STKBASEARRAYS;
      for (; i <= j; ++i)                { lhs.elt(i,j) = rhs.elt(i, j);}
      for (; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) =  Type(0);}
    }
    for (int j= last+1; j <= rhs.lastIdxCols(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i= STKBASEARRAYS; i <= last; ++i)
    { int j= STKBASEARRAYS;
      for (; j <i; ++j)                  { lhs.elt(i,j) = Type(0);}
      for (; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = Type(0);}
  }
};

//---------------------DIAGONAL----------------------------------
// diagonal <- diagonal
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::diagonal_, Arrays::diagonal_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

// diagonal <- vector
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::diagonal_, Arrays::vector_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

// diagonal <- point
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::diagonal_, Arrays::point_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

//---------------------LDO----------------------------------
// lower_triangular <- lower_triangular
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::lower_triangular_,   Arrays::lower_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j= STKBASEARRAYS; j <= last; ++j)
      for (int i=j; i <= rhs.lastIdxRows(); ++i)
      { lhs.elt(i,j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int i= STKBASEARRAYS; i <= last; ++i)
    {
      for (int j= STKBASEARRAYS; j <=i; ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
};

//---------------------LUP----------------------------------
// upper_triangular <- upper_triangular
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::upper_triangular_, Arrays::upper_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j= STKBASEARRAYS; j <= last; ++j)
    {
      for (int i= STKBASEARRAYS; i <= j; ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int j= last+1; j <= rhs.lastIdxCols(); ++j)
      for (int i= STKBASEARRAYS; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i= STKBASEARRAYS; i <= last; ++i)
    { for (int j=i; j <= rhs.lastIdxCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}}
  }
};


//---------------------VECTOR----------------------------------
//  vector <- diagonal
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::vector_, Arrays::diagonal_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

//  vector <- vector
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::vector_, Arrays::vector_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};
// vector <- point
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::vector_, Arrays::point_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

//---------------------POINT----------------------------------
//  point_ <- diagonal
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::point_, Arrays::diagonal_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

//  vector <- vector
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::point_, Arrays::vector_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};
// vector <- point
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::point_, Arrays::point_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i= STKBASEARRAYS; i <= rhs.lastIdx(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};


//---------------------NUMBER----------------------------------
//  point_ <- diagonal
template < typename Derived, typename Rhs>
struct BasedCopyCat<  Derived,  Rhs, Arrays::number_, Arrays::number_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { lhs.elt() = rhs.elt();}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { lhs.elt() = rhs.elt();}
};

template< typename Derived, typename Rhs>
struct BasedCopyCatSelector< Derived, Rhs, Arrays::by_col_>
{
  enum
  { tstructure_ = hidden::Traits<Derived>::structure_
  , sstructure_ = hidden::Traits<Rhs>::structure_
  };
  inline static void run(Derived& lhs, Rhs const& rhs )
  { BasedCopyCat<Derived, Rhs, tstructure_, sstructure_>::runByCol(lhs, rhs );}
};
template< typename Derived, typename Rhs>
struct BasedCopyCatSelector< Derived, Rhs, Arrays::by_row_>
{
  enum
  { tstructure_ = hidden::Traits<Derived>::structure_
  , sstructure_ = hidden::Traits<Rhs>::structure_
  };
  inline static void run(Derived& lhs, Rhs const& rhs )
  { BasedCopyCat<Derived, Rhs, tstructure_, sstructure_>::runByRow(lhs, rhs );}
};


} // namespace hidden

namespace hidden
{

// forward declarations
template< typename TDerived, typename SDerived, int TIdx_, int SIdx_, int Size_>
struct PtrCopycat;

/*  general copiers with known size */
template< typename TDerived, typename SDerived, int TIdx_, int SIdx_, int Size_>
struct PtrCopycat
{
  typedef typename Traits<TDerived>::Type TType;
  typedef typename Traits<SDerived>::Type SType;
  inline static void copyForward( TType* target, SType* source, int, int, int)
  { for (int i= 1, j=1; i <= Size_; i+=TIdx_, j+=SIdx_) { target[i] = TType(source[j]);}}
  inline static void copyBackward( TType* target, SType* source, int, int, int)
  { for (int i= 1+(Size_-1)*TIdx_, j=1+(Size_-1)*SIdx_; i >= 1; i-=TIdx_, j-=SIdx_) { target[i] = TType(source[j]);}}
};

template< typename TDerived, typename SDerived, int SIdx_, int Size_>
struct PtrCopycat<  TDerived,  SDerived,  UnknownSize,  SIdx_,  Size_>
{
  typedef typename Traits<TDerived>::Type TType;
  typedef typename Traits<SDerived>::Type SType;
  inline static void copyForward( TType* target, SType* source, int tidx, int, int)
  { for (int i= 1, j=1; i <= Size_; i+=tidx, j+=SIdx_) { target[i] = TType(source[j]);}}
  inline static void copyBackward( TType* target, SType* source, int tidx, int, int)
  { for (int i= 1+(Size_-1)*tidx, j=1+(Size_-1)*SIdx_; i >= 1; i-=tidx, j-=SIdx_) { target[i] = TType(source[j]);}}
};

template< typename TDerived, typename SDerived, int TIdx_, int Size_>
struct PtrCopycat<  TDerived,  SDerived,  TIdx_,  UnknownSize,  Size_>
{
  typedef typename Traits<TDerived>::Type TType;
  typedef typename Traits<SDerived>::Type SType;
  inline static void copyForward( TType* target, SType* source, int, int sidx, int)
  { for (int i= 1, j=1; i <= Size_; i+=TIdx_, j+=sidx) { target[i] = TType(source[j]);}}
  inline static void copyBackward( TType* target, SType* source, int, int sidx, int)
  { for (int i= 1+(Size_-1)*TIdx_, j=1+(Size_-1)*sidx; i >= 1; i-=TIdx_, j-=sidx) { target[i] = TType(source[j]);}}
};

template< typename TDerived, typename SDerived, int Size_>
struct PtrCopycat<  TDerived,  SDerived,  UnknownSize,  UnknownSize,  Size_>
{
  typedef typename Traits<TDerived>::Type TType;
  typedef typename Traits<SDerived>::Type SType;
  inline static void copyForward( TType* target, SType* source, int tidx, int sidx, int)
  { for (int i= 1, j=1; i <= Size_; i+=tidx, j+=sidx) { target[i] = TType(source[j]);}}
  inline static void copyBackward( TType* target, SType* source, int tidx, int sidx, int)
  { for (int i= 1+(Size_-1)*tidx, j=1+(Size_-1)*sidx; i >= 1; i-=tidx, j-=sidx) { target[i] = TType(source[j]);}}
};


template< typename TDerived, typename SDerived, int SIdx_>
struct PtrCopycat<  TDerived,  SDerived,  UnknownSize,  SIdx_,  UnknownSize>
{
  typedef typename Traits<TDerived>::Type TType;
  typedef typename Traits<SDerived>::Type SType;
  inline static void copyForward( TType* target, SType* source, int tidx, int, int size)
  { for (int i= 1, j=1; i <= size; i+=tidx, j+=SIdx_) { target[i] = TType(source[j]);}}
  inline static void copyBackward( TType* target, SType* source, int tidx, int, int size)
  { for (int i= 1+(size-1)*tidx, j=1+(size-1)*SIdx_; i >= 1; i-=tidx, j-=SIdx_) { target[i] = TType(source[j]);}}
};

template< typename TDerived, typename SDerived, int TIdx_>
struct PtrCopycat<  TDerived,  SDerived,  TIdx_,  UnknownSize,  UnknownSize>
{
  typedef typename Traits<TDerived>::Type TType;
  typedef typename Traits<SDerived>::Type SType;
  inline static void copyForward( TType* target, SType* source, int, int sidx, int size)
  { for (int i= 1, j=1; i <= size; i+=TIdx_, j+=sidx) { target[i] = TType(source[j]);}}
  inline static void copyBackward( TType* target, SType* source, int, int sidx, int size)
  { for (int i= 1+(size-1)*TIdx_, j=1+(size-1)*sidx; i >= 1; i-=TIdx_, j-=sidx) { target[i] = TType(source[j]);}}
};

template< typename TDerived, typename SDerived>
struct PtrCopycat<  TDerived,  SDerived,  UnknownSize,  UnknownSize,  UnknownSize>
{
  typedef typename Traits<TDerived>::Type TType;
  typedef typename Traits<SDerived>::Type SType;
  inline static void copyForward( TType* target, SType* source, int tidx, int sidx, int size)
  { for (int i= 1, j=1; i <= size; i+=tidx, j+=sidx) { target[i] = TType(source[j]);}}
  inline static void copyBackward( TType* target, SType* source, int tidx, int sidx, int size)
  { for (int i= 1+(size-1)*tidx, j=1+(size-1)*sidx; i >= 1; i-=tidx, j-=sidx) { target[i] = TType(source[j]);}}
};


} // namespace hidden


} // namespace STK

#endif /* STK_PTRCOPYCAT_H */
