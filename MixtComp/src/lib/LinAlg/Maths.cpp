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
 *  Created on: August 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <LinAlg/Maths.h>
#include <Various/Constants.h>
#include <cmath>
#include <limits>

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
