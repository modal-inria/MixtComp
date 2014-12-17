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

namespace STK
{

/* read a data file and its companion description file. */
bool DataHandler::readDataFromCsvFile( std::string const& datafile
                                          , std::string const& idData
                                          , std::string const& idModel)
{
  ReadWriteCsv data(datafile);
  // no names to read in the first line
  data.setWithNames(withNames_);
  // read the data set
  if (!data.read())
  {
    stk_cerr << _T("An error occur when reading the data file.\nWhat: ")
             << data.error();
    return false;
  }
  // add descriptor
  Variable<std::string> desc(2);
  desc[baseIdx] = idModel ; desc[baseIdx+1] = idData;
  // store data and descriptors
  if (!addInfo(idData, idModel)) return false;
  data_ += data;
  // store descriptor : this is the same for all the columns added
  for (int j=data.beginCols(); j< data.endCols(); ++j)
  { descriptor_.push_back(desc);}
  return true;
}

/* read a data file and its companion description file. */
bool DataHandler::readDataFromCsvFile(std::string const& datafile, std::string descriptorfile)
{
  ReadWriteCsv rwdata(datafile);
  // no names to read in the first line
  rwdata.setWithNames(withNames_);
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
  if (rwdata.sizeRows() == 0)
  {
    stk_cerr << _T("No data.\n");
    return false;
  }
  if (rwdesc.sizeRows() < 2)
  {
    stk_cerr << _T("No descriptor.\n");
    return false;
  }
  // parse descriptor file
  int firstRow = rwdesc.beginRows();
  for (int j=rwdesc.beginCols(); j< rwdesc.endCols(); j++)
  {
    std::string idModel = rwdesc.at(j).at(firstRow);
    std::string idData = rwdesc.at(j).at(firstRow+1);
    if (!addInfo(idData, idModel)) return false;
  }
  // store data and descriptors
  data_       += rwdata;
  descriptor_ += rwdesc;
  return true;
}

/* remove the data with the given idData */
void DataHandler::removeData(std::string const& idData)
{
  int rowIdData = descriptor_.beginRows()+1;
  for (int i = descriptor_.endCols()-1; i >= descriptor_.beginCols(); --i)
  { if (descriptor_.var(i)[rowIdData] == idData)
    {
      data_.eraseColumn(i);
      descriptor_.eraseColumn(i);
     }
  }
  info_.erase(idData);
}

/* lookup on the descriptors in order to get the columns of the ReadWriteCsv
 *  with the Id idData.
 *  @param idData id of the data to get
 **/
std::vector<int> DataHandler::colIndex(std::string const& idData) const
{
  int rowIdData = descriptor_.beginRows()+1;
  std::vector<int> colindex;
  for (int i = descriptor_.beginCols(); i <= descriptor_.lastIdxCols(); ++i)
  { if (descriptor_.var(i).at(rowIdData) == idData) colindex.push_back(i);}
  return colindex;
}


} // namespace STK



