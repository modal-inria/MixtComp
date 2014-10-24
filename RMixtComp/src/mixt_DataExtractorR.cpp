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
 *  Created on: July 7, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_DataExtractorR.h"

namespace mixt
{

DataExtractorR::DataExtractorR()
{}

DataExtractorR::~DataExtractorR()
{}

void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<STK::Array2D<int> >* p_augData,
                                const std::map<int, std::map<int, std::vector< std::pair<int, STK::Real> > > >* p_dataStatStorage)
{
  Rcpp::IntegerMatrix dataR(p_augData->data_.sizeRows(), // matrix to store the completed data set
                            p_augData->data_.sizeCols());
  Rcpp::List missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < p_augData->data_.sizeRows(); ++i)
  {
    for (int j = 0; j < p_augData->data_.sizeCols(); ++j)
    {
      dataR(i, j) = p_augData->data_(i, j);
    }
  }

  // imputation of missing value and export of their statistics
  int currMissSample = 0; // index of current missing value in the linear output
  std::map<int, std::map<int, std::pair<MisType, std::vector<int>   > > >::const_iterator itSamplePos;
  std::map<int, std::map<int, std::vector<std::pair<int, STK::Real> > > >::const_iterator itSampleData;
  for(itSamplePos  = p_augData->misData_.begin(),
      itSampleData = p_dataStatStorage->begin();
      itSamplePos  != p_augData->misData_.end() &&
      itSampleData != p_dataStatStorage->end();
      ++itSamplePos,
      ++itSampleData)
  {
    std::map<int, std::pair<MisType, std::vector<int>   > >::const_iterator itVarPos;
    std::map<int, std::vector<std::pair<int, STK::Real> > >::const_iterator itVarData;
    for(itVarPos  = itSamplePos->second.begin(),
        itVarData = itSampleData->second.begin();
        itVarPos  != itSamplePos->second.end() &&
        itVarData != itSampleData->second.end();
        ++itVarPos,
        ++itVarData)
    {
      int i = itSamplePos->first;
      int j = itVarPos->first;
      Rcpp::List currList; // storage for the current missing value
      currList[0] = i + 1; // R matrices rows start at 1
      currList[1] = j + 1; // R matrices cols start at 1
      int currElem;
      std::vector<std::pair<int, STK::Real> >::const_iterator itVec;
      for (currElem = 2,
           itVec = itVarData->second.begin();
           itVec != itVarData->second.end();
           ++currElem,
           ++itVec)
      {
        currList[currElem]     = itVec->first; // current modality
        currList[currElem + 1] = itVec->second; // probability of the modality
      }
      missingData[currMissSample] = currList;
      dataR(i, j) = itVarData->second[0].first; // imputation by the mode
      ++currMissSample;
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("posMissing") = missingData);
}

void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<STK::Array2D<STK::Real> >* p_augData,
                                const std::map<int, std::map<int, STK::Array2DVector<STK::Real> > >* p_dataStatStorage)
{
  Rcpp::NumericMatrix dataR(p_augData->data_.sizeRows(), // matrix to store the completed data set
                            p_augData->data_.sizeCols());
  Rcpp::List missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < p_augData->data_.sizeRows(); ++i)
  {
    for (int j = 0; j < p_augData->data_.sizeCols(); ++j)
    {
      dataR(i, j) = p_augData->data_(i, j);
    }
  }

  // imputation of missing value and export of their statistics
  int currMissSample = 0; // index of current missing value in the linear output
  std::map<int, std::map<int, std::pair<MisType, std::vector<STK::Real>   > > >::const_iterator itSamplePos;
  std::map<int, std::map<int, STK::Array2DVector<STK::Real>                 > >::const_iterator itSampleData;
  for(itSamplePos  = p_augData->misData_.begin(),
      itSampleData = p_dataStatStorage->begin();
      itSamplePos  != p_augData->misData_.end() &&
      itSampleData != p_dataStatStorage->end();
      ++itSamplePos,
      ++itSampleData)
  {
    std::map<int, std::pair<MisType, std::vector<STK::Real>   > >::const_iterator itVarPos;
    std::map<int, STK::Array2DVector<STK::Real>                 >::const_iterator itVarData;
    for(itVarPos  = itSamplePos->second.begin(),
        itVarData = itSampleData->second.begin();
        itVarPos  != itSamplePos->second.end() &&
        itVarData != itSampleData->second.end();
        ++itVarPos,
        ++itVarData)
    {
      int i = itSamplePos->first;
      int j = itVarPos->first;

      Rcpp::List currList; // storage for the current missing value
      currList[0] = i + 1; // R matrices rows start at 1
      currList[1] = j + 1; // R matrices cols start at 1
      currList[2] = itVarData->second[1]; // left bound
      currList[3] = itVarData->second[2]; // right bound
      missingData[currMissSample] = currList;

      dataR(i, j) = itVarData->second[0]; // imputation by the expectation
      ++currMissSample;
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("posMissing") = missingData);
}

Rcpp::List DataExtractorR::rcppReturnVal() const
{
  return data_;
}

} // namespace mixt
