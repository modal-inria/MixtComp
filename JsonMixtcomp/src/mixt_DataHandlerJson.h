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

#ifndef MIXT_DATAHANDLERJSON_H_
#define MIXT_DATAHANDLERJSON_H_


#include <vector>
#include <map>

#include "json.hpp"
#include "../../MixtComp/src/LinAlg/mixt_LinAlg.h"
#include "../../MixtComp/src/Data/mixt_AugmentedData.h"
#include "../../MixtComp/src/Data/mixt_MisValParser.h"
#include "../../MixtComp/src/IO/mixt_IO.h"
#include "../../MixtComp/src/Various/mixt_Enum.h"


namespace mixt {

class DataHandlerJson {
  public:
    /** default constructor */
    DataHandlerJson(const nlohmann::json resGetData_lm);

    /** @return the number of samples (the number of rows of the data) */
    Index nbSample() const {return nbInd_;}

        /** @return the number of variables (the number of columns of the data) */
    Index nbVariable() const {return nbVar_;}

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
                        Index& nbInd,
                        std::string& paramStr,
                        typename AugmentedData<DataType>::Type offset) const;

    /** Basic version of getData that only retrieve a vector of std::string. The responsability to
     * parse it is thus handed down to the calling structure. This is useful for advanced models, like Rank or Functional.*/
    std::string getData(std::string const& idData,
                        Vector<std::string>& dataStr,
                        Index& nbInd,
                        std::string& paramStr) const;
//
//    /** Return a rapidjson::Value  named list that contains the types of the variables */
    nlohmann::json jsonReturnType() const;

  private:
    Index nbInd_;

    Index nbVar_;

    /** Store the informations  of the mixtures in the form (idData, idModel) with
     * - idData: an arbitrary idData for a model
     * - model: a string which can be converted in an existing model
     * @sa stringToMixture */
    std::map<std::string, std::string> info_;

    /** map: id -> position in dataList_, as typedef-ed above */
    std::map<std::string, int> dataMap_;
//
    const nlohmann::json dataList_;
};

template<typename DataType>

std::string DataHandlerJson::getData(std::string const& idData,
                                  AugmentedData<DataType>& augData,
                                  Index& nbInd,
                                  std::string& paramStr,
                                  typename AugmentedData<DataType>::Type offset) const {
  std::string warnLog;
  typedef typename AugmentedData<DataType>::Type Type;
  typedef typename AugmentedData<Matrix<Type> >::MisVal MisVal;

  MisValParser<Type> mvp(offset);

  if (dataMap_.find(idData) != dataMap_.end()) { // check if the data requested is present in the input data
    Index pos = dataMap_.at(idData); // get the index of the element of the rList_ corresponding to idData
    nbInd = nbInd_; // return the number of individuals
    augData.resizeArrays(nbInd_);
    nlohmann::json currVar = dataList_[pos]; // get current named list
    nlohmann::json data = currVar["data"]; // get the data field in the Rcpp object
    paramStr = currVar["paramStr"];
    for (Index i = 0; i < nbInd_; ++i) {
      Type val;
      MisVal misVal;
      std::string currStr = data[i].get<std::string>();
      bool isValid = mvp.parseStr(currStr,
                                  val,
                                  misVal);
      if (isValid) {
        if (misVal.first == present_) {
          augData.setPresent(i, val);
          continue;
        }
        else {
          augData.setMissing(i, misVal);
          continue;
        }
      }
      else {
        std::stringstream sstm;
        sstm << "In " << idData << ", individual i: " << i << " present an error. "
             << currStr << " is not recognized as a valid format." << std::endl;
        warnLog += sstm.str();
      }
    }
  }
  else {
    std::stringstream sstm;
    sstm << "Data from the variable: " << idData << " has been requested but is absent from the provided data. "
         << "Please check that all the necessary data is provided." << std::endl;
    warnLog += sstm.str();
  }

  return warnLog;
}

} /* namespace mixt */
#endif /* MIXT_DATAHANDLERJSON_H_ */
