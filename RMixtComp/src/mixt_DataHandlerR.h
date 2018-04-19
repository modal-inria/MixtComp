/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
#include "MixtComp/src/IO/mixt_MisValParser.h"
#include "MixtComp/src/IO/mixt_IO.h"
#include "MixtComp/src/Various/mixt_Enum.h"

namespace mixt {

class DataHandlerR {
  public:
    /** default constructor */
    DataHandlerR(Rcpp::List rList);

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

    /** Return a Rcpp::List named list that contains the types of the variables */
    Rcpp::List rcppReturnType() const;
  private:
    Index nbInd_;

    Index nbVar_;

    /** Store the informations  of the mixtures in the form (idData, idModel) with
     * - idData: an arbitrary idData for a model
     * - model: a string which can be converted in an existing model
     * @sa stringToMixture */
    std::map<std::string, std::string> info_;

    /** map: id -> position in rList_, as typedef-ed above */
    std::map<std::string, int> dataMap_;

    /** A list of the mixtures transmitted by R, as named lists */
    Rcpp::List rList_;
};

template<typename DataType>
std::string DataHandlerR::getData(std::string const& idData,
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
    augData.resizeArrays(nbInd_); // R has already enforced that all data has the same number of rows, and now all mixture are univariate

    Rcpp::List currVar = rList_[pos]; // get current named list
    Rcpp::CharacterVector data = currVar("data"); // get the data field in the Rcpp object

    paramStr = Rcpp::as<std::string>(currVar("paramStr"));

    for (Index i = 0; i < nbInd_; ++i) {
      std::string currStr;
      Type val;
      MisVal misVal;

      currStr = data[i];

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
#endif /* MIXT_DATAHANDLERR_H_ */
