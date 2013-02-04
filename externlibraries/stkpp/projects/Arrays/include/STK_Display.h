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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project: stkpp::Arrays
 * Purpose:  Display in an output stream the 2D arrays.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_Display.h
 *  @brief This file define methods for displaying 2D Containers.
 **/

#ifndef STK_DISPLAY_H
#define STK_DISPLAY_H

#include "../../STKernel/include/STK_Integer.h"
#include "../../STKernel/include/STK_Real.h"

#include "../../DManager/include/STK_ExportToCsv.h"

namespace STK
{
/** @ingroup Arrays
 *  Method for displaying any two dimensional Array.
 *  The Container is exported in ReadWriteCsv and the the csv is written
 *  in the output stream.
 *  @param os the output stream
 *  @param V the 2D container to write
 **/
template<class TContainer2D>
ostream& out2D( ostream& os, ITContainer<TContainer2D> const& V)
{
  // Export  to csv the Array
  ExportToCsv exportcsv(V);
  // get the csv
  ReadWriteCsv* pData = exportcsv.p_readWriteCsv();
  // set delimiters to blank
  pData->setDelimiters(_T(" "));
  pData->setWithNames(false);
  // write the csv in os
  pData->write(os);
  // return ostream
  return os;
}


} // namespace STK

#endif // STK_DISPLAY_H
