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
