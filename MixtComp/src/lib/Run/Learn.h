/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 7, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef LEARN_H
#define LEARN_H

#include <IO/SGraph.h>

namespace mixt {

/**
 * The learn algorithm is encapsulated in this function.
 */
SGraph learn(const SGraph& algo, const SGraph& data, const SGraph& desc);

}

#endif
