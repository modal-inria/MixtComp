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
#include <Rcpp.h>
#include "stkpp/projects/DManager/include/STK_IDataHandler.h"
#include "stkpp/projects/STatistiK/include/STK_Stat_Functors.h"
#include "mixt_AugmentedData.h"

namespace mixt
{

/** combination of a sample number and column number to identify a value */
typedef std::pair<int, int> pos;

class DataHandlerR: public STK::IDataHandler
{
  public:
    /** map: id -> vector of indices in rList_ */
    typedef std::map<std::string, std::vector<int> > DataMap;

    /** default constructor */
    DataHandlerR();
    /** destructor */
    virtual ~DataHandlerR();

    /** @return the number of samples (the number of rows of the data) */
    inline virtual int nbSample() const { return nbSamples_;}
        /** @return the number of variables (the number of columns of the data) */
    inline virtual int nbVariable() const {return nbVariables_;};

    /** read a data file and its companion description file,
      and fill the infoMap_ (id -> model) and dataMap_ (id -> vector of positions)
      members */
    bool readDataFromRList(Rcpp::List);

    /** return in an Array2D<int> the data with the given idData */
    virtual void getData(std::string const& idData, STK::Array2D<int>& data, int& nbVariable) const;
    /** return in an Array2D<Real> the data with the given idData */
    virtual void getData(std::string const& idData, STK::Array2D<STK::Real>& data, int& nbVariable) const;
    /** return in an Array2D<string> the data with the given idData */
    virtual void getData(std::string const& idData, STK::Array2D<std::string>& data, int& nbVariable) const;

    void getData(std::string const& idData,
                 AugmentedData<STK::Array2D<int> >& augData,
                 int& nbVariable) const;
    
    /** return in an AugmentedData object the missing values of various types */
    void getData(std::string const& idData,
                 AugmentedData<STK::Array2D<STK::Real> >& augData,
                 int& nbVariable) const;

    void getData(std::string const& idData,
                 AugmentedData<STK::Array2D<std::string> >& augData,
                 int& nbVariable) const;
    
    /** write information on the localization of data in the rList */
    void writeDataMap() const;
  private:
    int nbSamples_;
    int nbVariables_;
    /** map: id -> vector of positions in rList_, as typedef-ed above */
    DataMap dataMap_;
    /** A list of the mixtures transmitted by R, as s4 objects */
    Rcpp::List rList_;
    
    /** read data structure independently of the type (integer, numeric, character) */
    template<class Vector>
    void readDataFromRListHelper(int i, Rcpp::S4 s4);

    template<typename Type, typename Vector>
    void getDataHelper(std::string const& idData,
                       AugmentedData<STK::Array2D<Type> >& augData,
                       int& nbVariable) const;
};

template<typename Vector>
void DataHandlerR::readDataFromRListHelper(int i, Rcpp::S4 s4)
{
  std::string modelname = s4.slot("model");
  std::string id = s4.slot("id");
  Rcpp::List ls = s4.slot("augData");
  Vector nv = ls["data"];
  
  nbSamples_ = nv.size(); // overwritten, because check has already been performed on the R side
  addInfo(id, modelname);
  std::vector<int>& v_pos = dataMap_[id]; // dataMap_[id] created if not already existing
  v_pos.push_back(i);
  ++nbVariables_;
}

template<typename Type, typename Vector>
void DataHandlerR::getDataHelper(std::string const& idData,
                                 AugmentedData<STK::Array2D<Type> >& augData,
                                 int& nbVariable) const
{
  typedef typename AugmentedData<STK::Array2D<Type> >::pos pos;

  getData(idData, augData.data_, nbVariable); // data array filling is not affected by the augmented data

#ifdef MC_DEBUG
  std::cout << augData.data_ << std::endl;
#endif

  std::vector<int> const& v_pos = dataMap_.at(idData); // get the elements of the rList_ corresponding to idData

  int index;
  int missingSize = 0;
  int missingFiniteValuesSize = 0;
  int missingIntervalsSize = 0;
  int missingLUIntervalsSize = 0;
  int missingRUIntervalsSize = 0;

  Vector nv_listVals;

  // data range filling
  for (int currVar = 0; currVar < augData.data_.sizeCols(); ++currVar)
  {
    augData.dataRanges_.push_back(std::pair<Type, Type>(STK::Stat::minSafe(augData.data_.col(currVar)),
                                                        STK::Stat::maxSafe(augData.data_.col(currVar))));
  }

  // reserving the augData containers to avoid push_back slowdown
  for (std::vector<int>::const_iterator it = v_pos.begin(); it != v_pos.end(); ++it)
  {
    Rcpp::S4 s4 = rList_[(*it)];
    Rcpp::List ls_augData = s4.slot("augData");

    Rcpp::List ls_listMissing      = ls_augData["listMissing"        ];
    Rcpp::List ls_listFiniteValues = ls_augData["listFiniteValues"   ];
    Rcpp::List ls_listIntervals    = ls_augData["listIntervals"      ];
    Rcpp::List ls_listLUIntervals  = ls_augData["listLUIntervals"    ];
    Rcpp::List ls_listRUIntervals  = ls_augData["listRUIntervals"    ];

    missingSize             += ls_listMissing     .size();
    missingFiniteValuesSize += ls_listFiniteValues.size();
    missingIntervalsSize    += ls_listIntervals   .size();
    missingLUIntervalsSize  += ls_listLUIntervals .size();
    missingRUIntervalsSize  += ls_listRUIntervals .size();
  }

  augData.v_missing_            .reserve(missingSize            );
  augData.v_missingFiniteValues_.reserve(missingFiniteValuesSize);
  augData.v_missingIntervals_   .reserve(missingIntervalsSize   );
  augData.v_missingLUIntervals_ .reserve(missingLUIntervalsSize );
  augData.v_missingRUIntervals_ .reserve(missingRUIntervalsSize );

  int j = augData.data_.firstIdxCols();
  for (std::vector<int>::const_iterator it = v_pos.begin(); it != v_pos.end(); ++it, ++j)
  {
    Rcpp::S4 s4 = rList_[(*it)];
    Rcpp::List ls_augData = s4.slot("augData");

    // filling v_missing_
    Rcpp::List ls_listMissing = ls_augData["listMissing"];
    for (int i = 0; i < ls_listMissing.size(); ++i)
    {
      augData.v_missing_.push_back(pos(ls_listMissing[i] - 1, j));
    }

    // filling v_missingFiniteValues_
    Rcpp::List ls_listFiniteValues = ls_augData["listFiniteValues"];
    for (int i = 0; i < ls_listFiniteValues.size(); ++i)
    {
      Rcpp::List ls_posVal = ls_listFiniteValues[i];
      index = ls_posVal["pos"];
      nv_listVals = ls_posVal["listvals"];
      augData.v_missingFiniteValues_.push_back(
        std::pair<pos, std::vector<Type> >(pos(index - 1, j),
                                           Rcpp::as<std::vector<Type> >(nv_listVals))
      );
    }

    // filling v_missingIntervals_
    Rcpp::List ls_listIntervals = ls_augData["listIntervals"];
    for (int i = 0; i < ls_listIntervals.size(); ++i)
    {
      Rcpp::List ls_posVal = ls_listIntervals[i];
      index = ls_posVal["pos"];
      nv_listVals = ls_posVal["listvals"];
      augData.v_missingIntervals_.push_back(
        std::pair<pos, std::pair<Type, Type> >(pos(index - 1, j),
                                               std::pair<Type, Type>(Type(nv_listVals[0]),
                                                                     Type(nv_listVals[1])))
      );
    }

    // filling v_missingLUIntervals_
    Rcpp::List ls_listLUIntervals = ls_augData["listLUIntervals"];
    for (int i = 0; i < ls_listLUIntervals.size(); ++i)
    {
      Rcpp::List ls_posVal = ls_listLUIntervals[i];
      index = ls_posVal["pos"];
      Type val = ls_posVal["listvals"];
      augData.v_missingLUIntervals_.push_back(
        std::pair<pos, Type>(pos(index - 1, j),
                             val)
      );
    }

    // filling v_missingRUIntervals_
    Rcpp::List ls_listRUIntervals = ls_augData["listRUIntervals"];
    for (int i = 0; i < ls_listRUIntervals.size(); ++i)
    {
      Rcpp::List ls_posVal = ls_listRUIntervals[i];
      index = ls_posVal["pos"];
      Type val = ls_posVal["listvals"];
      augData.v_missingRUIntervals_.push_back(
        std::pair<pos, Type>(pos(index - 1, j),
                             val)
      );
    }
  }
}

} /* namespace mixt */
#endif /* MIXT_DATAHANDLERR_H_ */
