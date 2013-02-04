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
 * created on: 28 nov. 2012
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_ArrayBaseInitializer.h
 *  @brief In this file .
 **/


#ifndef STK_ARRAYBASEINITIALIZER_H_
#define STK_ARRAYBASEINITIALIZER_H_

namespace STK
{
/** @brief Helper class used by the comma initializer operator
 *
 * This class is internally used to implement the comma initializer feature.
 * It is the return type of ArrayBase::operator<<, and most of the time this
 * is the only way it is used.
 **/
template<typename Derived>
struct ArrayInitializer
{
  public:
    typedef typename Derived::Type Type;
    /** Constructor */
    inline ArrayInitializer( Derived& array, const Type& s)
                           : array_(array), row_(array.firstIdxRows()), col_(array.firstIdxCols())
                           , nextRow_(row_)
    { // check if there is space
      if (array.empty())
      { STKRUNTIME_ERROR_NO_ARG(ArrayInitializer::ArrayInitializer,Too many coefficients passed to operator<<(array is empty));}
      array_.elt(row_, col_) = s;
      col_++;
      nextRow_++;
   }

    /* inserts a scalar value in the target matrix */
    ArrayInitializer& operator,(const Type& s)
    {
      if (col_>array_.lastIdxCols())
      { row_ = nextRow_;  col_ = array_.firstIdxCols(); nextRow_++;}
      if (row_ >array_.lastIdxRows())
      { STKRUNTIME_ERROR_NO_ARG(ArrayInitializer::operator,Too many coefficients passed to operator<<);}
      array_.elt(row_, col_) = s; col_++;
      return *this;
    }

    inline ~ArrayInitializer() {}

//    template<typename Rhs>
//    inline ArrayInitializer( Derived& array, const ArrayBase<Rhs>& other)
//                           : array_(array), row_(array.firstIdxRows()), col_(array.firstIdxCols())
//                           , nextRow_(row_)
//     { // check if there is space
//      if (array.empty())
//      { STKRUNTIME_ERROR_NO_ARG(ArrayInitializer::ArrayInitializer,Too many coefficients passed to operator<<(array is empty));}
//
//       array_.sub(Range(row_,other.sizeRows()), Range(col_,other.sizeCols())) = other;
//       col_     += other.sizeCols();
//       nextRow_ += other.sizeRows();
//     }
//
//    /* inserts a matrix earrayession in the target matrix */
//    template<typename Rhs>
//    ArrayInitializer& operator,(const ArrayBase<Rhs>& other)
//    {
//      if (col_>array_.lastIdxCols())
//      { row_=nextRow_;  col_ = array_.firstIdxCols();}
//      if (col_==array_.cols())
//      {
//        row_ =nextRow_;
//        col_ = 0;
//        nextRow_ = other.rows();
//  //      eigen_assert(row_+m_currentBlockRows<=array_.rows()
//  //        && "Too many rows passed to comma initializer (operator<<)");
//      }
//  //    eigen_assert(col_<array_.cols()
//  //      && "Too many coefficients passed to comma initializer (operator<<)");
//  //    eigen_assert(m_currentBlockRows==other.rows());
//      array_.sub( row_+other.rows(), col_+other.cols()) = other;
//      col_ += other.cols();
//      return *this;
//    }

//    /* @return the built matrix once all its coefficients have been set.
//      * Calling finished is 100% optional. Its purpose is to write expressions
//      * like this:
//      * @code
//      * array.sub((CArray33() << axis0, axis1, axis2).finished());
//      * @endcode
//      */
//    inline Derived& finished() { return array_; }
//
  protected:
    Derived& array_;  // target
    int row_;     // current row
    int col_;     // current col
    int nextRow_; // next row_
};

/* Convenient operator to set the coefficients of a matrix.
  *
  * The coefficients must be provided in a row major order and exactly match
  * the size of the matrix. Otherwise an assertion is raised.
  */
template<typename Derived>
inline ArrayInitializer<Derived> ArrayBase<Derived>::operator<< (const Type& s)
{ return ArrayInitializer<Derived>(this->asDerived(), s);}

///* @sa operator<<(const Type&) */
//template<typename Derived>
//template<typename Rhs>
//inline ArrayInitializer<Derived>
//ArrayBase<Derived>::operator<<(const ArrayBase<Rhs>& other)
//{ return ArrayInitializer<Derived>(this->asDerived(), other);}

} // namespace STK

#endif /* STK_ARRAYBASEINITIALIZER_H_ */
