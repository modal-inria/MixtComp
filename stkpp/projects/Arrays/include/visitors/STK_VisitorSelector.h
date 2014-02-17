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
 * created on: 27 sept. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_VisitorSelector.h
 *  @brief In this file we implement the selector of the visotr and apply pattern.
 **/
#ifndef STK_VISITORSELECTOR_H
#define STK_VISITORSELECTOR_H

namespace STK
{

namespace hidden
{

/** @ingroup hidden
 *  @brief general visitor */
template<typename Visitor, typename Derived, int Structure_, int SizeRows_, int SizeCols_>
struct VisitorSelector
{
  typedef Visitor2D<Visitor, Derived, hidden::Traits<Derived>::orient_> VisitorType;
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
  VisitorType::run(matrix, visitor);
  }
};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelector<Visitor, Derived, Arrays::diagonal_, SizeRows_, SizeCols_>
{
  inline static void run( Derived const& matrix, Visitor& visitor)
  { for(int j = matrix.firstIdx(); j <= matrix.lastIdx(); ++j) visitor(matrix.elt(j),j,j);}
};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelector<Visitor, Derived, Arrays::vector_, SizeRows_, SizeCols_>
{
  inline static void run( Derived const& v, Visitor& visitor)
  { for(int i = v.firstIdx(); i <= v.lastIdx(); ++i) visitor(v.elt(i), i, v.colIdx());}
};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelector<Visitor, Derived, Arrays::point_, SizeRows_, SizeCols_>
{
  inline static void run( Derived const& p, Visitor& visitor)
  { for(int j = p.firstIdx(); j <= p.lastIdx(); ++j) visitor(p.elt(j), p.rowIdx(), j);}
};

/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays (by_col) */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelector<Visitor, Derived, Arrays::upper_triangular_, SizeRows_, SizeCols_>
{
  typedef VisitorUpper<Visitor, Derived, hidden::Traits<Derived>::orient_> VisitorType;
  inline static void run( Derived const& matrix, Visitor& visitor)
  { VisitorType::run(matrix, visitor);}
};
/** @ingroup hidden
 *  @brief specialization for the lower triangular arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelector<Visitor, Derived, Arrays::lower_triangular_, SizeRows_, SizeCols_>
{
  typedef VisitorLower<Visitor, Derived, hidden::Traits<Derived>::orient_> VisitorType;
  inline static void run( Derived const& matrix, Visitor& visitor)
  { VisitorType::run(matrix, visitor);}
};


/** @ingroup hidden
 *  @brief general visitor */
template<typename Visitor, typename Derived, int Structure_, int SizeRows_, int SizeCols_>
struct ApplySelector
{
  typedef Visitor2D<Visitor, Derived, hidden::Traits<Derived>::orient_> VisitorType;
  inline static void run( Derived const& matrix, Visitor& visitor)
  {
    //  if (!this->asDerived().isRef() || ((this->firstIdxRows()==1)&&(this->firstIdxCols()==1)))
    //  {
    //    Derived& ARef = const_cast<Derived&> (matrix.asDerived());
    //    const int firstRow = ARef.firstIdxRows(), firstCol = ARef.firstIdxCols();
    //    ARef.shift(1, 1);
    //    enum { unrollrow_ = (sizeRows_ < MaxUnrollSquareRoot),
    //           unrollcol_ = (sizeCols_ < MaxUnrollSquareRoot)
    //         };
    //    hidden::GenVisitor< Visitor, Derived, orient_
    //                            , (unrollrow_) ? int(sizeRows_) : UnknownSize
    //                            , (unrollcol_) ? int(sizeCols_) : UnknownSize
    //                        >::run(ARef, visitor);
    //    ARef.shift(firstRow, firstCol);
    //  }
    //  else // just loop over the array using the orientation order
    //  { hidden::Visitor2D< Visitor, Derived, orient_>::run(this->asDerived(), visitor);}
    //      enum {// structure_ = structure_,
    //             unrollrow_ = (sizeRows_ < MaxUnrollSquareRoot),
    //             unrollcol_ = (sizeCols_ < MaxUnrollSquareRoot)
    //  };
  VisitorType::run(matrix, visitor);
  }
  inline static void apply( Derived& matrix, Visitor& visitor)
  {
    //  if (!this->asDerived().isRef() || ((this->firstIdxRows()==1)&&(this->firstIdxCols()==1)))
    //  {
    //    const int firstRow = this->firstIdxRows(), firstCol = this->firstIdxCols();
    //    if (!this->asDerived().isRef()) this->asDerived().shift(1, 1);
    //    enum { unrollrow_ = (sizeRows_ < MaxUnrollSquareRoot),
    //           unrollcol_ = (sizeCols_ < MaxUnrollSquareRoot)
    //         };
    //    hidden::GenVisitor< Visitor, Derived, orient_
    //                           , (unrollrow_) ? int(sizeRows_) : UnknownSize
    //                           , (unrollcol_) ? int(sizeCols_) : UnknownSize
    //                        >::apply(this->asDerived(), visitor);
    //    if (!this->asDerived().isRef()) this->asDerived().shift(firstRow, firstCol);
    //  }
    //  else
    //  {
    //    hidden::Visitor2D< Visitor, Derived, orient_>::apply(this->asDerived(), visitor);
    //  }
VisitorType::apply(matrix, visitor);}
};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct ApplySelector<Visitor, Derived, Arrays::diagonal_, SizeRows_, SizeCols_>
{
  inline static void apply( Derived& matrix, Visitor& visitor)
  { for(int j = matrix.firstIdx(); j <= matrix.lastIdx(); ++j) visitor(matrix.elt(j));}
};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct ApplySelector<Visitor, Derived, Arrays::vector_, SizeRows_, SizeCols_>
{
  inline static void apply( Derived& v, Visitor& visitor)
  { for(int i = v.firstIdx(); i <= v.lastIdx(); ++i) visitor(v.elt(i));}
};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct ApplySelector<Visitor, Derived, Arrays::point_, SizeRows_, SizeCols_>
{
  inline static void apply( Derived& p, Visitor& visitor)
  { for(int j = p.firstIdx(); j <= p.lastIdx(); ++j) visitor(p.elt(j));}
};

/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays (by_col) */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct ApplySelector<Visitor, Derived, Arrays::upper_triangular_, SizeRows_, SizeCols_>
{
  typedef VisitorUpper<Visitor, Derived, hidden::Traits<Derived>::orient_> VisitorType;
  inline static void apply( Derived& matrix, Visitor& visitor)
  { VisitorType::apply(matrix, visitor);}
};
/** @ingroup hidden
 *  @brief specialization for the lower triangular arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct ApplySelector<Visitor, Derived, Arrays::lower_triangular_, SizeRows_, SizeCols_>
{
  typedef VisitorLower<Visitor, Derived, hidden::Traits<Derived>::orient_> VisitorType;
  inline static void apply( Derived& matrix, Visitor& visitor)
  { VisitorType::apply(matrix, visitor);}
};

} // namespace hidden

} // namespace STK

#endif /* STK_VISITORSIMPL_H */
