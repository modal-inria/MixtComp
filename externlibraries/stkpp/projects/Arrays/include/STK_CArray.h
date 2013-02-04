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

 Contact : Serge.Iovleff@stkpp.org
 */

/*
 * Project:  stkpp::Arrays
 * created on: 25 nov. 2011
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_CArray.h
 *  @brief In this file we implement the final class CArray<TYPE>.
 **/

#ifndef STK_CARRAY2D_H
#define STK_CARRAY2D_H

#include "../../STKernel/include/STK_Constants.h"
#include "../../STKernel/include/STK_MetaTemplate.h"
#include "../../STKernel/include/STK_StaticAssert.h"

#include "../../Sdk/include/STK_Traits.h"

#include "STK_Arrays_Util.h"
#include "STK_CAllocator.h"
#include "STK_ICArray.h"

namespace STK
{
/** forward declaration of the CArray class */
template< typename Type
        , Arrays::Structure  Structure_
        , int SizeRows_ = UnknownSize
        , int SizeCols_ = UnknownSize
        , bool Orient_ = Arrays::by_col_
        >
class CArray;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for CArray class.
 */
template<typename Scalar, Arrays::Structure Structure_, int SizeRows_, int SizeCols_, bool Orient_>
struct Traits< CArray<Scalar, Structure_, SizeRows_, SizeCols_, Orient_> >
{
  private:
    class Void { };

    typedef CArray<Scalar, Arrays::point_, 1, SizeCols_, Arrays::by_col_> RowIndirect;
    typedef CArray<Scalar, Arrays::point_, 1, SizeCols_, Arrays::by_row_> RowDirect;

    typedef CArray<Scalar, Arrays::point_, 1, UnknownSize, Arrays::by_col_> SubRowIndirect;
    typedef CArray<Scalar, Arrays::point_, 1, UnknownSize, Arrays::by_row_> SubRowDirect;

    typedef CArray<Scalar, Arrays::vector_, SizeRows_, 1, Arrays::by_row_> ColIndirect;
    typedef CArray<Scalar, Arrays::vector_, SizeRows_, 1, Arrays::by_col_> ColDirect;

    typedef CArray<Scalar, Arrays::vector_, UnknownSize, 1, Arrays::by_row_> SubColIndirect;
    typedef CArray<Scalar, Arrays::vector_, UnknownSize, 1, Arrays::by_col_> SubColDirect;

    typedef CArray<Scalar, Structure_, SizeRows_, UnknownSize, Arrays::by_row_> FixedRowArrayIndirect;
    typedef CArray<Scalar, Structure_, UnknownSize, SizeCols_, Arrays::by_row_> FixedColArrayDirect;

    typedef CArray<Scalar, Structure_, SizeRows_, UnknownSize, Arrays::by_col_> FixedRowArrayDirect;
    typedef CArray<Scalar, Structure_, UnknownSize, SizeCols_, Arrays::by_col_> FixedColArrayIndirect;

  public:
    typedef Scalar Type;
    typedef CArray<Scalar, Arrays::number_, 1, 1, Orient_> Number;

    typedef typename If<Orient_, RowIndirect, RowDirect >::Result  Row;
    typedef typename If<Orient_, ColDirect, ColIndirect >::Result  Col;

    typedef typename If<Orient_, SubRowIndirect, SubRowDirect >::Result  SubRow;
    typedef typename If<Orient_, SubColDirect, SubColIndirect >::Result  SubCol;

    /** If one of the Size is 1, we have a Vector (a column) or a Point (a row)
     *  (What to do if both are = 1 : Scalar or array (1,1) ?).
     **/
    typedef typename If< (SizeRows_ == 1)||(SizeCols_ == 1)   // one row or one column
                       , typename If<(SizeCols_ == 1), SubCol, SubRow>::Result
                       , Void
                       >::Result SubVector;
    // FIXME does not seem optimal if we want only to get a subset of rows (columns)
    typedef CArray<Type, Structure_, UnknownSize, UnknownSize, Orient_> SubArray;
//    typedef typename If< Orient_ == Arrays::by_col_
//                       , typename If<SizeRows_ != UnknownSize, FixedRowArrayDirect, FixedColArrayIndirect>::Result
//                       , typename If<SizeCols_ != UnknownSize, FixedRowArrayIndirect, FixedColArrayDirect>::Result
//                       >::Result SubArray;
    // Transposed type
    typedef CArray< Type, (Arrays::Structure)TStructure<Structure_>::structure_, SizeCols_, SizeRows_, !Orient_> Transposed;
    // The CAllocator have to have the same structure than the CArray
    typedef CAllocator<Type, Structure_, SizeRows_, SizeCols_, Orient_> Allocator;

    enum
    {
      structure_ = Structure_,
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
template <typename Type, Arrays::Structure Structure_, int SizeRows_, int SizeCols_, bool Orient_>
class CArray: public ICArray < CArray<Type, Structure_, SizeRows_, SizeCols_, Orient_> >
{
  public:
    typedef ICArray < CArray<Type, Structure_, SizeRows_, SizeCols_, Orient_> > Base;
    typedef ArrayBase < CArray<Type, Structure_, SizeRows_, SizeCols_, Orient_> > LowBase;
    enum
    {
      structure_ = Structure_,
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
    inline CArray( const CArray &T, bool ref=false) : Base(T, ref) {}
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
    inline CArray& operator=(ArrayBase<Rhs> const& rhs)
    { return this->assign(rhs.asDerived());}
    /** operator = : overwrite the CArray with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    inline CArray& operator=(CArray const& rhs) { return LowBase::assign(rhs);}
};

/** @ingroup Arrays
 * @brief specialization for the point case.
 */
template <typename Type, int SizeRows_, int SizeCols_, bool Orient_>
class CArray <Type, Arrays::square_, SizeRows_, SizeCols_, Orient_>
      : public ICArray < CArray<Type, Arrays::square_, SizeRows_, SizeCols_, Orient_> >
{
  public:
    typedef ICArray < CArray<Type, Arrays::square_, SizeRows_, SizeCols_, Orient_> > Base;
    typedef ArrayBase < CArray<Type, Arrays::point_, SizeRows_, SizeCols_, Orient_> > LowBase;
    enum
    {
      structure_ = Arrays::square_,
      orient_    = Orient_,
      sizeRows_  = SizeRows_,
      sizeCols_  = SizeCols_,
      storage_   = Arrays::dense_
    };

    /** Default constructor. */
    inline CArray() : Base()
    { STK_STATICASSERT_SQUARE_SIZES_MISMATCH(SizeRows_==SizeCols_);}
    /** constructor with specified dimension.
     *  @param size range of the columns
     **/
    inline CArray( int const& size) : Base(size, size)
    { STK_STATICASSERT_SQUARE_SIZES_MISMATCH(SizeRows_==SizeCols_);}
    /** constructor with rbeg, rend, cbeg and cend specified,
     *  initialization with a constant.
     *  @param size range of the columns
     *  @param v initial value of the container
     **/
    inline CArray( int const& size, Type const& v) : Base(size, size, v)
    { STK_STATICASSERT_SQUARE_SIZES_MISMATCH(SizeRows_==SizeCols_);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    inline CArray( const CArray &T, bool ref=false) : Base(T, ref)
    { STK_STATICASSERT_SQUARE_SIZES_MISMATCH(SizeRows_==SizeCols_);}
    /** constructor by reference.
     *  @param allocator the allocator to wrap
     **/
    template<class OtherAllocator>
    inline CArray( OtherAllocator const& allocator) : Base(allocator)
    { STK_STATICASSERT_SQUARE_SIZES_MISMATCH(SizeRows_==SizeCols_);}
    /** destructor. */
    inline ~CArray() {}
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    inline CArray& operator=(Type const& v) { this->setValue(v); return *this;}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline CArray& operator=(Rhs const& T) { return LowBase::assign(T);}
    /** operator = : overwrite the CArray with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    inline CArray& operator=(CArray const& rhs) { return LowBase::assign(rhs);}
};

/** @ingroup Arrays
 * @brief specialization for the point case.
 */
template <typename Type, int SizeRows_, int SizeCols_, bool Orient_>
class CArray <Type, Arrays::point_, SizeRows_, SizeCols_, Orient_>
      : public ICArray < CArray<Type, Arrays::point_, SizeRows_, SizeCols_, Orient_> >
{
  public:
    typedef ICArray < CArray<Type, Arrays::point_, 1, SizeCols_, Orient_> > Base;
    typedef ArrayBase < CArray<Type, Arrays::point_, 1, SizeCols_, Orient_> > LowBase;
    enum
    {
      structure_ = Arrays::point_,
      orient_    = Orient_,
      sizeRows_  = 1,
      sizeCols_  = SizeCols_,
      storage_   = Arrays::dense_
    };

    /** Default constructor. */
    inline CArray() : Base()
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(SizeRows_==1);}
    /** constructor with specified dimension.
     *  @param sizeCols range of the columns
     **/
    inline CArray( int const& sizeCols) : Base(1, sizeCols)
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(SizeRows_==1);}
    /** constructor with rbeg, rend, cbeg and cend specified,
     *  initialization with a constant.
     *  @param sizeCols range of the columns
     *  @param v initial value of the container
     **/
    inline CArray( int const& sizeCols, Type const& v) : Base(1, sizeCols, v)
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(SizeRows_==1);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    inline CArray( const CArray &T, bool ref=false) : Base(T, ref)
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(SizeRows_==1);}
    /** constructor by reference.
     *  @param allocator the allocator to wrap
     **/
    template<class OtherAllocator>
    inline CArray( OtherAllocator const& allocator) : Base(allocator)
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(SizeRows_==1);}
    /** destructor. */
    inline ~CArray() {}
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    inline CArray& operator=(Type const& v) { this->setValue(v); return *this;}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline CArray& operator=(Rhs const& T) { return LowBase::assign(T);}
    /** operator = : overwrite the CArray with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    inline CArray& operator=(CArray const& rhs) { return LowBase::assign(rhs);}
};

/** @ingroup Arrays
 * @brief specialization for the vector case.
 */
template <typename Type, int SizeRows_, int SizeCols_, bool Orient_>
class CArray <Type, Arrays::vector_, SizeRows_, SizeCols_, Orient_>
      : public ICArray < CArray<Type, Arrays::vector_, SizeRows_, SizeCols_, Orient_> >
{
  public:
    typedef ICArray < CArray<Type, Arrays::vector_, SizeRows_, SizeCols_, Orient_> > Base;
    typedef ArrayBase < CArray<Type, Arrays::vector_, SizeRows_, SizeCols_, Orient_> > LowBase;
    enum
    {
      structure_ = Arrays::vector_,
      orient_    = Orient_,
      sizeRows_  = SizeRows_,
      sizeCols_  = 1,
      storage_   = Arrays::dense_
    };

    /** Default constructor. */
    inline CArray() : Base()
    { STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH(SizeCols_==1);}
    /** constructor with specified dimension.
     *  @param sizeRows size of the rows
     **/
    inline CArray( int const& sizeRows) : Base(sizeRows, 1)
    { STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH(SizeCols_==1);}
    /** constructor with sizeRows specified,
     *  initialization with a constant.
     *  @param sizeRows size of the rows
     *  @param v initial value of the container
     **/
    inline CArray( int const& sizeRows, Type const& v) : Base(sizeRows, 1, v)
    { STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH(SizeCols_==1);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    inline CArray( const CArray &T, bool ref=false) : Base(T, ref)
    { STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH(SizeCols_==1);}
    /** constructor by reference.
     *  @param allocator the allocator to wrap
     **/
    template<class OtherAllocator>
    inline CArray( OtherAllocator const& allocator) : Base(allocator)
    { STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH(SizeCols_==1);}
    /** destructor. */
    inline ~CArray() {}
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    inline CArray& operator=(Type const& v) { this->setValue(v); return *this;}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline CArray& operator=(Rhs const& T) { return LowBase::assign(T);}
    /** operator = : overwrite the CArray with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    inline CArray& operator=(CArray const& rhs) { return LowBase::assign(rhs);}
};

/** @ingroup Arrays
 * @brief specialization for the number case.
 */
template <typename Type, bool Orient_>
class CArray <Type, Arrays::number_, 1, 1, Orient_>
      : public ICArray < CArray<Type, Arrays::number_, 1, 1, Orient_> >
{
  public:
    typedef ICArray < CArray<Type, Arrays::number_, 1, 1, Orient_> > Base;
    typedef ArrayBase < CArray<Type, Arrays::number_, 1, 1, Orient_> > LowBase;
    enum
    {
      structure_ = Arrays::number_,
      orient_    = Orient_,
      sizeRows_  = 1,
      sizeCols_  = 1,
      storage_   = Arrays::dense_
    };
    /** Default constructor. */
    inline CArray() : Base() {}
    /** constructor with an initial value, initialization with a constant.
     *  @param v initial value of the container
     **/
    inline CArray( Type const& v) : Base(1, 1, v) {}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    inline CArray( const CArray &T, bool ref=false) : Base(T, ref) {}
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
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline CArray& operator=(Rhs const& T) { return LowBase::assign(T);}
    /** operator = : overwrite the CArray with the Right hand side rhs.
     *  @param rhs the container to copy
     **/
    inline CArray& operator=(CArray const& rhs) { return LowBase::assign(rhs);}
};

/** @ingroup Arrays
 *  ostream for CArray.
 *  @param s the output stream
 *  @param V the CArray to write
 **/
template <typename Type, Arrays::Structure Structure_ , int SizeRows_, int SizeCols_, bool Orient_>
ostream& operator<<(ostream& s, const CArray<Type, Structure_, SizeRows_, SizeCols_, Orient_>& V)
{ return out2D(s,V);}

typedef CArray<Real, Arrays::general_>                                  CArrayXX;
typedef CArray<Real, Arrays::general_, UnknownSize, 2, Arrays::by_col_> CArrayX2;
typedef CArray<Real, Arrays::general_, UnknownSize, 3, Arrays::by_col_> CArrayX3;
typedef CArray<Real, Arrays::general_, 2, 2, Arrays::by_col_>           CArray22;
typedef CArray<Real, Arrays::general_, 3, 3, Arrays::by_col_>           CArray33;

typedef CArray<Real, Arrays::square_>                                   CArraySquareXX;
typedef CArray<Real, Arrays::general_, 2, 2, Arrays::by_col_>           CArraySquare22;
typedef CArray<Real, Arrays::general_, 3, 3, Arrays::by_col_>           CArraySquare33;

typedef CArray<Real, Arrays::vector_, UnknownSize, 1, Arrays::by_col_>  CVectorX;
typedef CArray<Real, Arrays::vector_, 2, 1, Arrays::by_col_>            CVector2;
typedef CArray<Real, Arrays::vector_, 3, 1, Arrays::by_col_>            CVector3;

typedef CArray<Real, Arrays::point_, 1, UnknownSize, Arrays::by_row_>   CPointX;
typedef CArray<Real, Arrays::point_, 1, 2, Arrays::by_row_>             CPoint2;
typedef CArray<Real, Arrays::point_, 1, 3, Arrays::by_row_>             CPoint3;

typedef CArray<Real, Arrays::upper_triangular_>       CUpperXX;
typedef CArray<Real, Arrays::lower_triangular_>       CLowerXX;

} // namespace STK


#endif /* STK_CARRAY2D_H */
