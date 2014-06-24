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

/** @file STK_CArray.h
 *  @brief In this file we implement the final class CArray.
 **/

#ifndef STK_CARRAY_H
#define STK_CARRAY_H

#include "STKernel/include/STK_Constants.h"

#include "STK_ICArray.h"
#include "STK_Display.h"

namespace STK
{
template< typename Type, int SizeRows_ = UnknownSize, int SizeCols_ = UnknownSize, bool Orient_ = Arrays::by_col_>
class CArray;

template< typename Type, int SizeCols_, bool Orient_>
class CArrayPoint;
template< typename Type, int SizeRows_, bool Orient_>
class CArrayVector;
template< typename Type, int SizeRows_, int SizeCols_, bool Orient_>
class CArrayNumber;

typedef CArray<Real>                                  CArrayXX;
typedef CArray<Real, UnknownSize, 2, Arrays::by_col_> CArrayX2;
typedef CArray<Real, UnknownSize, 3, Arrays::by_col_> CArrayX3;
typedef CArray<Real, 2, 2, Arrays::by_col_>           CArray22;
typedef CArray<Real, 3, 3, Arrays::by_col_>           CArray33;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for CArray class.
 */
template<typename Type_, int SizeRows_, int SizeCols_, bool Orient_>
struct Traits< CArray<Type_, SizeRows_, SizeCols_, Orient_> >
{
  private:
    class Void { };

    typedef CArrayPoint<Type_, SizeCols_, Arrays::by_col_> RowIndirect;
    typedef CArrayPoint<Type_, SizeCols_, Arrays::by_row_> RowDirect;

    typedef CArrayPoint<Type_, UnknownSize, Arrays::by_col_> SubRowIndirect;
    typedef CArrayPoint<Type_, UnknownSize, Arrays::by_row_> SubRowDirect;

    typedef CArrayVector<Type_, SizeRows_, Arrays::by_row_> ColIndirect;
    typedef CArrayVector<Type_, SizeRows_, Arrays::by_col_> ColDirect;

    typedef CArrayVector<Type_, UnknownSize, Arrays::by_row_> SubColIndirect;
    typedef CArrayVector<Type_, UnknownSize, Arrays::by_col_> SubColDirect;

    typedef CArray<Type_, SizeRows_, UnknownSize, Arrays::by_row_> FixedRowArrayIndirect;
    typedef CArray<Type_, UnknownSize, SizeCols_, Arrays::by_row_> FixedColArrayDirect;

    typedef CArray<Type_, SizeRows_, UnknownSize, Arrays::by_col_> FixedRowArrayDirect;
    typedef CArray<Type_, UnknownSize, SizeCols_, Arrays::by_col_> FixedColArrayIndirect;

  public:
    typedef CArrayNumber<Type_, 1, 1, Orient_> Number;

    typedef typename If<Orient_, RowIndirect, RowDirect >::Result  Row;
    typedef typename If<Orient_, ColDirect, ColIndirect >::Result  Col;

    typedef typename If<Orient_, SubRowIndirect, SubRowDirect >::Result  SubRow;
    typedef typename If<Orient_, SubColDirect, SubColIndirect >::Result  SubCol;

    /** If one of the Size is 1, we have a Vector (a column) or a Point (a row)
     *  (What to do if both are = 1 : Type or array (1,1) ?).
     **/
    typedef typename If< (SizeRows_ == 1)||(SizeCols_ == 1)   // one row or one column
                       , typename If<(SizeCols_ == 1), SubCol, SubRow>::Result
                       , Void
                       >::Result SubVector;
    // FIXME does not seem optimal if we want only to get a subset of rows (columns)
    typedef CArray<Type_, UnknownSize, UnknownSize, Orient_> SubArray;
//    typedef typename If< Orient_ == Arrays::by_col_
//                       , typename If<SizeRows_ != UnknownSize, FixedRowArrayDirect, FixedColArrayIndirect>::Result
//                       , typename If<SizeCols_ != UnknownSize, FixedRowArrayIndirect, FixedColArrayDirect>::Result
//                       >::Result SubArray;
    // Transposed type
    typedef CArray< Type_, SizeCols_, SizeRows_, !Orient_> Transposed;
    // The CAllocator have to have the same structure than the CArray
    typedef CAllocator<Type_, Arrays::array2D_, SizeRows_, SizeCols_, Orient_> Allocator;

    typedef Type_ Type;
    enum
    {
      structure_ = Arrays::array2D_,
      orient_    = Orient_,
      sizeRows_  = SizeRows_,
      sizeCols_  = SizeCols_,
      storage_   = Arrays::dense_
    };
};

} // namespace hidden

/** @ingroup Arrays
 * @brief A CArray is a two dimensional array with a continuous storage like
 * a C-array.
 */
template <typename Type, int SizeRows_, int SizeCols_, bool Orient_>
class CArray: public ICArray < CArray<Type, SizeRows_, SizeCols_, Orient_> >
{
  public:
    typedef ICArray < CArray<Type, SizeRows_, SizeCols_, Orient_> > Base;
    typedef ArrayBase < CArray<Type, SizeRows_, SizeCols_, Orient_> > LowBase;
    enum
    {
      structure_ = Arrays::array2D_,
      orient_    = Orient_,
      sizeRows_  = SizeRows_,
      sizeCols_  = SizeCols_,
      storage_   = Arrays::dense_
    };

    /** Default constructor. */
    inline CArray() : Base() {}
    /** constructor with specified dimension.
     *  @param sizeRows size of the rows
     *  @param sizeCols size of the columns
     **/
    inline CArray( int const& sizeRows, int const& sizeCols) : Base(sizeRows, sizeCols) {}
    /** constructor with rbeg, rend, cbeg and cend specified,
     *  initialization with a constant.
     *  @param sizeRows size of the rows
     *  @param sizeCols size of the columns
     *  @param v initial value of the container
     **/
    inline CArray( int const& sizeRows, int const& sizeCols, Type const& v)
                 : Base(sizeRows, sizeCols, v)
    {}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    inline CArray( CArray const& T, bool ref=false) : Base(T, ref) {}
    /** wrapper constructor for 0 based C-Array.
     *  @param q pointer on the array
     *  @param nbRow number of rows
     *  @param nbCol number of columns
     **/
    inline CArray( Type* const& q, int nbRow, int nbCol): Base(q, nbRow, nbCol) {}
    /** constructor by reference.
     *  @param allocator the allocator to wrap
     **/
    template<class OtherAllocator>
    inline CArray( OtherAllocator const& allocator) : Base(allocator) {}
    /** destructor. */
    inline ~CArray() {}
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    inline CArray& operator=(Type const& v) { this->setValue(v); return *this;}
    /** operator = : overwrite this with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    template<class Rhs>
    inline CArray& operator=(ExprBase<Rhs> const& rhs)
    { return this->assign(rhs.asDerived());}
    /** operator = : overwrite the CArray with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    inline CArray& operator=(CArray const& rhs) { return LowBase::assign(rhs);}
};

} // namespace STK


#endif /* STK_CARRAY_H */
