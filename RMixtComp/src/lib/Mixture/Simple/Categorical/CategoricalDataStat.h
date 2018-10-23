/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 1, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CATEGORICALDATASTAT_H
#define MIXT_CATEGORICALDATASTAT_H

#include "../../../LinAlg/mixt_LinAlg.h"
#include "../../../Data/mixt_AugmentedData.h"

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

#endif // MIXT_CATEGORICALDATASTAT_H
