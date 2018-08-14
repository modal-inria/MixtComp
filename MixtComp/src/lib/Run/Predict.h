/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 10, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef PREDICT_H
#define PREDICT_H

#include <IO/SGraph.h>

namespace mixt {

/**
 * The learn algorithm is encapsulated in this function.
 */
SGraph predict(const SGraph& algo, const SGraph& data, const SGraph& desc, const SGraph& param);

}

#endif
