/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef CATEGORICAL_H
#define CATEGORICAL_H

#include <vector>
#include <set>

#include "LinAlg/mixt_LinAlg.h"
#include "Sampler/mixt_CategoricalSampler.h"
#include "Data/mixt_CategoricalDataStat.h"
#include "Likelihood/mixt_CategoricalLikelihood.h"

namespace mixt {

class Categorical {
public:
    typedef Vector<int> Data;
    typedef CategoricalDataStat DataStat;
    typedef CategoricalSampler Sampler;
    typedef CategoricalLikelihood<int> Likelihood;

	Categorical(
			const std::string& idName,
			int nbClass,
			Vector<Real>& param,
			const Vector<std::set<Index> >& classInd);

	Vector<bool> acceptedType() const;

	int computeNbFreeParameters() const;

	void mStep(const Vector<std::set<Index>>& classInd);

	std::vector<std::string> paramNames() const;

	std::string setData(
			std::string& paramStr,
			AugmentedData<Vector<int> >& augData,
			RunMode mode);

	void writeParameters() const;

	std::string checkSampleCondition(const Vector<std::set<Index>>& classInd) const;

	bool hasModalities() const;

	std::string initParam(const Vector<Index>& initObs);

	std::vector<bool> parametersInInterior();

private:
	std::string idName_;

	Index nClass_;

	Index nModality_;

	Vector<int>* p_data_;

	Vector<Real>& param_;
};

} // namespace mixt

#endif
