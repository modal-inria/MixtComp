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
#include "MixtComp/src/Various/mixt_Def.h"
#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DPoint.h"

namespace mixt
{

DataExtractorR::DataExtractorR()
{}

DataExtractorR::~DataExtractorR()
{}

void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<STK::Array2D<int> >* p_augData,
                                const Eigen::Matrix<std::vector<std::pair<int, STK::Real> >,
                                                    Eigen::Dynamic,
                                                    Eigen::Dynamic>* p_dataStatStorage)
{
#ifdef MC_DEBUG
  std::cout << "DataExtractorR::exportVals, int" << std::endl;
#endif
  Rcpp::IntegerMatrix dataR(p_augData->data_.sizeRows(), // matrix to store the completed data set
                            p_augData->data_.sizeCols());
  Rcpp::List missingData; // list to store all the missing values in a linear format

  for (int i = 0; i < p_augData->data_.sizeRows(); ++i)
  {
#ifdef MC_DEBUG
    std::cout << "\ti: " << i << ", j: " << 0 << std::endl;
#endif
    if (p_augData->misData_(i, 0).first == present_)
    {
#ifdef MC_DEBUG
      std::cout << "present_" << std::endl;
#endif
      dataR(i, 0) = p_augData->data_(i, 0);
    }
    else
    {
#ifdef MC_DEBUG
      std::cout << "not present_" << std::endl;
#endif
      Rcpp::List currList; // storage for the current missing value
      currList.push_back(i + 1); // R matrices rows start at 1
#ifdef MC_DEBUG
      std::cout << "p_dataStatStorage->elt(i, j).size(): " << (*p_dataStatStorage)(i, j).size() << std::endl;
#endif
      for (std::vector<std::pair<int, STK::Real> >::const_iterator itVec = (*p_dataStatStorage)(i, 0).begin();
           itVec != (*p_dataStatStorage)(i, 0).end();
           ++itVec)
      {
#ifdef MC_DEBUG
        std::cout << "itVec->first: " << itVec->first << ", itVec->second: " << itVec->second << std::endl;
#endif
        currList.push_back(itVec->first ); // current modality
        currList.push_back(itVec->second); // probability of the modality
      }
      missingData.push_back(currList);
      dataR(i, 0) = (*p_dataStatStorage)(i, 0)[0].first; // imputation by the mode
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = missingData);
}

void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<STK::Array2D<STK::Real> >* p_augData,
                                const STK::Array2D<STK::Array2DPoint<STK::Real> >* p_dataStatStorage)
{
  Rcpp::NumericMatrix dataR(p_augData->data_.sizeRows(), // matrix to store the completed data set
                            p_augData->data_.sizeCols());
  Rcpp::List missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < p_augData->data_.sizeRows(); ++i)
  {
    if (p_augData->misData_(i, 0).first == present_)
    {
      dataR(i, 0) = p_augData->data_(i, 0);
    }
    else
    {
      Rcpp::List currList; // storage for the current missing value
      currList.push_back(i + 1); // R matrices rows start at 1
      currList.push_back(p_dataStatStorage->elt(i, 0)[0]); // expectation
      currList.push_back(p_dataStatStorage->elt(i, 0)[1]); // left bound
      currList.push_back(p_dataStatStorage->elt(i, 0)[2]); // right bound

      missingData.push_back(currList);

      dataR(i, 0) = p_dataStatStorage->elt(i,0)[0]; // imputation by the expectation
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = missingData);
}

/** Export function for Poisson model */
void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<STK::Array2D<int> >* p_augData,
                                const STK::Array2D<STK::Array2DPoint<int> >* p_dataStatStorage)
{
  Rcpp::NumericMatrix dataR(p_augData->data_.sizeRows(), // matrix to store the completed data set
                            p_augData->data_.sizeCols());
  Rcpp::List missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < p_augData->data_.sizeRows(); ++i)
  {
    dataR(i, 0) = p_augData->data_(i, 0); // simple copy of the current data at the moment
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = missingData);
};

Rcpp::List DataExtractorR::rcppReturnVal() const
{
  return data_;
}

} // namespace mixt
