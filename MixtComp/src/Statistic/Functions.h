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

template<typename A>
Real quantile(const Vector<A>& v) {
	Vector<A> sorted = v; // copy is necessary as sort is applied on mutable vector
	sorted.sort();
}

}

#endif
