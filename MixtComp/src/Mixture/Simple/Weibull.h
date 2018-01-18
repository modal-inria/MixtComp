/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: 17th of January, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULL_H
#define WEIBULL_H

#include <utility>

#include "LinAlg/mixt_LinAlg.h"

namespace mixt {

/**
 * https://en.wikipedia.org/wiki/Weibull_distribution
 * Inspired by the code: https://github.com/vkubicki/ScalaTest/tree/master/src/main/scala/p02Statistics
 */
class Weibull {
public:
	/**
	 * Evaluate both the value and derivative of the
	 */
	std::pair<Real, Real> evalFuncDeriv(const Vector<Real>& x, Real k);

	Real positiveNewtonRaphson(const Vector<Real>& x, Real currK, Real nIt);
};

}

#endif
