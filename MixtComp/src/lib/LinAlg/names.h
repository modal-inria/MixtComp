/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSON_NAMES
#define JSON_NAMES

#include <iostream>
#include <LinAlg/mixt_LinAlg.h>

namespace mixt {

template<typename T>
class names {};

template<>
class names<Index> {
public:
	static const std::string name;
};

template<>
class names<Integer> {
public:
	static const std::string name;
};

template<>
class names<Real> {
public:
	static const std::string name;
};

}

#endif
