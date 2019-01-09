/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: May 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LIB_COMPOSER_CLASSDATASTAT_H
#define LIB_COMPOSER_CLASSDATASTAT_H

#include <Composer/ZClassInd.h>
#include "../LinAlg/LinAlg.h"
#include "../Data/AugmentedData.h"

namespace mixt {

class ClassDataStat {
public:
	ClassDataStat(ZClassInd& zClassInd);

	void setNbIndividual(int nbInd);

	void sampleVals(int sample, int iteration, int iterationMax);

	void imputeData(int ind, Matrix<Real>& tik);

	const Matrix<Real>& getDataStatStorage() const {
		return dataStatStorage_;
	}

private:
	/** Reference to augmented data */
	ZClassInd& zClassInd_;

	/** Dense description of the missing values */
	Matrix<Real> dataStatStorage_;

	void sample(int ind);
};

} // namespace mixt

#endif // CLASSDATASTAT_H
