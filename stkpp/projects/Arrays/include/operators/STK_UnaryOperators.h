
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

/** @file STK_UnaryOperators.h
 *  @brief In this file we implement the UnaryOperator class.
 **/

#ifndef STK_UNARYOPERATORS_H
#define STK_UNARYOPERATORS_H

#include "STK_SliceOperators.h"

namespace STK
{

// forward declaration
template<typename UnaryOp, typename Lhs> class UnaryOperator;

namespace hidden
{

/** @ingroup hidden
 *  @brief Traits class for the unary operators
 */
template<typename UnaryOp, typename Lhs>
struct Traits< UnaryOperator <UnaryOp, Lhs> >
{
  typedef typename UnaryOp::result_type Type;
  typedef RowOperator<UnaryOperator <UnaryOp, Lhs> > Row;
  typedef ColOperator<UnaryOperator <UnaryOp, Lhs> > Col;

  enum
  {
      structure_ = Lhs::structure_,
      orient_    = Lhs::orient_,
      sizeRows_  = Lhs::sizeRows_,
      sizeCols_  = Lhs::sizeCols_,
      storage_   = Lhs::storage_
  };
};

} // end namespace hidden

// forward declaration
template<typename UnaryOp, typename Lhs>
class UnaryOperatorBase;


/** @class UnaryOperator
  * @ingroup Arrays
  *
  * \brief Generic expression when unary operator is applied to an expression
  *
  * @tparam UnaryOp template functor implementing the operator
  * @tparam Lhs the type of the expression to which we are applying the unary operator
  *
  * This class represents an expression where a unary operator is applied to
  * an expression. It is the return type of all operations taking exactly 1
  * input expression, regardless of the presence of other inputs such as
  * numbers. For example, the operator* in the expression 3*matrix is
  * considered unary, because only the right-hand side is an expression, and its
  * return type is a specialization of UnaryOperator.
  *
  * Most of the time, this is the only way that it is used, so you typically
  * don't have to name UnaryOperator types explicitly.
  */
template<typename UnaryOp,  typename Lhs>
class UnaryOperator  : public UnaryOperatorBase< UnaryOp, Lhs >, public TRef<1>
{
  public:
    typedef UnaryOperatorBase< UnaryOp, Lhs > Base;
    typedef typename hidden::Traits< UnaryOperator >::Type Type;
    typedef typename hidden::Traits< UnaryOperator >::Row Row;
    typedef typename hidden::Traits< UnaryOperator >::Col Col;
    enum
    {
        structure_ = hidden::Traits< UnaryOperator >::structure_,
        orient_    = hidden::Traits< UnaryOperator >::orient_,
        sizeRows_  = hidden::Traits< UnaryOperator >::sizeRows_,
        sizeCols_  = hidden::Traits< UnaryOperator >::sizeCols_,
        storage_   = hidden::Traits< UnaryOperator >::storage_
    };
    inline UnaryOperator( Lhs const& rhs, UnaryOp const& functor = UnaryOp())
                        : Base(), rhs_(rhs), functor_(functor)
    {}
    /**  @return the range of the rows */
    inline Range const rows() const { return rhs_.rows();}
    /** @return the range of the Columns */
    inline Range const cols() const { return rhs_.cols();}
    /** @return the size of the vector */
    inline int const sizeRowsImpl() const { return rhs_.sizeRows();}
    /** @return the fixed size type if available to enable compile time optimizations */
    inline int const sizeColsImpl() const { return rhs_.sizeCols();}

    /** @return the right hand side expression */
    inline Lhs const& rhs() const { return rhs_; }
    /** @return the functor representing the unary operation */
    inline UnaryOp const& functor() const { return functor_; }

  protected:
    Lhs const& rhs_;
    UnaryOp const functor_;
};

/** @ingroup Arrays
  * @brief implement the access to the elements in the (2D) general case.
  **/
template<typename UnaryOp, typename Lhs>
class UnaryOperatorBase : public ExprBase< UnaryOperator<UnaryOp, Lhs> >
{
  public:
    typedef ExprBase< UnaryOperator<UnaryOp, Lhs> > Base;
    typedef typename UnaryOp::result_type Type;
    /** constructor. */
    inline UnaryOperatorBase() : Base() {}
    /** @return the element (i,j) of the operator.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const
    { return this->asDerived().functor()(this->asDerived().rhs().elt(i, j));}
    /** @return the element ith element of the operator
     *  @param i index of the ith element
     **/
    inline Type const elt1Impl(int i) const
    { return this->asDerived().functor()(this->asDerived().rhs().elt(i));}
    /** accesses to the element of the operator */
    inline Type const elt0Impl() const
    { return this->asDerived().functor()(this->asDerived().rhs().elt());}
};

} // namespace STK

#endif /* STK_UNARYOPERATORS_H */
