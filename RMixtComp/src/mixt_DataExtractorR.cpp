/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 7, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <iostream>
#include "mixt_DataExtractorR.h"
#include "MixtComp/src/LinAlg/mixt_LinAlg.h"
#include "MixtComp/src/Various/mixt_Enum.h"

namespace mixt {

void DataExtractorR::setNbMixture(int nbMixture) {
	mixtureName_.resize(nbMixture);
	data_.resize(nbMixture);
}

/** Export function for categorical model */
void DataExtractorR::exportVals(int indexMixture,
		bool hasModalities,
		std::string idName,
		const AugmentedData<Vector<int> >& augData,
		const Vector<std::vector<std::pair<int, Real> > >& dataStatStorage) {
	Index offset = hasModalities ? minModality : 0;
	Rcpp::IntegerVector dataR(augData.data_.rows()); // vector to store the completed data set
	Rcpp::List missingData; // list to store all the missing values in a linear format

	for (int i = 0; i < augData.data_.rows(); ++i) {
		dataR(i) = augData.data_(i) + offset; // direct data copy for all values. Imputation has already been carried out by the datastatcomputer at this point.

		if (augData.misData_(i).first != present_) {
			Rcpp::List currList; // storage for the current missing value
			currList.push_back(i + minIndex); // store position, R matrices rows start at 1

			for (std::vector<std::pair<int, Real> >::const_iterator itVec = dataStatStorage(i).begin();
					itVec != dataStatStorage(i).end();
					++itVec) {

				currList.push_back(Rcpp::List::create(itVec->first + offset,  // current modality)
						itVec->second)); // probability of the modality
			}

			missingData.push_back(currList);
		}
	}

	mixtureName_[indexMixture] = idName;
	data_[indexMixture] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
			Rcpp::Named("stat") = missingData);
}

/** Export function for classes (called from the composer) */
void DataExtractorR::exportVals(int indexMixture,
		std::string idName,
		const AugmentedData<Vector<Index> >& augData,
		const Matrix<Real>& tikC) {
	Rcpp::IntegerVector dataR(tikC.rows()); // vector to store the completed data set
	Rcpp::NumericMatrix tikR(tikC.rows(),
			tikC.cols()); // the empirical tik are completely exported, instead of the predominant modalities as in other categorical variables
	
	Rcpp::CharacterVector colNamesTikR(tikC.cols());
	for(int i = 0; i < tikC.cols(); ++i){
	  colNamesTikR(i) = std::string("k: ") + type2str(i+1);
	}
	colnames(tikR) = colNamesTikR;
	
	for (int i = 0; i < tikC.rows(); ++i) {
		dataR(i) = augData.data_(i) + minModality; // direct data copy for all values. Imputation has already been carried out by the datastatcomputer at this point.
		for (int j = 0; j < tikC.cols(); ++j) {
			tikR(i, j) = tikC(i, j);
		}
	}

	mixtureName_[indexMixture] = idName;
	data_[indexMixture] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
			Rcpp::Named("stat") = tikR);
}

/** Export function for gaussian model */
void DataExtractorR::exportVals(int indexMixture,
		bool hasModalities,
		std::string idName,
		const AugmentedData<Vector<Real> >& augData,
		const Vector<RowVector<Real> >& dataStatStorage) {
	Rcpp::NumericVector dataR(augData.data_.rows()); // vector to store the completed data set
	Rcpp::List missingData; // list to store all the missing values in a linear format

	// basic copy of the data to the export object
	for (int i = 0; i < augData.data_.rows(); ++i) {
		dataR(i) = augData.data_(i);
		if (augData.misData_(i).first != present_) {
			Rcpp::NumericVector currRow(4); // storage for the current missing value
		  currRow(0) = i + 1; // R matrices rows start at 1
		  currRow(1) = dataStatStorage(i)[0]; // median
		  currRow(2) = dataStatStorage(i)[1]; // left bound
		  currRow(3) = dataStatStorage(i)[2]; // right bound
		  
			missingData.push_back(currRow);
		}
		
	}

	mixtureName_[indexMixture] = idName;
	data_[indexMixture] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
			Rcpp::Named("stat") = missingData);
}

/** Export function for Ordinal and Poisson model */
void DataExtractorR::exportVals(int indexMixture,
		bool hasModalities,
		std::string idName,
		const AugmentedData<Vector<int> >& augData,
		const Vector<RowVector<int> >& dataStatStorage) {
	Index offset = hasModalities ? minModality : 0;
	Rcpp::IntegerVector dataR(augData.data_.rows()); // vector to store the completed data set
	Rcpp::List missingData; // list to store all the missing values in a linear format

	// basic copy of the data to the export object
	for (int i = 0; i < augData.data_.rows(); ++i) {
		dataR(i) = augData.data_(i) + offset;
		if (augData.misData_(i).first != present_) {
			Rcpp::NumericVector currRow(4); // storage for the current missing value
			currRow(0) = i + 1; // R matrices rows start at 1
			currRow(1) = dataStatStorage(i)[0] + offset; // median
			currRow(2) = dataStatStorage(i)[1] + offset; // left bound
			currRow(3) = dataStatStorage(i)[2] + offset; // right bound
			
			missingData.push_back(currRow);
		}
	}

	mixtureName_[indexMixture] = idName;
	data_[indexMixture] = Rcpp::List::create(Rcpp::Named("completed") = dataR,
			Rcpp::Named("stat") = missingData);
};

/** Export function for Rank model */
void DataExtractorR::exportVals(int indexMixture,
		std::string idName,
		const Vector<RankIndividual>& data,
		const std::vector<RankStat>& dataStat) {
	int nbInd = data.rows();
	int nbPos = data(0).nbPos();

	std::list<Rcpp::NumericVector> dataR; // List to store the completed data set, on element per individual
	std::list<Rcpp::List> statR; // List to store the statistics on partially observed data, one element per partially observed individual

	for (int i = 0, ie = nbInd; i < ie; ++i) {
		Rcpp::NumericVector rankR(nbPos);
		for (int p = 0; p < nbPos; ++p) {
			rankR(p) = data(i).x().o()(p) + minModality;
		}
		dataR.push_back(rankR);

		if (!data(i).allPresent()) {
			const std::list<std::pair<RankVal, Real> >& statStorageMu = dataStat[i].statStorageMu(); // helper reference to point to current statStorage
			Rcpp::List individualProba; // list of pairs {vector representing rank, proba} for the current individual

			individualProba.push_back(i); // first element in the list is the index

			for (std::list<std::pair<RankVal, Real> >::const_iterator it = statStorageMu.begin(), ite = statStorageMu.end();
					it != ite;
					++it) {
				const RankVal& rankCPP = it->first; // current rank in C++
				Rcpp::IntegerVector rankR(nbPos); // current rank in R
				for (int p = 0; p < nbPos; ++p) {
					rankR(p) = rankCPP.o()(p) + minModality;
				}
				individualProba.push_back(Rcpp::List::create(rankR,
						it->second));
			}

			statR.push_back(Rcpp::wrap(individualProba));
		}
	}

	mixtureName_[indexMixture] = idName;
	Rcpp::List lsData = Rcpp::wrap(dataR);
	Rcpp::List lsStat = Rcpp::wrap(statR);
	data_[indexMixture] = Rcpp::List::create(Rcpp::Named("completed") = lsData,
			Rcpp::Named("stat") = lsStat);
}


/** Export function for Functional model*/
void DataExtractorR::exportVals(int indexMixture,
		std::string idName,
		const Vector<Function>& data) {
	int nbInd = data.rows();

	std::list<Rcpp::NumericVector> dataR; // List to store the completed data set, on element per individual
	std::list<Rcpp::NumericVector> timeR; // List to store the times where the function has been measured, on element per individual

	for (int i = 0; i < nbInd; ++i) {
		const int nbPos = data(i).nTime();
		Rcpp::NumericVector ttR(nbPos);
		Rcpp::NumericVector obsR(nbPos);
		const Vector<Real> xtmp = data(i).x();
		const Vector<Real> ttmp = data(i).t();
		for (int p = 0; p < nbPos; ++p) {
			obsR(p) = xtmp(p);
			ttR(p) = ttmp(p);
		}
		dataR.push_back(obsR);
		timeR.push_back(ttR);
	}

	mixtureName_[indexMixture] = idName;
	Rcpp::List lsData = Rcpp::wrap(dataR);
	Rcpp::List lsStat = Rcpp::wrap(timeR);
	data_[indexMixture] = Rcpp::List::create(Rcpp::Named("data") = lsData,
			Rcpp::Named("time") = lsStat);
}


Rcpp::List DataExtractorR::rcppReturnVal() const {
	Rcpp::CharacterVector mixtureNameR = Rcpp::wrap(mixtureName_);
	Rcpp::List dataR = Rcpp::wrap(data_);
	dataR.attr("names") = mixtureNameR;
	return dataR;
}

} // namespace mixt
