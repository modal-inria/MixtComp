/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cmath>
#include <limits>
#include "mixt_Math.h"
#include <Various/mixt_Constants.h>

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

Real minimizePositiveNewtonRaphson(Index nIt, Real relTol, const std::function<std::pair<Real, Real>(Real)>& evalFunc, Real x) {
	Real lastX = std::numeric_limits<Real>::max();
	Real currX = x;

	while (0 < nIt && relTol < std::abs((currX - lastX) / lastX)) {
		std::pair<Real, Real> eval = evalFunc(x);

		Real candidate = x - eval.first / eval.second;
		if (0.0 < candidate)
			x = candidate;
		else
			x = x / 2.0;

		lastX = currX;
		currX = x;
		nIt -= 1;
	}

	return x;
}

Real minimizePositiveNewtonRaphson(const std::function<std::pair<Real, Real>(Real)>& evalFunc, Real x) {
	Real lastX = std::numeric_limits<Real>::max();
	Real currX = x;

	while (relTolOptim < std::abs((currX - lastX) / lastX)) {
		std::pair<Real, Real> eval = evalFunc(x);

		Real candidate = x - eval.first / eval.second;
		if (0.0 < candidate)
			x = candidate;
		else
			x = x / 2.0;

		lastX = currX;
		currX = x;
	}

	return x;
}

}
