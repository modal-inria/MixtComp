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
 * created on: 10 août 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_CAllocator.h
 *  @brief In this file we define the CAllocator templated class.
 **/

#ifndef STK_ALLOCATORCARRAY_H
#define STK_ALLOCATORCARRAY_H

#include "../../Sdk/include/STK_StaticAssert.h"
#include "../../STKernel/include/STK_Constants.h"
#include "../../Sdk/include/STK_MetaTemplate.h"
#include "../../Arrays/include/STK_Traits.h"

#include "STK_ITContainer2D.h"
#include "STK_AllocatorBase.h"
#include "STK_Arrays_Util.h"

namespace STK
{

/** @ingroup Arrays
 *  @brief  Interface Base class for the CAllocator classes.
 *
 *  The allocator stores the data in a single vector by row or by column.
 *  In order to obtain the element (i,j) of the Array, we have to apply,
 *  either the formula @c i*idx_+j or @c j*idx_+i. The class deriving from this
 *  Interface cannot be an expression, thus it derive from the ITContainer2D
 *  interface. Concrete implementation depend of the structure of the allocator
 *  and of the Orientation.
 *
 *  The pseudo-virtual function to implement have the following description
 *  @code
 *    // for all matrices classes except diagonal and square arrays
 *    void shift2Impl( int const& firstRow, int const& firstCol)
 *    Derived& resize2Impl(int const& sizeRows, int const& sizeCols)
 *    // for all vector classes, diagonal and square arrays
 *    void shift1Impl(int const& beg)
 *    Derived& resize1Impl(int const& size)
 *    SubVector sub1Impl(Range const& I)
 *  @endcode
 *
 *  @sa OrientedCAllocator, StructuredCAllocator, CAllocator
 **/
template<class Derived> class CAllocatorBase;

/** @ingroup Arrays
 *  @brief Allocator for dense Array classes.
 *  The data are stored in two dimensions.
 *  It can be the columns or the rows allocator of any dense container.
 */
template<typename Type, Arrays::Structure Structure_, int SizeRows_, int SizeCols_, bool Orient_>
class CAllocator;
/** @brief  Base class for the oriented allocator classes.*/
template<class Derived, bool Orient_> class OrientedCAllocator;
/** @brief  Base class for the Structured allocator classes.*/
template<class Derived, int SizeRows_, int SizeCols_, bool Orient_> class StructuredCAllocator;

namespace hidden
{
/** @ingroup hidden
 *  Give the Structure of the transposed allocator.
 **/
template<int Structure_> struct TStructure;

/** specialisation for array2D_ */
template<> struct TStructure<Arrays::array2D_>
{ enum { structure_ = Arrays::array2D_}; };
/** specialisation for square_ */
template<> struct TStructure<Arrays::square_>
{ enum { structure_ = Arrays::square_}; };
/** specialisation for lower_triangular_ */
template<> struct TStructure<Arrays::lower_triangular_>
{ enum { structure_ = Arrays::upper_triangular_}; };
/** specialisation for upper_triangular_ */
template<> struct TStructure<Arrays::upper_triangular_>
{ enum { structure_ = Arrays::lower_triangular_}; };
/** specialisation for diagonal_ */
template<> struct TStructure<Arrays::diagonal_>
{ enum { structure_ = Arrays::diagonal_}; };
/** specialisation for vector_ */
template<> struct TStructure<Arrays::vector_>
{ enum { structure_ = Arrays::point_}; };
/** specialisation for point_ */
template<> struct TStructure<Arrays::point_>
{ enum { structure_ = Arrays::vector_}; };
/** specialisation for number_ */
template<> struct TStructure<Arrays::number_>
{ enum { structure_ = Arrays::number_}; };

} // namespace hidden


namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for CAllocator.
 */
template< typename Scalar, Arrays::Structure Structure_, int SizeRows_, int SizeCols_, bool Orient_>
struct Traits< CAllocator<Scalar, Structure_, SizeRows_, SizeCols_, Orient_> >
{
  private:
    class Void { };

    typedef CAllocator<Scalar, Arrays::point_, 1, SizeCols_, Arrays::by_col_> RowIndirect;
    typedef CAllocator<Scalar, Arrays::point_, 1, SizeCols_, Arrays::by_row_> RowDirect;

    typedef CAllocator<Scalar, Arrays::point_, 1, UnknownSize, Arrays::by_col_> SubRowIndirect;
    typedef CAllocator<Scalar, Arrays::point_, 1, UnknownSize, Arrays::by_row_> SubRowDirect;

    typedef CAllocator<Scalar, Arrays::vector_, SizeRows_, 1, Arrays::by_row_> ColIndirect;
    typedef CAllocator<Scalar, Arrays::vector_, SizeRows_, 1, Arrays::by_col_> ColDirect;

    typedef CAllocator<Scalar, Arrays::vector_, UnknownSize, 1, Arrays::by_row_> SubColIndirect;
    typedef CAllocator<Scalar, Arrays::vector_, UnknownSize, 1, Arrays::by_col_> SubColDirect;

    typedef CAllocator<Scalar, Arrays::array2D_, SizeRows_, UnknownSize, Arrays::by_row_> FixedRowArrayIndirect;
    typedef CAllocator<Scalar, Arrays::array2D_, UnknownSize, SizeCols_, Arrays::by_row_> FixedColArrayDirect;

    typedef CAllocator<Scalar, Arrays::array2D_, SizeRows_, UnknownSize, Arrays::by_col_> FixedRowArrayDirect;
    typedef CAllocator<Scalar, Arrays::array2D_, UnknownSize, SizeCols_, Arrays::by_col_> FixedColArrayIndirect;

  public:

    typedef Scalar Type;
    typedef CAllocator<Scalar, Arrays::number_, 1, 1, Orient_> Number;


    typedef typename If<Orient_, RowIndirect, RowDirect >::Result  Row;
    typedef typename If<Orient_, ColDirect, ColIndirect >::Result  Col;

    typedef typename If<Orient_, SubRowIndirect, SubRowDirect >::Result  SubRow;
    typedef typename If<Orient_, SubColDirect, SubColIndirect >::Result  SubCol;

    /** If one of the Size is 1, we have a Vector (a column) or a Point (a row)
     *  (What to do if both are = 1 : Scalar or array (1,1) ?).
     **/
    typedef typename If< (SizeRows_ == 1)||(SizeCols_ == 1)   // one row or one column
                       , typename If<(SizeCols_ == 1)
                                    , typename If<SizeRows_==1, Number, SubCol>::Result
                                    , SubRow>::Result
                       , Void
                       >::Result SubVector;
    // use this as default. FIXME: Not optimal in case we just get a SubArray
    // with unmodified rows or cols size.
    typedef CAllocator<Type, Structure_, UnknownSize, UnknownSize, Orient_> SubArray;
    // transposed Type
    typedef CAllocator< Type, (Arrays::Structure)TStructure<Structure_>::structure_, SizeCols_, SizeRows_, !Orient_> Transposed;

    enum
    {
      structure_ = Structure_,
      orient_    = Orient_,
      sizeRows_  = SizeRows_,
      sizeCols_  = SizeCols_,
      storage_   = Arrays::dense_ // always dense
    };
};

} // namespace hidden


template<class Derived>
class CAllocatorBase : public ITContainer2D<Derived>
{
  protected:
    /** Base class */
    typedef ITContainer2D<Derived> Base;
    /** Default constructor */
    inline CAllocatorBase( Range const& I, Range const& J, int const& idx)
                         : Base(I, J), idx_(idx) {}
    /** copy constructor */
    inline CAllocatorBase( CAllocatorBase const& A,  int const& idx)
                         : Base(A), idx_(idx) {}
    /** destructor */
    inline ~CAllocatorBase() {}

  public:
    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::Row RowVector;
    typedef typename hidden::Traits<Derived>::Col ColVector;
    typedef typename hidden::Traits<Derived>::SubRow SubRowVector;
    typedef typename hidden::Traits<Derived>::SubCol SubColVector;
    typedef typename hidden::Traits<Derived>::SubArray SubArray;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;
    typedef typename hidden::Traits<Derived>::Transposed Transposed;

    /** @return the index of the allocator*/
    inline int const& idx() const { return idx_;}
    /** Access to the ith row of the Allocator.
     *  @param i index of the row
     *  @return a reference on the ith row
     **/
    inline RowVector row(int const& i) const
    { return  RowVector(this->asDerived(), Range(i,1), this->cols(), idx_);}
    /** Access to the row (i,J) of the Allocator.
     *  @param i index of the row
     *  @param J range of the columns
     *  @return a reference on the ith row
     **/
    inline SubRowVector row(int const& i, Range const& J) const
    { return  SubRowVector(this->asDerived(), Range(i,1), J, idx_);}
    /** Access to the jth column of the Allocator.
     *  @param j index of the column
     *  @return a reference on the jth column
     **/
    inline ColVector col(int const& j) const
    { return  ColVector(this->asDerived(), this->rows(), Range(j,1), idx_);}
    /** Access to the column (I,j) of the Allocator.
     *  @param I range of the rows
     *  @param j index of the column
     *  @return a reference on the jth column
     **/
    inline SubColVector col(Range const& I, int const& j) const
    { return  SubColVector(this->asDerived(), I, Range(j,1), idx_);}
    /** Access to the sub-part (I,J) of the Allocator.
     *  @param I range of the rows
     *  @param J range of the columns
     *  @return a reference on a sub-part of the Allocaor
     **/
    inline SubArray sub(Range const& I, Range const& J) const
    { return SubArray(this->asDerived(), I, J, idx_);}
    /** Access to the sub-part (I,J) of the Allocator.
     *  @param I range of the rows
     *  @return a reference on a sub-part of the Allocaor
     **/
    inline SubVector sub(Range const& I) const
    { return this->asDerived().sub1Impl(I);}
    /** transpose the Allocator.
     *  @return a transposed allocator
     **/
    inline Transposed transpose() const
    { return Transposed(this->asDerived(), this->cols(), this->rows(), idx_);}
    /** swap this with T.
     *  @param T the container to swap
     **/
    inline void exchange(CAllocatorBase &T)
    {  Base::exchange(T);
       std::swap(idx_, T.idx_);
    }
    /** shift the first indexes of the allocator.
     *  @param firstRow the index of the first row
     *  @param firstCol the index of the first column
     **/
    inline void shift( int const& firstRow, int const& firstCol)
    { this->asDerived().shift2Impl(firstRow, firstCol);}
    /** resize the allocator
     *  @param sizeRows size of the rows
     *  @param sizeCols size of the columns
     **/
   inline Derived& resize(int const& sizeRows, int const& sizeCols)
    {
      this->asDerived().resize2Impl(sizeRows, sizeCols);
      return this->asDerived();
    }
    /** shift the first indexes of the vector or point.
     *  @param beg the index of the first row or column
     **/
    inline void shift(int const& beg)
    { this->asDerived().shift1Impl(beg);}
    /** Resize the vector or the point
     *  @param size the size to set to the vector
     **/
    inline Derived& resize(int const& size)
    { this->asDerived().resize1Impl(size);
      return this->asDerived();
    }
  protected:
    /** index of the data set */
    int idx_;
    /** set index of the data. */
    void setIdx( int const& idx) { idx_ = idx;}
};

/** @brief Specialization for row _oriented Allocators.*/
template<class Derived>
class OrientedCAllocator<Derived, Arrays::by_col_>: public CAllocatorBase<Derived>
{
  public:
    typedef CAllocatorBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;

  protected:
    inline OrientedCAllocator( Range const& I, Range const& J)
                             : Base(I, J, I.size())
    {}
    /** copy constructor */
    inline OrientedCAllocator( OrientedCAllocator const& A,  int const& idx)
                             : Base(A, idx) {}
    inline ~OrientedCAllocator() {}
  public:
    /** @return a constant reference on the element (i,j) of the Allocator.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const& elt2Impl(int const& i, int const& j) const
    { return this->asDerived().p_data()[j*this->idx_ + i];}
    /** @return a reference on the element (i,j) of the Allocator.
     *  @param i index of the row
     *  @param j index of the columns
     **/
    inline Type& elt2Impl(int const& i, int const& j)
    { return this->asDerived().p_data()[j*this->idx_ + i] ;}
    /** set a value to this container. This method should not be used: used
     *  rather @c Visitor.
     *  @param v the value to set
     **/
    void setValue(Type const& v)
    {
      for (int j= this->firstIdxCols(); j <= this->lastIdxCols(); ++j)
        for (int i = this->firstIdxRows(); i<=this->lastIdxRows(); ++i)
        { this->elt(i, j) = v;}
    }

  protected:
    /** @brief Compute the range of the 1D Allocator when we want to
     *  allocate a 2D array with I indexes in the first dimension and J indexes
     *  in the second dimension.
     *  @param I the range of the first dimension
     *  @param J the range of the second dimension
     *  @return The range of the 1D allocator
     **/
    static inline Range prod(Range const& I, Range const& J)
    { return Range(I.size()*J.firstIdx()+I.firstIdx(), I.size()*J.size()); }
    /** return the increment to apply to a zero based pointer corresponding to
     *  the actual first row and first column indexes. */
    inline int shiftInc(int const& firstRow, int const& firstCol)
    { return this->idx_*firstCol+firstRow; }
    /** return the index corresponding to the actual size of the allocator. */
    inline int sizedIdx() const {return this->asDerived().sizeRows();}
    /** set the index corresponding to the actual size of the allocator. */
    inline void setSizedIdx() {this->idx_ = this->asDerived().sizeRows();}
};

/** @brief Specialization for col_ oriented Allocators.*/
template<class Derived>
class OrientedCAllocator<Derived, Arrays::by_row_>: public CAllocatorBase<Derived>
{
  public:
    typedef CAllocatorBase<Derived> Base;
    typedef typename hidden::Traits<Derived>::Type Type;
  protected:
    /** constructor with specified ranges */
    inline OrientedCAllocator( Range const& I, Range const& J)
                             : Base(I, J, J.size())
    {}
    /** copy constructor */
    inline OrientedCAllocator( OrientedCAllocator const& A,  int const& idx)
                             : Base(A, idx)
    {}
    inline ~OrientedCAllocator() {}
  public:
    /** @return a constant reference on the element (i,j) of the Allocator.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const& elt2Impl(int const& i, int const& j) const
    { return this->asDerived().p_data()[i*this->idx_ + j];}
    /** @return a reference on the element (i,j) of the Allocator.
     *  @param i index of the row
     *  @param j index of the columns
     **/
    inline Type& elt2Impl(int const& i, int const& j)
    { return this->asDerived().p_data()[i*this->idx_ + j] ;}
    /** set a value to this container. This method should not be used: used
     *  rather @c Visitor.
     *  @param v the value to set
     **/
    void setValue(Type const& v)
    {
      for (int i = this->firstIdxRows(); i<=this->lastIdxRows(); ++i)
        for (int j= this->firstIdxCols(); j <= this->lastIdxCols(); ++j)
        { this->elt(i, j) = v;}
    }

  protected:
    /** @brief Compute the range of the 1D Allocator when we want to
     *  allocate a 2D array with I indexes in the first dimension and J indexes
     *  in the second dimension.
     *  @param I the range of the first dimension
     *  @param J the range of the second dimension
     *  @return The range of the 1D allocator
     **/
    static inline Range prod(Range const& I, Range const& J)
    { return Range(J.size()*I.firstIdx()+J.firstIdx(), I.size()*J.size());}
    /** return the increment corresponding to the actual first row an column. */
    inline int shiftInc(int const& firstRow, int const& firstCol)
    { return this->idx_*firstRow+firstCol; }
    /** return the index corresponding to the actual size of the allocator. */
    inline int sizedIdx() const {return this->asDerived().sizeCols();}
    /** set the index corresponding to the actual size of the allocator. */
    inline void setSizedIdx() { this->idx_ = this->asDerived().sizeCols();}
};

/** @brief  Base class for the general structured case.*/
template<class Derived, int SizeRows_, int SizeCols_, bool Orient_>
class StructuredCAllocator : public OrientedCAllocator<Derived, Orient_>
{
  public:
    typedef OrientedCAllocator<Derived, Orient_> Base;
    typedef typename hidden::Traits<Derived>::Type Type;
    /** Default constructor */
    inline StructuredCAllocator( Range const& I, Range const& J) : Base(I, J) {}
    /** copy constructor */
    inline StructuredCAllocator( StructuredCAllocator const& A,  int const& idx) : Base(A, idx) {}
    /** move T to this.
     *  @param T the container to move
     **/
    inline StructuredCAllocator& move(StructuredCAllocator const& T)
    { return *this;}
    /** shift the first indexes of the allocator.
     *  @param firstIdx the index of the first row and column
     **/
    void shift1Impl(int const& firstIdx)
    { this->asDerived().shift2Impl(firstIdx, firstIdx);}
};

/** @brief specialization for the number_ case.*/
template<class Derived, bool Orient_>
class StructuredCAllocator<Derived, 1, 1, Orient_> : public OrientedCAllocator<Derived, Orient_>
{
  public:
    typedef OrientedCAllocator<Derived, Orient_> Base;
    typedef typename hidden::Traits<Derived>::Type Type;
  protected:
    /** Default constructor */
    inline StructuredCAllocator( Range const& I, Range const& J)
                      : Base(I, J), row_(I.firstIdx()), col_(J.firstIdx())
    {}
    /** copy constructor */
    inline StructuredCAllocator( StructuredCAllocator const& A,  int const& idx)
                      : Base(A, idx), row_(A.row_), col_(A.col_) {}
    inline ~StructuredCAllocator() {}
    /** move T to this.
     *  @param T the container to move
     **/
    inline StructuredCAllocator& move(StructuredCAllocator const& T)
    { row_ = T.row_; col_ = T.col_; return *this;}
  public:
    /** @return a constant reference on the element of the Allocator. */
    inline Type const elt0Impl() const { return this->elt(row_, col_);}
    /** @return a reference on the element of the Allocator. */
    inline Type& elt0Impl() { return this->elt(row_, col_);}

    /** shift the first indexes of the allocator.
     *  @param firstIdx the index of the first row and column
     **/
    void shift1Impl(int const& firstIdx)
    { this->asDerived().shift2Impl(firstIdx, firstIdx);}
  private:
    int row_;
    int col_;
};

/** @brief specialization for the point_ case.*/
template<class Derived, int SizeCols_, bool Orient_>
class StructuredCAllocator<Derived, 1, SizeCols_, Orient_> : public OrientedCAllocator<Derived, Orient_>
{
  private:
    /** row of the point (needed when this is a reference) */
    int row_;

  public:
    typedef OrientedCAllocator<Derived, Orient_> Base;
    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;
  protected:
    /** Default constructor */
    inline StructuredCAllocator( Range const& I, Range const& J)
                              : Base(I, J), row_(I.firstIdx()) {}
    /** copy constructor */
    inline StructuredCAllocator( StructuredCAllocator const& A,  int const& idx)
                              : Base(A, idx), row_(A.row_) {}
    /** move T to this.
     *  @param T the container to move
     **/
    inline StructuredCAllocator& move(StructuredCAllocator const& T)
    { row_ = T.row_; return *this;}
  public:
    /** @return a constant reference on the element (i,j) of the Allocator.
     *  @param j index of the column
     **/
    inline Type const elt1Impl( int const& j) const
    { return this->elt(row_, j);}
    /** @return a reference on the element (i,j) of the Allocator.
     *  @param j index of the columns
     **/
    inline Type& elt1Impl( int const& j)
    { return this->elt(row_, j);}
    /** shift the first indexes of the allocator.
     *  @param first the index of the first column and first row */
    inline void shift1Impl(int const& first) { shift2Impl(first, first);}
    /** shift the first indexes of the allocator.
     *  @param firstRow the row of the point
     *  @param firstCol the index of the first column
     **/
    void shift2Impl(int const& firstRow, int const& firstCol)
    { this->asDerived().shift2Impl(firstRow, firstCol); row_ = firstRow;}
    /** resize the allocator.
     *  @param sizeCols the size of the point
     **/
    void resize1Impl(int const& sizeCols)
    { this->asDerived().resize2Impl(1, sizeCols); row_ = this->rows().firstIdx();}
    /** @return a sub-vector in the specified range of the Allocator.
     *  @param J range of the sub-vector
     **/
    inline SubVector sub1Impl( Range const& J) const { return Base::row(row_, J);}
};

/** @brief specialization for the vector_ case.*/
template<class Derived, int SizeRows_, bool Orient_>
class StructuredCAllocator<Derived, SizeRows_, 1, Orient_>
      : public OrientedCAllocator<Derived, Orient_>
{
  private:
    int col_;

  public:
    typedef OrientedCAllocator<Derived, Orient_> Base;
    typedef typename hidden::Traits<Derived>::Type Type;
    typedef typename hidden::Traits<Derived>::SubVector SubVector;
  protected:
    /** Default constructor */
    inline StructuredCAllocator( Range const& I, Range const& J)
                               : Base(I, J), col_(J.firstIdx())
    {}
    /** copy constructor */
    inline StructuredCAllocator( StructuredCAllocator const& A,  int const& idx)
                               : Base(A, idx), col_(A.col_) {}
    /** move T to this.
     *  @param T the container to move
     **/
    inline StructuredCAllocator& move(StructuredCAllocator const& T)
    { col_ = T.col_; return *this;}
  public:
    /** @return a constant reference on the element (i,j) of the Allocator.
     *  @param i index of the row
     **/
    inline Type const elt1Impl( int const& i) const
    { return this->elt(i, col_);}
    /** @return a reference on the element (i,j) of the Allocator.
     *  @param i index of the row
     **/
    inline Type& elt1Impl( int const& i)
    { return this->elt(i, col_);}
    /** shift the first indexes of the allocator.
     *  @param first the index of the first row and first column
     **/
    inline void shift1Impl(int const& first) { shift2Impl(first, first);}
    /** shift the first indexes of the allocator.
     *  @param firstRow the index of the first row
     *  @param firstCol the column of the vector
     **/
    void shift2Impl(int const& firstRow, int const& firstCol)
    { this->asDerived().shift2Impl(firstRow, firstCol); col_ = firstCol;}
    /** resize the allocator.
     *  @param sizeRow the size of the vector
     **/
    void resize1Impl(int const& sizeRow)
    { this->asDerived().resize2Impl(sizeRow, col_);}
    /** @return a sub-vector in the specified range of the Allocator.
     *  @param I range of the sub-vector
     **/
    inline SubVector sub1Impl( Range const& I) const { return Base::col(I, col_);}
};


/** @ingroup Arrays
 *  @brief Allocator for the dense CArray classes.
 *  The size of the Allocator is known in both dimension
 */
template<typename Type, Arrays::Structure Structure_, int SizeRows_, int SizeCols_, bool Orient_>
class CAllocator
      : public StructuredCAllocator<CAllocator<Type, Structure_, SizeRows_, SizeCols_, Orient_>, SizeRows_, SizeCols_, Orient_  >
      , public AllocatorBase<Type>
{
  public:
    typedef StructuredCAllocator<CAllocator, SizeRows_, SizeCols_, Orient_  > Base;
    typedef AllocatorBase<Type> Allocator;
    inline CAllocator() : Base(SizeRows_, SizeCols_)
                        , Allocator(this->prod(SizeRows_, SizeCols_))
    {
      STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(  !((SizeRows_ != 1) && ((Structure_ == Arrays::point_)  || (Structure_ == Arrays::number_))) );
      STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH( !((SizeCols_ != 1) && ((Structure_ == Arrays::vector_) || (Structure_ == Arrays::number_))) );
      STK_STATICASSERT_SCALAR_SIZE_MISMATCH( !(((SizeRows_ != 1) || ((SizeCols_ != 1))) && (Structure_ == Arrays::number_)) );
    }
    inline CAllocator( int, int)
                     : Base(SizeRows_, SizeCols_)
                     , Allocator(this->prod(SizeRows_, SizeCols_))
    {
      STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(  !((SizeRows_ != 1) && ((Structure_ == Arrays::point_)  || Structure_ == Arrays::number_)) );
      STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH( !((SizeCols_ != 1) && ((Structure_ == Arrays::vector_) || Structure_ == Arrays::number_)) );
      STK_STATICASSERT_SCALAR_SIZE_MISMATCH( !(((SizeRows_ != 1) || ((SizeCols_ != 1))) && (Structure_ == Arrays::number_)) );
    }
    inline CAllocator( int, int, Type const& v)
                     : Base(SizeRows_, SizeCols_)
                     , Allocator(this->prod(SizeRows_, SizeCols_))
    {
      STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(  !((SizeRows_ != 1) && ((Structure_ == Arrays::point_)  || (Structure_ == Arrays::number_))) );
      STK_STATICASSERT_VECTOR_SIZECOLS_MISMATCH( !((SizeCols_ != 1) && ((Structure_ == Arrays::vector_) || Structure_ == Arrays::number_)) );
      STK_STATICASSERT_SCALAR_SIZE_MISMATCH( !(((SizeRows_ != 1) || ((SizeCols_ != 1))) && (Structure_ == Arrays::number_ )) );
      this->setValue(v);
    }
    inline CAllocator( CAllocator const& T, bool ref = true)
                     : Base(T, ref ? T.idx() : T.sizedIdx()), Allocator(T, ref)
    { if (!ref) { Allocator::copy(T);} }
    inline CAllocator( Allocator const& A, Range const& I, Range const& J, int const& idx)
                     : Base(I, J), Allocator(A, true)
    { this->setIdx(idx);}
    /** wrapper constructor for 0 based C-Array*/
    inline CAllocator( Type* const& q, int , int )
                     : Base(Range(0,SizeRows_), Range(0,SizeCols_)), Allocator(q, SizeRows_ * SizeCols_, true)
    {}
    ~CAllocator() {}
    inline void exchange(CAllocator &T) { Allocator::exchange(T); Base::exchange(T);}
    inline CAllocator& move(CAllocator const& T)
    {
      if (this == &T) return *this;
      Allocator::move(T);
      Base::move(T);
      Base::setRanges(T.rows(), T.cols());
      this->setIdx(T.idx());
      return *this;
    }
    void shift2Impl(int const& firstRow, int const& firstCol)
    {
      if ((firstRow == this->firstIdxRows())&&(firstCol == this->firstIdxCols())) return;
      // check for reference
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(CAllocator::shift2Impl, firstRow, firstCol, cannot operate on reference);}
      // set new ranges and translate main pointer
      TContainer2D<SizeRows_, SizeCols_>::shift(firstRow, firstCol);
      this->shiftData(this->shiftInc(firstRow, firstCol));
    }
    inline CAllocator& resize2Impl( int, int) { return *this;}
    inline void realloc(int, int) {}
};

/** @brief Specialized Allocator for the dense Arrays classes.
 *  The sizes of the columns and of the rows are unknown. The Orientation is
 *  either by rows or by column.
 */
template<typename Type, Arrays::Structure Structure_, bool Orient_>
class CAllocator<Type, Structure_, UnknownSize, UnknownSize, Orient_>
     : public StructuredCAllocator<CAllocator<Type, Structure_, UnknownSize, UnknownSize, Orient_>, UnknownSize, UnknownSize, Orient_ >
     , public AllocatorBase<Type>
{
  public:
    typedef StructuredCAllocator<CAllocator, UnknownSize, UnknownSize, Orient_ > Base;
    typedef AllocatorBase<Type> Allocator;
    /** Default constructor */
    inline CAllocator() : Base(0, 0), Allocator(this->prod(0, 0)) {}
    /** Constructor with specified size.
     *  @param sizeRows size of the rows
     *  @param sizeCols size of the columns
     **/
    inline CAllocator( int const& sizeRows, int const& sizeCols)
                     : Base(sizeRows, sizeCols)
                     , Allocator(this->prod(sizeRows, sizeCols))
    {}
    /** Constructor with specified size and specified value.
     *  @param sizeRows size of the rows
     *  @param sizeCols size of the columns
     *  @param v the initial value
     **/
    inline CAllocator( int const& sizeRows, int const& sizeCols, Type const& v)
                     : Base(sizeRows, sizeCols)
                     , Allocator(this->prod(sizeRows, sizeCols))
    { this->setValue(v);}
    /** Copy or wrapper constructor.
     *  @param T : the array to copy
     *  @param ref : is this a wrapper of T ?
     **/
    inline CAllocator( CAllocator const& T, bool ref = true)
                     : Base(T, ref ? T.idx(): T.sizedIdx())
                     , Allocator(T, ref)
    { if (!ref) { Allocator::copy(T);}}
    /** Wrapper constructor. This become a reference on (some part of) the Allocator A.
     *  @param A original allocator
     *  @param I range of rows to wrap.
     *  @param J range of columns to wrap.
     *  @param idx the index of the data
     **/
    inline CAllocator( Allocator const& A, Range const& I, Range const& J, int const& idx)
                     : Base(I, J), Allocator(A, true)
    { this->setIdx(idx);}
    /** wrapper constructor for 0 based C-Array*/
    inline CAllocator( Type* const& q, int nbRow, int nbCol)
                     : Base(Range(0,nbRow), Range(0,nbCol)), Allocator(q, nbRow * nbCol, true)
    {}
    /** Destructor */
    ~CAllocator() {}
    /** exchange this with T.
     *  @param T the allocator to exchange
     **/
    inline void exchange(CAllocator &T) { Allocator::exchange(T); Base::exchange(T);}
    /** move T to this.
     *  @param T the container to move
     **/
    inline CAllocator& move(CAllocator const& T)
    {
      Allocator::move(T);
      Base::move(T);
      TContainer2D<UnknownSize, UnknownSize>::setRanges(T.rows(), T.cols());
      this->setIdx(T.idx());
      return *this;
    }
    void shift2Impl(int const& firstRow, int const& firstCol)
    {
      if ((firstRow == this->firstIdxRows())&&(firstCol == this->firstIdxCols())) return;
      // check for reference
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG( CAllocator::shift2Impl, firstRow, firstCol, cannot operate on reference);}
      // set new ranges and  translate main pointer
      TContainer2D<UnknownSize, UnknownSize>::shift(firstRow, firstCol);
      this->shiftData(this->shiftInc(firstRow, firstCol));
    }
    CAllocator& resize2Impl( int const& sizeRows, int const& sizeCols)
    {
     // check size
     if ((sizeRows <= 0)||(sizeCols<=0))
     {
       // free any allocated memory if this is not a reference
       this->freeData();
       // set Range values and null pointer
       this->setPtrData(0, this->prod(sizeRows, sizeCols), false);
       this->setRanges(sizeRows, sizeCols);
       this->setSizedIdx();
       return *this;
     }
     // allocate
     Allocator::malloc(this->prod(sizeRows, sizeCols));
     this->setRanges(sizeRows, sizeCols);
     this->setSizedIdx();
     return *this;
    }
   /** @brief function for memory reallocation.
    * The function assume you want to increase or reduce the size without
    *  modification of the bases ranges.
    *  @param sizeRows range of the rows
    *  @param sizeCols range of the columns
    **/
   void realloc(int const& sizeRows, int const& sizeCols)
   {
     if ((sizeRows == this->sizeRows())&&(sizeCols == this->sizeCols())) return;
     // create a copy the original data set
     CAllocator copy;
     this->exchange(copy);
     try
     {
       // create new container
       resize2Impl(sizeRows, sizeCols);
       shift2Impl(copy.firstIdxRows(), copy.firstIdxCols());
       // copy data
       const int lastRow = std::min(copy.lastIdxRows(), this->lastIdxRows());
       const int lastCol = std::min(copy.lastIdxCols(), this->lastIdxCols());
       for (int j= this->firstIdxCols(); j <= lastCol; ++j)
         for (int i = this->firstIdxRows(); i<=lastRow; ++i)
         { this->elt(i, j) = copy.elt(i, j);}
     }
     catch (std::bad_alloc & error)  // if an alloc error occur
     {
       this->exchange(copy); // restore the original container
       STKRUNTIME_ERROR_2ARG(CAllocator::realloc, sizeRows, sizeCols, memory allocation failed);
     }
   }
};

/** @brief Specialized Allocator for the dense Arrays classes.
 *  The number of rows is known, the number of columns unknown
 **/
template<typename Type, Arrays::Structure Structure_, int SizeRows_, bool Orient_>
class CAllocator<Type, Structure_, SizeRows_, UnknownSize, Orient_>
      : public StructuredCAllocator<CAllocator<Type, Structure_, SizeRows_, UnknownSize, Orient_>, SizeRows_, UnknownSize, Orient_  >
      , public AllocatorBase<Type>
{
  public:
    typedef StructuredCAllocator<CAllocator, SizeRows_, UnknownSize, Orient_  > Base;
    typedef AllocatorBase<Type> Allocator;
    inline CAllocator() : Base(SizeRows_, 0), Allocator(this->prod(SizeRows_, 0))
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH( !((SizeRows_ != 1) && ((Structure_ == Arrays::point_)  || (Structure_ == Arrays::number_))) );}
    inline CAllocator( int, int const& sizeCols)
                     : Base(SizeRows_, sizeCols), Allocator(this->prod(SizeRows_, sizeCols))
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH( !((SizeRows_ != 1) && ((Structure_ == Arrays::point_)  || (Structure_ == Arrays::number_))) );}
    inline CAllocator( int, int const& sizeCols, Type const& v)
                     : Base(SizeRows_, sizeCols), Allocator(this->prod(SizeRows_, sizeCols))
    { this->setValue(v);
      STK_STATICASSERT_POINT_SIZEROWS_MISMATCH( !((SizeRows_ != 1) && ((Structure_ == Arrays::point_)  || (Structure_ == Arrays::number_))) );
    }
    inline CAllocator( CAllocator const& T, bool ref = true)
                     : Base(T, ref ? T.idx() : T.sizedIdx()), Allocator(T, ref)
    { if (!ref) { Allocator::copy(T);} }
    inline CAllocator( Allocator const& T, Range const& I, Range const& J, int const& idx)
                     : Base(I, J), Allocator(T, true)
    { this->setIdx(idx);}
    /** wrapper constructor for 0 based C-Array*/
    inline CAllocator( Type* const& q, int , int nbCol)
                     : Base(Range(0,SizeRows_), Range(0,nbCol)), Allocator(q, SizeRows_ * nbCol, true)
    {}
    ~CAllocator() {}
    inline void exchange(CAllocator &T) { Allocator::exchange(T); Base::exchange(T);}
    inline CAllocator& move(CAllocator const& T)
    {
      if (this == &T) return *this;
      Allocator::move(T);
      Base::move(T);
      Base::setRanges(T.rows(), T.cols());
      this->setIdx(T.idx());
      return *this;
    }
    void shift2Impl(int const& firstRow, int const& firstCol)
    {
      if ((firstRow == this->firstIdxRows())&&(firstCol == this->firstIdxCols())) return;
      // check for reference
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(CAllocator::shift2Impl, firstRow, firstCol, cannot operate on reference);}
      // set new ranges and  translate main pointer
      TContainer2D<SizeRows_, UnknownSize>::shift(firstRow, firstCol);
      this->shiftData(this->shiftInc(firstRow, firstCol));
    }
    CAllocator& resize2Impl( int, int sizeCols)
    {
      // check size
      if (sizeCols<=0)
      {
        // free any allocated memory if it is not a reference
        this->freeData();
        // set Range values and null pointer
        this->setPtrData(0, this->prod(SizeRows_, sizeCols), false);
        this->setRanges(SizeRows_, sizeCols);
        this->setSizedIdx();
        return *this;
      }
      // allocate
      Allocator::malloc(this->prod(SizeRows_, sizeCols));
      this->setRanges(SizeRows_, sizeCols);
      this->setSizedIdx();
      return *this;
    }
    void realloc(int, int const& sizeCols)
    {     // create a copy the original data set
      CAllocator copy;
      this->exchange(copy);
      try
      {
        // create new container
        resize2Impl(SizeRows_, sizeCols);
        shift2Impl(copy.firstIdxRows(), copy.firstIdxCols());
        // copy data
        const int lastCol = std::min(copy.lastIdxCols(), this->lastIdxCols());
        for (int j= this->firstIdxCols(); j <= lastCol; ++j)
          for (int i = this->firstIdxRows(); i<=this->lastIdxRows(); ++i)
        { this->elt(i, j) = copy.elt(i, j);}

      }
      catch (std::bad_alloc const& error)  // if an alloc error occur
      {
        this->exchange(copy); // restore the original container
        STKRUNTIME_ERROR_2ARG(CAllocator::realloc, SizeRows_, sizeCols, memory allocation failed);
      }
    }
};

/** @brief Specialized Allocator for the dense Arrays classes.
 *  The sizes of the columns is known, the number of rows is unknown
 */
template<typename Type, Arrays::Structure Structure_, bool Orient_, int SizeCols_>
class CAllocator<Type, Structure_, UnknownSize, SizeCols_, Orient_>
      : public StructuredCAllocator<CAllocator<Type, Structure_, UnknownSize, SizeCols_, Orient_>, UnknownSize, SizeCols_, Orient_  >
      , public AllocatorBase<Type>
{
  public:
    typedef StructuredCAllocator<CAllocator, UnknownSize, SizeCols_, Orient_  > Base;
    typedef AllocatorBase<Type> Allocator;
    inline CAllocator() : Base(0, SizeCols_)
                             , Allocator()
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(  !((SizeCols_ != 1) && ((Structure_ == Arrays::vector_)  || (Structure_ == Arrays::number_))) );}
    inline CAllocator( int const& sizeRows, int)
                          : Base(sizeRows, SizeCols_)
                          , Allocator(this->prod(sizeRows, SizeCols_))
    { STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(  !((SizeCols_ != 1) && ((Structure_ == Arrays::vector_)  || (Structure_ == Arrays::number_))) );}
    inline CAllocator( int const& sizeRows, int, Type const& v)
                     : Base(sizeRows, SizeCols_)
                     , Allocator(this->prod(sizeRows, SizeCols_))
    {
      STK_STATICASSERT_POINT_SIZEROWS_MISMATCH(  !((SizeCols_ != 1) && ((Structure_ == Arrays::vector_)  || (Structure_ == Arrays::number_))) );
      this->setValue(v);
    }
    inline CAllocator( CAllocator const& T, bool ref = true)
                     : Base(T, ref ? T.idx() : T.sizeRows()), Allocator(T, ref)
    { if (!ref) { Allocator::copy(T);} }
    inline CAllocator( Allocator const& A, Range const& I, Range const& J, int const& idx)
                     : Base(I, J), Allocator(A, true)
    { this->setIdx(idx);}
    /** wrapper constructor for 0 based C-Array*/
    inline CAllocator( Type* const& q, int nbRow, int )
                     : Base(Range(0,nbRow), Range(0,SizeCols_)), Allocator(q, nbRow * SizeCols_, true)
    {}
    ~CAllocator() {}
    inline void exchange(CAllocator &T) { Allocator::exchange(T); Base::exchange(T);}
    inline CAllocator& move(CAllocator const& T)
    {
      if (this == &T) return *this;
      Allocator::move(T);
      Base::move(T);
      Base::setRanges(T.rows(), T.cols());
      this->setIdx(T.idx());
      return *this;
    }
    void shift2Impl(int const& firstRow, int const& firstCol)
    {
      // check if there is something to do
      if ((firstRow == this->firstIdxRows())&&(firstCol == this->firstIdxCols())) return;
      // check for reference
      if (this->isRef())
      { STKRUNTIME_ERROR_2ARG(CAllocator::shift2Impl, firstRow, firstCol, cannot operate on reference.);}
      // set new ranges and  translate main pointer
      TContainer2D<UnknownSize, SizeCols_>::shift(firstRow, firstCol);
      this->shiftData(this->shiftInc(firstRow, firstCol));
    }
    CAllocator& resize2Impl( int const& sizeRows, int)
    {
      // check size
      if (sizeRows <= 0)
      {
        // free any allocated memory if it is not a reference
        this->freeData();
        // set Range values and null pointer
        this->setPtrData(0, this->prod(sizeRows, SizeCols_), false);
        this->setRanges(sizeRows, SizeCols_);
        this->setSizedIdx();
        return *this;
     }
     // allocate
     Allocator::malloc(this->prod(sizeRows,  SizeCols_));
     this->setRanges(sizeRows,  SizeCols_);
     this->setSizedIdx();
     return *this;
   }
   void realloc(int const& sizeRows, int)
   {
     // create a copy the original data set
     CAllocator copy;
     this->exchange(copy);
     try
     {
       // create new container
       resize2Impl(sizeRows, SizeCols_);
       shift2Impl(copy.firstIdxRows(), copy.firstIdxCols());
       // copy data
       const int lastRow = std::min(copy.lastIdxRows(), this->lastIdxRows())
                   , lastCol = this->lastIdxCols();
       for (int j= this->firstIdxCols(); j <= lastCol; ++j)
         for (int i = this->firstIdxRows(); i<=lastRow; ++i)
         { this->elt(i, j) = copy.elt(i, j);}
     }
     catch (std::bad_alloc & error)  // if an alloc error occur
     {
       this->exchange(copy); // restore the original container
       STKRUNTIME_ERROR_2ARG(CAllocator::realloc, sizeRows, SizeCols_, memory allocation failed);
     }
   }
};

/** @ingroup Arrays
 *  ostream for CArray.
 *  @param s the output stream
 *  @param V the CArray to write
 **/
template <typename Type, Arrays::Structure Structure_, int SizeRows_, int SizeCols_, bool Orient_>
ostream& operator<<(ostream& s, const CAllocator<Type, Structure_, SizeRows_, SizeCols_, Orient_>& V)
{ return out2D(s,V);}


} // namespace STK

#endif /* STK_ALLOCATORCARRAY_H */
