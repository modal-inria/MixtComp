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

/** @file STK_DotOperators.h
 *  @brief In this file we implement the DotOperator class.
 **/

#ifndef STK_DOTOPERATOR_H
#define STK_DOTOPERATOR_H

#include "../STK_CAllocator.h"

namespace STK
{


template<typename Lhs, typename Rhs> class DotProduct;

namespace hidden
{

/** @ingroup hidden
 *  @brief Traits class for the DotProduct class
 */
template< typename Lhs, typename Rhs>
struct Traits< DotProduct < Lhs, Rhs> >
{
  enum
  {
    structure_ = Arrays::number_,
    orient_    = Arrays::by_col_,
    sizeRows_  = 1,
    sizeCols_  = 1,
    storage_   = Arrays::dense_
  };
  typedef typename hidden::Promote< typename Lhs::Type, typename Rhs::Type>::result_type Type;
  typedef CAllocator<Type, (Arrays::Structure)structure_, sizeRows_, sizeCols_, (Arrays::Orientation)orient_> Allocator;
};

} // end namespace hidden


/** @ingroup Arrays
  *
  * @brief Generic expression where a dot operator is
  * applied to two expressions.
  *
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class represents an expression  where a product operator is applied to
  * two expressions. The left hand side being a point_ (a row-oriented vector)
  * and the right hand side a vector_ (a column-oriented vector).
  *
  * Most of the time, this is the only way that it is used, so you typically
  * don't have to name DotProduct types explicitly.
  **/
template<typename Lhs, typename Rhs>
class DotProduct : public ExprBase< DotProduct<Lhs, Rhs> >
                 , public TRef<1>
{
  public:
    typedef typename hidden::Traits<DotProduct>::Type Type;
    typedef typename hidden::Traits<DotProduct>::Allocator Allocator;

    enum
    {
      structure_ = hidden::Traits<DotProduct>::structure_,
      orient_    = hidden::Traits<DotProduct>::orient_,
      sizeRows_  = hidden::Traits<DotProduct>::sizeRows_,
      sizeCols_  = hidden::Traits<DotProduct>::sizeCols_,
      storage_   = hidden::Traits<DotProduct>::storage_
    };

    inline DotProduct( const Lhs& lhs, const Rhs& rhs)
                     : lhs_(lhs), rhs_(rhs)
                     , result_()
    {
      if (lhs.cols() != rhs.rows())
      { STKRUNTIME_ERROR_NO_ARG(DotProduct, sizes mismatch for 2D array);}
      result_.shift(lhs.beginRows(), rhs.beginCols());
      result_.elt() = lhs.dot(rhs);
    }
    /**  @return the range of the rows */
    inline Range rows() const { return result_.rows();}
    /** @return the range of the columns */
    inline Range cols() const { return result_.cols();}
    /** @return the number of rows */
    inline int sizeRowsImpl() const { return result_.sizeRows();}
    /** @return the number of columns */
    inline int sizeColsImpl() const { return result_.sizeCols();}
    /** @return the left hand side expression */
    inline Lhs const& lhs() const { return lhs_; }
    /** @return the right hand side nested expression */
    inline Rhs const& rhs() const { return rhs_; }
    /** @return the result */
    inline Allocator const& result() const { return result_; }
    /** access to the element */
    inline Type const elt0Impl() const { return result_.elt();}

  protected:
    Lhs const& lhs_;
    Rhs const& rhs_;

  private:
    Allocator result_;
};

}  // namespace STK

#endif /* STK_DOTOPERATOR_H */
