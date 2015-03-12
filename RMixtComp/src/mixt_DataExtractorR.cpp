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

#include <iostream>
#include "mixt_DataExtractorR.h"
#include "MixtComp/src/Various/mixt_Def.h"
#include "MixtComp/src/LinAlg/mixt_LinAlg.h"

namespace mixt
{

DataExtractorR::DataExtractorR()
{}

DataExtractorR::~DataExtractorR()
{}

/** Export function for categorical model */
void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<Matrix<int> >* p_augData,
                                const Matrix<std::vector<std::pair<int, Real> > >* p_dataStatStorage)
{
#ifdef MC_DEBUG
  std::cout << "DataExtractorR::exportVals, int" << std::endl;
#endif
  Rcpp::IntegerMatrix dataR(p_augData->data_.rows(), // matrix to store the completed data set
                            p_augData->data_.cols());
  Rcpp::List missingData; // list to store all the missing values in a linear format

  for (int i = 0; i < p_augData->data_.rows(); ++i)
  {
#ifdef MC_DEBUG
    std::cout << "\ti: " << i << ", j: " << 0 << std::endl;
#endif
    dataR(i, 0) = p_augData->data_(i, 0); // direct data copy for all values. Imputation has already been carried out by the datastatcomputer at this point.
    if (p_augData->misData_(i, 0).first != present_)
    {
#ifdef MC_DEBUG
      std::cout << "not present_" << std::endl;
#endif
      Rcpp::List currList; // storage for the current missing value
      currList.push_back(i + 1); // store position, R matrices rows start at 1
#ifdef MC_DEBUG
      std::cout << "p_dataStatStorage->elt(i, j).size(): " << (*p_dataStatStorage)(i, j).size() << std::endl;
#endif
      for (std::vector<std::pair<int, Real> >::const_iterator itVec = (*p_dataStatStorage)(i, 0).begin();
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
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = missingData);
}

/** Export function for classes (called from the composer) */
void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<Vector<int> >& augData,
                                const Matrix<Real>& tikC)
{
  Rcpp::IntegerVector dataR(tikC.rows()); // vector to store the completed data set
  Rcpp::NumericMatrix tikR(tikC.rows(),
                           tikC.cols()); // the empirical tik are completely exported, instead of the predominant modalities as in other categorical variables

  for (int i = 0; i < tikC.rows(); ++i)
  {
    dataR(i) = augData.data_(i) + 1; // direct data copy for all values. Imputation has already been carried out by the datastatcomputer at this point.
    for (int j = 0; j < tikC.cols(); ++j)
    {
      tikR(i, j) = tikC(i, j);
    }
  }
  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = tikR);
}

/** Export function for gaussian model */
void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<Matrix<Real> >* p_augData,
                                const Matrix<RowVector<Real> >* p_dataStatStorage)
{
  Rcpp::NumericMatrix dataR(p_augData->data_.rows(), // matrix to store the completed data set
                            p_augData->data_.cols());
  Rcpp::List missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < p_augData->data_.rows(); ++i)
  {
    dataR(i, 0) = p_augData->data_(i, 0);
    if (p_augData->misData_(i, 0).first != present_)
    {
      Rcpp::List currList; // storage for the current missing value
      currList.push_back(i + 1); // R matrices rows start at 1
      currList.push_back((*p_dataStatStorage)(i, 0)[0]); // expectation
      currList.push_back((*p_dataStatStorage)(i, 0)[1]); // left bound
      currList.push_back((*p_dataStatStorage)(i, 0)[2]); // right bound

      missingData.push_back(currList);
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = missingData);
}

/** Export function for Poisson model */
void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<Matrix<int> >* p_augData,
                                const Matrix<RowVector<int> >* p_dataStatStorage)
{
  Rcpp::IntegerMatrix dataR(p_augData->data_.rows(), // matrix to store the completed data set
                            p_augData->data_.cols());
  Rcpp::List missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < p_augData->data_.rows(); ++i)
  {
    dataR(i, 0) = p_augData->data_(i, 0);
    if (p_augData->misData_(i, 0).first != present_)
    {
      Rcpp::List currList; // storage for the current missing value
      currList.push_back(i + 1); // R matrices rows start at 1
      currList.push_back((*p_dataStatStorage)(i, 0)[0]); // expectation
      currList.push_back((*p_dataStatStorage)(i, 0)[1]); // left bound
      currList.push_back((*p_dataStatStorage)(i, 0)[2]); // right bound

      missingData.push_back(currList);
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = missingData);
};

Rcpp::List DataExtractorR::rcppReturnVal() const
{
  return data_;
}

} // namespace mixt
