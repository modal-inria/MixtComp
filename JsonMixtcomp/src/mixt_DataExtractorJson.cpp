/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016
*/

/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/



#include <iostream>
#include "mixt_DataExtractorJson.h"
#include "../../MixtComp/src/LinAlg/mixt_LinAlg.h"
#include "../../MixtComp/src/Various/mixt_Enum.h"

namespace mixt {

void DataExtractorJson::setNbMixture(int nbMixture) {
  mixtureName_.resize(nbMixture);
  data_.resize(nbMixture);
}

/** Export function for categorical model */
void DataExtractorJson::exportVals(int indexMixture,
                                   bool hasModalities,
                                   std::string idName,
                                   const AugmentedData<Vector<int> >& augData,
                                   const Vector<std::vector<std::pair<int, Real> > >& dataStatStorage) {
  Index offset = hasModalities ? minModality : 0;
  nlohmann::json dataJson; // vector to store the completed data set
  nlohmann::json missingData; // list to store all the missing values in a linear format

  for (int i = 0; i < augData.data_.rows(); ++i) {
    dataJson[i] = augData.data_(i) + minModality; // direct data copy for all values. Imputation has already been carried out by the datastatcomputer at this point.

    if (augData.misData_(i).first != present_) {
      nlohmann::json currList; // storage for the current missing value
      currList.push_back(i + minIndex); // store position, R matrices rows start at 1

      for (std::vector<std::pair<int, Real> >::const_iterator itVec = dataStatStorage(i).begin();
           itVec != dataStatStorage(i).end();
           ++itVec) {

        currList.push_back(nlohmann::json (itVec->first + minModality,  // current modality)
                                              itVec->second)); // probability of the modality
      }

      missingData.push_back(currList);
    }
  }
  mixtureName_[indexMixture] = idName;
  data_[indexMixture]["completed"] = dataJson;
  data_[indexMixture]["stat"] = missingData;
}


/** Export function for classes (called from the composer) */
void DataExtractorJson::exportVals(int indexMixture,
                                std::string idName,
                                const AugmentedData<Vector<Index> >& augData,
                                const Matrix<Real>& tikC) {
  nlohmann::json dataJson; // vector to store the completed data set
  nlohmann::json tikJson; // the empirical tik are completely exported, instead of the predominant modalities as in other categorical variables
  for (int i = 0; i < tikC.rows(); ++i) {
    dataJson[i] = augData.data_(i) + minModality; // direct data copy for all values. Imputation has already been carried out by the datastatcomputer at this point.
    for (int j = 0; j < tikC.cols(); ++j) {
      tikJson[i][j] = tikC(i, j);
    }
  }

  mixtureName_[indexMixture] = idName;
  data_[indexMixture]["completed"] = dataJson;
  data_[indexMixture]["stat"] = tikJson;
}


/** Export function for gaussian model */
void DataExtractorJson::exportVals(int indexMixture,
                                   bool hasModalities,
                                   std::string idName,
                                   const AugmentedData<Vector<Real> >& augData,
                                   const Vector<RowVector<Real> >& dataStatStorage) {
  nlohmann::json dataJson; // vector to store the completed data set
  nlohmann::json missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < augData.data_.rows(); ++i) {
    dataJson[i] = augData.data_(i);
    if (augData.misData_(i).first != present_) {
      nlohmann::json currList; // storage for the current missing value
      currList.push_back(i + 1); // R matrices rows start at 1
      currList.push_back(dataStatStorage(i)[0]); // median
      currList.push_back(dataStatStorage(i)[1]); // left bound
      currList.push_back(dataStatStorage(i)[2]); // right bound

      missingData.push_back(currList);
    }
  }

  mixtureName_[indexMixture] = idName;
  data_[indexMixture]["completed"] = dataJson;
  data_[indexMixture]["stat"] = missingData;
}


/** Export function for Ordinal and Poisson model */
void DataExtractorJson::exportVals(int indexMixture,
                                   bool hasModalities,
                                   std::string idName,
                                   const AugmentedData<Vector<int> >& augData,
                                   const Vector<RowVector<int> >& dataStatStorage) {
  Index offset = hasModalities ? minModality : 0;
  nlohmann::json dataJson; // vector to store the completed data set
  nlohmann::json missingData; // list to store all the missing values in a linear format

  // basic copy of the data to the export object
  for (int i = 0; i < augData.data_.rows(); ++i) {
    dataJson[i] = augData.data_(i);
    if (augData.misData_(i).first != present_) {
      nlohmann::json currList; // storage for the current missing value
      currList.push_back(i + 1); // R matrices rows start at 1
      currList.push_back(dataStatStorage(i)[0]); // median
      currList.push_back(dataStatStorage(i)[1]); // left bound
      currList.push_back(dataStatStorage(i)[2]); // right bound

      missingData.push_back(currList);
    }
  }

  mixtureName_[indexMixture] = idName;
  data_[indexMixture]["completed"] = dataJson;
  data_[indexMixture]["stat"] = missingData;
}

/** Export function for Rank model */
void DataExtractorJson::exportVals(int indexMixture,
                                std::string idName,
                                const Vector<RankIndividual>& data,
                                const std::vector<RankStat>& dataStat) {
  int nbInd = data.rows();
  int nbPos = data(0).nbPos();

  std::list<nlohmann::json> dataJson; // List to store the completed data set, on element per individual
  std::list<nlohmann::json> statJson; // List to store the statistics on partially observed data, one element per partially observed individual

  for (int i = 0, ie = nbInd; i < ie; ++i) {
    nlohmann::json rankJson(nbPos);
    for (int p = 0; p < nbPos; ++p) {
      rankJson[p] = data(i).x().o()(p) + minModality;
    }
    dataJson.push_back(rankJson);

    if (!data(i).allPresent()) {
      const std::list<std::pair<RankVal, Real> >& statStorageMu = dataStat[i].statStorageMu(); // helper reference to point to current statStorage
      nlohmann::json individualProba; // list of pairs {vector representing rank, proba} for the current individual

      individualProba.push_back(i); // first element in the list is the index

      for (std::list<std::pair<RankVal, Real> >::const_iterator it = statStorageMu.begin(), ite = statStorageMu.end();
           it != ite;
           ++it) {
        const RankVal& rankCPP = it->first; // current rank in C++
        nlohmann::json rankJson(nbPos); // current rank in R
        for (int p = 0; p < nbPos; ++p) {
          rankJson[p] = rankCPP.o()(p) + minModality;
        }
        nlohmann::json rankInd = rankJson;
        rankInd.push_back(it->second);
        individualProba.push_back(rankInd);
      }

      statJson.push_back(individualProba);
    }
  }

  mixtureName_[indexMixture] = idName;
  data_[indexMixture]["completed"] = dataJson;
  data_[indexMixture]["stat"] = statJson;

}

nlohmann::json DataExtractorJson::jsonReturnVal() const {
  nlohmann::json mixtureNameJson ;
  nlohmann::json dataJson;

  for (int i = 0; i < mixtureName_.size(); ++i) {
#ifdef MC_VERBOSE
    std::cout << mixtureName_[i] << std::endl;
#endif
    dataJson[mixtureName_[i]] = data_[i];
  }

  return dataJson;
}

} // namespace mixt
