/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RANKLIKELIHOOD_H
#define RANKLIKELIHOOD_H

#include <LinAlg/LinAlg.h>
#include <vector>


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

#endif // RANKLIKELIHOOD_H
