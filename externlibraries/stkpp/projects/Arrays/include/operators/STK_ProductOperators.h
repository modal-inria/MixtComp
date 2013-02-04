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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::Arrays
 * created on: 20 oct. 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_ProductOperators.h
 *  @brief In this file we implement the ProductOperator class.
 **/

#ifndef STK_PRODUCTOPERATOR_H
#define STK_PRODUCTOPERATOR_H


#define EGAL(arg1, arg2) ((arg1::structure_ == int(Arrays::arg2)))

#include "../STK_CAllocator.h"
#include "../product/STK_ProductImpl.h"

namespace STK
{

// forward declarations
template<typename> class Array2D;
template<typename> class Array2DSquare;
template<typename> class Array2DUpperTriangular;
template<typename> class Array2DLowerTriangular;
template<typename> class Array2DDiagonal;
template<typename> class Array2DPoint;
template<typename> class Array2DVector;

template< typename, Arrays::Structure, int, int, bool> class CArray;

namespace hidden
{
/** @ingroup hidden
 *  @brief Helper class to get the correct enum characteristic of operator*
 **/
template<typename Lhs, typename Rhs>
struct ProductResultTypeBase
{
  enum
  {
    sizeRows_ = Traits<Lhs>::sizeRows_,
    sizeCols_ = Traits<Rhs>::sizeCols_,
    // if there is more block on the left side, we use bp --> result is by_col_
    orient_   =  ( Traits<Lhs>::sizeRows_ == UnknownSize || Traits<Rhs>::sizeRows_ == UnknownSize
                 ||Traits<Lhs>::sizeCols_ == UnknownSize || Traits<Rhs>::sizeCols_ == UnknownSize
                 )
                 ? Traits<Rhs>::orient_
                 : (Traits<Lhs>::sizeRows_)/blockSize < (Traits<Rhs>::sizeCols_)/blockSize
                   ? int(Arrays::by_row_) : int(Arrays::by_col_),
    storage_  = ( Traits<Lhs>::storage_ == int(Arrays::dense_)) || (Traits<Rhs>::storage_ == int(Arrays::dense_))
                ? int(Arrays::dense_) : int(Arrays::sparse_)
  };
};

/** @ingroup hidden
 *  @brief Helper class to get the correct returned Structure of operator*.
 *  @note the impossible cases are tracked in MatrixByMatrixProduct class.
 **/
template<typename Lhs, typename Rhs, int LStructure_, int RStructure_>
struct ProductResultType;

//----------------------------------
// general case
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductResultType<Lhs, Rhs, Arrays::general_, RStructure_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::general_  };
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2D<Type> Result2D;
  typedef CArray<Type, Arrays::general_, Base::sizeRows_, Base::sizeCols_, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::general_, Base::sizeRows_, Base::sizeCols_, Base::orient_> Allocator;
};

template<typename Lhs, typename Rhs>
struct ProductResultType<Lhs, Rhs, Arrays::general_, Arrays::vector_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::vector_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DVector<Type> Result2D;
  typedef CArray<Type, Arrays::vector_, Base::sizeRows_, 1, Arrays::by_col_> CResult;
  typedef CAllocator<Type, Arrays::vector_, Base::sizeRows_ , 1, Arrays::by_col_> Allocator;
};

//----------------------------------
// square case
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductResultType<Lhs, Rhs, Arrays::square_, RStructure_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::general_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2D<Type> Result2D;
  typedef CArray<Type, Arrays::general_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::general_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};
template<typename Lhs, typename Rhs>
struct ProductResultType<Lhs, Rhs, Arrays::square_, Arrays::square_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::square_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DSquare<Type> Result2D;
  typedef CArray<Type, Arrays::square_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::square_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};
template<typename Lhs, typename Rhs>
struct ProductResultType<Lhs, Rhs, Arrays::square_, Arrays::vector_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::vector_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DVector<Type> Result2D;
  typedef CArray<Type, Arrays::vector_, Base::sizeRows_, 1, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::vector_,Base::sizeRows_ , 1, Base::orient_> Allocator;
};

//----------------------------------
// lower triangular case
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductResultType<Lhs, Rhs, Arrays::lower_triangular_, RStructure_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::general_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2D<Type> Result2D;
  typedef CArray<Type, Arrays::general_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::general_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};
template<typename Lhs, typename Rhs>
struct ProductResultType<Lhs, Rhs, Arrays::lower_triangular_, Arrays::lower_triangular_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::lower_triangular_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DLowerTriangular<Type> Result2D;
  typedef CArray<Type, Arrays::lower_triangular_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> CResult;
  typedef Array2DLowerTriangular<Type> Allocator; // no CAllocator
};
template<typename Lhs, typename Rhs>
struct ProductResultType<Lhs, Rhs, Arrays::lower_triangular_, Arrays::vector_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
    typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::vector_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DVector<Type> Result2D;
  typedef CArray<Type, Arrays::vector_, Base::sizeRows_, 1, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::vector_,Base::sizeRows_ , 1, Base::orient_> Allocator;
};

//----------------------------------
// upper triangular case
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductResultType<Lhs, Rhs, Arrays::upper_triangular_, RStructure_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
    typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::general_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2D<Type> Result2D;
  typedef CArray<Type, Arrays::general_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::general_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};
template<typename Lhs, typename Rhs>
struct ProductResultType<Lhs, Rhs, Arrays::upper_triangular_, Arrays::upper_triangular_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
    typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::upper_triangular_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DUpperTriangular<Type> Result2D;
  typedef CArray<Type, Arrays::upper_triangular_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> CResult;
  typedef Array2DUpperTriangular<Type> Allocator;  // no CAllocator
};
template<typename Lhs, typename Rhs>
struct ProductResultType<Lhs, Rhs, Arrays::upper_triangular_, Arrays::vector_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
    typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::vector_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DVector<Type> Result2D;
  typedef CArray<Type, Arrays::vector_, Base::sizeRows_, 1, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::vector_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};

//----------------------------------
// vector case
template<typename Lhs, typename Rhs>
struct ProductResultType<Lhs, Rhs, Arrays::vector_, Arrays::point_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::general_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2D<Type> Result2D;
  typedef CArray<Type, Arrays::general_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::general_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};

//----------------------------------
// point case
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductResultType<Lhs, Rhs, Arrays::point_, RStructure_>
        : public ProductResultTypeBase<Lhs, Rhs>
{
  typedef ProductResultTypeBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::point_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DPoint<Type> Result2D;
  typedef CArray<Type, Arrays::point_,1, Base::sizeCols_, Base::orient_> CResult;
  typedef CAllocator<Type, Arrays::point_,1, Base::sizeCols_, Base::orient_> Allocator;
};

} // namespace hidden

/** @ingroup Arrays
  * @class PoiontByMatrixProduct
  * @brief Generic expression where a product operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions. The left hand Side is a point, the Right Hand Side
  * is any compatible exprssion or matrix.
  **/
template<typename Lhs, typename Rhs> class PointByMatrixProduct;

namespace hidden {

/** @ingroup hidden
 *  @brief Traits class for the BinaryOperator
 */
template< typename Lhs, typename Rhs>
struct Traits< PointByMatrixProduct < Lhs, Rhs> >  : public ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>
{
  typedef typename ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Type Type;
  typedef typename ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Allocator Allocator;
  enum
  {
    structure_ = Arrays::point_,
    sizeRows_  = 1,
    sizeCols_  = Lhs::sizeCols_,
    orient_    = Arrays::by_row_,
    storage_   = Arrays::dense_
  };
};

} // end namespace hidden


template<typename Lhs, typename Rhs>
class PointByMatrixProduct : public ArrayBase< PointByMatrixProduct<Lhs, Rhs> >
                            , public TRef<1>
{
  public:
    typedef ArrayBase< PointByMatrixProduct<Lhs, Rhs> > Base;
    typedef typename hidden::Traits<PointByMatrixProduct>::Type Type;
    typedef typename hidden::Traits<PointByMatrixProduct>::Allocator Allocator;

    enum
    {
      structure_ = hidden::Traits<PointByMatrixProduct>::structure_,
      orient_    = hidden::Traits<PointByMatrixProduct>::orient_,
      sizeRows_  = hidden::Traits<PointByMatrixProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<PointByMatrixProduct>::sizeCols_,
      storage_   = hidden::Traits<PointByMatrixProduct>::storage_
    };

    inline PointByMatrixProduct( const Lhs& lhs, const Rhs& rhs)
                              : Base(), lhs_(lhs), rhs_(rhs)
                              , result_(1, rhs.sizeCols(), Type(0))
    {
      if (lhs.range() != rhs.rows())
      { STKRUNTIME_ERROR_NO_ARG(PointByMatrixProduct, sizes mismatch);}
      result_.shift(rhs_.firstIdxCols());
      hidden::ProductImpl<Lhs, Rhs, Allocator>::run(lhs, rhs, result_);
    }
    /**  @return the range of the rows */
    inline Range const& range() const { return result_.cols();}
    /**  @return the range of the rows */
    inline Range const& rows() const { return result_.rows();}
    /** @return the range of the columns */
    inline Range const& cols() const { return result_.cols();}
    /** @return the size of the vector */
    inline int const sizeRows() const { return int(1);}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int const sizeCols() const { return result_.sizeCols();}

    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& result() const { return result_; }
    /** @return the ith element */
    inline Type const elt(int i) const { return result_.elt(i);}

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;

  private:
    Allocator result_;
};

/** @ingroup Arrays
  * @class MatrixByVectorProduct
  * @brief Generic expression where a product operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions. The left hand Side can be of any kind, the Right Hand Side
  * is a vector or vectorial expression.
  **/
template<typename Lhs, typename Rhs> class MatrixByVectorProduct;

namespace hidden {

/** @ingroup hidden
 *  @brief Traits class for the BinaryOperator
 */
template< typename Lhs, typename Rhs>
struct Traits< MatrixByVectorProduct < Lhs, Rhs> >  : public ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>
{
  typedef typename ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Type Type;
  typedef typename ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Allocator Allocator;
  enum
  {
    structure_ = Arrays::vector_,
    sizeRows_  = Lhs::sizeRows_,
    sizeCols_  = 1,
    orient_    = Arrays::by_col_,
    storage_   = Arrays::dense_
  };
};

} // end namespace hidden


template<typename Lhs, typename Rhs>
class MatrixByVectorProduct : public ArrayBase< MatrixByVectorProduct<Lhs, Rhs> >
                            , public TRef<1>
{
  public:
    typedef ArrayBase< MatrixByVectorProduct<Lhs, Rhs> > Base;
    typedef typename hidden::Traits<MatrixByVectorProduct>::Type Type;
    typedef typename hidden::Traits<MatrixByVectorProduct>::Allocator Allocator;

    enum
    {
      structure_ = hidden::Traits<MatrixByVectorProduct>::structure_,
      orient_    = hidden::Traits<MatrixByVectorProduct>::orient_,
      sizeRows_  = hidden::Traits<MatrixByVectorProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<MatrixByVectorProduct>::sizeCols_,
      storage_   = hidden::Traits<MatrixByVectorProduct>::storage_
    };

    inline MatrixByVectorProduct( const Lhs& lhs, const Rhs& rhs)
                              : Base(), lhs_(lhs), rhs_(rhs)
                              , result_(lhs.sizeRows(), 1, Type(0))
    {
      if (lhs.cols() != rhs.range())
      { STKRUNTIME_ERROR_NO_ARG(MatrixByVectorProduct, sizes mismatch);}
      result_.shift(lhs_.firstIdxRows());
      hidden::ProductImpl<Lhs, Rhs, Allocator>::run(lhs, rhs, result_);
    }
    /**  @return the range of the rows */
    inline Range const& range() const { return result_.rows();}
    /**  @return the range of the rows */
    inline Range const& rows() const { return result_.rows();}
    /** @return the range of the columns */
    inline Range const& cols() const { return result_.cols();}
    /** @return the size of the vector */
    inline int const sizeRows() const { return result_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int const sizeCols() const { return int(1);}

    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& result() const { return result_; }
    /** @return the ith element */
    inline Type const elt(int i) const { return result_.elt(i);}

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;

  private:
    Allocator result_;
};

/** @ingroup Arrays
  * @class MatrixByDiagonalProduct
  * @brief Generic expression where a product operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions. The left hand Side can be of any kind, the Left Hand Side
  * is a diagonal matrix or expression.
  **/
template<typename Lhs, typename Rhs> class MatrixByDiagonalProduct;

namespace hidden {

/** @ingroup hidden
 *  @brief Traits class for the BinaryOperator
 */
template< typename Lhs, typename Rhs>
struct Traits< MatrixByDiagonalProduct < Lhs, Rhs> >
{
  enum
  {
    structure_ = Lhs::structure_,
    sizeRows_  = Lhs::sizeRows_,
    sizeCols_  = Lhs::sizeCols_,
    // preserve the Lhs storage orientation
    orient_    = Lhs::orient_,
    storage_   = (int(Lhs::storage_) == int(Arrays::dense_))
               ?  int(Arrays::dense_) : int(Arrays::sparse_)
  };
  typedef typename Lhs::Type Type;
};

} // end namespace hidden


template<typename Lhs, typename Rhs>
class MatrixByDiagonalProduct : public ArrayBase< MatrixByDiagonalProduct<Lhs, Rhs> >
                              , public TRef<1>
{
  public:
    typedef ArrayBase< MatrixByDiagonalProduct<Lhs, Rhs> > Base;
    typedef typename hidden::Traits<MatrixByDiagonalProduct>::Type Type;

    enum
    {
      structure_ = hidden::Traits<MatrixByDiagonalProduct>::structure_,
      orient_    = hidden::Traits<MatrixByDiagonalProduct>::orient_,
      sizeRows_  = hidden::Traits<MatrixByDiagonalProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<MatrixByDiagonalProduct>::sizeCols_,
      storage_   = hidden::Traits<MatrixByDiagonalProduct>::storage_
    };

    inline MatrixByDiagonalProduct( const Lhs& lhs, const Rhs& rhs)
                                  : Base(), lhs_(lhs), rhs_(rhs)
    {
      if (lhs.cols() != rhs.rows())
      { STKRUNTIME_ERROR_NO_ARG(MatrixByDiagonalProduct, sizes mismatch);}
    }
    /**  @return the range of the rows */
    inline Range const rows() const { return lhs_.rows();}
    /** @return the range of the columns */
    inline Range const cols() const { return lhs_.cols();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeRows() const { return lhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeCols() const { return rhs_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** access to the element (i,j) */
    inline Type const elt(int i, int j) const
    { return lhs_.elt(i,j)*rhs_.elt(j);}

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;
};

/** @ingroup Arrays
  * @class DiagonalByMatrixProduct
  * @brief Generic expression where a product operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions. The left hand Side is a diagonal matrix or expression,
  * the Right Hand Side is of any kind.
  **/
template<typename Lhs, typename Rhs> class DiagonalByMatrixProduct;

namespace hidden {

/** @ingroup hidden
 *  @brief Traits class for the BinaryOperator
 */
template< typename Lhs, typename Rhs>
struct Traits< DiagonalByMatrixProduct < Lhs, Rhs> >
{
  enum
  {
    structure_ = Rhs::structure_,
    sizeRows_  = Rhs::sizeRows_,
    sizeCols_  = Rhs::sizeCols_,
    // preserve the Rhs storage orientation
    orient_    = Rhs::orient_,
    storage_   = Rhs::storage_
  };
  typedef typename Lhs::Type Type;
};

} // end namespace hidden


template<typename Lhs, typename Rhs>
class DiagonalByMatrixProduct : public ArrayBase< DiagonalByMatrixProduct<Lhs, Rhs> >
                              , public TRef<1>
{
  public:
    typedef ArrayBase< DiagonalByMatrixProduct<Lhs, Rhs> > Base;
    typedef typename hidden::Traits<DiagonalByMatrixProduct>::Type Type;

    enum
    {
      structure_ = hidden::Traits<DiagonalByMatrixProduct>::structure_,
      orient_    = hidden::Traits<DiagonalByMatrixProduct>::orient_,
      sizeRows_  = hidden::Traits<DiagonalByMatrixProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<DiagonalByMatrixProduct>::sizeCols_,
      storage_   = hidden::Traits<DiagonalByMatrixProduct>::storage_
    };

    inline DiagonalByMatrixProduct( const Lhs& lhs, const Rhs& rhs)
                                  : Base(), lhs_(lhs), rhs_(rhs)
    {
      if (lhs.cols() != rhs.rows())
      { STKRUNTIME_ERROR_NO_ARG(DiagonalByMatrixProduct, sizes mismatch);}
    }
    /**  @return the range of the rows */
    inline Range const rows() const { return lhs_.rows();}
    /** @return the range of the columns */
    inline Range const cols() const { return rhs_.cols();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeRows() const { return lhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeCols() const { return rhs_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** access to the element (i,j) */
    inline Type const elt(int i, int j) const
    { return lhs_.elt(i)*rhs_.elt(i,j);}

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;
};

/** @ingroup Arrays
  * @class MatrixByMatrixProduct
  * @brief Generic expression where a product operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions.
  * It is the return type of product operators, by which we mean only those
  * product operators where both the left-hand side and the right-hand side
  * are expressions. For example, the return type of matrix1*matrix2 is a
  * ProductOperator. The return type of number*matrix is a unary operator.
  *
  * Most of the time, this is the only way that it is used, so you typically
  * don't have to name MatrixByMatrixProduct types explicitly.
  **/
template<typename Lhs, typename Rhs> class MatrixByMatrixProduct;

namespace hidden
{

/** @ingroup hidden
 *  @brief Traits class for the MatrixByMatrixProduct class
 */
template< typename Lhs, typename Rhs>
struct Traits< MatrixByMatrixProduct < Lhs, Rhs> > : public ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>
{
  typedef typename ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Type Type;
  typedef typename ProductResultType<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Allocator Allocator;
};

} // end namespace hidden

// forward declaration
template< typename Lhs, typename Rhs, int LhsStructure_ = Lhs::structure_, int RhsStructure_ = Rhs::structure_>
class ProductBase;

template<typename Lhs, typename Rhs>
class MatrixByMatrixProduct : public ProductBase< Lhs, Rhs >
                            , public TRef<1>
{
  public:
    typedef ProductBase< Lhs, Rhs> Base;
    typedef typename hidden::Traits<MatrixByMatrixProduct>::Type Type;
    typedef typename hidden::Traits<MatrixByMatrixProduct < Lhs, Rhs> >::Allocator Allocator;

    enum
    {
      // All the valid cases for MatrixByMatrix operator
     isValid_ =(  (EGAL(Lhs,general_) && !EGAL(Rhs,point_)  && !EGAL(Rhs,number_) && !EGAL(Rhs,diagonal_) )
               || (EGAL(Lhs,square_)  && !EGAL(Rhs,point_)  && !EGAL(Rhs,number_) && !EGAL(Rhs,diagonal_) )
               || (EGAL(Lhs,lower_triangular_) && !EGAL(Rhs,point_)  && !EGAL(Rhs,number_) && !EGAL(Rhs,diagonal_) )
               || (EGAL(Lhs,upper_triangular_) && !EGAL(Rhs,point_)  && !EGAL(Rhs,number_) && !EGAL(Rhs,diagonal_) )
               || (EGAL(Lhs,vector_) && EGAL(Rhs,point_) )
               || (EGAL(Lhs,point_)  && !EGAL(Rhs,point_) && !EGAL(Rhs,vector_) && !EGAL(Rhs,number_) && !EGAL(Rhs,diagonal_) )
               ),

      structure_ = hidden::Traits<MatrixByMatrixProduct>::structure_,
      orient_    = hidden::Traits<MatrixByMatrixProduct>::orient_,
      sizeRows_  = hidden::Traits<MatrixByMatrixProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<MatrixByMatrixProduct>::sizeCols_,
      storage_   = hidden::Traits<MatrixByMatrixProduct>::storage_
    };

    inline MatrixByMatrixProduct( const Lhs& lhs, const Rhs& rhs)
                              : Base(), lhs_(lhs), rhs_(rhs)
                              , result_(lhs.sizeRows(), rhs.sizeCols(), Type(0))
    {
      STK_STATICASSERT_PRODUCT_OPERATOR_MISMATCH( isValid_ );
      if (lhs.cols() != rhs.rows())
      { STKRUNTIME_ERROR_NO_ARG(MatrixByMatrixProduct, sizes mismatch for 2D array);}
      result_.shift(lhs_.firstIdxRows(), rhs_.firstIdxCols());
      orient_ ? hidden::ProductImpl<Lhs, Rhs, Allocator>::runbp(lhs, rhs, result_)
              : hidden::ProductImpl<Lhs, Rhs, Allocator>::runpb(lhs, rhs, result_);
    }
    /**  @return the range of the rows */
    inline Range const rows() const { return result_.rows();}
    /** @return the range of the columns */
    inline Range const cols() const { return result_.cols();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeRows() const { return result_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeCols() const { return result_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the result */
    inline Allocator const& result() const { return result_; }

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;

  private:
    Allocator result_;
};

/** @ingroup Arrays
  * @brief implement the access to the elements in the (2D) general case.
  **/
template< typename Lhs, typename Rhs, int LhsStructure_, int RhsStructure_>
class ProductBase : public ArrayBase< MatrixByMatrixProduct<Lhs, Rhs> >
{
  public:
    typedef ArrayBase< MatrixByMatrixProduct< Lhs, Rhs> > Base;
    typedef typename Base::Type Type;

    /** constructor. */
    inline ProductBase() : Base() {}
    /** access to the element (i,j) */
    inline Type const elt(int i, int j) const
    { return this->asDerived().result().elt(i,j);}
    /** access to the element i */
    inline Type const elt(int i) const
    { return this->asDerived().result().elt(i);}
    /** access to the element */
    inline Type const elt() const
    { return this->asDerived().result().elt();}
};

}  // namespace STK

#undef EGAL

#endif /* STK_PRODUCTOPERATOR_H */
