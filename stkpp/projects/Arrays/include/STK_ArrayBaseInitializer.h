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
 * created on: 28 nov. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ArrayBaseInitializer.h
 *  @brief In this file we implement the ArrayInitializer class.
 **/


#ifndef STK_ARRAYBASEINITIALIZER_H
#define STK_ARRAYBASEINITIALIZER_H

#include "Sdk/include/STK_Macros.h"

namespace STK
{
/** @brief Helper class used by the comma initializer operator
 *
 * This class is internally used to implement the comma initializer feature.
 * It is the return type of ArrayBase::operator<<, and most of the time this
 * is the only way it is used.
 **/
template<typename Derived>
class ArrayInitializer
{
  public:
    typedef typename Derived::Type Type;
    /** Constructor */
    inline ArrayInitializer( Derived& array, const Type& s)
                           : array_(array), row_(array.beginRows()), col_(array.beginCols())
    { // check if there is space
      if (array.empty())
      { STKRUNTIME_ERROR_NO_ARG(ArrayInitializer::ArrayInitializer,array is empty);}
      toFirstElt();
      array_.elt(row_, col_) = s;
   }
    //    template<typename Rhs>
    //    inline ArrayInitializer( Derived& array, const ArrayBase<Rhs>& other)
    //                           : array_(array), row_(array.beginRows()), col_(array.beginCols())
    //     { // check if there is space
    //      if (array.empty())
    //      { STKRUNTIME_ERROR_NO_ARG(ArrayInitializer::ArrayInitializer,Too many coefficients passed to operator<<(array is empty));}
    //
    //       array_.sub(Range(row_,other.sizeRows()), Range(col_,other.sizeCols())) = other;
    //       col_     += other.sizeCols();
    //     }

    /** inserts a scalar value in the target matrix */
    ArrayInitializer& operator,(const Type& s)
    {
      toNextElt();
      array_.elt(row_, col_) = s;
      return *this;
    }
    /** destructor */
    inline ~ArrayInitializer() {}

  protected:
    /** Set (row_, col_) to the first element of the array.
     * @note Useful only if the structure of the array does not allow element
     * in the (firstIdxRows(), beginCols()) position.
     **/
    void toFirstElt()
    {
      while ( (array_.rangeColsInRow(row_).size() <= 0) && (row_ <= array_.lastIdxRows()))
      { row_++;}
      if (row_ > array_.lastIdxRows())
      { STKRUNTIME_ERROR_NO_ARG(ArrayInitializer::toFirstElt,array is empty);}
      col_ = array_.rangeColsInRow(row_).begin();
    }
    /** Computr the next element*/
    void toNextElt()
    {
      // for the current row go to first available column if necessary
      if (col_ < array_.rangeColsInRow(row_).begin())
      { col_ = array_.rangeColsInRow(row_).begin();}
      else // otherwise just increment
      { col_++;}
      // check if we need to go to next row
      if (col_ > array_.rangeColsInRow(row_).lastIdx())
      {
        row_++;
        while ( (array_.rangeColsInRow(row_).size() <= 0) && (row_ <= array_.lastIdxRows()))
        { row_++;}
        if (row_ > array_.lastIdxRows())
        { STKRUNTIME_ERROR_NO_ARG(ArrayInitializer::toNextElt,Too many coefficients passed to operator<<);}
        col_ = array_.rangeColsInRow(row_).begin();
      }
    }
  protected:
    Derived& array_;  // target
    int row_;     // current row
    int col_;     // current col
};

/* Convenient operator to set the coefficients of a matrix.
 *
 * The coefficients must be provided in a row major order and exactly match
 * the size of the matrix. Otherwise an assertion is raised.
 */
template<typename Derived>
inline ArrayInitializer<Derived> ArrayBase<Derived>::operator<< (const Type& s)
{ return ArrayInitializer<Derived>(this->asDerived(), s);}

// @sa operator<<(const Type&) */
//template<typename Derived>
//template<typename Rhs>
//inline ArrayInitializer<Derived>
//ArrayBase<Derived>::operator<<(const ArrayBase<Rhs>& other)
//{ return ArrayInitializer<Derived>(this->asDerived(), other);}

} // namespace STK

#endif /* STK_ARRAYBASEINITIALIZER_H */
