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
 *  Created on: January 23rd, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <LinAlg/LinAlg.h>

namespace mixt {

/**
 * Empiric computation of a quantile, given observed data.
 */
template<typename A>
A quantile(const Vector<A>& v, Real p) {
	Vector<A> sorted = v; // copy is necessary as sort is applied on mutable vector
	sorted.sort();
	Index cutPoint = p * v.size();
	return sorted(cutPoint);
}

/**
 * Empiric computation of cdf, given observed data.
 */
template<typename A>
Real cdf(const Vector<A>& v, Real a) {
	Vector<A> sorted = v; // copy is necessary as sort is applied on mutable vector
	sorted.sort();
	Index size = v.size();

	Index i = 0;
	for (; i < size; ++i) {
		if (a <= sorted(i))
			break;
	}

	return Real(i) / Real(size - 1);
}

}

#endif
