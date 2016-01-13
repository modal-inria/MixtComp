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
                                const AugmentedData<Vector<int> >& augData,
                                const Vector<std::vector<std::pair<int, Real> > >& dataStatStorage) {
  Rcpp::IntegerVector dataR(augData.data_.rows()); // vector to store the completed data set
  Rcpp::List missingData; // list to store all the missing values in a linear format

  for (int i = 0; i < augData.data_.rows(); ++i) {
    dataR(i) = augData.data_(i); // direct data copy for all values. Imputation has already been carried out by the datastatcomputer at this point.

    if (augData.misData_(i).first != present_) {
      Rcpp::List currList; // storage for the current missing value
      currList.push_back(i + 1); // store position, R matrices rows start at 1

      for (std::vector<std::pair<int, Real> >::const_iterator itVec = dataStatStorage(i).begin();
           itVec != dataStatStorage(i).end();
           ++itVec) {

        currList.push_back(Rcpp::List::create(itVec->first + minModality,  // current modality)
                                              itVec->second)); // probability of the modality
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
                                const AugmentedData<Vector<Real> >& augData,
                                const Vector<RowVector<Real> >& dataStatStorage)
{
  Rcpp::NumericVector dataR(augData.data_.rows()); // vector to store the completed data set
  Rcpp::List missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < augData.data_.rows(); ++i)
  {
    dataR(i) = augData.data_(i);
    if (augData.misData_(i).first != present_)
    {
      Rcpp::List currList; // storage for the current missing value
      currList.push_back(i + 1); // R matrices rows start at 1
      currList.push_back(dataStatStorage(i)[0]); // median
      currList.push_back(dataStatStorage(i)[1]); // left bound
      currList.push_back(dataStatStorage(i)[2]); // right bound

      missingData.push_back(currList);
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = missingData);
}

/** Export function for Poisson model */
void DataExtractorR::exportVals(std::string idName,
                                const AugmentedData<Vector<int> >& augData,
                                const Vector<RowVector<int> >& dataStatStorage)
{
  Rcpp::IntegerVector dataR(augData.data_.rows()); // vector to store the completed data set
  Rcpp::List missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < augData.data_.rows(); ++i)
  {
    dataR(i) = augData.data_(i);
    if (augData.misData_(i).first != present_)
    {
      Rcpp::List currList; // storage for the current missing value
      currList.push_back(i + 1); // R matrices rows start at 1
      currList.push_back(dataStatStorage(i)[0]); // median
      currList.push_back(dataStatStorage(i)[1]); // left bound
      currList.push_back(dataStatStorage(i)[2]); // right bound

      missingData.push_back(currList);
    }
  }

  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
                                     Rcpp::Named("stat") = missingData);
};

/** Export function for Rank model */
void DataExtractorR::exportVals(std::string idName,
                                const Vector<RankIndividual>& data,
                                const std::vector<RankStat>& dataStat) {
  int nbInd = data.rows();
  int nbPos = data(0).nbPos();

  std::list<Rcpp::NumericVector> dataR; // List to store the completed data set, on element per individual
  std::list<Rcpp::List> statR; // List to store the statistics on partially observed data, one element per partially observed individual

  for (int i = 0, ie = nbInd; i < ie; ++i) {
    Rcpp::NumericVector rankR(nbPos);
    for (int p = 0; p < nbPos; ++p) {
      rankR(p) = data(i).x().o()(p);
    }
    dataR.push_back(rankR);

    if (!data(i).allPresent()) {
      const std::list<std::pair<RankVal, Real> >& statStorageMu = dataStat[i].statStorageMu(); // helper reference to point to current statStorage
      std::list<Rcpp::List> individualProba; // list of pairs {vector representing rank, proba} for the current individual

      individualProba.push_back(i); // first element in the list is the index

      for (std::list<std::pair<RankVal, Real> >::const_iterator it = statStorageMu.begin(), ite = statStorageMu.end();
           it != ite;
           ++it) {
        const RankVal& rankCPP = it->first; // current rank in C++
        Rcpp::IntegerVector rankR(nbPos); // current rank in R
        for (int p = 0; p < nbPos; ++p) {
          rankR(p) = rankCPP.o()(p);
        }
        individualProba.push_back(Rcpp::List::create(rankR,
                                                     it->second));
      }

      statR.push_back(Rcpp::wrap(individualProba));
    }
  }

  Rcpp::List lsData = Rcpp::wrap(dataR);
  Rcpp::List lsStat = Rcpp::wrap(statR);
  data_[idName] = Rcpp::List::create(Rcpp::Named("completed") = lsData,
                                     Rcpp::Named("stat") = lsStat);
}

Rcpp::List DataExtractorR::rcppReturnVal() const
{
  return data_;
}

} // namespace mixt
