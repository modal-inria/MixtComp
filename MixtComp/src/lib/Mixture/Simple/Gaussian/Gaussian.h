/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <vector>
#include <set>

#include <LinAlg/LinAlg.h>
#include <Data/ConfIntDataStat.h>
#include "GaussianLikelihood.h"
#include "GaussianSampler.h"

namespace mixt {

class Gaussian {
public:
	static const std::string name;

	typedef Vector<Real> Data;
	typedef ConfIntDataStat<Real> DataStat;
	typedef GaussianSampler Sampler;
	typedef GaussianLikelihood Likelihood;

	Gaussian(const std::string& idName, int nbClass, Vector<Real>& param);

	Vector<bool> acceptedType() const;

	int computeNbFreeParameters() const;

	bool hasModalities() const;

	std::string setData(const std::string& paramStr,
			AugmentedData<Vector<Real> >& augData, RunMode mode);

	/**
	 * Algorithm based on http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Incremental_algorithm
	 * using the biased estimator which corresponds to the maximum likelihood estimator
	 */
	std::string mStep(const Vector<std::set<Index>>& classInd);

	std::vector<std::string> paramNames() const;

	void writeParameters() const;

	std::string checkSampleCondition(const Vector<std::set<Index>>& classInd) const;

	void initParam();

private:
	std::string idName_;

	int nClass_;

	Vector<Real>& param_;

	Vector<Real>* p_data_;
};

}

#endif /* GAUSSIAN_H */
