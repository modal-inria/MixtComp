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
 *  Created on: June 29, 2017
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RNG_H
#define RNG_H

#include <iostream>
#include <time.h>

#include <Various/Constants.h>

namespace mixt {

bool deterministicMode();

int deterministicInitialSeed();

template<class T>
std::size_t seed(const T* const p_caller) {
	static Index nCall = deterministicInitialSeed();

	if (deterministicMode() == false) {
		return size_t(p_caller) + time(0);
	} else {
		++nCall;
		//	std::cout << "seed, nCall: " << nCall << std::endl;
		return nCall;
	}
}

} // namespace mixt

#endif
