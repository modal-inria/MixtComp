/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Aug 9, 2018
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef STRINGTOAUGMENTEDDATA_H
#define STRINGTOAUGMENTEDDATA_H

#include <string>
#include <vector>

#include "mixt_MisValParser.h"
#include <Data/mixt_AugmentedData.h>
#include <LinAlg/mixt_LinAlg.h>
#include "SGraph.h"

namespace mixt {

/**
 * Equivalent to Datahandler::getData in old architecture.
 */
template<typename DataType>
std::string StringToAugmentedData(const std::string& idName, const std::vector<std::string>& data, AugmentedData<DataType>& augData, Index offset) {
	std::string warnLog;
	typedef typename AugmentedData<DataType>::Type Type;
	typedef typename AugmentedData<Matrix<Type> >::MisVal MisVal;

	MisValParser<Type> mvp(offset);

	Index nbInd = data.size(); // return the number of individuals
	augData.resizeArrays(nbInd); // R has already enforced that all data has the same number of rows, and now all mixture are univariate

	for (Index i = 0; i < nbInd; ++i) {
		std::string currStr;
		Type val;
		MisVal misVal;

		currStr = data[i];

		bool isValid = mvp.parseStr(currStr, val, misVal);
		if (isValid) {
			if (misVal.first == present_) {
				augData.setPresent(i, val);
				continue;
			} else {
				augData.setMissing(i, misVal);
				continue;
			}
		} else {
			std::stringstream sstm;
			sstm << "In " << idName << ", individual i: " << i << " present an error. " << currStr << " is not recognized as a valid format." << std::endl;
			warnLog += sstm.str();
		}
	}
	return warnLog;
}

}

#endif
