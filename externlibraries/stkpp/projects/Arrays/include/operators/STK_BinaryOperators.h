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
 * created on: 17 oct. 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_BinaryOperators.h
 *  @brief In this file we implement the BinaryOperator class.
 **/


#ifndef STK_BINARYOPERATORS_H
#define STK_BINARYOPERATORS_H

#define EGAL(arg1, arg2) ((arg1::structure_ == int(Arrays::arg2)))

namespace STK
{

namespace hidden
{
/** @ingroup hidden
 *  @brief Helper class to get the correct returned Structure of a binary operator
 **/
template<int LStructure_, int RStructure_> struct BinaryReturnStructure;

template<> struct BinaryReturnStructure<Arrays::general_, Arrays::general_>
{ enum { structure_ = Arrays::general_};};
template<> struct BinaryReturnStructure<Arrays::general_, Arrays::square_>
{ enum { structure_ = Arrays::square_};};
template<> struct BinaryReturnStructure<Arrays::general_, Arrays::diagonal_>
{ enum { structure_ = Arrays::general_};};

template<> struct BinaryReturnStructure<Arrays::square_, Arrays::general_>
{ enum { structure_ = Arrays::square_};};
template<> struct BinaryReturnStructure<Arrays::square_, Arrays::square_>
{ enum { structure_ = Arrays::square_};};
template<> struct BinaryReturnStructure<Arrays::square_, Arrays::diagonal_>
{ enum { structure_ = Arrays::square_};};

template<> struct BinaryReturnStructure<Arrays::lower_triangular_, Arrays::lower_triangular_>
{ enum { structure_ = Arrays::lower_triangular_};};
template<> struct BinaryReturnStructure<Arrays::lower_triangular_, Arrays::diagonal_>
{ enum { structure_ = Arrays::lower_triangular_};};

template<> struct BinaryReturnStructure<Arrays::upper_triangular_, Arrays::upper_triangular_>
{ enum { structure_ = Arrays::upper_triangular_};};
template<> struct BinaryReturnStructure<Arrays::upper_triangular_, Arrays::diagonal_>
{ enum { structure_ = Arrays::upper_triangular_};};

template<> struct BinaryReturnStructure<Arrays::diagonal_, Arrays::general_>
{ enum { structure_ = Arrays::general_};};
template<> struct BinaryReturnStructure<Arrays::diagonal_, Arrays::square_>
{ enum { structure_ = Arrays::square_};};
template<> struct BinaryReturnStructure<Arrays::diagonal_, Arrays::diagonal_>
{ enum { structure_ = Arrays::diagonal_};};
template<> struct BinaryReturnStructure<Arrays::diagonal_, Arrays::point_>
{ enum { structure_ = Arrays::diagonal_};};
template<> struct BinaryReturnStructure<Arrays::diagonal_, Arrays::vector_>
{ enum { structure_ = Arrays::diagonal_};};

template<> struct BinaryReturnStructure<Arrays::vector_, Arrays::diagonal_>
{ enum { structure_ = Arrays::diagonal_};};
template<> struct BinaryReturnStructure<Arrays::vector_, Arrays::vector_>
{ enum { structure_ = Arrays::vector_};};
template<> struct BinaryReturnStructure<Arrays::vector_, Arrays::point_>
{ enum { structure_ = Arrays::vector_};};

template<> struct BinaryReturnStructure<Arrays::point_, Arrays::diagonal_>
{ enum { structure_ = Arrays::diagonal_};};
template<> struct BinaryReturnStructure<Arrays::point_, Arrays::vector_>
{ enum { structure_ = Arrays::point_};};
template<> struct BinaryReturnStructure<Arrays::point_, Arrays::point_>
{ enum { structure_ = Arrays::point_};};

template<> struct BinaryReturnStructure<Arrays::number_, Arrays::number_>
{ enum { structure_ = Arrays::number_};};

} //namespace hidden


template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperator;

namespace hidden {

/** @ingroup hidden
 *  @brief Traits class for the BinaryOperator
 */
template<typename BinaryOp, typename Lhs, typename Rhs>
struct Traits< BinaryOperator <BinaryOp, Lhs, Rhs> >
{
  enum
  {
    // find the Structure using helper class BinaryReturnStructure
    structure_ = hidden::BinaryReturnStructure<Lhs::structure_, Rhs::structure_>::structure_,
    // preserve the Lhs storage orientation
    orient_    = Lhs::orient_,
    sizeRows_  = Lhs::sizeRows_,
    sizeCols_  = Lhs::sizeCols_,
    storage_   = (int(Lhs::storage_) == int(Arrays::dense_)) || (int(Rhs::storage_) == int(Arrays::dense_))
               ?  int(Arrays::dense_) : int(Arrays::sparse_),
    is0D_      = ( ( hidden::Traits<Lhs>::structure_ == int(Arrays::number_))
                 )&&
                 ( ( hidden::Traits<Rhs>::structure_ == int(Arrays::number_))
                 ),
    is1D_      = ( ( hidden::Traits<Lhs>::structure_ == int(Arrays::diagonal_))
                 ||( hidden::Traits<Lhs>::structure_ == int(Arrays::vector_))
                 ||( hidden::Traits<Lhs>::structure_ == int(Arrays::point_))
                 )&&
                 ( ( hidden::Traits<Rhs>::structure_ == int(Arrays::diagonal_))
                 ||( hidden::Traits<Rhs>::structure_ == int(Arrays::vector_))
                 ||( hidden::Traits<Rhs>::structure_ == int(Arrays::point_))
                 ),
    is2D_      = ( ( hidden::Traits<Lhs>::structure_ == int(Arrays::general_))
                  ||( hidden::Traits<Lhs>::structure_ == int(Arrays::square_))
                  ||( hidden::Traits<Lhs>::structure_ == int(Arrays::lower_triangular_))
                  ||( hidden::Traits<Lhs>::structure_ == int(Arrays::upper_triangular_))
                  )&&
                  ( ( hidden::Traits<Rhs>::structure_ == int(Arrays::general_))
                  ||( hidden::Traits<Rhs>::structure_ == int(Arrays::square_))
                  ||( hidden::Traits<Rhs>::structure_ == int(Arrays::lower_triangular_))
                  ||( hidden::Traits<Rhs>::structure_ == int(Arrays::upper_triangular_))
                  ),
    is2D1D_    =  ( ( hidden::Traits<Lhs>::structure_ == int(Arrays::general_))
                  ||( hidden::Traits<Lhs>::structure_ == int(Arrays::square_))
                  ||( hidden::Traits<Lhs>::structure_ == int(Arrays::lower_triangular_))
                  ||( hidden::Traits<Lhs>::structure_ == int(Arrays::upper_triangular_))
                  )&&
                  ( ( hidden::Traits<Rhs>::structure_ == int(Arrays::diagonal_))
                  ),
    is1D2D_     = ( ( hidden::Traits<Lhs>::structure_ == int(Arrays::diagonal_))
                  )&&
                  ( ( hidden::Traits<Rhs>::structure_ == int(Arrays::general_))
                  ||( hidden::Traits<Rhs>::structure_ == int(Arrays::square_))
                  ||( hidden::Traits<Rhs>::structure_ == int(Arrays::lower_triangular_))
                  ||( hidden::Traits<Rhs>::structure_ == int(Arrays::upper_triangular_))
                  ),
    binaryDim_ = is0D_ ? Arrays::bin0D_
                       : is1D_ ? Arrays::bin1D_
                               : is2D_ ? Arrays::bin2D_
                                       : is2D1D_ ? Arrays::bin2D1D_
                                                 : Arrays::bin1D2D_
  };
  // handle the case when the type is different in LHS and Rhs.
  typedef typename BinaryOp::result_type Type;
};

} // end namespace hidden


// forward declaration
template< typename BinaryOp, typename Lhs, typename Rhs
        , int dim = hidden::Traits< BinaryOperator <BinaryOp, Lhs, Rhs> >::binaryDim_ >
class BinaryOperatorBase;

/** @class BinaryOperator
  * @ingroup Arrays
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
  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;
    BinaryOp const functor_;

  public:
    typedef BinaryOperatorBase<BinaryOp, Lhs, Rhs> Base;

    enum
    {
      // All the valid cases for binary operators
     isValid_ =(  (EGAL(Lhs,number_) && EGAL(Rhs,number_) )
               || (EGAL(Lhs,lower_triangular_) && (EGAL(Rhs,lower_triangular_) || EGAL(Rhs,diagonal_)) )
               || (EGAL(Lhs,upper_triangular_) && (EGAL(Rhs,upper_triangular_) || EGAL(Rhs,diagonal_)) )
               || (EGAL(Lhs,general_)  && (EGAL(Rhs,general_)  || EGAL(Rhs,square_) || EGAL(Rhs,diagonal_)) )
               || (EGAL(Lhs,square_)   && (EGAL(Rhs,general_)  || EGAL(Rhs,square_) || EGAL(Rhs,diagonal_)) )
               || (EGAL(Lhs,vector_)   && (EGAL(Rhs,diagonal_) || EGAL(Rhs,vector_) || EGAL(Rhs,point_)) )
               || (EGAL(Lhs,point_)    && (EGAL(Rhs,diagonal_) || EGAL(Rhs,vector_) || EGAL(Rhs,point_)) )
               || (EGAL(Lhs,diagonal_) && (EGAL(Rhs,diagonal_) || EGAL(Rhs,vector_) || EGAL(Rhs,point_)) )
               ),
      isVector_  = hidden::Traits<BinaryOperator>::structure_== int(Arrays::vector_),
      isPoint_   = hidden::Traits<BinaryOperator>::structure_== int(Arrays::point_),
      is0D_      = hidden::Traits<BinaryOperator>::is0D_,
      is1D_      = hidden::Traits<BinaryOperator>::is2D_,
      is2D_      = hidden::Traits<BinaryOperator>::is2D_,
      is1D2D_      = hidden::Traits<BinaryOperator>::is2D_,
      is2D1D_      = hidden::Traits<BinaryOperator>::is2D_,
      structure_ = hidden::Traits<BinaryOperator>::structure_,
      orient_    = hidden::Traits<BinaryOperator>::orient_,
      sizeRows_  = hidden::Traits<BinaryOperator>::sizeRows_,
      sizeCols_  = hidden::Traits<BinaryOperator>::sizeCols_,
      storage_   = hidden::Traits<BinaryOperator>::storage_
    };
    inline BinaryOperator( const Lhs& lhs, const Rhs& rhs, const BinaryOp& func = BinaryOp())
                         : Base(), lhs_(lhs), rhs_(rhs), functor_(func)
    { // FIXME : not safe. Add more test at compile time (and runtime ?)
      STK_STATICASSERT_BINARY_OPERATOR_MISMATCH( isValid_ );
      STK_STATICASSERT_COLS_DIMENSIONS_MISMATCH(!( (int(Lhs::sizeCols_) != UnknownSize)
                                               &&  (int(Rhs::sizeCols_) != UnknownSize)
                                               &&  (int(Lhs::sizeCols_) != Rhs::sizeCols_)
                                               &&  (is2D_||is1D2D_||is2D1D_)
                                                 ));
      STK_STATICASSERT_ROWS_DIMENSIONS_MISMATCH(!( (int(Lhs::sizeRows_) != UnknownSize)
                                               &&  (int(Rhs::sizeRows_) != UnknownSize)
                                               &&  (int(Lhs::sizeRows_) != Rhs::sizeRows_)
                                               &&  (is2D_||is1D2D_||is2D1D_)
                                                 ));
      if ((lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) && (is2D_||is1D2D_||is2D1D_))
      { STKRUNTIME_ERROR_NO_ARG(BinaryOperator, sizes mismatch for 2D array);}
    }
    /**  @return the range of the rows */
    inline Range const rows() const
    { return (int(Lhs::sizeRows_)==UnknownSize) ? rhs_.rows() : lhs_.rows();}
    /** @return the range of the columns */
    inline Range const cols() const
    { return (int(Lhs::sizeCols_)==UnknownSize) ? rhs_.cols() : lhs_.cols();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeRows() const
    { return (int(Lhs::sizeRows_)==UnknownSize) ? rhs_.sizeRows() : lhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int sizeCols() const
    { return (int(Lhs::sizeCols_)==UnknownSize) ? rhs_.sizeCols() : lhs_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the functor representing the binary operation */
    inline BinaryOp const& functor() const { return functor_; }
};


/** @ingroup Arrays
  * @brief implement the access to the elements in the (2D) general case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase<BinaryOp, Lhs, Rhs, Arrays::bin2D_> : public ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::result_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** access to the element i, j */
    inline Type const elt(int i, int j) const
    { return this->asDerived().functor()( this->asDerived().lhs().elt(i, j), this->asDerived().rhs().elt(i, j));}
};

/** @ingroup Arrays
  * @brief specialization in the diagonal-* case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase<BinaryOp, Lhs, Rhs, Arrays::bin1D2D_>
      : public ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param1_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt(int i, int j) const
    { return (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i), this->asDerived().rhs().elt(i, j))
                    : this->asDerived().functor()( Type(), this->asDerived().rhs().elt(i, j));}
};

/** @ingroup Arrays
  * @brief specialization in the *-diagonal case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase<BinaryOp, Lhs, Rhs, Arrays::bin2D1D_>
      : public ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    typedef typename BinaryOp::param2_type Type;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt(int i, int j) const
    { return (i==j) ? this->asDerived().functor()( this->asDerived().lhs().elt(i,j), this->asDerived().rhs().elt(i))
                    : this->asDerived().functor()( this->asDerived().lhs().elt(i, j), Type());}
};

/** @ingroup Arrays
  * @brief specialization in the diagonal-vector case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase<BinaryOp, Lhs, Rhs, Arrays::bin1D_>
      : public ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    /** constructor. */
    inline BinaryOperatorBase() : Base()
    {}
    /** accesses to the element i, j */
    inline typename BinaryOp::result_type const elt(int i) const
    { return this->asDerived().functor()( this->asDerived().lhs().elt(i), this->asDerived().rhs().elt(i));}
    /**  @return the range */
    inline Range const range() const
    { return (Lhs::sizeCols_==UnknownSize) ? this->asDerived().rhs().range() : this->asDerived().lhs().range();}
};

/** @ingroup Arrays
  * @brief specialization in the number-number case.
  **/
template<typename BinaryOp, typename Lhs, typename Rhs>
class BinaryOperatorBase<BinaryOp, Lhs, Rhs, Arrays::bin0D_>
      : public ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> >
{
  public:
    typedef ArrayBase< BinaryOperator<BinaryOp, Lhs, Rhs> > Base;
    /** constructor. */
    inline BinaryOperatorBase() : Base() {}
    /** accesses to the element */
    inline typename BinaryOp::result_type const elt() const
    { return this->asDerived().functor()( this->asDerived().lhs().elt(), this->asDerived().rhs().elt());}
};

} // namespace STK

#undef EGAL

#endif /* STK_BINARYOPERATORS_H */
