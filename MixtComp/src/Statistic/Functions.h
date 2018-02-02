/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: January 23rd, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "LinAlg/mixt_LinAlg.h"

namespace mixt {

/**
 * Empiric computation of a quantile, given observed data.
 */
template<typename A>
const A& quantile(const Vector<A>& v, Real p) {
	Vector<A> sorted = v; // copy is necessary as sort is applied on mutable vector
	sorted.sort();
	Index cutPoint = p * v.size();
	return sorted(cutPoint);
}

/**
 * Empiric computation of cdf, given observed data.
 */
template<typename A>
Real pdf(const Vector<A>& v, Real a) {
	Vector<A> sorted = v; // copy is necessary as sort is applied on mutable vector
	sorted.sort();
	Index size = v.size();

	Index i = 0;
	for (; i < size; ++i) {
		if (a <= sorted(i)) break;
	}

	return Real(i) / Real(size - 1);
}

}

#endif
