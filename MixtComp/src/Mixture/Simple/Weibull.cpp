/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 17th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "Weibull.h"

#include <cmath>
#include <iostream>

namespace mixt {

std::pair<Real, Real> Weibull::evalFuncDeriv(const Vector<Real>& x, Real k) {
	Index nObs = x.size();
	Vector<Real> xk(nObs);
	for (Index i = 0; i < nObs; ++i) {
		xk(i) = std::pow(x(i), k);
	}

	Vector<Real> lnx(nObs);
	for (Index i = 0; i < nObs; ++i) {
		lnx(i) = std::log(x(i));
	}

	Vector<Real> xklnx(nObs);
	for (Index i = 0; i < nObs; ++i) {
		xklnx(i) = xk(i) * lnx(i);
	}

	Vector<Real> xklnxlnx(nObs);
	for (Index i = 0; i < nObs; ++i) {
		xklnxlnx(i) = xklnx(i) * lnx(i);
	}

	Real sumxk = xk.sum();
	Real sumlnx = lnx.sum();
	Real sumxklnx = xklnx.sum();
	Real sumxklnxlnx = xklnxlnx.sum();

	Real f = sumxklnx / sumxk - 1.0 / k - 1 / Real(nObs) * sumlnx;
	Real df = (sumxklnxlnx * sumxk - sumxklnx * sumxklnx) / (sumxk * sumxk) + 1.0 / (k * k);

	return std::pair<Real, Real>(f, df);
}

Real Weibull::positiveNewtonRaphson(const Vector<Real>& x, Real currK, Real nIt) {
	if (nIt < 0)
		return currK;
	else {
		std::pair<Real, Real> eval = evalFuncDeriv(x, currK);
		Real candidate = currK - eval.first / eval.second;
		if (0.0 < candidate)
			return positiveNewtonRaphson(x, candidate  , nIt - 1);
		else
			return positiveNewtonRaphson(x, currK / 2.0, nIt - 1);
	}
}

Real Weibull::estimateLambda(const Vector<Real>& x, Real k) {
	Index nObs = x.size();

	Vector<Real> xk(nObs);
	for (Index i = 0; i < nObs; ++i) {
		xk(i) = std::pow(x(i), k);
	}
	Real sumxk = xk.sum();

	return std::pow(1.0 / Real(nObs) * sumxk, 1.0 / k);
}

}
