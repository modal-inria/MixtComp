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

/** @file STK_DiagOperator.h
 *  @brief In this file we implement the DiagOperator class.
 **/

#ifndef STK_DIAGOPERATOR_H
#define STK_DIAGOPERATOR_H


#include "Sdk/include/STK_StaticAssert.h"
#include "STK_SliceOperators.h"

namespace STK
{

// forward declaration
template< typename Array> class DiagOperator;

namespace hidden
{
/** @ingroup hidden
 *  @brief Traits class for the diag operator
 */
template<typename Lhs>
struct Traits< DiagOperator <Lhs> >
{
  typedef RowOperator<DiagOperator < Lhs> > Row;
  typedef ColOperator<DiagOperator < Lhs> > Col;
  typedef typename Lhs::Type Type;
  enum
  {
    structure_ = Arrays::diagonal_,
    orient_    = Lhs::orient_,
    sizeRows_  = Lhs::sizeCols_,
    sizeCols_  = Lhs::sizeRows_,
    storage_   = Lhs::storage_
  };
};

} // end namespace hidden

// forward declaration
template<typename Lhs> class DiagOperatorBase;


/** @ingroup Arrays
 *  @class DiagOperator
  *
  * \brief Generic expression when a vector expression is "diagonalized".
  *
  * @tparam Lhs the type of the expression to which we are applying the
  * diagonalize operator.
  *
  * This class represents an expression where a diagonalize operator is applied to
  * a vector expression. It is the return type of the diagonalize operation.
  *
  * Most of the time, this is the only way that it is used, so you typically
  * don't have to name DiagOperator type explicitly.
  */
template< typename Lhs>
class DiagOperator  : public DiagOperatorBase< Lhs >, public TRef<1>
{
  public:
    typedef DiagOperatorBase< Lhs > Base;
    typedef typename hidden::Traits< DiagOperator<Lhs> >::Type Type;
    typedef typename hidden::Traits< DiagOperator<Lhs> >::Row Row;
    typedef typename hidden::Traits< DiagOperator<Lhs> >::Col Col;
    enum
    {
        structure_ = hidden::Traits< DiagOperator<Lhs> >::structure_,
        orient_    = hidden::Traits< DiagOperator<Lhs> >::orient_,
        sizeRows_  = hidden::Traits< DiagOperator<Lhs> >::sizeRows_,
        sizeCols_  = hidden::Traits< DiagOperator<Lhs> >::sizeCols_,
        storage_   = hidden::Traits< DiagOperator<Lhs> >::storage_
    };
    /** Constructor */
    inline DiagOperator( Lhs const& lhs) : Base(), lhs_(lhs)
    {
      STK_STATICASSERT_VECTOR_ONLY(Lhs);
    }
    /**  @return the range of the rows */
    inline Range const range() const { return lhs_.range();}
    /** @return the first index of the rows */
    inline int const beginImpl() const { return lhs_.begin();}
    /** @return the ending index of the rows */
    inline int const endImpl() const { return lhs_.end();}
    /** @return the number of rows */
    inline int const sizeImpl() const { return lhs_.size();}

    /**  @return the range of the rows */
    inline Range const rows() const { return lhs_.range();}
    /** @return the first index of the rows */
    inline int const beginRowsImpl() const { return lhs_.begin();}
    /** @return the ending index of the rows */
    inline int const endRowsImpl() const { return lhs_.end();}
    /** @return the number of rows */
    inline int const sizeRowsImpl() const { return lhs_.size();}

    /** @return the range of the Columns */
    inline Range const cols() const { return lhs_.range();}
    /** @return the first index of the columns */
    inline int const beginColsImpl() const { return lhs_.begin();}
    /** @return the ending index of the columns */
    inline int const endColsImpl() const { return lhs_.end();}
    /** @return the number of columns */
    inline int const sizeColsImpl() const { return lhs_.size();}

    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }

  protected:
    Lhs const& lhs_;
};

/** @ingroup Arrays
  * @brief implement the access to the elements in the (2D) general case.
  **/
template< typename Lhs>
class DiagOperatorBase : public ExprBase< DiagOperator< Lhs> >
{
  public:
    typedef typename hidden::Traits< DiagOperator<Lhs> >::Type Type;
    typedef ExprBase< DiagOperator< Lhs> > Base;
    /** constructor. */
    inline DiagOperatorBase() : Base() {}
    /** @return the element (i,j) of the transposed expression.
     *  @param i, j index of the row and of the column
     **/
    inline Type const elt2Impl(int i, int j) const
    { return (this->asDerived().lhs().elt(i, j));}
    /** @return the element ith element of the transposed expression
     *  @param i index of the ith element
     **/
    inline Type const elt1Impl(int i) const
    { return (this->asDerived().lhs().elt(i));}
    /** accesses to the element of the transposed expression */
    inline Type const elt0Impl() const
    { return (this->asDerived().lhs().elt());}
};

} // namespace STK

#endif /* STK_DIAGOPERATOR_H */
