/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Purpose:  Define the Upper Triangular Matrix class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Array2DUpperTriangular.h
  * @brief In this file we define the MatrixTriangular class
 **/

#ifndef STK_MATRIXUPPERTRIANGULAR_H
#define STK_MATRIXUPPERTRIANGULAR_H



#include "STK_Array2DVector.h"
#include "STK_Array2DPoint.h"

namespace STK
{
template<typename> class Array2DUpperTriangular;

/** @ingroup Arrays
  * @brief Specialization of the Array2D class for Type values.
  *
  * A Array2DLowerTriangular is a column oriented 2D container of Type
  * whcih is lower triangular.
 **/
typedef Array2DUpperTriangular<Real> MatrixUpperTriangular;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for the Array2DUpperTriangular class.
 **/
template<typename _Type>
struct Traits< Array2DUpperTriangular<_Type> >
{
  private:
    class Void {};
  public:
    typedef _Type          Type;
    typedef Array2DPoint<_Type> Row;
    typedef Array2DVector<_Type> Col;
    typedef Array2DPoint<_Type> SubRow;
    typedef Array2DVector<_Type> SubCol;
    typedef Array2DUpperTriangular<_Type> SubArray;
    typedef Void SubVector;
    enum
    {
      structure_ = Arrays::upper_triangular_,
      orient_    = Arrays::by_col_,
      sizeRows_  = UnknownSize,
      sizeCols_  = UnknownSize,
      storage_ = Arrays::dense_ // always dense
    };
};
}

/** @ingroup Arrays
  * @brief Declaration of the upper triangular matrix class.
  *
  * A Array2DUpperTriangular is a column oriented 2D upper triangular
  * container of Type. It is possible to add/remove rows and columns
  * but in this case the container will no more be triangular.
  * The container can be set upper triangular again using the method
  * IArray2D::update().
  **/
template<typename Type>
class Array2DUpperTriangular : public IArray2D< Array2DUpperTriangular<Type> >
{
  public:
    typedef Array2DPoint<Type> Row;
    typedef Array2DVector<Type> Col;
    typedef Array2DPoint<Type> SubRow;
    typedef Array2DVector<Type> SubCol;
    typedef Array2DUpperTriangular<Type> SubArray;

    enum
    {
      structure_ = Arrays::upper_triangular_,
      orient_    = Arrays::by_col_,
      sizeRows_  = UnknownSize,
      sizeCols_  = UnknownSize,
      storage_ = Arrays::dense_ // always dense
    };
    /** Type for the Interface Class.                                 */
    typedef IArray2D< Array2DUpperTriangular<Type> > Base;
    typedef ArrayBase < Array2DUpperTriangular<Type> > LowBase;

    /** Default constructor */
    Array2DUpperTriangular() : Base() {}
    /** constructor with specified ranges
     *  @param I range of the Rows
     *  @param J range of the Cols
     **/
    Array2DUpperTriangular( Range const& I, Range const& J) : Base(I, J) {}
    /** constructor with rows_ and rageHo_ specified, initialization with a
     *  specified value.
     *  @param I range of the Rows
     *  @param J range of the Cols
     *  @param v initial value in the container
     **/
    Array2DUpperTriangular( Range const& I, Range const& J, Type const& v)
                          : Base(I, J)
    { this->setValue(v);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    Array2DUpperTriangular( Array2DUpperTriangular const& T, bool ref=false)
                          : Base(T, ref) {}
    /** constructor by reference in a given range, ref_=1.
     *  @param T the container to wrap
     *  @param I range of the Rows to wrap
     *  @param J range of the Cols to wrap
     **/
    Array2DUpperTriangular( Base const& T, Range const& I, Range const& J)
                          : Base(T, I, J) {}
    /** Wrapper constructor Contruct a reference container.
     *  @param q pointer on the data
     *  @param I range of the  Rows to wrap
     *  @param J range of the Cols to wrap
     **/
    Array2DUpperTriangular( Type** q, Range const& I, Range const& J)
                          : Base(q, I, J) {}
    /** destructor. */
    ~Array2DUpperTriangular() {}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline Array2DUpperTriangular& operator=(Rhs const& T)
    { return LowBase::operator=(T);}
    /** Operator = : overwrite the Array2DUpperTriangular with T.      */
    Array2DUpperTriangular& operator=(const Array2DUpperTriangular &T)
    { return LowBase::assign(T);}
    /** Operator = : overwrite with a constant value. */
    inline Array2DUpperTriangular& operator=(Type const& v) { this->setValue(v); return *this;}
};

} // namespace STK

#endif
// STK_MATRIXUPPERTRIANGULAR_H
