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
