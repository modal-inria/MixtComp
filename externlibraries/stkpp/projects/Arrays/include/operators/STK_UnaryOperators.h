
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
 * Project:  stkpp::
 * created on: 17 oct. 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_UnaryOperators.h
 *  @brief In this file we implement the UnaryOperator class.
 **/

#ifndef STK_UNARYOPERATORS_H
#define STK_UNARYOPERATORS_H

namespace STK
{

template<typename UnaryOp, typename Rhs>
class UnaryOperator;

namespace hidden
{

/** @ingroup hidden
 *  @brief Traits class for the unary operators
 */
template<typename UnaryOp, typename Rhs>
struct Traits< UnaryOperator <UnaryOp, Rhs> >
{
  typedef typename UnaryOp::result_type Type;
  enum
  {
      structure_ = Rhs::structure_,
      orient_    = Rhs::orient_,
      sizeRows_  = Rhs::sizeRows_,
      sizeCols_  = Rhs::sizeCols_,
      storage_   = Rhs::storage_
  };
};

} // end namespace hidden

// forward declaration
template<typename UnaryOp, typename Rhs>
class UnaryOperatorBase;


/** @class UnaryOperator
  * @ingroup Arrays
  *
  * \brief Generic expression when unary operator is applied to an expression
  *
  * @tparam UnaryOp template functor implementing the operator
  * @tparam Rhs the type of the expression to which we are applying the unary operator
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
template<typename UnaryOp,  typename Rhs>
class UnaryOperator  : public UnaryOperatorBase< UnaryOp, Rhs >
, public TRef<1>
{
  public:
    typedef UnaryOperator<UnaryOp, Rhs> Derived;
    typedef UnaryOperatorBase< UnaryOp, Rhs > Base;
    typedef typename hidden::Traits<Derived>::Type Type;
    enum
    {
        structure_ = hidden::Traits<Derived>::structure_,
        orient_    = hidden::Traits<Derived>::orient_,
        sizeRows_  = hidden::Traits<Derived>::sizeRows_,
        sizeCols_  = hidden::Traits<Derived>::sizeCols_,
        storage_   = hidden::Traits<Derived>::storage_
    };
    inline UnaryOperator( Rhs const& rhs, UnaryOp const& functor = UnaryOp())
                        : Base(), rhs_(rhs), functor_(functor)
    {}
    /**  @return the range of the rows */
    inline Range const rows() const { return rhs_.rows();}
    /** @return the range of the Columns */
    inline Range const cols() const { return rhs_.cols();}
    /** @return the right hand side expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the functor representing the unary operation */
    inline UnaryOp const& functor() const { return functor_; }

  protected:
    Rhs const& rhs_;
    UnaryOp const functor_;
};

/** @ingroup Arrays
  * @brief implement the access to the elements in the (2D) general case.
  **/
template<typename UnaryOp, typename Rhs>
class UnaryOperatorBase : public ArrayBase< UnaryOperator<UnaryOp, Rhs> >
{
  public:
    typedef ArrayBase< UnaryOperator<UnaryOp, Rhs> > Base;
    typedef typename UnaryOp::result_type Type;
    /** constructor. */
    inline UnaryOperatorBase() : Base() {}
    /** @return the element (i,j) of the operator.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt(int i, int j) const
    { return this->asDerived().functor()(this->asDerived().rhs().elt(i, j));}
    /** @return the element (i,j) of the operator.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const operator()(int i, int j) const
    { return elt(i,j);}
    /** @return the element ith element of the operator
     *  @param i index of the ith element
     **/
    inline Type const elt(int i) const
    { return this->asDerived().functor()(this->asDerived().rhs().elt(i));}
    /** @return the element ith element of the operator
     *  @param i index of the ith element
     **/
    inline Type const operator[](int const& i) const { return elt(i);}
    /** accesses to the element of the operator */
    inline Type const elt() const
    { return this->asDerived().functor()(this->asDerived().rhs().elt());}
    /** @return the element of the operator */
    inline Type const operator()() const { return elt();}
};

} // namespace STK

#endif /* STK_UNARYOPERATORS_H */
