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

/** @file STK_ArrayBaseAssign.h
 *  @brief In this file we implement the copy and assign methods used when
 *  copying an array or an expression in an other array.
 **/


#ifndef STK_ARRAYBASEASSIGN_H
#define STK_ARRAYBASEASSIGN_H

// this macro will be true if the assignation is correct and false otherwise
#define CORRECT_ASSIGN(dst,src) \
( (    ( dst==Arrays::array2D_ || dst==Arrays::square_) \
     &&( src==Arrays::array2D_ || src==Arrays::square_ || src==Arrays::diagonal_ || src==Arrays::lower_triangular_ || src==Arrays::upper_triangular_) \
  ) \
  ||( dst==Arrays::array2D_ && src==Arrays::vector_) \
  ||( dst==Arrays::array2D_ && src==Arrays::point_) \
  ||( dst==Arrays::lower_triangular_ && src==Arrays::lower_triangular_) \
  ||( dst==Arrays::upper_triangular_ && src==Arrays::upper_triangular_) \
  ||(   (dst==Arrays::diagonal_ || dst==Arrays::vector_ || dst==Arrays::point_) \
     && (src==Arrays::diagonal_ || src==Arrays::vector_ || src==Arrays::point_) \
    ) \
  ||( dst==Arrays::number_ && src==Arrays::number_) \
)

namespace STK
{


namespace hidden
{

/** @ingroup hidden
 *  @brief Copycat to use at compile time.
 *  If n is the number of structures, there is potentially n^2 ways to
 *  copy rhs inside lhs.
 *  */
template < typename Derived, typename Rhs, int TStructure_, int RhsStructure_>
struct Copycat;
//---------------------GENERAL----------------------------------
// general <- general
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::array2D_, Arrays::array2D_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i)
      for (int j = rhs.beginCols(); j < rhs.endCols(); ++j)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    for (int j = rhs.beginCols(); j < rhs.endCols(); ++j)
      for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// general <- vector
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::array2D_, Arrays::vector_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    int j = rhs.beginCols();
    for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i)
    { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    int j = rhs.beginCols();
    for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i)
    { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// general <- point
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::array2D_, Arrays::point_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    int i = rhs.beginRows();
    for (int j = rhs.beginCols(); j < rhs.endCols(); ++j)
    { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    int i = rhs.beginRows();
    for (int j = rhs.beginCols(); j < rhs.endCols(); ++j)
    { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// general <- square
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::array2D_, Arrays::square_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    for (int i = rhs.begin(); i< rhs.end(); ++i)
      for (int j = rhs.begin(); j <= rhs.lastIdx(); ++j)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    for (int j = rhs.begin(); j <= rhs.lastIdx(); ++j)
      for (int i = rhs.begin(); i< rhs.end(); ++i)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// general <- diagonal
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::array2D_, Arrays::diagonal_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    lhs.setValue(Type(0));
    for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i,i) = rhs.elt(i);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    lhs.setValue(Type(0));
    for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i,i) = rhs.elt(i);}
  }
};

// general <- lower_triangular
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::array2D_,   Arrays::lower_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j = rhs.beginCols(); j <= last; ++j)
    { int i = rhs.beginRows();
      for (; i < j; ++i)              { lhs.elt(i,j) = Type(0);}
      for (; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int j= last+1; j < rhs.endCols(); ++j)
      for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = Type(0);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int i = rhs.beginRows(); i <= last; ++i)
    { int j = rhs.beginCols();
      for (; j <=i; ++j)              { lhs.elt(i,j) = rhs.elt(i, j);}
      for (; j < rhs.endCols(); ++j) { lhs.elt(i,j) = Type(0);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j < rhs.endCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
};

// general <- upper_triangular
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::array2D_, Arrays::upper_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j = rhs.beginCols(); j <= last; ++j)
    { int i = rhs.beginRows();
      for (; i <= j; ++i)             { lhs.elt(i,j) = rhs.elt(i, j);}
      for (; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) =  Type(0);}
    }
    for (int j= last+1; j < rhs.endCols(); ++j)
      for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i = rhs.beginRows(); i <= last; ++i)
    { int j = rhs.beginCols();
      for (; j <i; ++j)               { lhs.elt(i,j) = Type(0);}
      for (; j < rhs.endCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j < rhs.endCols(); ++j) { lhs.elt(i,j) = Type(0);}
  }
};

//---------------------SQUARE----------------------------------
// square <- general
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::square_, Arrays::array2D_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i)
      for (int j = rhs.beginCols(); j < rhs.endCols(); ++j)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    for (int j = rhs.beginCols(); j < rhs.endCols(); ++j)
      for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// square <- square
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::square_, Arrays::square_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    for (int i = rhs.begin(); i< rhs.end(); ++i)
      for (int j = rhs.begin(); j <= rhs.lastIdx(); ++j)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    for (int j = rhs.begin(); j <= rhs.lastIdx(); ++j)
      for (int i = rhs.begin(); i< rhs.end(); ++i)
      { lhs.elt(i, j) = rhs.elt(i, j);}
  }
};

// square <- diagonal
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::square_, Arrays::diagonal_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    lhs.setValue(Type(0));
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i = rhs.beginRows(); i <= last; ++i) { lhs.elt(i,i) = rhs.elt(i);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    lhs.setValue(Type(0));
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i = rhs.beginRows(); i <= last; ++i) { lhs.elt(i,i) = rhs.elt(i);}
  }
};

// square_ <- lower_triangular
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::square_,   Arrays::lower_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j = rhs.beginCols(); j <= last; ++j)
    { int i = rhs.beginRows();
      for (; i < j; ++i)                 { lhs.elt(i,j) = Type(0);}
      for (; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int j= last+1; j < rhs.endCols(); ++j)
      for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = Type(0);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int i = rhs.beginRows(); i <= last; ++i)
    { int j = rhs.beginCols();
      for (; j <=i; ++j)                 { lhs.elt(i,j) = rhs.elt(i, j);}
      for (; j < rhs.endCols(); ++j) { lhs.elt(i,j) = Type(0);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j < rhs.endCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
};

// square_ <- upper triangular
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::square_, Arrays::upper_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j = rhs.beginCols(); j <= last; ++j)
    { int i = rhs.beginRows();
      for (; i <= j; ++i)                { lhs.elt(i,j) = rhs.elt(i, j);}
      for (; i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) =  Type(0);}
    }
    for (int j= last+1; j < rhs.endCols(); ++j)
      for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i = rhs.beginRows(); i <= last; ++i)
    { int j = rhs.beginCols();
      for (; j <i; ++j)                  { lhs.elt(i,j) = Type(0);}
      for (; j < rhs.endCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j < rhs.endCols(); ++j) { lhs.elt(i,j) = Type(0);}
  }
};

//---------------------LDO----------------------------------
// lower_triangular <- lower_triangular
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::lower_triangular_, Arrays::lower_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j = rhs.beginCols(); j <= last; ++j)
      for (int i=j; i <= rhs.lastIdxRows(); ++i)
      { lhs.elt(i,j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int i = rhs.beginRows(); i <= last; ++i)
    {
      for (int j = rhs.beginCols(); j <=i; ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int i= last+1; i <= rhs.lastIdxRows(); ++i)
      for (int j=1; j < rhs.endCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
};

//---------------------LUP----------------------------------
// upper_triangular <- upper_triangular
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::upper_triangular_, Arrays::upper_triangular_>
{
  typedef typename hidden::Traits<Derived>::Type Type;
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(rhs.lastIdxRows(), rhs.lastIdxCols());
    for (int j = rhs.beginCols(); j <= last; ++j)
    {
      for (int i = rhs.beginRows(); i <= j; ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
    }
    for (int j= last+1; j < rhs.endCols(); ++j)
      for (int i = rhs.beginRows(); i <= rhs.lastIdxRows(); ++i) { lhs.elt(i,j) = rhs.elt(i, j);}
  }
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  {
    const int last = std::min(lhs.lastIdxRows(), lhs.lastIdxCols());
    for (int i = rhs.beginRows(); i <= last; ++i)
    { for (int j=i; j < rhs.endCols(); ++j) { lhs.elt(i,j) = rhs.elt(i, j);}}
  }
};


//---------------------DIAGONAL----------------------------------
// diagonal <- diagonal
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::diagonal_, Arrays::diagonal_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

// diagonal <- vector
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::diagonal_, Arrays::vector_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

// diagonal <- point
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::diagonal_, Arrays::point_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

//---------------------VECTOR----------------------------------
//  vector <- diagonal
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::vector_, Arrays::diagonal_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

//  vector <- vector
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::vector_, Arrays::vector_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};
// vector <- point
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::vector_, Arrays::point_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

//---------------------POINT----------------------------------
//  point_ <- diagonal
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::point_, Arrays::diagonal_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};

//  vector <- vector
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::point_, Arrays::vector_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};
// vector <- point
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::point_, Arrays::point_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { for (int i = rhs.begin(); i< rhs.end(); ++i) { lhs.elt(i) = rhs.elt(i);}}
};


//---------------------NUMBER----------------------------------
//  point_ <- diagonal
template < typename Derived, typename Rhs>
struct Copycat<  Derived,  Rhs, Arrays::number_, Arrays::number_>
{
  inline static void runByCol(Derived& lhs, Rhs const& rhs )
  { lhs.elt() = rhs.elt();}
  inline static void runByRow(Derived& lhs, Rhs const& rhs )
  { lhs.elt() = rhs.elt();}
};

} // namespace hidden


namespace hidden
{

/** @ingroup hidden
 * utility class that select if the copy will be by row or by column
 **/
template < typename Derived, typename Rhs, int TOrient_>
struct CopycatSelector;

/** specialization for column oriented arrrays */
template< typename Derived, typename Rhs>
struct CopycatSelector< Derived, Rhs, Arrays::by_col_>
{
  enum
  { tstructure_ = hidden::Traits<Derived>::structure_
  , sstructure_ = hidden::Traits<Rhs>::structure_
  };
  inline static void run(Derived& lhs, Rhs const& rhs )
  { Copycat<Derived, Rhs, tstructure_, sstructure_>::runByCol(lhs, rhs );}
};

/** specialization for row oriented arrrays */
template< typename Derived, typename Rhs>
struct CopycatSelector< Derived, Rhs, Arrays::by_row_>
{
  enum
  { tstructure_ = hidden::Traits<Derived>::structure_
  , sstructure_ = hidden::Traits<Rhs>::structure_
  };
  inline static void run(Derived& lhs, Rhs const& rhs )
  { Copycat<Derived, Rhs, tstructure_, sstructure_>::runByRow(lhs, rhs );}
};

} // namespace hidden

namespace hidden
{

/** @ingroup hidden
 * utility class that select the resize method to call
 **/
template< typename Lhs, typename Rhs, int TStructure_>
struct resizeSelector;

/** 2D general case */
template< typename Lhs, typename Rhs, int TStructure_>
struct resizeSelector
{
  static void run(Lhs& dst, ExprBase<Rhs> const& src )
  { dst.resize(src.rows(), src.cols());}
};
/** specialization for the diagonal_ case */
template< typename Lhs, typename Rhs>
struct resizeSelector<Lhs, Rhs, Arrays::diagonal_>
{
  static void run(Lhs& dst, ExprBase<Rhs> const& src )
  { dst.resize(src.range());}
};
/** specialization for the vector_ case */
template< typename Lhs, typename Rhs>
struct resizeSelector<Lhs, Rhs, Arrays::vector_>
{
  static void run(Lhs& dst, ExprBase<Rhs> const& src )
  { dst.resize(src.range());}
};
/** specialization for the point_ case */
template< typename Lhs, typename Rhs>
struct resizeSelector<Lhs, Rhs, Arrays::point_>
{
  static void run(Lhs& dst, ExprBase<Rhs> const& src )
  { dst.resize(src.range());}
};

}  // namespace hidden

/* @brief assign src to this
 **/
template<class Derived>
template<class Rhs>
Derived& ArrayBase<Derived>::assign(ExprBase<Rhs> const& rhs)
{
    enum
    {
      lhs_struct_ = hidden::Traits<Derived>::structure_
    , lhs_orient_ = hidden::Traits<Derived>::orient_
    , lhs_sizeRows_ = hidden::Traits<Derived>::sizeRows_
    , lhs_sizeCols_ = hidden::Traits<Derived>::sizeCols_
    , rhs_struct_ = hidden::Traits<Rhs>::structure_
    , rhs_orient_ = hidden::Traits<Rhs>::orient_
    , rhs_sizeRows_ = hidden::Traits<Rhs>::sizeRows_
    , rhs_sizeCols_ = hidden::Traits<Rhs>::sizeCols_
    };
    STK_STATICASSERT(CORRECT_ASSIGN((Arrays::Structure)lhs_struct_, (Arrays::Structure)rhs_struct_),YOU_TRIED_TO_ASSIGN_A_NOT_COMPATIBLE_ARRAY);
  // choose the correct way to resize if necessary
  hidden::resizeSelector<Derived, Rhs, lhs_struct_>::run(this->asDerived(), rhs.asDerived());
  // choose the correct way to copy
  hidden::CopycatSelector<Derived, Rhs,  orient_>::run(this->asDerived(), rhs.asDerived());
  return this->asDerived();
}

/* Adding a Rhs to this. */
template<class Derived>
template<typename Rhs>
Derived&  ArrayBase<Derived>::operator+=( ExprBase<Rhs> const& other)
{
  this->asDerived() = this->asDerived() + other;
  return this->asDerived();
}
/* substract a Rhs to this. */
template<class Derived>
template<typename Rhs>
Derived&  ArrayBase<Derived>::operator-=( ExprBase<Rhs> const& other)
{
  this->asDerived() = this->asDerived() - other;
  return this->asDerived();
}

/* Adding a constant to this. */
template<class Derived>
Derived& ArrayBase<Derived>::operator+=( Type const other)
{
  this->asDerived() = this->asDerived() + other;
  return this->asDerived();
}
/* Substract a constant to this. */
template<class Derived>
Derived& ArrayBase<Derived>::operator-=( Type const other)
{
  this->asDerived() = this->asDerived() - other;
  return this->asDerived();
}
/* product of this by a constant. */
template<class Derived>
Derived& ArrayBase<Derived>::operator*=( Type const other)
{
  this->asDerived() = this->asDerived() * other;
  return this->asDerived();
}
/* dividing this by a constant. */
template<class Derived>
Derived& ArrayBase<Derived>::operator/=( Type const other)
{
  this->asDerived() = this->asDerived() / other;
  return this->asDerived();
}

} // namespace STK

#undef CORRECT_ASSIGN

#endif /* STK_ARRAYBASEASSIGN_H */
