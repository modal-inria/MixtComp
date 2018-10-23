/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cmath>
#include "mixt_Math.h"

namespace mixt {

int fac(int n) {
	return std::round(tgamma(n + 1));
}

Real logFac(int n) {
	return lgamma(n + 1);
}

bool realEqual(Real a, Real b) {
	return (a == b || std::abs(a - b) < std::abs(std::min(a, b)) * std::numeric_limits<Real>::epsilon()); // Test 1: Very cheap, but can result in false negatives, Test 2: More expensive, but comprehensive
}

Real positiveNewtonRaphson(Index nIt, const std::function<std::pair<Real, Real>(Real)>& evalFunc, Real x) {
	if (nIt == 0)
		return x;
	else {
		std::pair<Real, Real> eval = evalFunc(x);
		Real candidate = x - eval.first / eval.second;
		if (0.0 < candidate)
			return positiveNewtonRaphson(nIt - 1, evalFunc, candidate);
		else
			return positiveNewtonRaphson(nIt - 1, evalFunc, x / 2.0);
	}
}

}
