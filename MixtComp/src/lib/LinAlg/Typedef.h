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
 *  Created on: February 24, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <cstdlib>
#include <string>

namespace mixt {

/** Eigen storage is column-major by default, which suits the main way of accessing data,
 * by looping over individuals instead of variables */
typedef double Real;
// typedef std::size_t Index;
// typedef int Index;
typedef unsigned long Index;

typedef long Integer;

Real toReal(const std::string& s);

Index toIndex(const std::string& s);

}

#endif // TYPEDEF_H
