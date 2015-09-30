/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: Nov 21, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_DataHandlerR.h
 *  @brief In this file 
 **/

#ifndef MIXT_DATAHANDLERR_H_
#define MIXT_DATAHANDLERR_H_

#include <vector>
#include <map>
#include <Rcpp.h>
#include "boost/regex.hpp"
#include "MixtComp/src/LinAlg/mixt_LinAlg.h"
#include "MixtComp/src/Data/mixt_AugmentedData.h"
#include "MixtComp/src/Data/mixt_MisValParser.h"
#include "MixtComp/src/Various/mixt_Def.h"
#include "MixtComp/src/IO/mixt_IO.h"

namespace mixt
{

class DataHandlerR
{
  public:
    /** default constructor */
    DataHandlerR(Rcpp::List rList);

    /** @return the number of samples (the number of rows of the data) */
    int nbSample() const {return nbInd_;}

        /** @return the number of variables (the number of columns of the data) */
    int nbVariable() const {return nbVariables_;}

    /** read a data file and its companion description file,
      and fill the infoMap_ (id -> model) and dataMap_ (id -> vector of positions)
      members */
    std::string listData();
    
    /** @return the map with the idDatas and idModel of the models */
    const std::map<std::string, std::string>& info() const{return info_;}

    void writeInfo() const;

    /** write information on the localization of data in the rList */
    void writeDataMap() const;

    template<typename DataType>
    std::string getData(std::string const& idData,
                        AugmentedData<DataType>& augData,
                        int& nbInd,
                        std::string& param,
                        typename AugmentedData<DataType>::Type offset) const;

    /** Basic version of getData that only retrieve a vector of std::string. The responsability to
     * parse it is thus handed down to the calling structure. */
    std::string getData(std::string const& idData,
                        Vector<std::string>& dataStr,
                        int& nbInd,
                        std::string& param) const;
  private:
    int nbInd_;

    int nbVariables_;

    /** Store the informations  of the mixtures in the form (idData, idModel) with
     * - idData: an arbitrary idData for a model
     * - model: a string which can be converted in an existing model
     * @sa stringToMixture */
    std::map<std::string, std::string> info_;

    /** map: id -> vector of positions in rList_, as typedef-ed above */
    std::map<std::string, int> dataMap_;

    /** A list of the mixtures transmitted by R, as named lists */
    Rcpp::List rList_;
};

template<typename DataType>
std::string DataHandlerR::getData(std::string const& idData,
                                  AugmentedData<DataType>& augData,
                                  int& nbInd,
                                  std::string& param,
                                  typename AugmentedData<DataType>::Type offset) const
{
  std::string warnLog;
  typedef typename AugmentedData<DataType>::Type Type;
  typedef typename AugmentedData<Matrix<Type> >::MisVal MisVal;

  MisValParser<Type> mvp(offset);

  if (dataMap_.find(idData) != dataMap_.end()) // check if the data requested is present in the input data
  {
    int pos = dataMap_.at(idData); // get the index of the element of the rList_ corresponding to idData
    nbInd = nbInd_;
    augData.resizeArrays(nbInd_); // R has already enforced that all data has the same number of rows, and now all mixture are univariate

    Rcpp::List currVar = rList_[pos]; // get current named list
    Rcpp::CharacterVector data = currVar("data");

    std::string currStr;
    Type val;
    MisVal misVal;

    for (int i = 0; i < nbInd_; ++i)
    {
      currStr = data[i];

      bool isValid = mvp.parseStr(currStr,
                                  val,
                                  misVal);

      if (isValid)
      {
        if (misVal.first == present_)
        {
          augData.setPresent(i, val);
          continue;
        }
        else
        {
          augData.setMissing(i, misVal);
          continue;
        }
      }
      else
      {
        std::stringstream sstm;
        sstm << "In " << idData << ", individual i: " << i << " present an error. "
             << currStr << " is not recognized as a valid format." << std::endl;
        warnLog += sstm.str();
      }
    }
  }
  else
  {
    std::stringstream sstm;
    sstm << "Data from the variable: " << idData << " has been requested but is absent from the provided data."
         << " Please check that all the necessary data is provided." << std::endl;
    warnLog += sstm.str();
  }
  return warnLog;
}

} /* namespace mixt */
#endif /* MIXT_DATAHANDLERR_H_ */
