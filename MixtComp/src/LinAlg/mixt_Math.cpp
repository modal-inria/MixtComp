/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 24, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <cmath>
#include "mixt_Math.h"

int fac(int n) {
	return tgamma(n + 1);
}

Real logFac(int n) {
	return lgamma(n + 1);
}

bool realEqual(Real a, Real b) {
	return (a == b || std::abs(a - b) < std::abs(std::min(a, b)) * std::numeric_limits<Real>::epsilon()) ; // Test 1: Very cheap, but can result in false negatives, Test 2: More expensive, but comprehensive
}
