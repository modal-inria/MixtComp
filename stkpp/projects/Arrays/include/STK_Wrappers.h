/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014  Serge Iovleff

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
 * created on: 25 juil. 2014
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Wrappers.h
 *  @brief In this file we define the wrapping classes for expressions and arrays.
 **/

#ifndef STK_WRAPPERS_H
#define STK_WRAPPERS_H

#include "STK_ExprBaseVisitor.h"
#include "STK_ExprBaseDot.h"
#include "STK_ExprBaseProduct.h"

#include "STK_ArrayBaseApplier.h"
#include "STK_ArrayBaseAssign.h"
#include "STK_ArrayBaseInitializer.h"

namespace STK
{

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for the Wrapper class.
 **/
template<class WrappedExpr>
struct Traits< ExprWrapper<WrappedExpr> >
{
  public:
    typedef typename WrappedExpr::Type Type;
    typedef typename WrappedExpr::Row Row;
    typedef typename WrappedExpr::Col Col;

    enum
    {
      structure_ = WrappedExpr::structure_,
      orient_    = WrappedExpr::orient_,
      sizeRows_  = WrappedExpr::sizeRows_,
      sizeCols_  = WrappedExpr::sizeCols_,
      storage_   = WrappedExpr::storage_
    };
};

} // namespace hidden


/** @ingroup Arrays
 *
 * @brief A wrapper of an existing expression.
 *
 * @tparam WrappedExpr the equivalent expression wrapped
 *
 * This class represents a matrix or vector expression mapping an existing
 * expression. It can be used to let STK++ interface without any overhead with
 * non-stk expressions, such as templated expression from other libraries.
 *
 * The pseudo virtual function to implement by the wrapped expression
 * in the derived classes have the following definitions:
 * @code
 *   Range cols() const;
 *   int beginColsImpl() const;
 *   int endColsImpl() const;
 *   int sizeColsImpl() const;
 *   Range rows() const;
 *   int beginRowsImpl() const;
 *   int endRowsImpl() const;
 *   int sizeRowsImpl() const;
 * @endcode
 * and for the accessors the following definitions:
 * @code
 *   Type const elt2Impl( int i, int j) const;
 *   Type& elt2Impl( int i, int j);
 * @endcode
 * for the 2D arrays (@ref Arrays::array2D_, @ref Arrays::square_, @ref Arrays::upper_triangular_
 * and @ref Arrays::lower_triangular_),
 * @code
 *   Type const elt1Impl( int i) const;
 *   Type& elt1Impl( int i);
 * @endcode
 * for the diagonal arrays (@ref Arrays::diagonal_) and the 1D arrays
 * (@ref Arrays::vector_, @ref Arrays::point_)
 * and
 * @code
 *   Type const elt0Impl() const;
 *   Type& elt0Impl();
 * @endcode
 * for the @ref Arrays::number_ arrays.
 **/
template<class WrappedExpr>
class ExprWrapper : public ExprBase<WrappedExpr>
{
  public:
    typedef ExprBase<WrappedExpr> Base;
    typedef typename hidden::Traits<WrappedExpr>::Type Type;

    enum
    {
      structure_ = hidden::Traits<WrappedExpr>::structure_,
      orient_    = hidden::Traits<WrappedExpr>::orient_,
      sizeRows_  = hidden::Traits<WrappedExpr>::sizeRows_,
      sizeCols_  = hidden::Traits<WrappedExpr>::sizeCols_,
      storage_   = hidden::Traits<WrappedExpr>::storage_
    };

    /** Default constructor */
    inline ExprWrapper(WrappedExpr const& wrapped) : Base(), wrapped_(wrap) {}
    /** destructor */
    inline ~ExprWrapper() {}

    inline WrappedExpr const& wrapped() const { return wrapped_;}

    /** @return the Vertical range */
    inline Range rows() const { return wrapped_.rows();}
    /** @return the index of the first row */
    inline int beginRowsImpl() const { return wrapped_.beginRowsImpl();}
    /** @return the ending index of the rows */
    inline int endRowsImpl() const { return wrapped_.endRowsImpl();}
    /** @return the number of rows */
    inline int sizeRowsImpl() const { return wrapped_.sizeRowsImpl();}

    /**@return the Horizontal range */
    inline Range cols() const { return wrapped_.cols();}
    /** @return the index of the first column */
    inline int beginColsImpl() const { return wrapped_.beginColsImpl();}
    /**  @return the ending index of columns */
    inline int endColsImpl() const { return wrapped_.endColsImpl();}
    /** @return the number of columns */
    inline int sizeColsImpl() const { return wrapped_.sizeColsImpl();}

    /** @return the element (i,j) of the operator.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const { return (wrapped_.elt(i, j));}
    /** @return the ith element of the operator
     *  @param i index of the ith element
     **/
    inline Type const elt1Impl(int i) const { return (wrapped_.elt(i));}
    /** @return the element of the operator */
    inline Type const elt0Impl() const { return (wrapped_.elt());}

  private:
    WrappedExpr const& wrapped_;
};


namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for the Wrapper class.
 **/
template<class WrappedArray>
struct Traits< ArrayWrapper<WrappedArray> >
{
  public:
    typedef typename WrappedArray::Type Type;
    typedef typename WrappedArray::Row Row;
    typedef typename WrappedArray::Col Col;

    enum
    {
      structure_ = WrappedArray::structure_,
      orient_    = WrappedArray::orient_,
      sizeRows_  = WrappedArray::sizeRows_,
      sizeCols_  = WrappedArray::sizeCols_,
      storage_   = WrappedArray::storage_
    };
};

} // namespace hidden

/** @ingroup Arrays
 *
 * @brief A wrapper of an existing array of data.
 *
 * @tparam WrappedArray the equivalent array wrapped
 *
 * This class represents a matrix or vector wrapping an existing array of data.
 * It can be used to let STK++ interface without any overhead with non-stk data
 * structures, such as plain C arrays or structures from other libraries.
 **/
template<class WrappedArray>
class ArrayWrapper : public ArrayBase<WrappedArray>
{
  public:
    typedef ArrayBase<WrappedArray> Base;
    typedef typename hidden::Traits<WrappedArray>::Type Type;

    enum
    {
      structure_ = hidden::Traits<WrappedArray>::structure_,
      orient_    = hidden::Traits<WrappedArray>::orient_,
      sizeRows_  = hidden::Traits<WrappedArray>::sizeRows_,
      sizeCols_  = hidden::Traits<WrappedArray>::sizeCols_,
      storage_   = hidden::Traits<WrappedArray>::storage_
    };

    /** Default constructor */
    inline ArrayWrapper(ArrayWrapper& wrapped) : Base(), wrapped_(wrapped) {}
    /** destructor */
    inline ~ArrayWrapper() {}

    inline WrappedArray& wrapped() { return wrapped_;}

    /** @return the Vertical range */
    inline Range rows() const { return wrapped_.rows();}
    /** @return the index of the first row */
    inline int beginRowsImpl() const { return wrapped_.beginRowsImpl();}
    /** @return the ending index of the rows */
    inline int endRowsImpl() const { return wrapped_.endRowsImpl();}
    /** @return the number of rows */
    inline int sizeRowsImpl() const { return wrapped_.sizeRowsImpl();}

    /**@return the Horizontal range */
    inline Range cols() const { return wrapped_.cols();}
    /** @return the index of the first column */
    inline int beginColsImpl() const { return wrapped_.beginColsImpl();}
    /**  @return the ending index of columns */
    inline int endColsImpl() const { return wrapped_.endColsImpl();}
    /** @return the number of columns */
    inline int sizeColsImpl() const { return wrapped_.sizeColsImpl();}

    /** @return the element (i,j) expression.
     *  @param i, j indexes of the row and column
     **/
    inline Type const elt2Impl(int i, int j) const { return (wrapped_.elt(i, j));}
    /** @return the element (i,j) of the operator.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type& elt2Impl(int i, int j) { return (wrapped_.elt(i, j));}
    /** @return the ith element of the array
     *  @param i index of the ith element
     **/
    inline Type const elt1Impl(int i) const { return (wrapped_.elt(i));}
    /** @return the ith element of the array
     *  @param i index of the ith element
     **/
    inline Type& elt1Impl(int i) { return (wrapped_.elt(i));}
    /** @return the element of the array */
    inline Type const elt0Impl() const { return (wrapped_.elt());}
    /** @return the element of the array */
    inline Type& elt0Impl() { return (wrapped_.elt());}

  private:
    WrappedArray& wrapped_;
};


} // namespace STK

#endif /* STK_EXPRWRAPPER_H */
