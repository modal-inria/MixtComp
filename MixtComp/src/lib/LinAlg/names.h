/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef JSON_NAMES
#define JSON_NAMES

#include <LinAlg/LinAlg.h>
#include <iostream>

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
class names<int> {
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
