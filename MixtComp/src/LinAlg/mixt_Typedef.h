/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: February 24, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_TYPEDEF_H
#define MIXT_TYPEDEF_H

#include <cstdlib>

/** Eigen storage is column-major by default, which suits the main way of accessing data,
 * by looping over individuals instead of variables */
typedef double Real;
typedef std::size_t Index;

#endif // MIXT_TYPEDEF_H
