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
 * created on: 20 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ProductOperators.h
 *  @brief In this file we implement the ProductOperator class.
 **/

#ifndef STK_PRODUCTOPERATOR_H
#define STK_PRODUCTOPERATOR_H


#define EGAL(arg1, arg2) ((arg1::structure_ == int(Arrays::arg2)))

#include "../STK_CAllocator.h"
#include "STK_ProductImpl.h"

namespace STK
{

// forward declarations (needed because there is no CAllocator with this structure)
template<typename> class Array2DUpperTriangular;
template<typename> class Array2DLowerTriangular;

namespace hidden
{

/** @ingroup hidden
 *  @brief Helper class to get the correct enum characteristic of operator*
 *
 *  TODO the orientation can be not enough efficient if for example, we have
 *  vector, point or diagonal on lhs/rhs sides.
 **/
template<typename Lhs, typename Rhs>
struct ProductTraitsBase
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
 *  @brief Traits class to get the correct returned Structure, Type, allocator,...
 *  of operator*. This Traits class is used by the functors classes operating
 *  on the Array2D  classes.
 *  @note the impossible cases are tracked in MatrixByMatrixProduct class.
 **/
template<typename Lhs, typename Rhs, int LStructure_, int RStructure_>
struct ProductTraits;

//------------------------------------------------------------
// general lhs case. result is general except if rhs is vector
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductTraits<Lhs, Rhs, Arrays::array2D_, RStructure_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::array2D_, Base::sizeRows_, Base::sizeCols_, Base::orient_> Allocator;
  enum
  {
    structure_ = Arrays::array2D_,
    sizeRows_  = Base::sizeRows_,
    sizeCols_  = Base::sizeCols_,
    orient_    = Base::orient_,
    storage_   = Base::storage_
  };
};
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::array2D_, Arrays::vector_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::vector_, Base::sizeRows_ , 1, Arrays::by_col_> Allocator;
  enum
  {
    structure_ = Arrays::vector_,
    sizeRows_  = Base::sizeRows_,
    sizeCols_  = 1,
    orient_    = Arrays::by_col_,
    storage_   = Base::storage_
  };
};

//----------------------------------
// square lhs case. result is general except if rhs is vector, square or diagonal
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductTraits<Lhs, Rhs, Arrays::square_, RStructure_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::array2D_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
  enum
  {
    structure_ = Arrays::array2D_,
    sizeRows_  = Base::sizeRows_,
    sizeCols_  = Base::sizeCols_,
    orient_    = Base::orient_,
    storage_   = Base::storage_
  };
};
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::square_, Arrays::square_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::square_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
  enum
  {
    structure_ = Arrays::square_,
    sizeRows_  = Base::sizeRows_,
    sizeCols_  = Base::sizeCols_,
    orient_    = Base::orient_,
    storage_   = Base::storage_
  };
};
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::square_, Arrays::diagonal_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  enum
  {
    structure_ = Arrays::square_,
    sizeRows_  = Base::sizeRows_,
    sizeCols_  = Base::sizeCols_,
    orient_    = Base::orient_,
    storage_   = Base::storage_
  };
};
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::square_, Arrays::vector_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::vector_,Base::sizeRows_ , 1, Base::orient_> Allocator;
  enum
  {
    structure_ = Arrays::vector_,
    sizeRows_  = Base::sizeRows_,
    sizeCols_  = 1,
    orient_    = Arrays::by_col_,
    storage_   = Base::storage_
  };
};

//----------------------------------
// diagonal  lhs case. result is Rhs structure
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductTraits<Lhs, Rhs, Arrays::diagonal_, RStructure_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::array2D_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
  enum
  {
    structure_ = RStructure_,
    sizeRows_  = Base::sizeRows_,
    sizeCols_  = Base::sizeCols_,
    orient_    = Base::orient_,
    storage_   = Base::storage_
  };
};
//----------------------------------
// lower triangular case
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductTraits<Lhs, Rhs, Arrays::lower_triangular_, RStructure_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::array2D_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::array2D_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::lower_triangular_, Arrays::lower_triangular_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::lower_triangular_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DLowerTriangular<Type> Allocator; // no CAllocator
};
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::lower_triangular_, Arrays::vector_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::vector_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::vector_,Base::sizeRows_ , 1, Base::orient_> Allocator;
};

//----------------------------------
// upper triangular case
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductTraits<Lhs, Rhs, Arrays::upper_triangular_, RStructure_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::array2D_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::array2D_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::upper_triangular_, Arrays::upper_triangular_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::upper_triangular_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef Array2DUpperTriangular<Type> Allocator;  // no CAllocator
};
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::upper_triangular_, Arrays::vector_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::vector_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
  enum
  {
    structure_ = Arrays::vector_,
    sizeRows_  = Lhs::sizeRows_,
    sizeCols_  = 1,
    orient_    = Arrays::by_col_,
    storage_   = Arrays::dense_
  };

};

//----------------------------------
// vector case
template<typename Lhs, typename Rhs>
struct ProductTraits<Lhs, Rhs, Arrays::vector_, Arrays::point_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  enum { structure_ = Arrays::array2D_};
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::array2D_,Base::sizeRows_ , Base::sizeCols_, Base::orient_> Allocator;
};

//----------------------------------
// point case
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductTraits<Lhs, Rhs, Arrays::point_, RStructure_>
        : public ProductTraitsBase<Lhs, Rhs>
{
  typedef ProductTraitsBase<Lhs, Rhs> Base;
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, Arrays::point_,1, Base::sizeCols_, Base::orient_> Allocator;
  enum
  {
    structure_ = Arrays::point_,
    sizeRows_  = 1,
    sizeCols_  = Rhs::sizeCols_,
    orient_    = Arrays::by_row_,
    storage_   = Base::storage_
  };
};

} // namespace hidden

/** @ingroup Arrays
  * @class PointByMatrixProduct
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
struct Traits< PointByMatrixProduct < Lhs, Rhs> >  : public ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_>
{
  typedef typename ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Type Type;
  typedef typename ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Allocator Allocator;
};

} // end namespace hidden


template<typename Lhs, typename Rhs>
class PointByMatrixProduct : public ExprBase< PointByMatrixProduct<Lhs, Rhs> >
                            , public TRef<1>
{
  public:
    typedef ExprBase< PointByMatrixProduct<Lhs, Rhs> > Base;
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
    inline Range range() const { return result_.cols();}
    /**  @return the range of the rows */
    inline Range rows() const { return result_.rows();}
    /** @return the range of the columns */
    inline Range cols() const { return result_.cols();}
    /** @return the size of the vector */
    inline int const sizeRowsImpl() const { return 1;}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int const sizeColsImpl() const { return result_.sizeCols();}

    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& result() const { return result_; }
    /** @return the ith element */
    inline Type const elt1Impl(int i) const { return result_.elt(i);}

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
 *  @brief Traits class for the MatrixByVectorProduct class
 */
template< typename Lhs, typename Rhs>
struct Traits< MatrixByVectorProduct < Lhs, Rhs> >  : public ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_>
{
  typedef ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_> Base;
  typedef typename Base::Type Type;
  typedef typename Base::Allocator Allocator;
};

} // end namespace hidden


template<typename Lhs, typename Rhs>
class MatrixByVectorProduct : public ExprBase< MatrixByVectorProduct<Lhs, Rhs> >
                            , public TRef<1>
{
  public:
    typedef ExprBase< MatrixByVectorProduct<Lhs, Rhs> > Base;
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
    inline Range range() const { return result_.rows();}
    /**  @return the range of the rows */
    inline Range rows() const { return result_.rows();}
    /** @return the range of the columns */
    inline Range cols() const { return result_.cols();}
    /** @return the size of the vector */
    inline int const sizeRowsImpl() const { return result_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int const sizeColsImpl() const { return int(1);}

    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& result() const { return result_; }
    /** @return the ith element */
    inline Type const elt1Impl(int i) const { return result_.elt(i);}

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
class MatrixByDiagonalProduct : public ExprBase< MatrixByDiagonalProduct<Lhs, Rhs> >
                              , public TRef<1>
{
  public:
    typedef ExprBase< MatrixByDiagonalProduct<Lhs, Rhs> > Base;
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
    inline int sizeRowsImpl() const { return lhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeColsImpl() const { return rhs_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** access to the element (i,j) */
    inline Type const elt2Impl(int i, int j) const
    { return lhs_.elt(i,j)*rhs_.elt(j);}

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;
};

/** @ingroup Arrays
  * @class PointByMatrixProduct
  * @brief Generic expression where a product operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions. The left hand Side is a point, the Right Hand Side
  * is any diagonal expression.
  **/
template<typename Lhs, typename Rhs> class PointByDiagonalProduct;

namespace hidden
{

/** @ingroup hidden
 *  @brief Traits class for the BinaryOperator
 */
template< typename Lhs, typename Rhs>
struct Traits< PointByDiagonalProduct < Lhs, Rhs> >  : public ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_>
{
  typedef typename ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Type Type;
  typedef typename ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_>::Allocator Allocator;
  enum
  {
    structure_ = Arrays::point_,
    sizeRows_  = 1,
    sizeCols_  = Rhs::sizeCols_,
    orient_    = Arrays::by_row_,
    storage_   = Arrays::dense_
  };
};

} // end namespace hidden


template<typename Lhs, typename Rhs>
class PointByDiagonalProduct : public ExprBase< PointByDiagonalProduct<Lhs, Rhs> >
                            , public TRef<1>
{
  public:
    typedef ExprBase< PointByDiagonalProduct<Lhs, Rhs> > Base;
    typedef typename hidden::Traits<PointByDiagonalProduct>::Type Type;
    typedef typename hidden::Traits<PointByDiagonalProduct>::Allocator Allocator;

    enum
    {
      structure_ = hidden::Traits<PointByDiagonalProduct>::structure_,
      orient_    = hidden::Traits<PointByDiagonalProduct>::orient_,
      sizeRows_  = hidden::Traits<PointByDiagonalProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<PointByDiagonalProduct>::sizeCols_,
      storage_   = hidden::Traits<PointByDiagonalProduct>::storage_
    };

    inline PointByDiagonalProduct( const Lhs& lhs, const Rhs& rhs)
                                 : Base(), lhs_(lhs), rhs_(rhs)
    {
      if (lhs.range() != rhs.range())
      { STKRUNTIME_ERROR_NO_ARG(PointByDiagonalProduct, sizes mismatch);}
    }
    /**  @return the range of the rows */
    inline Range const rows() const { return lhs_.rows();}
    /** @return the range of the columns */
    inline Range const cols() const { return lhs_.cols();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeRowsImpl() const { return lhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeColsImpl() const { return rhs_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** access to the element (i,j) */
    inline Type const elt1Impl(int i) const
    { return lhs_.elt(i)*rhs_.elt(i);}

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;
};


/** @ingroup Arrays
  * @class DiagonalByVectorProduct
  * @brief Generic expression where a product operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side, a diagonal expression
  * @tparam Rhs the type of the right-hand side, a vector expression
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions. The left hand Side is a diagonal expression,
  * the Right Hand Side is a vector expression.
  **/
template<typename Lhs, typename Rhs> class DiagonalByVectorProduct;

namespace hidden {

/** @ingroup hidden
 *  @brief Traits class for the DiagonalByVectorProduct class
 */
template< typename Lhs, typename Rhs>
struct Traits< DiagonalByVectorProduct < Lhs, Rhs> >
{
  enum
  {
    structure_ = Arrays::vector_,
    sizeRows_  = Lhs::sizeRows_,
    sizeCols_  = 1,
    // preserve the Rhs storage orientation
    orient_    = Arrays::by_col_,
    storage_   = Rhs::storage_
  };
  typedef typename Lhs::Type Type;
};

} // end namespace hidden


template<typename Lhs, typename Rhs>
class DiagonalByVectorProduct : public ExprBase< DiagonalByVectorProduct<Lhs, Rhs> >
                              , public TRef<1>
{
  public:
    typedef ExprBase< DiagonalByVectorProduct<Lhs, Rhs> > Base;
    typedef typename hidden::Traits<DiagonalByVectorProduct>::Type Type;

    enum
    {
      structure_ = hidden::Traits<DiagonalByVectorProduct>::structure_,
      orient_    = hidden::Traits<DiagonalByVectorProduct>::orient_,
      sizeRows_  = hidden::Traits<DiagonalByVectorProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<DiagonalByVectorProduct>::sizeCols_,
      storage_   = hidden::Traits<DiagonalByVectorProduct>::storage_
    };

    inline DiagonalByVectorProduct( const Lhs& lhs, const Rhs& rhs)
                                  : Base(), lhs_(lhs), rhs_(rhs)
    {
      if (lhs.range() != rhs.range())
      { STKRUNTIME_ERROR_NO_ARG(DiagonalByVectorProduct, sizes mismatch);}
    }
    /**  @return the range of the rows */
    inline Range const rows() const { return lhs_.rows();}
    /** @return the range of the columns */
    inline Range const cols() const { return rhs_.cols();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeRowsImpl() const { return lhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeColsImpl() const { return rhs_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** access to the element (i,j) */
    inline Type const elt1Impl(int i) const
    { return lhs_.elt(i)*rhs_.elt(i);}

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;
};

/** @ingroup Arrays
  * @class VectorByPointProduct
  * @brief Generic expression where a product operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side, a vector expression
  * @tparam Rhs the type of the right-hand side, a row-vector expression
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions. The left hand Side is a vector expression,
  * the Right Hand Side is a row-vector expression.
  **/
template<typename Lhs, typename Rhs> class VectorByPointProduct;

namespace hidden {

/** @ingroup hidden
 *  @brief Traits class for the DiagonalByVectorProduct class
 */
template< typename Lhs, typename Rhs>
struct Traits< VectorByPointProduct < Lhs, Rhs> >
{
  enum
  {
    structure_ = Arrays::array2D_,
    sizeRows_  = Lhs::sizeRows_,
    sizeCols_  = Rhs::sizeCols_,
    // preserve the Lhs storage orientation
    orient_    = Arrays::by_col_,
    storage_   = Arrays::dense_
  };
  typedef typename Lhs::Type Type;
};

} // end namespace hidden

template<typename Lhs, typename Rhs>
class VectorByPointProduct : public ExprBase< VectorByPointProduct<Lhs, Rhs> >
                            , public TRef<1>
{
  public:
    typedef ExprBase< VectorByPointProduct<Lhs, Rhs> > Base;
    typedef typename hidden::Traits<VectorByPointProduct>::Type Type;

    enum
    {
      structure_ = hidden::Traits<VectorByPointProduct>::structure_,
      orient_    = hidden::Traits<VectorByPointProduct>::orient_,
      sizeRows_  = hidden::Traits<VectorByPointProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<VectorByPointProduct>::sizeCols_,
      storage_   = hidden::Traits<VectorByPointProduct>::storage_
    };

    inline VectorByPointProduct( const Lhs& lhs, const Rhs& rhs)
                               : Base(), lhs_(lhs), rhs_(rhs)
    {}
    /**  @return the range of the rows */
    inline Range const rows() const { return lhs_.rows();}
    /** @return the range of the columns */
    inline Range const cols() const { return rhs_.cols();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeRowsImpl() const { return lhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeColsImpl() const { return rhs_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** access to the element (i,j) */
    inline Type const elt2Impl(int i, int j) const
    { return lhs_.elt(i)*rhs_.elt(j);}

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
class DiagonalByMatrixProduct : public ExprBase< DiagonalByMatrixProduct<Lhs, Rhs> >
                              , public TRef<1>
{
  public:
    typedef ExprBase< DiagonalByMatrixProduct<Lhs, Rhs> > Base;
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
    inline int sizeRowsImpl() const { return lhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeColsImpl() const { return rhs_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** access to the element (i,j) */
    inline Type const elt2Impl(int i, int j) const
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
struct Traits< MatrixByMatrixProduct < Lhs, Rhs> > : public ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_>
{
  typedef ProductTraits<Lhs, Rhs, Lhs::structure_, Rhs::structure_> Base;
  typedef typename Base::Type Type;
  typedef typename Base::Allocator Allocator;
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
     isValid_ =(  (EGAL(Lhs,array2D_) && !EGAL(Rhs,point_)  && !EGAL(Rhs,number_) && !EGAL(Rhs,diagonal_) )
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
    inline int sizeRowsImpl() const { return result_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeColsImpl() const { return result_.sizeCols();}
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
class ProductBase : public ExprBase< MatrixByMatrixProduct<Lhs, Rhs> >
{
  public:
    typedef ExprBase< MatrixByMatrixProduct< Lhs, Rhs> > Base;
    typedef typename Base::Type Type;

    /** constructor. */
    inline ProductBase() : Base() {}
    /** access to the element (i,j) */
    inline Type const elt2Impl(int i, int j) const
    { return this->asDerived().result().elt(i,j);}
    /** access to the element i */
    inline Type const elt1Impl(int i) const
    { return this->asDerived().result().elt(i);}
    /** access to the element */
    inline Type const elt0Impl() const
    { return this->asDerived().result().elt();}
};

}  // namespace STK

#undef EGAL

#endif /* STK_PRODUCTOPERATOR_H */
