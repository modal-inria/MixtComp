/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <LinAlg/LinAlg.h>
#include <vector>
#include <set>

#include "Data/ConfIntDataStat.h"
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

	std::string checkSampleCondition(
			const Vector<std::set<Index>>& classInd) const;

	void initParam();

private:
	std::string idName_;

	int nClass_;

	Vector<Real>& param_;

	Vector<Real>* p_data_;
};

}

#endif /* GAUSSIAN_H */
