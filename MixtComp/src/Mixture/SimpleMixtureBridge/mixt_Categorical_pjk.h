/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_CATEGORICAL_PJK
#define MIXT_CATEGORICAL_PJK

#include <vector>
#include <set>

#include "Data/mixt_AugmentedData.h"
#include "LinAlg/mixt_LinAlg.h"
#include "Various/mixt_Enum.h"
#include "Mixture/mixt_IMixture.h"

namespace mixt {

class Categorical_pjk {
public:
	Categorical_pjk(
			const std::string& idName,
			int nbClass,
			Vector<Real>& param,
			const Vector<std::set<Index> >& classInd);

	Vector<bool> acceptedType() const;

	int computeNbFreeParameters() const;

	void mStep();

	std::vector<std::string> paramNames() const;

	std::string setData(
			std::string& paramStr,
			AugmentedData<Vector<int> >& augData,
			RunMode mode);

	void writeParameters() const;

	std::string checkSampleCondition() const;

	bool hasModalities() const;

	std::string initParam(const Vector<Index>& initObs);

	std::vector<bool> parametersInInterior();

private:
	std::string idName_;

	Index nClass_;

	Index nModality_;

	Vector<int>* p_data_;

	Vector<Real>& param_;

	const Vector<std::set<Index> >& classInd_;
};

} // namespace mixt

#endif
