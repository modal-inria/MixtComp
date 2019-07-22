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
 *  Created on: July 1, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef CATEGORICALDATASTAT_H
#define CATEGORICALDATASTAT_H

#include <LinAlg/LinAlg.h>
#include "../../../Data/AugmentedData.h"

namespace mixt {

class CategoricalDataStat {
public:
	CategoricalDataStat(AugmentedData<Vector<int> >& augData, Real confidenceLevel);

	void setNbIndividual(int nbInd);

	void sampleVals(int sample, int iteration, int iterationMax);
	void imputeData(int ind);

	const Vector<std::vector<std::pair<int, Real> > >& getDataStatStorage() const {
		return dataStatStorage_;
	}
	;

private:
	/** Reference to augmented data */
	AugmentedData<Vector<int> >& augData_;

	/** Sparse description of the missing values */
	Vector<std::vector<std::pair<int, Real> > > dataStatStorage_;

	/** Vector of vectors to count sampled values across iterations, for the current individual,
	 * stat_(i)(n)
	 * i: individual
	 * n: sampled value */
	Vector<Vector<Real> > stat_;

	/** Confidence level */
	Real confidenceLevel_;

	void sample(int ind);
};

} // namespace mixt

#endif // CATEGORICALDATASTAT_H
