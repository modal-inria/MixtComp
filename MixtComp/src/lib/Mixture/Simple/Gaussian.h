/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 15th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <vector>
#include <set>

#include "LinAlg/mixt_LinAlg.h"
#include "Sampler/mixt_GaussianSampler.h"
#include "Data/mixt_ConfIntDataStat.h"
#include "Likelihood/mixt_GaussianLikelihood.h"

namespace mixt {

class Gaussian {
public:
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

	/** Statistic object to describe Normal law */
	NormalStatistic normal_;
};

}

#endif /* GAUSSIAN_H */
