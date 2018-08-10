/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#ifndef NEGATIVEBINOMIAL_H
#define NEGATIVEBINOMIAL_H

#include <vector>
#include <set>

#include "../../../Data/mixt_AugmentedData.h"
#include "../../../Data/mixt_ConfIntDataStat.h"
#include "NegativeBinomialLikelihood.h"
#include "NegativeBinomialSampler.h"

namespace mixt {

class NegativeBinomial {
public:
	static const std::string name;

	typedef Vector<int> Data;
	typedef ConfIntDataStat<int> DataStat;
	typedef NegativeBinomialSampler Sampler;
	typedef NegativeBinomialLikelihood Likelihood;

	NegativeBinomial(const std::string& idName, int nbClass, Vector<Real>& param);

	Vector<bool> acceptedType() const;

	int computeNbFreeParameters() const;

	bool hasModalities() const;

	std::string setData(const std::string& paramStr,
			AugmentedData<Vector<int> >& augData, RunMode mode);

	std::string mStep(const Vector<std::set<Index>>& classInd);

	Real estimateN(const Vector<int>& x, Real n0) const;

	Real estimateP(const Vector<int>& x, Real n) const;

	std::pair<Real, Real> evalFuncDeriv(const Vector<int>& x, Real n) const;

	std::vector<std::string> paramNames() const;

	void writeParameters() const;

	std::string checkSampleCondition(
			const Vector<std::set<Index>>& classInd) const;

	void initParam();

private:
	std::string idName_;
	int nClass_; /// Number of classes
	Vector<Real>& param_;
	Vector<int>* p_data_;
};

} // namespace mixt

#endif // NEGATIVEBINOMIAL_H
