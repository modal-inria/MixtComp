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
 * Project:  IOaccess
 * Purpose:  Implementation of the classes ImportExportToCsv.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 */

/** @file STK_ExportToCsv.cpp
 *  @brief In this file we implement the classes ImportCsv and ExportToCsv.
 **/

#include "../include/STK_ExportToCsv.h"

namespace STK
{

ExportToCsv::ExportToCsv() : p_data_(0)
                           , isColNamed_(true)
{
  // create an empty ReadWriteCsv
  p_data_ = new ReadWriteCsv();
}

ExportToCsv::ExportToCsv( const DataFrame& df)
                        : p_data_(0)
                        , isColNamed_(true)
{
  // create an empty ReadWriteCsv
  p_data_ = new ReadWriteCsv();
  p_data_->setReserve(df.sizeRows());
  p_data_->resize(0, df.sizeCols());

  // for each field Try a String conversion
  for(int iVar = df.beginCols(), irw = p_data_->begin(); iVar<=df.lastIdxCols(); iVar++, irw++)
  { if (df.elt(iVar)) df.elt(iVar)->exportAsString(p_data_->var(irw));}
}

ExportToCsv::~ExportToCsv()
{ if (p_data_) delete p_data_;}

/* Set a name to each column of the ReadWriteCsv using the form
 *  prefix + number.
 *  @param prefix the prefix to use for the names of the columns
 **/
void ExportToCsv::setColumnsNames(String const& prefix)
{
  // get first and last column indexes
  const int first = p_data_->begin(), last = p_data_->lastIdx();
  // set names of the variables
  for(int i = first; i<=last; i++)
  {
    p_data_->setName(i, prefix + typeToString(i)) ;
  }
  isColNamed_ = true;
}

} // namespace STK
