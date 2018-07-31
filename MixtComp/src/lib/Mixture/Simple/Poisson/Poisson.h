/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef POISSON_H
#define POISSON_H

#include <vector>
#include <set>

#include "Data/mixt_AugmentedData.h"
#include "Data/mixt_ConfIntDataStat.h"
#include "PoissonLikelihood.h"
#include "PoissonSampler.h"

namespace mixt {

class Poisson {
public:
	typedef Vector<int> Data;
	typedef ConfIntDataStat<int> DataStat;
	typedef PoissonSampler Sampler;
	typedef PoissonLikelihood Likelihood;

	Poisson(const std::string& idName, int nbClass, Vector<Real>& param);

	Vector<bool> acceptedType() const;

	int computeNbFreeParameters() const;

	bool hasModalities() const;

	std::string setData(const std::string& paramStr,
			AugmentedData<Vector<int> >& augData, RunMode mode);

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
	Vector<int>* p_data_;
};

} // namespace mixt

#endif // MIXT_POISSON_K_H
