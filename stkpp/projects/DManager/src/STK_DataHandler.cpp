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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::DManager
 * created on: 16 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_DataHandler.cpp
 *  @brief In this file we implement the DataHandler class.
 **/

#include "../include/STK_DataHandler.h"
#include "Arrays/include/STK_Display.h"

namespace STK
{

/** read a data file and its companion description file. */
bool DataHandler::readDataFromCsvFile(std::string const& datafile, std::string descriptorfile)
{
  ReadWriteCsv rwdata(datafile);
  // no names to read in the first line
  rwdata.setWithNames(false);
  // read the data set
  if (!rwdata.read())
  {
    stk_cerr << _T("An error occur when reading the data file.\nWhat: ")
             << rwdata.error();
    return false;
  }
  ReadWriteCsv rwdesc(descriptorfile);
  // no names to read in the first line
  rwdesc.setWithNames(false);
  // read the data set
  if (!rwdesc.read())
  {
    stk_cerr << _T("An error occur when reading the descriptor file.\nWhat: ")
             << rwdesc.error();
    return false;
  }
  // check logic
  if (rwdata.size() != rwdesc.size())
  {
    stk_cerr << _T("Data file and descriptor file does not have the same number of column.\n");
    return false;
  }
  if (rwdata.rows().size() == 0)
  {
    stk_cerr << _T("No data.\n");
    return false;
  }
  if (rwdesc.rows().size() < 2)
  {
    stk_cerr << _T("No descriptor.\n");
    return false;
  }
  // parse descriptor file
  int firstRow = rwdesc.firstIdxRows();
  for (int j=rwdesc.firstIdxCols(); j<= rwdesc.lastIdxCols(); j++)
  {
    std::string idModel = rwdesc.at(j).at(firstRow);
    std::string idData = rwdesc.at(j).at(firstRow+1);
    if (!addInfo(idData, idModel)) return false;
  }
  // store data and descriptors
  data_ += rwdata;
  descriptor_ += rwdesc;
  return true;
}

std::vector<int> DataHandler::colIndex(std::string const& id) const
{
  int rowNames = descriptor_.firstIdxRows() +1;
  std::vector<int> colindex;
  for (int i = descriptor_.firstIdxCols(); i <= descriptor_.lastIdxCols(); ++i)
  {
    if (descriptor_.var(i).at(rowNames) == id) colindex.push_back(i);
  }
  return colindex;
}

void DataHandler::getData(std::string const& id, Array2D<int>& data, int& nbVariable) const
{
  std::vector<int> colindex = colIndex(id);
  nbVariable = colindex.size();
  data.resize(nbSample(), nbVariable);
  int j= data.firstIdxCols();
  for (std::vector<int>::const_iterator it = colindex.begin(); it != colindex.end(); ++it, ++j)
  {
    for (int i = data_.firstRow(*it); i <= data_.lastRow(*it); ++i)
    { data(i, j) = STK::stringToType<int>(data_(i,*it));}
  }
}

void DataHandler::getData(std::string const& id, Array2D<Real>& data, int& nbVariable) const
{
  std::vector<int> colindex = colIndex(id);
  nbVariable = colindex.size();
  data.resize(data_.rows(), nbVariable);
  int j= data.firstIdxCols();
  for (std::vector<int>::const_iterator it = colindex.begin(); it != colindex.end(); ++it, ++j)
  {
    for (int i = data_.firstRow(*it); i <= data_.lastRow(*it); ++i)
    { data(i, j) = STK::stringToType<Real>(data_(i,*it));}
  }
}

void DataHandler::getData(std::string const& id, Array2D<std::string>& data, int& nbVariable) const
{
  std::vector<int> colindex = colIndex(id);
  nbVariable = colindex.size();
  data.resize(data_.rows(), nbVariable);
  int j= data.firstIdxCols();
  for (std::vector<int>::const_iterator it = colindex.begin(); it != colindex.end(); ++it, ++j)
  {
    for (int i = data_.firstRow(*it); i <= data_.lastRow(*it); ++i)
    { data(i, j) = data_(i,*it);}
  }
}


} // namespace STK



