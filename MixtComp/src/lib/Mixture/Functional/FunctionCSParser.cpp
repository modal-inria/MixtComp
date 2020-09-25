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
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <boost/algorithm/string.hpp>
#include <regex>

#include "../../IO/MisValParser.h"
#include "FunctionCSParser.h"

namespace mixt {

std::string parseFunctionalStr(Index nSub, Index nCoeff, const std::vector<std::string>& dataStr, Vector<FunctionCS>& vecInd) {
	std::string warnLog;

	Index nInd = dataStr.size();
	vecInd.resize(nInd);

	MisValParser<Real> mvp(0.); // no need for offset as data is continuous
	std::string funcStr = strNumber + std::string(" *: *") + strNumber; // string will be split between a number, :, and anything that follows. This will in turn be parsed in a second pass.
	std::regex funcRe(funcStr);
	std::regex numRe(strNumber);
	std::smatch matchesInd;

	std::vector<std::string> strs;
	for (Index ind = 0; ind < nInd; ++ind) {
		boost::split(strs, dataStr[ind], boost::is_any_of(rankPosSep)); // same separator used as for values in Rank model

		Index nTime = strs.size();
		vecInd(ind).setSize(nTime, nSub);

		for (Index i = 0; i < nTime; ++i) {
			Real t;
			Real x;

			if (std::regex_match(strs[i], matchesInd, funcRe)) { // value is present
				t = str2type<Real>(matchesInd[1].str());
				x = str2type<Real>(matchesInd[2].str()); // at the moment, only a numerical value for x is supported
			} else {
				std::stringstream sstm;
				sstm << "Individual ind: " << ind << ", timestep i: " << i << " does not conform to the syntax time:value where time is a decimal number "
						<< "and value is a descriptor of a present / missing value." << std::endl;
				warnLog += sstm.str();
				return warnLog;
			}

			vecInd(ind).setValTime(i, t, x);
		}

		vecInd(ind).computeVandermonde(nCoeff);
	}

	return warnLog;
}

} // namespace mixt
