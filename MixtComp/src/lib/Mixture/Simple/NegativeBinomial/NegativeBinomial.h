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
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#ifndef NEGATIVEBINOMIAL_H
#define NEGATIVEBINOMIAL_H

#include <vector>
#include <set>

#include "../../../Data/AugmentedData.h"
#include "../../../Data/ConfIntDataStat.h"
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
