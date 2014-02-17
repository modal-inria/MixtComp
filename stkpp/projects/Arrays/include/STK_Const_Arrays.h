/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Arrays
 * created on: 25 juil. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Const_Arrays.h
 *  @brief In this file we define the constant Arrays.
 **/


#ifndef STK_CONST_ARRAYS_H
#define STK_CONST_ARRAYS_H

#include "STK_ArrayBase.h"

namespace STK
{

// forward declaration
namespace Const
{
template< typename Type_, int Size_ = UnknownSize> class Identity;
template< typename Type_, int Size_ = UnknownSize> class Square;
template< typename Type_, int Size_ = UnknownSize> class Vector;
template< typename Type_, int Size_ = UnknownSize> class Point;
template< typename Type_, int SizeRows_ = UnknownSize, int SizeCols_ = UnknownSize>
class General;
template< typename Type_, int SizeRows_ = UnknownSize, int SizeCols_ = UnknownSize>
class UpperTriangular;
template< typename Type_, int SizeRows_ = UnknownSize, int SizeCols_ = UnknownSize>
class LowerTriangular;
}

namespace hidden
{
/** @ingroup hidden
 *  @brief Traits class for the identity constant matrix
 */
template< typename Type_, int Size_>
struct Traits< Const::Identity <Type_, Size_> >
{
  typedef Type_ Type;
  enum
  {
    structure_ = Arrays::diagonal_,
    sizeRows_  = Size_,
    sizeCols_  = Size_,
    orient_    = Arrays::by_col_,
    storage_   = Arrays::sparse_
  };
};
/** @ingroup hidden
 *  @brief Traits class for the identity constant matrix
 */
template< typename Type_, int Size_>
struct Traits< Const::Vector <Type_, Size_> >
{
  typedef Type_ Type;
  enum
  {
    structure_ = Arrays::vector_,
    sizeRows_  = Size_,
    sizeCols_  = 1,
    orient_    = Arrays::by_col_,
    storage_   = Arrays::sparse_
  };
};
/** @ingroup hidden
 *  @brief Traits class for the constant vector_
 */
template< typename Type_, int Size_>
struct Traits< Const::Point <Type_, Size_> >
{
  typedef Type_ Type;
  enum
  {
    structure_ = Arrays::point_,
    sizeRows_  = 1,
    sizeCols_  = Size_,
    orient_    = Arrays::by_row_,
    storage_   = Arrays::sparse_
  };
};
/** @ingroup hidden
 *  @brief Traits class for the square constant matrix
 */
template< typename Type_, int Size_>
struct Traits< Const::Square <Type_, Size_> >
{
  typedef Type_ Type;
  enum
  {
    structure_ = Arrays::square_,
    sizeRows_  = Size_,
    sizeCols_  = Size_,
    orient_    = Arrays::by_col_,
    storage_   = Arrays::sparse_
  };
};
/** @ingroup hidden
 *  @brief Traits class for the general constant matrix
 */
template< typename Type_, int SizeRows_, int SizeCols_>
struct Traits< Const::General<Type_, SizeRows_, SizeCols_> >
{
  typedef Type_ Type;
  enum
  {
    structure_ = Arrays::array2D_,
    sizeRows_  = SizeRows_,
    sizeCols_  = SizeCols_,
    orient_    = Arrays::by_col_,
    storage_   = Arrays::sparse_
  };
};
/** @ingroup hidden
 *  @brief Traits class for the upper triangular constant matrix
 */
template< typename Type_, int SizeRows_, int SizeCols_>
struct Traits< Const::UpperTriangular<Type_, SizeRows_, SizeCols_> >
{
  typedef Type_ Type;
  enum
  {
    structure_ = Arrays::upper_triangular_,
    sizeRows_  = SizeRows_,
    sizeCols_  = SizeCols_,
    orient_    = Arrays::by_col_,
    storage_   = Arrays::sparse_
  };
};
/** @ingroup hidden
 *  @brief Traits class for the lower triangular constant matrix
 */
template< typename Type_, int SizeRows_, int SizeCols_>
struct Traits< Const::LowerTriangular<Type_, SizeRows_, SizeCols_> >
{
  typedef Type_ Type;
  enum
  {
    structure_ = Arrays::lower_triangular_,
    sizeRows_  = SizeRows_,
    sizeCols_  = SizeCols_,
    orient_    = Arrays::by_col_,
    storage_   = Arrays::sparse_
  };
};

} // namespace hidden

namespace Const
{
/**@ingroup Arrays
 * Define the constant identity matrix
 * \f[
 * I_n =
 * \left(
 * \begin{array}{ccccc}
 *  1\\
 *   & 1 & & \text{\huge0}\\
 *   &   & \ddots \\
 *   & \text{\huge0} & & \ddots\\
 *   &               & &  & 1
 * \end{array}
 * \right).
 * \f]
 * The size can be either a fixed template argument or a dynamic size.
 * Exemple:
 * @code
 *  STK::Const::Square<Real,3> S3; // S3 is a 3x3 square matrix of 1.
 *  STK::Const::Square<Real> S(10); // S is a 10x10 identity matrix of 1.
 * @endcode
 * @tparam Size_ the size of the identity Matrix. Default is UnknownSize.
 **/
template< typename Type_, int Size_ >
class Identity : public ITArrayBase<Identity<Type_, Size_>, Size_, Size_ >
{
  public:
    typedef ITArrayBase<Identity<Type_, Size_>, Size_, Size_ > Base;
    typedef Type_ Type;
    enum
    {
      structure_ = hidden::Traits< Const::Identity <Type_, Size_> >::structure_,
      orient_    = hidden::Traits< Const::Identity <Type_, Size_> >::orient_,
      sizeRows_  = hidden::Traits< Const::Identity <Type_, Size_> >::sizeRows_,
      sizeCols_  = hidden::Traits< Const::Identity <Type_, Size_> >::sizeCols_,
      storage_   = hidden::Traits< Const::Identity <Type_, Size_> >::storage_
    };
    /** default constructor */
    Identity() : Base() {}
    /** constructor with specified dimension */
    Identity(Range I) : Base(I,I) {}
    /** @return the element (i,j) of the identity matrix.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const
    { return (i==j ? Type(1) : Type(0));}
    /** @return the element ith element of the identity matrix
     *  @param i index of the ith element
     **/
    inline Type const elt1Impl(int i) const { return Type(1);}
};

/**@ingroup Arrays
 * Define the constant square matrix
 * \f[
 * S =
 * \left(
 * \begin{array}{ccccc}
 *  1\\
 *   & 1 & & \text{\huge 1}\\
 *   &   & \ddots \\
 *   & \text{\huge 1} & & \ddots\\
 *   &               & &  & 1
 * \end{array}
 * \right).
 * \f]
 * The size can be either a fixed template argument or a dynamic size.
 * Exemple:
 * @code
 *  STK::Const::Square<Real,3> S3; // S3 is a 3x3 square matrix of Real
 *  STK::Const::Square<Real> S(10); // S is a 10x10 square matrix of Real
 * @endcode
 * @tparam Size_ the size of the square Matrix. Default is UnknownSize.
 **/
template< typename Type_, int Size_ >
class Square : public ITArrayBase<Square<Type_, Size_>, Size_, Size_ >
{
  public:
    typedef ITArrayBase<Square<Type_, Size_>, Size_, Size_ > Base;
    typedef typename hidden::Traits< Const::Square <Type_, Size_> >::Type Type;
    enum
    {
      structure_ = hidden::Traits< Const::Square <Type_, Size_> >::structure_,
      orient_    = hidden::Traits< Const::Square <Type_, Size_> >::orient_,
      sizeRows_  = hidden::Traits< Const::Square <Type_, Size_> >::sizeRows_,
      sizeCols_  = hidden::Traits< Const::Square <Type_, Size_> >::sizeCols_,
      storage_   = hidden::Traits< Const::Square <Type_, Size_> >::storage_
    };
    /** default constructor */
    Square() : Base() {}
    /** constructor with specified dimension */
    Square(Range I) : Base(I, I) {}
    /** @return the element (i,j) of the constant square matrix.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const { return (Type(1));}
};

/**@ingroup Arrays
 * Define the constant general matrix
 * \f[
 * S =
 * \left(
 * \begin{array}{ccccc}
 *  1      &  & \hdots & & 1\\
 *         &  &        & &  \\
 *  \vdots &  &        & & \vdots\\
 *         &  &        & & \\
 *  1      &  & \hdots & & 1
 * \end{array}
 * \right).
 * \f]
 * The sizes can be either two fixed template arguments or dynamic sizes.
 * Exemple:
 * @code
 *  STK::Const::General<Real,3, 4> G3; // G3 is a 3x4 matrix of Real
 *  STK::Const::General<Real> S(10, 20); // S is a 10x20 matrix of Real
 * @endcode
 * @tparam SizeRows_ the number of row of the matrix. Default is UnknownSize.
 * @tparam SizeCols_ the number of column of the matrix. Default is UnknownSize.
 **/
template< typename Type_, int SizeRows_, int SizeCols_ >
class General : public ITArrayBase<General<Type_, SizeRows_, SizeCols_>, SizeRows_, SizeCols_ >
{
  public:
    typedef ITArrayBase<General<Type_, SizeRows_, SizeCols_>, SizeRows_, SizeCols_ > Base;
    typedef Type_ Type;
    enum
    {
      structure_ = hidden::Traits< Const::General <Type_, SizeRows_, SizeCols_> >::structure_,
      orient_    = hidden::Traits< Const::General <Type_, SizeRows_, SizeCols_> >::orient_,
      sizeRows_  = hidden::Traits< Const::General <Type_, SizeRows_, SizeCols_> >::sizeRows_,
      sizeCols_  = hidden::Traits< Const::General <Type_, SizeRows_, SizeCols_> >::sizeCols_,
      storage_   = hidden::Traits< Const::General <Type_, SizeRows_, SizeCols_> >::storage_
    };
    /** default constructor */
    General() : Base() {}
    /** constructor with specified dimension */
    General(Range rangeRows, Range rangeCols) : Base(rangeRows, rangeCols) {}
    /** @return the element (i,j) of the constant square matrix.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const { return (Type(1));}
};

/**@ingroup Arrays
 * Define the constant upper triangular matrix
 * \f[
 * S =
 * \left(
 * \begin{array}{ccccc}
 *  1\\
 *   & 1 & & \text{\huge 1}\\
 *   &   & \ddots \\
 *   & \text{\huge 0} & & \ddots\\
 *   &               & &  & 1
 * \end{array}
 * \right).
 * \f]
 * The sizes can be either two fixed template arguments or dynamic sizes.
 * Exemple:
 * @code
 *  STK::Const::UpperTriangular<Real,3, 4> U3; // U3 is a 3x4 upper triangular matrix of Real
 *  STK::Const::UpperTriangular<Real> S(10, 20); // S is a 10x20 upper triangula matrix of Real
 * @endcode
 * @tparam SizeRows_ the number of row of the matrix. Default is UnknownSize.
 * @tparam SizeCols_ the number of column of the matrix. Default is UnknownSize.
 **/
template< typename Type_, int SizeRows_, int SizeCols_ >
class UpperTriangular : public ITArrayBase<UpperTriangular<Type_, SizeRows_, SizeCols_>, SizeRows_, SizeCols_ >
{
  public:
    typedef ITArrayBase<UpperTriangular<Type_, SizeRows_, SizeCols_>, SizeRows_, SizeCols_ > Base;
    typedef Type_ Type;
    enum
    {
      structure_ = hidden::Traits< Const::UpperTriangular <Type_, SizeRows_, SizeCols_> >::structure_,
      orient_    = hidden::Traits< Const::UpperTriangular <Type_, SizeRows_, SizeCols_> >::orient_,
      sizeRows_  = hidden::Traits< Const::UpperTriangular <Type_, SizeRows_, SizeCols_> >::sizeRows_,
      sizeCols_  = hidden::Traits< Const::UpperTriangular <Type_, SizeRows_, SizeCols_> >::sizeCols_,
      storage_   = hidden::Traits< Const::UpperTriangular <Type_, SizeRows_, SizeCols_> >::storage_
    };
    /** default constructor */
    UpperTriangular() : Base() {}
    /** constructor with specified dimension */
    UpperTriangular(Range rangeRows, Range rangeCols) : Base(rangeRows, rangeCols) {}
    /** @return the element (i,j) of the constant upper triangular matrix.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const { return (Type(1));}
};

/**@ingroup Arrays
 * Define the constant lower triangular matrix
 * \f[
 * S =
 * \left(
 * \begin{array}{ccccc}
 *  1\\
 *   & 1 & & \text{\huge 0}\\
 *   &   & \ddots \\
 *   & \text{\huge 1} & & \ddots\\
 *   &               & &  & 1
 * \end{array}
 * \right).
 * \f]
 * The sizes can be either two fixed template arguments or dynamic sizes.
 * Exemple:
 * @code
 *  STK::Const::LowerTriangular<Real,3, 4> G3; // G3 is a 3x4 lower triangular matrix of Real
 *  STK::Const::LowerTriangular<Real> S(10, 20); // S is a 10x20 lower triangular matrix of Real
 * @endcode
 * @tparam SizeRows_ the number of row of the matrix. Default is UnknownSize.
 * @tparam SizeCols_ the number of column of the matrix. Default is UnknownSize.
 **/
template< typename Type_, int SizeRows_, int SizeCols_ >
class LowerTriangular : public ITArrayBase<LowerTriangular<Type_, SizeRows_, SizeCols_>, SizeRows_, SizeCols_ >
{
  public:
    typedef ITArrayBase<LowerTriangular<Type_, SizeRows_, SizeCols_>, SizeRows_, SizeCols_ > Base;
    typedef Type_ Type;
    enum
    {
      structure_ = hidden::Traits< Const::LowerTriangular <Type_, SizeRows_, SizeCols_> >::structure_,
      orient_    = hidden::Traits< Const::LowerTriangular <Type_, SizeRows_, SizeCols_> >::orient_,
      sizeRows_  = hidden::Traits< Const::LowerTriangular <Type_, SizeRows_, SizeCols_> >::sizeRows_,
      sizeCols_  = hidden::Traits< Const::LowerTriangular <Type_, SizeRows_, SizeCols_> >::sizeCols_,
      storage_   = hidden::Traits< Const::LowerTriangular <Type_, SizeRows_, SizeCols_> >::storage_
    };
    /** default constructor */
    LowerTriangular() : Base() {}
    /** constructor with specified dimension */
    LowerTriangular(Range rangeRows, Range rangeCols) : Base(rangeRows, rangeCols) {}
    /** @return the element (i,j) of the constant lower triangular matrix.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const { return (Type(1));}
};

/**@ingroup Arrays
 * Define the constant point
 * \f[
 * S =
 * \left(
 * \begin{array}{cccc}
 *  1 & 1 & \ldots  & 1
 * \end{array}
 * \right).
 * \f]
 * The size can be either a fixed template argument or a dynamic size.
 * Exemple:
 * @code
 *  STK::Const::Point<Real,3> p3; // p3 is a row-vector of size 3
 *  STK::Const::Point<Real> p(10); // p is a row-vector of Real of size 10
 * @endcode
 * @tparam Size_ the size of the row-vector. Default is UnknownSize.
 **/
template< typename Type_, int Size_ >
class Point : public ITArrayBase<Point<Type_, Size_>, Size_, Size_ >
{
  public:
    typedef ITArrayBase<Point<Type_, Size_>, Size_, Size_ > Base;
    typedef typename hidden::Traits< Const::Point <Type_, Size_> >::Type Type;
    enum
    {
      structure_ = hidden::Traits< Const::Point <Type_, Size_> >::structure_,
      orient_    = hidden::Traits< Const::Point <Type_, Size_> >::orient_,
      sizeRows_  = hidden::Traits< Const::Point <Type_, Size_> >::sizeRows_,
      sizeCols_  = hidden::Traits< Const::Point <Type_, Size_> >::sizeCols_,
      storage_   = hidden::Traits< Const::Point <Type_, Size_> >::storage_
    };
    /** default constructor */
    Point() : Base() {}
    /** constructor with specified dimension */
    Point(Range I) : Base(1, I) {}
    /** @return the j-th element  of the constant row-vector.
     *  @param j index of the element
     **/
    inline Type const elt1Impl(int j) const { return (Type(1));}
    /** @return the element (i,j) of the constant row-vector.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const { return (Type(1));}
};

/**@ingroup Arrays
 * Define the constant point
 * \f[
 * S =
 * \left(
 * \begin{array}{cccc}
 *  1 & 1 & \ldots  & 1
 * \end{array}
 * \right).
 * \f]
 * The size can be either a fixed template argument or a dynamic size.
 * Exemple:
 * @code
 *  STK::Const::Point<Real,3> p3; // p3 is a row-vector of size 3
 *  STK::Const::Point<Real> p(10); // p is a row-vector of Real of size 10
 * @endcode
 * @tparam Size_ the size of the row-vector. Default is UnknownSize.
 **/
template< typename Type_, int Size_ >
class Vector : public ITArrayBase<Vector<Type_, Size_>, Size_, Size_ >
{
  public:
    typedef ITArrayBase<Vector<Type_, Size_>, Size_, Size_ > Base;
    typedef typename hidden::Traits< Const::Vector <Type_, Size_> >::Type Type;
    enum
    {
      structure_ = hidden::Traits< Const::Vector <Type_, Size_> >::structure_,
      orient_    = hidden::Traits< Const::Vector <Type_, Size_> >::orient_,
      sizeRows_  = hidden::Traits< Const::Vector <Type_, Size_> >::sizeRows_,
      sizeCols_  = hidden::Traits< Const::Vector <Type_, Size_> >::sizeCols_,
      storage_   = hidden::Traits< Const::Vector <Type_, Size_> >::storage_
    };
    /** default constructor */
    Vector() : Base() {}
    /** constructor with specified dimension */
    Vector(Range I) : Base(I, 1) {}
    /** @return the i-th element of the constant vector.
     *  @param i index of the element
     **/
    inline Type const elt1Impl(int i) const { return (Type(1));}
    /** @return the element (i,j) of the constant vector.
     *  @param i index of the row
     *  @param j index of the column
     **/
    inline Type const elt2Impl(int i, int j) const { return (Type(1));}
};


} // namespace const

} // namespace STK

#endif /* STK_CONST_ARRAYS_H */
