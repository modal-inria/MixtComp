/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_DATAHANDLERCSV_H_
#define MIXT_DATAHANDLERCSV_H_

#include <vector>
#include <map>
#include "boost/regex.hpp"

#include "Arrays/include/STK_Array2D.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Various/mixt_Def.h"
#include "../Various/mixt_IO.h"

#include "DManager/include/STK_ReadWriteCsv.h"

namespace mixt
{

class DataHandlerCsv
{
  public:
    /** map: idData -> model */
    typedef std::map<std::string, std::string> InfoMap;
    /** map: id -> vector of indices in rList_ */
    typedef std::map<std::string, std::vector<int> > DataMap;

    /** default constructor */
    DataHandlerCsv(std::string fileData,
                   std::string fileDesc);
    /** destructor */
    virtual ~DataHandlerCsv();

    /** @return the number of samples (the number of rows of the data) */
    int nbSample() const {return nbSamples_;};
        /** @return the number of variables (the number of columns of the data) */
    int nbVariable() const {return nbVariables_;};

    /** read a data file and its companion description file,
      and fill the infoMap_ (id -> model) and dataMap_ (id -> vector of positions)
      members */
    bool listData();
    
    /** @return the map with the idDatas and idModel of the models */
    InfoMap const& info() const{return info_;};
    bool addInfo(std::string const& idData, std::string const& idModel);
    void writeInfo(std::ostream& os) const;
    /** write information on the localization of data in the rList */
    void writeDataMap() const;

    template<typename Type>
    void getData(std::string const& idData,
                 AugmentedData<Matrix<Type> >& augData,
                 int& nbVariable) const;
  private:
    /** objects to parse the csv files */
    STK::ReadWriteCsv dataContent_;
    STK::ReadWriteCsv descContent_;

    int nbSamples_;
    int nbVariables_;

    /** Store the informations  of the mixtures in the form (idData, idModel) with
     * - idData: an arbitrary idData for a model
     * - model: a string which can be converted in an existing model
     * @sa stringToMixture */
    InfoMap info_;
    /** map: id -> vector of columns indices, as typedef-ed above */
    DataMap dataMap_;
};

template<typename Type>
void DataHandlerCsv::getData(std::string const& idData,
                             AugmentedData<Matrix<Type> >& augData,
                             int& nbVariable) const
{
#ifdef MC_DEBUG
  std::cout << "DataHandlerCsv::getDataHelper()" << std::endl;
  std::cout << "\tidData: " << idData << std::endl;
//  std::cout << augData.data_ << std::endl;
#endif

  std::vector<int> const& v_pos = dataMap_.at(idData); // get the elements of the rList_ corresponding to idData
  nbVariable = v_pos.size();// resize the data
  augData.resizeArrays(nbSamples_, nbVariable); // R has already enforced that all data has the same number of rows

  // definitions of regular expressions to capture / reject numbers
  std::string strNumber("((?:-|\\+)?(?:\\d+(?:\\.\\d*)?)|(?:\\.\\d+))");
  std::string strBlank(" *");
  std::string strLeftPar(" *\\[ *");
  std::string strRightPar(" *\\] *");
  std::string centralColon(" *: *");
  std::string minusInf("-inf");
  std::string plusInf("\\+inf");

  boost::regex reNumber(strNumber);
  boost::regex reValue(strBlank + // " *(-*[0-9.]+) *"
                       strNumber +
                       strBlank);
  boost::regex reFiniteValues(" *\\{.*\\} *");
  boost::regex reIntervals(strLeftPar + // " *\\[ *(-*[0-9.]+) *: *(-*[0-9.]+) *\\] *"
                           strNumber +
                           centralColon +
                           strNumber +
                           strRightPar);
  boost::regex reLuIntervals(strLeftPar +  // " *\\[ *-inf *: *(-*[0-9.]+) *\\] *"
                             minusInf +
                             centralColon +
                             strNumber +
                             strRightPar);
  boost::regex reRuIntervals(strLeftPar + // " *\\[ *(-*[0-9.]+) *: *\\+inf *\\] *"
                             strNumber +
                             centralColon +
                             plusInf +
                             strRightPar);

  boost::smatch matches;

  int j = 0; // index of the current variable
  for (std::vector<int>::const_iterator it = v_pos.begin(); it != v_pos.end(); ++it, ++j) // loop on the elements of the rList_ corresponding to idData
  {
#ifdef MC_DEBUG
    std::cout << "DataHandlerCsv::getData" << std::endl;
    std::cout << "\tj: " << j << std::endl;
#endif
    int currVar = (*it); // current column in input
//    Rcpp::CharacterVector data = currVar("data");
    for (int i = 1; i < nbSamples_ + 1; ++i) // data begins at the second line
    {
#ifdef MC_DEBUG
    std::cout << "DataHandlerR::getData" << std::endl;
    std::cout << "\ti: " << i << "\tj: " << j << std::endl;
#endif
      std::string currStr(dataContent_(i, currVar));

      if (boost::regex_match(currStr, matches, reValue))
      {
        augData.setPresent(i, j, str2type<Type>(matches[1].str()));
        continue;
      }

      if (boost::regex_match(currStr, matches, reFiniteValues))
      {
        std::string::const_iterator start = currStr.begin();
        std::string::const_iterator end   = currStr.end();
        std::list<Type> results;
        boost::smatch m;
        while (boost::regex_search(start, end, m, reNumber ))
        {
          results.push_back(str2type<Type>(m[0].str()));
          start = m[0].second;
        }
        if (results.size() > 0)
        {
          typename AugmentedData<Matrix<Type> >::MisVal misVal;
          misVal.first = missingFiniteValues_;
          misVal.second.reserve(results.size());;
          misVal.second.insert(misVal.second.end(),
                               results.begin(),
                               results.end());
          augData.setMissing(i, j, misVal);
          continue;
        }
      }

      if (boost::regex_match(currStr, matches, reIntervals))
      {
        typename AugmentedData<Matrix<Type> >::MisVal misVal;
        misVal.first = missingIntervals_;
        misVal.second.resize(2);
        misVal.second[0] = str2type<Type>(matches[1].str());
        misVal.second[1] = str2type<Type>(matches[2].str());
        augData.setMissing(i, j, misVal);
#ifdef MC_DEBUG
        std::cout << "\tmissingIntervals_" << std::endl;
        std::cout << augData.misData_[i][j].second.size() << std::endl;
#endif
        continue;
      }

      if (boost::regex_match(currStr, matches, reLuIntervals))
      {
        typename AugmentedData<Matrix<Type> >::MisVal misVal;
        misVal.first = missingLUIntervals_;
        misVal.second.push_back(str2type<Type>(matches[1].str()));
        augData.setMissing(i, j, misVal);
        continue;
      }

      if (boost::regex_match(currStr, matches, reRuIntervals))
      {
        typename AugmentedData<Matrix<Type> >::MisVal misVal;
        misVal.first = missingRUIntervals_;
        misVal.second.push_back(str2type<Type>(matches[1].str()));
        augData.setMissing(i, j, misVal);
        continue;
      }

      // if missing value is none of the above...
      typename AugmentedData<Matrix<Type> >::MisVal misVal;
      misVal.first = missing_;
      augData.setMissing(i, j, misVal);
    }
  }
  augData.computeRanges();
  augData.removeMissing();
}

} /* namespace mixt */
#endif /* MIXT_DATAHANDLERCSV_H_ */
