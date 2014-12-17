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
 * created on: 17 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_BinaryOperators.h
 *  @brief In this file we implement the BinaryOperator class.
 **/


#ifndef STK_BINARYOPERATORS_H
#define STK_BINARYOPERATORS_H

#include "STK_SliceOperators.h"

#define EGAL(arg1, arg2) ((arg1::structure_ == int(Arrays::arg2)))

namespace STK
{

namespace Arrays
{

/** Kind of the operands in a BinaryOperator. */
enum BinOpKind
{
  binOp0d_ = 0, ///< both operand are number_
  binOp1d_ = 1, ///< both operand are vector_ or point_
  binOp2d_ = 2, ///< both operand are array2d_ or square_
  binOpDiag2d_,
  binOp2dDiag_,
  binOp2dUp_,
  binOpUp2d_,
  binOp2dLow_,
  binOpLow2d_,
  binOpDiagUp_,
  binOpUpDiag_,
  binOpDiagLow_,
  binOpLowDiag_,
  binOpUpUp_,
  binOpUpLow_,
  binOpLowUp_,
  binOpLowLow_
};

} // namespace Arrays

namespace hidden
{
/** @ingroup hidden
 *  @brief Helper class to get the correct returned Structure of a binary operator
 **/
template<int LStructure_, int RStructure_> struct BinaryTraits;

// Lhs is array2D_
template<> struct BinaryTraits<Arrays::array2D_, Arrays::array2D_>
{ enum { structure_ = Arrays::array2D_, binOpKind_ = Arrays::binOp2d_};};
template<> struct BinaryTraits<Arrays::array2D_, Arrays::square_>
{ enum { structure_ = Arrays::square_, binOpKind_ = Arrays::binOp2d_};};
template<> struct BinaryTraits<Arrays::array2D_, Arrays::diagonal_>
{ enum { structure_ = Arrays::array2D_, binOpKind_= Arrays::binOp2dDiag_};};
template<> struct BinaryTraits<Arrays::array2D_, Arrays::upper_triangular_>
{ enum { structure_ = Arrays::array2D_, binOpKind_= Arrays::binOp2dUp_};};
template<> struct BinaryTraits<Arrays::array2D_, Arrays::lower_triangular_>
{ enum { structure_ = Arrays::array2D_, binOpKind_= Arrays::binOp2dLow_};};

// Lhs is square_
template<> struct BinaryTraits<Arrays::square_, Arrays::array2D_>
{ enum { structure_ = Arrays::square_, binOpKind_ = Arrays::binOp2d_};};
template<> struct BinaryTraits<Arrays::square_, Arrays::square_>
{ enum { structure_ = Arrays::square_, binOpKind_ = Arrays::binOp2d_};};
template<> struct BinaryTraits<Arrays::square_, Arrays::diagonal_>
{ enum { structure_ = Arrays::square_, binOpKind_= Arrays::binOp2dDiag_};};
template<> struct BinaryTraits<Arrays::square_, Arrays::upper_triangular_>
{ enum { structure_ = Arrays::square_, binOpKind_= Arrays::binOp2dUp_};};
template<> struct BinaryTraits<Arrays::square_, Arrays::lower_triangular_>
{ enum { structure_ = Arrays::square_, binOpKind_= Arrays::binOp2dLow_};};

// lhs is diagonal_
template<> struct BinaryTraits<Arrays::diagonal_, Arrays::array2D_>
{ enum { structure_ = Arrays::array2D_, binOpKind_= Arrays::binOpDiag2d_};};
template<> struct BinaryTraits<Arrays::diagonal_, Arrays::square_>
{ enum { structure_ = Arrays::square_, binOpKind_= Arrays::binOpDiag2d_};};
template<> struct BinaryTraits<Arrays::diagonal_, Arrays::diagonal_>
{ enum { structure_ = Arrays::diagonal_, binOpKind_= Arrays::binOp1d_};};
template<> struct BinaryTraits<Arrays::diagonal_, Arrays::point_>
{ enum { structure_ = Arrays::diagonal_, binOpKind_= Arrays::binOp1d_};};
template<> struct BinaryTraits<Arrays::diagonal_, Arrays::vector_>
{ enum { structure_ = Arrays::diagonal_, binOpKind_= Arrays::binOp1d_};};
template<> struct BinaryTraits<Arrays::diagonal_, Arrays::upper_triangular_>
{ enum { structure_ = Arrays::upper_triangular_, binOpKind_= Arrays::binOpDiagUp_};};
template<> struct BinaryTraits<Arrays::diagonal_, Arrays::lower_triangular_>
{ enum { structure_ = Arrays::lower_triangular_, binOpKind_= Arrays::binOpDiagLow_};};

// lhs is lower_triangular_
template<> struct BinaryTraits<Arrays::lower_triangular_, Arrays::array2D_>
{ enum { structure_ = Arrays::array2D_, binOpKind_= Arrays::binOpLow2d_};};
template<> struct BinaryTraits<Arrays::lower_triangular_, Arrays::square_>
{ enum { structure_ = Arrays::square_, binOpKind_= Arrays::binOpLow2d_};};
template<> struct BinaryTraits<Arrays::lower_triangular_, Arrays::diagonal_>
{ enum { structure_ = Arrays::lower_triangular_, binOpKind_= Arrays::binOpLowDiag_};};
template<> struct BinaryTraits<Arrays::lower_triangular_, Arrays::lower_triangular_>
{ enum { structure_ = Arrays::lower_triangular_, binOpKind_= Arrays::binOpLowLow_};};
template<> struct BinaryTraits<Arrays::lower_triangular_, Arrays::upper_triangular_>
{ enum { structure_ = Arrays::array2D_, binOpKind_= Arrays::binOpLowUp_};};

// lhs is upper_triangular_
template<> struct BinaryTraits<Arrays::upper_triangular_, Arrays::array2D_>
{ enum { structure_ = Arrays::array2D_, binOpKind_= Arrays::binOpUp2d_};};
template<> struct BinaryTraits<Arrays::upper_triangular_, Arrays::square_>
{ enum { structure_ = Arrays::square_, binOpKind_= Arrays::binOpUp2d_};};
template<> struct BinaryTraits<Arrays::upper_triangular_, Arrays::diagonal_>
{ enum { structure_ = Arrays::upper_triangular_, binOpKind_= Arrays::binOpUpDiag_};};
template<> struct BinaryTraits<Arrays::upper_triangular_, Arrays::lower_triangular_>
{ enum { structure_ = Arrays::array2D_, binOpKind_= Arrays::binOpUpLow_};};
template<> struct BinaryTraits<Arrays::upper_triangular_, Arrays::upper_triangular_>
{ enum { structure_ = Arrays::upper_triangular_, binOpKind_= Arrays::binOpUpUp_};};

// Lhs is vector_
template<> struct BinaryTraits<Arrays::vector_, Arrays::diagonal_>
{ enum { structure_ = Arrays::vector_, binOpKind_= Arrays::binOp1d_};};
template<> struct BinaryTraits<Arrays::vector_, Arrays::vector_>
{ enum { structure_ = Arrays::vector_, binOpKind_= Arrays::binOp1d_};};
template<> struct BinaryTraits<Arrays::vector_, Arrays::point_>
{ enum { structure_ = Arrays::vector_, binOpKind_= Arrays::binOp1d_};};

// Lhs is point_
template<> struct BinaryTraits<Arrays::point_, Arrays::diagonal_>
{ enum { structure_ = Arrays::point_, binOpKind_= Arrays::binOp1d_};};
template<> struct BinaryTraits<Arrays::point_, Arrays::vector_>
{ enum { structure_ = Arrays::point_, binOpKind_= Arrays::binOp1d_};};
template<> struct BinaryTraits<Arrays::point_, Arrays::point_>
{ enum { structure_ = Arrays::point_, binOpKind_= Arrays::binOp1d_};};

// Lhs is number_
template<> struct BinaryTraits<Arrays::number_, Arrays::number_>
{ enum { structure_ = Arrays::number_, binOpKind_= Arrays::binOp0d_};};

} //namespace hidden


template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperator;

namespace hidden {

/** @ingroup hidden
 *  @brief Traits class for the BinaryOperator
 */
template<typename BinaryOp, typename Lhs, typename Rhs>
struct Traits< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  enum
  {
    // find the kind of binary operator and the Structure using helper class BinaryTraits
    binOpKind_ = BinaryTraits<Lhs::structure_, Rhs::structure_>::binOpKind_,
    structure_ = hidden::BinaryTraits<Lhs::structure_, Rhs::structure_>::structure_,
    // helper flags
    isLhs1D_ = EGAL(Lhs,vector_)||EGAL(Lhs,point_)||EGAL(Lhs,diagonal_),
    isRhs1D_ = EGAL(Rhs,vector_)||EGAL(Rhs,point_)||EGAL(Rhs,diagonal_),
    isLhs2D_ = EGAL(Lhs,array2D_)||EGAL(Lhs,square_)||EGAL(Lhs,diagonal_)||EGAL(Lhs,lower_triangular_)||EGAL(Lhs,upper_triangular_),
    isRhs2D_ = EGAL(Rhs,array2D_)||EGAL(Rhs,square_)||EGAL(Rhs,diagonal_)||EGAL(Rhs,lower_triangular_)||EGAL(Rhs,upper_triangular_),
    isRes0D_ = EGAL(Lhs,number_) && EGAL(Rhs,number_),
    isRes1D_ = (EGAL(Lhs,vector_)||EGAL(Lhs,point_)) && (EGAL(Rhs,vector_)||EGAL(Rhs,point_)),
    isRes2D_ = isLhs2D_ && isRhs2D_,
    is1D1D_  = isLhs1D_ && isRhs1D_,

    // preserve the Lhs storage orientation. Could be optimized ?
    orient_    = Lhs::orient_,
    // try fixed sizes for 2D containers
    sizeRows_  = ((Lhs::sizeRows_== UnknownSize) ? int(Rhs::sizeRows_) : Lhs::sizeRows_),
    sizeCols_  = ((Lhs::sizeCols_== UnknownSize) ? int(Rhs::sizeCols_) : Lhs::sizeCols_),
    storage_   = (Lhs::storage_ == int(Arrays::dense_)) || (Rhs::storage_ == int(Arrays::dense_))
               ?  int(Arrays::dense_) : int(Arrays::sparse_)
  };
  // handle the case when the type is different in LHS and Rhs.
  typedef typename BinaryOp::result_type Type;
  typedef RowOperator< BinaryOperator<BinaryOp, Lhs, Rhs> > Row;
  typedef ColOperator< BinaryOperator<BinaryOp, Lhs, Rhs> > Col;

};

} // end namespace hidden


/** @ingroup Arrays
  * @class BinaryOperatorBase
 *  @brief implement the access to the elements in the (2D) general case.
  **/
template< typename BinaryOp, typename Lhs, typename Rhs
        , int kind = hidden::Traits< BinaryOperator <BinaryOp, Lhs, Rhs> >::binOpKind_ >
class BinaryOperatorBase;

/** @ingroup Arrays
  * @class BinaryOperator
  *
  * @brief Generic expression where a binary operator is
  * applied to two expressions
  *
  * @tparam BinaryOp template functor implementing the operator
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class represents an expression  where a binary operator is applied to
  * two expressions.
  * It is the return type of binary operators, by which we mean only those
  * binary operators where both the left-hand side and the right-hand side
  * are expressions. For example, the return type of matrix1+matrix2 is a
  * BinaryOperator. The return type of number+matrix is a unary operator.
  *
  * Most of the time, this is the only way that it is used, so you typically
  * don't have to name BinaryOperator types explicitly.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperator : public BinaryOperatorBase< BinaryOp, Lhs, Rhs >
                     , public TRef<1>
{
  public:
    typedef BinaryOperatorBase<BinaryOp, Lhs, Rhs, hidden::Traits< BinaryOperator <BinaryOp, Lhs, Rhs> >::binOpKind_> Base;

    typedef typename hidden::Traits<BinaryOperator >::Type Type;
    typedef typename hidden::Traits<BinaryOperator >::Row Row;
    typedef typename hidden::Traits<BinaryOperator >::Col Col;

    enum
    {
      isRes0D_   = hidden::Traits<BinaryOperator>::isRes0D_,
      isRes1D_   = hidden::Traits<BinaryOperator>::isRes1D_,
      isRes2D_   = hidden::Traits<BinaryOperator>::isRes2D_,
      is1D1D_    = hidden::Traits<BinaryOperator>::is1D1D_,

      structure_ = hidden::Traits<BinaryOperator>::structure_,
      orient_    = hidden::Traits<BinaryOperator>::orient_,
      sizeRows_  = hidden::Traits<BinaryOperator>::sizeRows_,
      sizeCols_  = hidden::Traits<BinaryOperator>::sizeCols_,
      storage_   = hidden::Traits<BinaryOperator>::storage_,

      // All the valid cases for binary operators
      isValid_ =( isRes0D_ || isRes1D_  || isRes2D_ || is1D1D_)
    };
    inline BinaryOperator( const Lhs& lhs, const Rhs& rhs, const BinaryOp& func = BinaryOp())
                            : Base(), lhs_(lhs), rhs_(rhs), functor_(func)
    { // FIXME : not safe. Add more test in the 1D case at compile time (and runtime ?)
      STK_STATICASSERT_BINARY_OPERATOR_MISMATCH( isValid_ );
      STK_STATICASSERT_COLS_DIMENSIONS_MISMATCH(!( (int(Lhs::sizeCols_) != UnknownSize)
                                               &&  (int(Rhs::sizeCols_) != UnknownSize)
                                               &&  (int(Lhs::sizeCols_) != int(Rhs::sizeCols_))
                                               &&  (isRes2D_)
                                                 ));
      STK_STATICASSERT_ROWS_DIMENSIONS_MISMATCH(!( (int(Lhs::sizeRows_) != UnknownSize)
                                               &&  (int(Rhs::sizeRows_) != UnknownSize)
                                               &&  (int(Lhs::sizeRows_) != int(Rhs::sizeRows_))
                                               &&  (isRes2D_)
                                                 ));
      if ((lhs.rows() != rhs.rows()) && (isRes2D_))
      { STKRUNTIME_ERROR_2ARG(BinaryOperator, lhs.rows(), rhs.rows(), Rows sizes mismatch for 2D array);}
      if (( lhs.cols() != rhs.cols()) && (isRes2D_))
      { STKRUNTIME_ERROR_2ARG(BinaryOperator, lhs.cols(), rhs.cols(), Columns sizes mismatch for 2D array);}
    }
    /**  @return the range of the rows */
    inline Range const rows() const { return lhs_.rows();}
    /** @return the first index of the rows */
    inline int beginRowsImpl() const { return(lhs_.beginRows());}
    /** @return the ending index of the rows */
    inline int endRowsImpl() const { return(lhs_.endRows());}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeRowsImpl() const { return((sizeRows_ != UnknownSize) ? sizeRows_ : rhs_.sizeRows());}

    /** @return the range of the columns */
    inline Range const cols() const { return lhs_.cols();}
    /** @return the first index of the columns */
    inline int beginColsImpl() const { return(lhs_.beginCols());}
    /** @return the ending index of the columns */
    inline int endColsImpl() const { return(lhs_.endCols());}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeColsImpl() const { return((sizeCols_ != UnknownSize) ? sizeCols_ : rhs_.sizeCols());}

    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the functor representing the binary operation */
    inline BinaryOp const& functor() const { return functor_; }

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;
    BinaryOp const functor_;
};

/** @ingroup Arrays
  * @brief specialization in the number-number case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase<BinaryOp, Lhs, Rhs, Arrays::binOp0d_>
      : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element */
    inline typename BinaryOp::result_type const elt0Impl() const
    { return this->asDerived().functor()( this->asDerived().lhs().elt(), this->asDerived().rhs().elt());}
};

/** @ingroup Arrays
  * @brief specialization in the diagonal-vector case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase<BinaryOp, Lhs, Rhs, Arrays::binOp1d_>
      : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    /** constructor. */
    inline BinaryOperatorBase() : Base()
    {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt1Impl(int i) const
    { return this->asDerived().functor()( this->asDerived().lhs().elt(i), this->asDerived().rhs().elt(i));}
    /**  @return the range */
    inline Range const range() const
    { return (Lhs::sizeCols_==UnknownSize) ? this->asDerived().rhs().range() : this->asDerived().lhs().range();}
};

/** @ingroup Arrays
  * @class BinaryOperatorBase
 *  @brief implement the access to the elements in the (2D) general case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOp2d_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::result_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** access to the element i, j */
    inline Type const elt2Impl(int i, int j) const
    { return this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i,j));}
};

/** @ingroup Arrays
  * @brief specialization in the diagonal-* case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpDiag2d_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param1_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i), this->asDerived().rhs().elt(i, j))
                    : this->asDerived().functor()( Type(), this->asDerived().rhs().elt(i, j));}
};

/** @ingroup Arrays
  * @brief specialization in the *-diagonal case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOp2dDiag_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i))
                    : this->asDerived().functor()( this->asDerived().lhs().elt(i, j), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the *-upper_triangular_ case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOp2dUp_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i<=j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i,j))
                    : this->asDerived().functor()( this->asDerived().lhs().elt(i,j), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the upper_triangular_-* case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpUp2d_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i<=j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i,j))
                    : this->asDerived().functor()( Type(), this->asDerived().rhs().elt(i,j));}
};

/** @ingroup Arrays
  * @brief specialization in the *-lower_triangular_ case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOp2dLow_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i>=j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i,j))
                    : this->asDerived().functor()( this->asDerived().lhs().elt(i,j), Type(0));}
};

/** @ingroup Arrays
  * @brief specialization in the lower_triangular_-* case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpLow2d_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i>=j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i,j))
                    : this->asDerived().functor()( Type(0), this->asDerived().rhs().elt(i,j));}
};

/** @ingroup Arrays
  * @brief specialization in the diagonal-upper_triangular_ case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpDiagUp_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param1_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i), this->asDerived().rhs().elt(i, j))
                    : (i<j) ? this->asDerived().functor()( Type(), this->asDerived().rhs().elt(i, j))
                            : this->asDerived().functor()( Type(), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the upper_triangular_-diagonal case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpUpDiag_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i))
                    : (i<j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i, j), Type(0))
                            : this->asDerived().functor()( Type(), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the diagonal-lower_triangular_ case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpDiagLow_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param1_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i), this->asDerived().rhs().elt(i, j))
                    : (i>j) ? this->asDerived().functor()( Type(), this->asDerived().rhs().elt(i, j))
                            : this->asDerived().functor()( Type(), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the lower_triangular_-diagonal case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpLowDiag_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i))
                    : (i>j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i, j), Type(0))
                            : this->asDerived().functor()( Type(), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the upper_triangular_-upper_triangular_ case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpUpUp_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return  (i<=j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i, j), this->asDerived().rhs().elt(i, j))
                    : this->asDerived().functor()( Type(), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the lower_triangular_-lower_triangular_ case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpLowLow_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return  (i>=j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i, j), this->asDerived().rhs().elt(i, j))
                    : this->asDerived().functor()( Type(), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the upper_triangular_-lower_triangular_ case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpUpLow_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return  (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i, j), this->asDerived().rhs().elt(i, j))
                     : (i<j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i, j), Type(0))
                             : this->asDerived().functor()( Type(), this->asDerived().rhs().elt(i, j));}
};

/** @ingroup Arrays
  * @brief specialization in the lower_triangular_-upper_triangular_ case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase< BinaryOp, Lhs, Rhs, Arrays::binOpLowUp_>
                        : public ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ExprBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt2Impl(int i, int j) const
    { return  (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i, j), this->asDerived().rhs().elt(i, j))
                     : (i<j) ? this->asDerived().functor()( Type(), this->asDerived().rhs().elt(i, j))
                             : this->asDerived().functor()( this->asDerived().lhs().elt(i, j), Type(0));}
};

} // namespace STK

#undef EGAL

#endif /* STK_BINARYOPERATORS_H */
