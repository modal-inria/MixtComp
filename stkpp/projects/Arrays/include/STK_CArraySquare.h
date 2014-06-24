/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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
 * created on: 25 nov. 2011
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_CArraySquare.h
 *  @brief In this file we implement the final class CArraySquare.
 **/

#ifndef STK_CARRAYSQUARE_H
#define STK_CARRAYSQUARE_H

#include "STKernel/include/STK_Constants.h"

#include "STK_ICArray.h"

namespace STK
{
template< typename Type_, int Size_ = UnknownSize, bool Orient_ = Arrays::by_col_>
class CArraySquare;

template< typename Type, int SizeRows_, int SizeCols_, bool Orient_>
class CArray;
template< typename Type, int SizeCols_, bool Orient_>
class CArrayPoint;
template< typename Type, int SizeRows_, bool Orient_>
class CArrayVector;
template< typename Type, int SizeRows_, int SizeCols_, bool Orient_>
class CArrayNumber;



typedef CArraySquare<Real>                        CArraySquareXX;
typedef CArraySquare<Real, 2, Arrays::by_col_>    CArraySquare22;
typedef CArraySquare<Real, 3, Arrays::by_col_>    CArraySquare33;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for CArray class.
 */
template<typename Type_, int Size_, bool Orient_>
struct Traits< CArraySquare<Type_, Size_, Orient_> >
{
  private:
    class Void { };

    typedef CArrayPoint<Type_, Size_, Arrays::by_col_> RowIndirect;
    typedef CArrayPoint<Type_, Size_, Arrays::by_row_> RowDirect;

    typedef CArrayPoint<Type_, UnknownSize, Arrays::by_col_> SubRowIndirect;
    typedef CArrayPoint<Type_, UnknownSize, Arrays::by_row_> SubRowDirect;

    typedef CArrayVector<Type_, Size_, Arrays::by_row_> ColIndirect;
    typedef CArrayVector<Type_, Size_, Arrays::by_col_> ColDirect;

    typedef CArrayVector<Type_, UnknownSize, Arrays::by_row_> SubColIndirect;
    typedef CArrayVector<Type_, UnknownSize, Arrays::by_col_> SubColDirect;

    typedef CArraySquare<Type_, Size_, Arrays::by_row_> FixedRowArrayIndirect;
    typedef CArraySquare<Type_, Size_, Arrays::by_row_> FixedColArrayDirect;

    typedef CArraySquare<Type_, Size_, Arrays::by_col_> FixedRowArrayDirect;
    typedef CArraySquare<Type_, Size_, Arrays::by_col_> FixedColArrayIndirect;

  public:
    typedef CArrayNumber<Type_, 1, 1, Orient_> Number;

    typedef typename If<Orient_, RowIndirect, RowDirect >::Result  Row;
    typedef typename If<Orient_, ColDirect, ColIndirect >::Result  Col;

    typedef typename If<Orient_, SubRowIndirect, SubRowDirect >::Result  SubRow;
    typedef typename If<Orient_, SubColDirect, SubColIndirect >::Result  SubCol;

    /** If one of the Size is 1, we have a Vector (a column) or a Point (a row)
     *  (What to do if both are = 1 : Type or array (1,1) ?).
     **/
    typedef typename If< (Size_ == 1)||(Size_ == 1)   // one row or one column
                       , typename If<(Size_ == 1), SubCol, SubRow>::Result
                       , Void
                       >::Result SubVector;
    // FIXME does not seem optimal if we want only to get a subset of rows (columns)
    typedef CArray<Type_, UnknownSize, UnknownSize, Orient_> SubArray;
//    typedef typename If< Orient_ == Arrays::by_col_
//                       , typename If<SizeRows_ != UnknownSize, FixedRowArrayDirect, FixedColArrayIndirect>::Result
//                       , typename If<SizeCols_ != UnknownSize, FixedRowArrayIndirect, FixedColArrayDirect>::Result
//                       >::Result SubArray;
    // Transposed type
    typedef CArraySquare< Type_, Size_, !Orient_> Transposed;
    // The CAllocator have to have the same structure than the CArray
    typedef CAllocator<Type_, Arrays::square_, Size_, Size_, Orient_> Allocator;

    typedef Type_ Type;
    enum
    {
      structure_ = Arrays::square_,
      orient_    = Orient_,
      sizeRows_  = Size_,
      sizeCols_  = Size_,
      storage_   = Arrays::dense_
    };
};

} // namespace hidden

/** @ingroup Arrays
 *  @brief specialization for the square case.
 */
template <typename Type_, int Size_, bool Orient_>
class CArraySquare
      : public ICArray < CArraySquare<Type_, Size_, Orient_> >
{
  public:
    typedef ICArray < CArraySquare<Type_, Size_, Orient_> > Base;
    typedef ArrayBase < CArraySquare<Type_, Size_, Orient_> > LowBase;

    typedef typename hidden::Traits< CArraySquare <Type_, Size_> >::Type Type;
    enum
    {
      structure_ = hidden::Traits< CArraySquare <Type_, Size_, Orient_> >::structure_,
      orient_    = hidden::Traits< CArraySquare <Type_, Size_, Orient_> >::orient_,
      sizeRows_  = hidden::Traits< CArraySquare <Type_, Size_, Orient_> >::sizeRows_,
      sizeCols_  = hidden::Traits< CArraySquare <Type_, Size_, Orient_> >::sizeCols_,
      storage_   = hidden::Traits< CArraySquare <Type_, Size_, Orient_> >::storage_
    };
    /** Default constructor. */
    inline CArraySquare() : Base() {}
    /** constructor with specified dimension.
     *  @param size range of the columns
     **/
    inline CArraySquare( int const& size) : Base(size, size) {}
    /** constructor with rbeg, rend, cbeg and cend specified,
     *  initialization with a constant.
     *  @param size range of the columns
     *  @param v initial value of the container
     **/
    inline CArraySquare( int const& size, Type const& v) : Base(size, size, v) {}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    inline CArraySquare( CArraySquare const& T, bool ref=false) : Base(T, ref) {}
    /** wrapper constructor for 0 based C-Array.
     *  @param q pointer on the array
     *  @param size number of rows/columns
     **/
    inline CArraySquare( Type* const& q, int size): Base(q, size, size) {}
    /** constructor by reference.
     *  @param allocator the allocator to wrap
     **/
    template<class OtherAllocator>
    inline CArraySquare( OtherAllocator const& allocator) : Base(allocator) {}
    /** destructor. */
    inline ~CArraySquare() {}
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    inline CArraySquare& operator=(Type const& v) { this->setValue(v); return *this;}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline CArraySquare& operator=(Rhs const& T) { return LowBase::assign(T);}
    /** operator = : overwrite the CArray with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    inline CArraySquare& operator=(CArraySquare const& rhs) { return LowBase::assign(rhs);}
};

} // namespace STK


#endif /* STK_CARRAYSQUARE_H */
