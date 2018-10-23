/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKLIKELIHOOD_H
#define MIXT_RANKLIKELIHOOD_H

#include <vector>

#include "../../LinAlg/mixt_LinAlg.h"

namespace mixt {

/** Harmonic mean estimator of the marginal likelihood from observations of the
 * conditional likelihood */
class RankLikelihood {
public:
	void init(int nbInd, int nbClass, int nbObs);

	void observe(int i, int k, Real val);

	void getHMean(Matrix<Real>& mat) const;

private:
	int nbInd_;
	int nbClass_;
	int nbObs_;

	Matrix<std::vector<Real> > storage_;
};

} // namespace mixt

#endif // MIXT_RANKLIKELIHOOD_H
