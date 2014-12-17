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
 * created on: 15 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_DataHandler.h
 *  @brief In this file we define the DataHandler class.
 **/


#ifndef STK_DATAHANDLER_H
#define STK_DATAHANDLER_H

#include <vector>

#include "STK_IDataHandler.h"
#include "STK_ReadWriteCsv.h"
#include "Arrays/include/STK_Array2D.h"

namespace STK
{
// forward declaration
class DataHandler;

namespace hidden
{
/** @ingroup hidden
 *  Specialization of the  DataHandlerTraits for DataHandler
 **/
template<typename Type>
struct DataHandlerTraits<DataHandler, Type>
{
  typedef Array2D<Type> Data;
};

} // namespace hidden

/** @ingroup DManager
 * @c implementation of the IDataHandler class using ReadWriteCsv and Array2D.
 *  The DataHandler class allow to read various csv files with their description
 *  files and to get the columns identified by an idData in an @c Array2D.
 *  All data are stored in memory in a ReadWriteCsv structure.
 */
class DataHandler : public IDataHandler
{
  public:
    typedef IDataHandler::InfoMap InfoMap;
    /** default constructor */
    inline DataHandler() : IDataHandler(), withNames_(false)
    { data_.setWithNames(false); descriptor_.setWithNames(false);}
    /** destructor */
    inline virtual ~DataHandler() {}

    /** @return the number of sample (the number of rows of the data) */
    inline virtual int nbSample() const { return data_.sizeRows();}
    /** @return the number of sample (the number of columns of the data) */
    inline virtual int nbVariable() const { return data_.size();}
    /** get the whole data set */
    inline ReadWriteCsv const& data() const { return data_;}
    /** get the whole descriptor set */
    inline ReadWriteCsv const& descriptor() const { return descriptor_;}

    /** set withNames flag */
    inline void setWithNames(bool withNames) { withNames_ = withNames;}
    /** read a data file and its companion description file. */
    bool readDataFromCsvFile(std::string const& datafile, std::string descriptorfile);
    /** @brief read a data set from an Array2D.
     * read a data file
     * @param datafile the data file to get
     * @param idData the id of the data
     * @param idModel an id identifying the model to use with the data set
     **/
    bool readDataFromCsvFile(std::string const& datafile, std::string const& idData, std::string const& idModel);

    /** @brief read a data set from an Array2D.
     * This method should be essentially used:
     * - for testing some statistical method as the data  will be converted in a
     * String format (whcih is not an efficient way to store the data..).
     * - if the data are already stored in a String format.
     * @param data the data set
     * @param idData the id of the data
     * @param idModel an id identifying the model to use with the data set
     **/
    template<typename Type>
    bool readDataFromArray2D(Array2D<Type> const& data, std::string const& idData, std::string const& idModel);

    /** return in an Array2D<int> the data with the given idData */
    template<typename Type>
    void getData(std::string const& idData, Array2D<Type>& data, int& nbVariable) const;

    /** remove the data with the given idData */
    void removeData(std::string const& idData);

  protected:
    /** lookup on the descriptors in order to get the columns of the ReadWriteCsv
     *  with the Id idData.
     *  @param idData id of the data to get
     **/
    std::vector<int> colIndex(std::string const& idData) const;

  private:
    /** first line with names ?*/
    bool withNames_;
    /** data files */
    ReadWriteCsv data_;
    /** descriptor files with two rows. On the first row we get the idModel,
     * on the second row, we get the idData
     **/
    ReadWriteCsv descriptor_;
};


template<typename Type>
void DataHandler::getData(std::string const& idData, Array2D<Type>& data, int& nbVariable) const
{
  std::vector<int> indexes = colIndex(idData);
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("In DataHandler::getData, idData = ") << idData << _T("\n");
  stk_cout << _T("columns found = ");
  for (std::vector<int>::const_iterator it = indexes.begin(); it != indexes.end(); ++it)
  { stk_cout << (*it) << _T(" ");}
  stk_cout << _T("\n");
#endif
  nbVariable = indexes.size();
  data.resize(nbSample(), nbVariable);
  int j= data.beginCols();
  for (std::vector<int>::const_iterator it = indexes.begin(); it != indexes.end(); ++it, ++j)
  {
    for (int i = data_.firstRow(*it); i <= data_.lastRow(*it); ++i)
    { data(i, j) = STK::stringToType<Type>(data_(i,*it));}
  }
}

template<typename Type>
bool DataHandler::readDataFromArray2D(Array2D<Type> const& data, std::string const& idData, std::string const& idModel)
{
  // add descriptor
  Variable<std::string> desc(2);
  desc[baseIdx] = idModel ; desc[baseIdx+1] = idData;
  if (!addInfo(idData, idModel)) return false;
  // store data at the end of the ReadWriteCsv array in a string format
  for (int j=data.beginCols(); j<= data.lastIdxCols(); ++j)
  {
    data_.push_back();
    data_.back().resize(data.rows());
    for (int i= data.beginRows(); i <= data.lastIdxRows(); ++i)
    { data_.back()[i] = typeToString(data(i,j), std::scientific);}
    // store descriptor : this is the same for all the columns added
    descriptor_.push_back(desc);
  }
  return true;
}

} // namespace STK

#endif /* STK_DATAHANDLER_H */
