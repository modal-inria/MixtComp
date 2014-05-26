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
 * Purpose:  Define the Array2DLowerTriangular class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Array2DLowerTriangular.h
  * @brief In this file we define the Array2DLowerTriangular class
 **/

#ifndef STK_MATRIXLOWERTRIANGULAR_H
#define STK_MATRIXLOWERTRIANGULAR_H




#include "STK_Array2DVector.h"
#include "STK_Array2DPoint.h"

namespace STK
{
template<typename> class Array2DLowerTriangular;

/** @ingroup Arrays
  * @brief Specialization of the Array2D class for lower triangular
  * matrices.
  *
  * An Array2DLowerTriangular is a column oriented 2D container of Real
  * whcih is lower triangular.
 **/
typedef Array2DLowerTriangular<Real> MatrixLowerTriangular;

namespace hidden
{
/** @ingroup hidden
 *  @brief Specialization of the Traits class for the Array2DLowerTriangular class.
 **/
template<class _Type>
struct Traits< Array2DLowerTriangular<_Type> >
{  private:
    class Void {};
  public:
    typedef _Type          Type;
    typedef Array2DPoint<_Type> Row;
    typedef Array2DVector<_Type> Col;
    typedef Array2DPoint<_Type> SubRow;
    typedef Array2DVector<_Type> SubCol;
    typedef Array2DLowerTriangular<_Type> SubArray;
    typedef Void SubVector;

   enum
   {
     structure_ = Arrays::lower_triangular_,
     orient_    = Arrays::by_col_,
     sizeRows_  = UnknownSize,
     sizeCols_  = UnknownSize,
     storage_ = Arrays::dense_ // always dense
   };
};

}

/** @ingroup Arrays
  * @brief Declaration of the lower triangular matrix class.
  *
  * A Array2DLowerTriangular is a column oriented 2D lower triangular
  * container of element. It is possible to add/remove rows and columns
  * but in this case the container will no more be triangular.
  * The container can be set lower triangular again using the method
  * IArray2D::update().
  *
  * One can give any range for the rows/columns but if the diagonal of the
  * matrix is defined for @em i=j and the lower part of the matrix is
  * the set of index [i,j], with i<j.
 **/
template<class Type >
class Array2DLowerTriangular : public IArray2D< Array2DLowerTriangular<Type> >
{
  public:
    typedef Array2DPoint<Type> Row;
    typedef Array2DVector<Type> Col;
    typedef Array2DPoint<Type> SubRow;
    typedef Array2DVector<Type> SubCol;
    typedef Array2DLowerTriangular<Type> SubArray;

   enum
   {
     structure_ = Arrays::lower_triangular_,
     orient_    = Arrays::by_col_,
     sizeRows_  = UnknownSize,
     sizeCols_  = UnknownSize,
     storage_ = Arrays::dense_ // always dense
   };
    /** Type for the Interface Class.*/
    typedef IArray2D< Array2DLowerTriangular<Type> > Base;
    typedef ArrayBase < Array2DLowerTriangular<Type> > LowBase;

    /** Default constructor */
    Array2DLowerTriangular() : Base() {}
    /** Constructor with specified ranges
     *  @param I range of the Rows
     *  @param J range of the Cols
     **/
    Array2DLowerTriangular( Range const& I, Range const& J) : Base(I, J) {}
    /** constructor with rows_ and rageHo_ specified, initialization with a
     *  specified value.
     *  @param I range of the Rows
     *  @param J range of the Cols
     *  @param v initial value in the container
     **/
    Array2DLowerTriangular( Range const& I, Range const& J, Type const& v)
                          : Base(I, J)
    { this->setValue(v);}
    /** Copy constructor
     *  @param T the container to copy
     *  @param ref true if T is wrapped
     **/
    Array2DLowerTriangular( const Array2DLowerTriangular &T, bool ref=false)
                          : Base(T, ref) {}
    /** constructor by reference, ref_=1.
     *  @param T the container to wrap
     *  @param I range of the Rows to wrap
     *  @param J range of the Cols to wrap
     **/
    Array2DLowerTriangular( const Base& T, Range const& I, Range const& J)
                          : Base(T, I, J) {}
    /** Wrapper constructor Contruct a reference container.
     *  @param q pointer on the data
     *  @param I range of the  Rows to wrap
     *  @param J range of the Cols to wrap
     **/
    Array2DLowerTriangular( Type** q, Range const& I, Range const& J)
                          : Base(q, I, J) {}
    /** destructor : use destructor of IArray2D. */
    ~Array2DLowerTriangular() {}
    /** operator = : overwrite the CArray with the Right hand side T.
     *  @param T the container to copy
     **/
    template<class Rhs>
    inline Array2DLowerTriangular& operator=(Rhs const& T)
    { return LowBase::operator=(T);}
    /** operator = : overwrite the Array2DLowerTriangular with T.
     *  @param T the container to copy
     **/
    Array2DLowerTriangular& operator=( Array2DLowerTriangular const& T) { return LowBase::assign(T);}
    /** operator= : set the container to a constant value.
     *  @param v the value to set
     **/
    inline Array2DLowerTriangular& operator=(Type const& v) { this->setValue(v); return *this;}
};

} // namespace STK

#endif
// STK_MATRIXLOWERTRIANGULAR_H
