/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (elt your option) any later version.

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
 * Purpose:  Implement the DataFrame (Table) class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
**/

/** @file STK_DataFrame.cpp
 *  @brief In this file we implement the DataFrame (Table) class.
 **/

#include "../include/STK_ExportToCsv.h"

namespace STK
{
/* Default constructor . */
DataFrame::DataFrame() : Base(), Base2D() {}

/* Copy constructor */
DataFrame::DataFrame( DataFrame const& T, bool ref)
                    : Base(T.range())
                    , Base2D(T)
{
  // we need to copy explicitly the data
  for (int j=begin(); j<=lastIdx(); j++) // for all columns
    if (T[j])                            // if there is data
      elt(j) = T[j]->clone(ref); // set the adress of a clone
}

/* destructor */
DataFrame::~DataFrame() { if (!isRef()) freeCols();}

/* clear the object. */
void DataFrame::clear()
{
  freeMem();
  // set default range for list
  Base::setRange();
  // set default range for container2D
  Base2D::setRanges(0, 0);
}

/* Operator = : overwrite the DataFrame with T. */
DataFrame& DataFrame::operator=(DataFrame const& T)
{
  // Resize if necessary.
  if (sizeCols() != T.sizeCols()) Base::resize(T.cols());

  // Copy without overlapping.
  if ((T.begin()>=begin()))
  {
    for (int jt=T.begin(), j=begin(); jt<=T.lastIdx(); j++, jt++)
    {
      // clear old mem if any
      if (elt(j)) delete elt(j);
      // if there is a variable, create a copy
      if (T[jt]) elt(j) = T[jt]->clone();
      else       elt(j) = 0;
    }
  }
  else
  {
    for (int jt=T.lastIdx(), j=lastIdx(); jt>=T.begin(); j--, jt--)
    {
      // clear old mem if any
      if (elt(j)) delete elt(j);
      // if there is a variable, create a copy
      if (T[jt]) elt(j) = T[jt]->clone();
      else       elt(j) = 0;
    }
  }
  return *this;
}

/* resize the container:
 * - call @c shift(I.begin(), J.begin()
 * - call @c popBackCols() (@c insertRows()) and/or @c popBackCols()
 *  (@c popBackRows()).
 *  The implicit assumption made by this method is that it is easier and
 *  faster to add column than add rows to the 2D container.
 * @param I the new range for the rows of the container
 * @param J the new range for the columns of the container
 **/
void DataFrame::resize( Range const& I, Range const& J)
{
  // check if there is something to do
  if ((rows() == I) && (cols() == J)) return;
  //  translate beg
  shift(I.begin(), J.begin());
  // number of rows to del or add
  int rinc = I.lastIdx() - lastIdxRows();
  // number of cols to del or add
  int cinc = J.lastIdx() - lastIdxCols();
  // check if we add cols
  if (cinc >=0)   // work first on rows
  {
    if (rinc < 0) popBackRows(-rinc); // less rows
    else          pushBackRows(rinc); // more rows
    pushBackCols(cinc); // add columns
  }
  else // work first on columns
  {
    popBackCols(-cinc); // remove columns
    if (rinc < 0) popBackRows(-rinc); // less rows
    else          pushBackRows(rinc); // more rows
  }
}

/* New beginning index for the object. */
void DataFrame::shift(int const& cbeg)
{
  // list1D shift
  Base::shift(cbeg);
  // IContainer2D shift for Col
  shiftbeginCols(cbeg);
}

/* New beginning index for the object. */
void DataFrame::shift(int const& rbeg, int const& cbeg)
{
 if (firstIdxRows() == rbeg && beginCols() == cbeg) return;
 if (isRef())
 { STKRUNTIME_ERROR_2ARG(DataFrame::shift,rbeg,cbeg,cannot operate on reference); }
  // list1D shift
  shift(cbeg);
  // For each column update Variable
  for (int j=begin(); j<=lastIdx(); j++)
    if (elt(j)) { elt(j)->shift(rbeg);}
  // update range of the rows
  shiftFirstIdxRows(rbeg);
}


/* Del last column of the container.                                     */
void DataFrame::popBackCols(int const& n)
{
  // if n<=0 nothing to do
  if (n<=0) return;
  // if there is columns to erase
  if (sizeCols()<n)
  { STKOUT_OF_RANGE_1ARG(DataFrame::popBackCols,n,sizeCols()<n);}
  // for all columns, delete variables
  for (int j=lastIdxCols() - n +1; j<=lastIdxCols(); j++)
    if (elt(j)) delete elt(j);
  // popBack() of List1D
  Base::popBack(n);
  // update IContainer2D
  decLastIdxCols(n);
  // if it was the last elt, free mem
  if (this->sizeCols() == 0) freeMem();
}

/* Delete n columns elt the nuber pos of the container. */
void DataFrame::eraseCols( int pos, int const& n)
{
  // if n<=0 nothing to do
  if (n<=0) return;
  // check conditions
  if (pos<begin())
  { STKOUT_OF_RANGE_2ARG(DataFrame::eraseCols,pos, n,pos<begin());}
  if (pos>lastIdx())
  { STKOUT_OF_RANGE_2ARG(DataFrame::eraseCols,pos, n,pos>lastIdx());}
  if (lastIdx() < pos+n-1)
  { STKOUT_OF_RANGE_2ARG(DataFrame::eraseCols,pos, n,lastIdx()<pos+n-1);}
  // for all columns, delete variables
  for (int j=pos+n-1; j>=pos; j--)
    if (elt(j)) { delete elt(j);}
  // delete elements of the List1D
  erase(pos, n);
  // update cols_
  decLastIdxCols(n);
  // if it was the last col, free mem
  if (this->sizeCols() == 0) freeMem();
}

/* Insert variable V elt the position i to the container.              */
void DataFrame::insertVariable(int pos, IVariable* const & V)
{
  // List1D
  List1D<IVariable*>::insertElt(pos);
  elt(pos) = V;
  // the variable have to be in the same range
  elt(pos)->shift(firstIdxRows());
  // update horizontal range (the number of column)
  incLastIdxCols(1);

  // update rows with NA values
  int inc = sizeRows() - V->size();
  if (inc == 0) return; // same size
  if (inc > 0) // V has less rows
  { // put NA values to the inserted column
    elt(pos)->pushBackNAValues(inc);
  }
  else
  { // put NA values to the other columns
    for (int i=this->beginCols(); i <pos; i++)
      if (elt(i)) { elt(i)->pushBackNAValues(-inc);}
    for (int i=pos+1; i <=this->lastIdxCols(); i++)
      if (elt(i)) { elt(i)->pushBackNAValues(-inc);}
    // update LastVe
    incLastIdxRows(-inc);
  }
}

/* Merge this with a dataframe (horizontally). */
void DataFrame::pushBackVariable( IVariable* const &V)
{
  // List1D
  pushBack();
  back() = V;
  // update horizontal range (the number of column)
  incLastIdxCols(1);
  // adjust the first index of the inserted variable
  elt(lastIdxCols())->shift(firstIdxRows());
  // update rows with NA values
  int inc = sizeRows() - V->size();
  if (inc == 0) return; // same size
  if (inc > 0) //V has less rows
  { // put NA values to the inserted columns
    elt(lastIdxCols())->pushBackNAValues(inc);
  }
  else
  { // put NA values to the oter columns
    for (int i=this->begin(); i <lastIdxCols(); i++)
      if (elt(i)) { elt(i)->pushBackNAValues(-inc);}
    // update LastVe
    incLastIdxRows(-inc);
  }
}

/* merge this DataFrame with other. */
void DataFrame::merge( DataFrame const& other)
{
  if (other.empty()) return;
  // is this structure just a pointer?
  if (this->isRef())
  { STKRUNTIME_ERROR_NO_ARG(DataFrame::merge(other),*this is a reference.);}
  // is T just a pointer?
  if (other.isRef())
  { STKRUNTIME_ERROR_NO_ARG(DataFrame::merge(other),other is a reference.);}
  // break const reference
  DataFrame& otherRef = const_cast<DataFrame&>(other);
  // update rows with NA values
  int pos(lastIdx()+1), inc = sizeRows() - otherRef.sizeRows();

  otherRef.shift(firstIdxRows(), beginCols());
  Base::merge(otherRef);

  if (inc == 0) return; // same size

  if (inc > 0) // other has less rows
  { // put NA values to the merged columns
    for (int i= lastIdx(); i >=pos; i--)
      if (elt(i)) { elt(i)->pushBackNAValues(inc);}
  }
  else
  { // put NA values to the existing columns
    for (int i=this->begin(); i <pos; i++)
      if (elt(i)) { elt(i)->pushBackNAValues(-inc);}

    // update LastVe
    incLastIdxRows(-inc);
  }

}

/* Insert the DatatFrame D elt the column pos to the container. */
void DataFrame::insertDataFrame( int pos, const DataFrame& D)
{
  if (D.empty()) return;
  // is this structure just a pointer?
  if (this->isRef())
  { STKRUNTIME_ERROR_1ARG(DataFrame::insertDataFrame,pos,cannot operate on reference);}
  if (begin()>pos)
  { STKOUT_OF_RANGE_1ARG(DataFrame::insertDataFrame,pos,begin()>pos);}
  if (lastIdx()+1<pos)
  { STKOUT_OF_RANGE_1ARG(DataFrame::insertDataFrame,pos,lastIdx()+1<pos);}
  // List1D
  insertElt(pos, D.sizeCols());
  // insert all columns of D
  for (int i = D.beginCols(), icol = pos; i <=D.lastIdxCols(); i++, icol++)
  {
    if (D.elt(i))
    {
      elt(icol) = D.elt(i)->clone();
      elt(icol)->shift(firstIdxRows());
    }
    else { elt(icol) = 0;}
  }
  // update LastHo
  incLastIdxCols(D.sizeCols());
  // update rows with NA values
  int inc = sizeRows() - D.sizeRows();
  if (inc == 0) return; // same size
  if (inc > 0) // D has less rows
  { // put NA values to the inserted columns
    for (int i= pos+D.sizeCols()-1; i >=pos; i--)
      if (elt(i)) { elt(i)->pushBackNAValues(inc);}
  }
  else
  { // put NA values to the oter columns
    for (int i=this->beginCols(); i <pos; i++)
      if (elt(i)) { elt(i)->pushBackNAValues(-inc);}
    for (int i=pos+D.sizeCols(); i <=this->lastIdx(); i++)
      if (elt(i)) { elt(i)->pushBackNAValues(-inc);}
    // update LastVe
    incLastIdxRows(-inc);
  }
}

/* Merge this with a dataframe (horizontally).
*/
void DataFrame::pushBackDataFrame( DataFrame const &D)
{
  if (D.empty()) return;
  // is this structure just a pointer?
  if (this->isRef())
  { STKRUNTIME_ERROR_NO_ARG(DataFrame::pushBackDataFrame,cannot operate on reference);}
  // compute pos
  int pos(lastIdx()+1);
  // List1D
  pushBack(D.sizeCols());
  // insert all columns of D
  for (int i = D.begin(), icol = pos; i <=D.lastIdx(); i++, icol++)
  {
    if (D.elt(i))
    {
      elt(icol) = D.elt(i)->clone();
      elt(icol)->shift(firstIdxRows());
    }
  }
  // update LastHo
  incLastIdxCols(D.sizeCols());
  // update rows with NA values
  int inc = sizeRows() - D.sizeRows();
  if (inc == 0) return; // same size
  if (inc > 0) // D has less rows
  { // put NA values to the inserted columns
    for (int i= lastIdx(); i >=pos; i--)
      if (elt(i)) { elt(i)->pushBackNAValues(inc);}
  }
  else
  { // put NA values to the oter columns
    for (int i=this->begin(); i <pos; i++)
      if (elt(i)) { elt(i)->pushBackNAValues(-inc);}
    // update LastVe
    incLastIdxRows(-inc);
  }
}

/* Add columns to the container. */
void DataFrame::pushBackCols(int const& n)
{
  // if n<=0 nothing to do
  if (n <= 0) return;
  // add n columns to list1D
  insert(Range(lastIdx()+1, n), 0);
  // update IContainer2D
  incLastIdxCols(n);
}

/* Insert columns elt the specified position to the container. */
void DataFrame::insertCols( int pos, int const& n)
{
  if (n <= 0) return;        // if n<=0 nothing to do
  // check conditions
  if (pos<begin())
  { STKOUT_OF_RANGE_2ARG(DataFrame::insertCols,pos, n,pos<begin());}
  if (pos>lastIdx())
  { STKOUT_OF_RANGE_2ARG(Dataframe::insertCols,pos, n,pos>lastIdx());}
  // insert n elements in list1D
  insert(Range(pos, n), 0);
  // update IContainer2D
  incLastIdxCols(n);
}

/* Add n rows to the container. */
void DataFrame::pushBackRows(int const& n)
{
  // if n<=0 nothing to do
  if (n<=0) return;
  // for each column append row
  for (int j=begin(); j<=lastIdx(); j++)
  {
    if (elt(j)) { elt(j)->pushBack(n);}
  }
  // update range of the container
  incRangeRows(n);
}

/* Insert n rows elt the ith position of the container.                */
void DataFrame::insertRows( int pos, int const& n)
{
  // if n<=0 nothing to do
  if (n<=0) return;
  if (firstIdxRows() > pos)
  { STKOUT_OF_RANGE_2ARG(DataFrame::insertRows,pos, n,beginRows() > pos);}
  if (lastIdxRows()+1 < pos)
  { STKOUT_OF_RANGE_2ARG(DataFrame::insertRows,pos, n,lastIdxRows()+1 < pos);}
  // insert rows to each variables
  for (int j=begin(); j<=lastIdx(); j++)
    if (elt(j)) { elt(j)->insertElt(pos, n);}
  // update rows_
  incLastIdxRows(n);
}

/* Dell last row of the container. */
void DataFrame::popBackRows(int const& n)
{
  if (sizeRows() < n)
  { STKOUT_OF_RANGE_1ARG(DataFrame::popBackRows,n,sizeRows() < n);}
  // del last row to each variable
  for (int j=begin(); j<=lastIdx(); j++)
    if (elt(j)) { elt(j)->popBack(n);}
  decLastIdxRows(n);
}

/* Dell n rows elt the ith position to the container. */
void DataFrame::eraseRows( int pos, int const& n)
{
  // if n<=0 nothing to do
  if (n<=0) return;
  if (firstIdxRows() > pos)
  { STKOUT_OF_RANGE_2ARG(DataFrame::eraseRows,pos, n,beginRows() > pos);}
  if (lastIdxRows() < pos)
  { STKOUT_OF_RANGE_2ARG(DataFrame::eraseRows,pos, n,lastIdxRows() < pos);}
  if (lastIdxRows() < pos+n-1)
  { STKOUT_OF_RANGE_2ARG(DataFrame::eraseRows,pos, n,lastIdxRows() < pos+n-1);}
  // for each variable erase elts
  for (int j=begin(); j<=lastIdx(); j++)
    if (elt(j)) { elt(j)->erase(pos, n);}
  // update rows_
  decLastIdxRows(n);
}

/* Protected function for memory deallocation. */
void DataFrame::freeMem()
{
  if (isRef()) return;
  // liberate variables
  freeCols();
  Base::freeMem();
  setRows();
  setCols();
}

/* Protected function for rows memory deallocation.*/
void DataFrame::freeCols()
{
  if (isRef()) return;
  // for all columns
  for (int j=begin(); j<=lastIdx(); j++)
    if (elt(j))          // if there is mem allocated
    { delete elt(j); elt(j) = 0;}
  // set default range
  setRows();
}

// write a selection
void DataFrame::writeDataFrame( ostream& os, int const& left
                              , int const& right
                              ) const
{
  // Export  to csv the DataFrame
  ExportToCsv csv(*this);
  // get the csv
  ReadWriteCsv* pData = csv.p_readWriteCsv();
  // set delimiters to blank
  pData->setDelimiters(STRING_BLANK);
  // write the csv
  pData->writeSelection(os, beginRows(), lastIdxRows(), left, right);
}

/* Print a DataFrame. */
ostream& operator<< (ostream& s, const DataFrame& V)
{
  s << std::right;
  V.writeDataFrame(s, V.beginCols(), V.lastIdxCols());
  return s;
}

} // namespace STK
