/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: September 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Mixture/Rank/RankLikelihood.h>
#include <Various/Constants.h>
#include <iostream>


namespace mixt {
void RankLikelihood::init(int nbInd, int nbClass, int nbObs) {
	nbInd_ = nbInd;
	nbClass_ = nbClass;
	nbObs_ = nbObs;

	storage_.resize(nbInd_, nbClass_);
	for (int k = 0; k < nbClass_; ++k) {
		for (int i = 0; i < nbInd_; ++i) {
			storage_(i, k).clear();
			storage_(i, k).reserve(nbObs_);
		}
	}
}

void RankLikelihood::observe(int i, int k, Real val) {
	storage_(i, k).push_back(val);
}

void RankLikelihood::getHMean(Matrix<Real>& mat) const {
	mat.resize(nbInd_, nbClass_);
	for (int k = 0; k < nbClass_; ++k) {
		for (int i = 0; i < nbInd_; ++i) {
			if (storage_(i, k).size() == 0) {
				mat(i, k) = minInf;
			} else {
				Real val = 0.;
				for (std::vector<Real>::const_iterator it =
						storage_(i, k).begin(), itEnd = storage_(i, k).end();
						it != itEnd; ++it) {
					val += 1. / *it;
				}
				mat(i, k) = -std::log(val / Real(storage_(i, k).size()));
			}
		}
	}
}

} // namespace mixt
