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
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef CATEGORICAL_H
#define CATEGORICAL_H

#include <LinAlg/LinAlg.h>
#include <vector>
#include <set>

#include "CategoricalDataStat.h"
#include "CategoricalSampler.h"
#include "CategoricalLikelihood.h"

namespace mixt {

class Categorical {

public:
	static const std::string name;

	typedef Vector<int> Data;
	typedef CategoricalDataStat DataStat;
	typedef CategoricalSampler Sampler;
	typedef CategoricalLikelihood<int> Likelihood;

	Categorical(const std::string& idName, int nbClass, Vector<Real>& param);

	Vector<bool> acceptedType() const;

	int computeNbFreeParameters() const;

	std::string mStep(const Vector<std::set<Index>>& classInd);

	std::vector<std::string> paramNames() const;

	std::string setData(std::string& paramStr,
			AugmentedData<Vector<int> >& augData, RunMode mode);

	void writeParameters() const;

	std::string checkSampleCondition(
			const Vector<std::set<Index>>& classInd) const;

	bool hasModalities() const;

	void initParam();

private:
	std::string idName_;

	Index nClass_;

	Index nModality_;

	Vector<int>* p_data_;

	Vector<Real>& param_;
};

} // namespace mixt

#endif
