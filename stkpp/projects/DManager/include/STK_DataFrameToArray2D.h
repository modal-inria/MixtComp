/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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
 * Project:  stkpp::DManager
 * created on: 9 juin 2011
 * Purpose:  Create an utility class in order to transfer the Data from
 * a DataFrame in an Array2D.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_DataFrameToArray2D.h
 *  @brief In this file we define the DataFrameToArray2D class.
 **/

#ifndef STK_DATAFRAMETOARRAY2D_H
#define STK_DATAFRAMETOARRAY2D_H

#include "../../Arrays/include/STK_Array2D.h"

namespace STK
{

/** @brief The DataFrameToArray2D class allow to export the data of some @c TYPE
 *  stored in a @c DataFrame to be exported in an @c Array2D.
 *
 *  The 2D container is created on the stack and will be deleted with the
 *  @c DataFrameToArray2D structure. It is possible to release the 2D container
 *  by calling explicitly the @c release(). In this case the end user will have
 *  to free directly the Array2D.
 */
template<class Type>
class DataFrameToArray2D
{
  public:
    /** Constructor
     *  @param df the DataFrame to export
     */
    DataFrameToArray2D( DataFrame const& df): df_(df), p_data_(0) {}
    /** destructor */
    ~DataFrameToArray2D() { if (p_data_) delete p_data_;}
    /** run the export. */
    void run()
    {
      p_data_ = new Array2D<Type>();
      // for each field Try a type conversion
      for(int iVar = df_.beginCols(); iVar<=df_.lastIdxCols(); iVar++)
      {
        IVariable* const p_var = df_.elt(iVar);
        // if there is a variable
        if (p_var)
        {
          // check the type of the variable
          if (p_var->getType() == IdTypeImpl<Type>::returnType())
          {
            Variable<Type>* p_variable = static_cast<Variable<Type>* >(p_var);
            p_data_->pushBackCols(*p_variable);
          }
        }
      }
    }
    /** Accessor. get the 2D container. This method is not constant
     * in order to the user to modified directly the 2D container.
     * @return a ptr on the 2D container constructed
     **/
    inline Array2D<Type>* p_data() { return p_data_;}
    /** release the Array2D. It will be freed by the user. */
    inline void release() { p_data_ =0;}
    /** remove NA values from the Array2D
     *  @param byRow @c true if the user want to delete the rows with NA values,
     *  @c false if the user want to remove the column with NA values.
     **/
    void eraseNAValues(bool byRow)
    {
      // check if there exists data
      if (!p_data_) return;
      // get the first index
      const int firstCol = p_data_->beginCols(), firstRow = p_data_->beginRows();
      // get the last index
      int lastCol = p_data_->lastIdxCols(), lastRow = p_data_->lastIdxRows();
      if (byRow)
      {
        // loop on the rows
        for (int i= firstRow; i <= lastRow; ++i)
        {
          bool asNA = false;
          // loop on the element of the row
          for (int j= firstCol; j <= lastCol; ++j)
          {
            if (Arithmetic<Type>::isNA(p_data_->elt(i,j)))
            {
              asNA = true;
              break;
            }
          }
          // remove current row
          if (asNA)
          {
            p_data_->eraseRows(i);
            i--;
            lastRow--;
          }
        }
      }
      else
      {
        // loop on the column
        for (int j= firstCol; j <= lastCol; ++j)
        {
          bool asNA = false;
          // loop on the element of the column
          for (int i= firstRow; i <= lastRow; ++i)
          {
            if (Arithmetic<Type>::isNA(p_data_->elt(i,j)))
            {
              asNA = true;
              break;
            }
          }
          // remove current column
          if (asNA)
          {
            p_data_->eraseCols(j);
            j--;
            lastCol--;
          }
        }
      }
    }

  private:
    /** A reference to the DataFrame we want to exprt */
    DataFrame const& df_;
    /** A pointer on the 2D Container the class will create. */
    Array2D<Type>* p_data_;
};

} // namespace STK

#endif /* STK_DATAFRAMETOARRAY2D_H */
