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
 *  Created on: September 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MISVALPARSER_H
#define MISVALPARSER_H

#include <IO/IO.h>
#include <IO/SpecialStr.h>
#include <iostream>
#include <regex>
#include <set>

#include "../Various/Enum.h"

namespace mixt {

template<typename Type>
class MisValParser {
public:
	/** Missing value descriptor: type of missing, and list of parameters */
	typedef typename std::pair<MisType, std::vector<Type> > MisVal;

	/** Note that https://regex101.com/ REALLY helps settings those regular expressions.  Note that since raw strings are not used, and as such
	 * escape characters have to be doubled, which impairs test on regex101 for example. */
	MisValParser(Type offset) :
			offset_(offset), reNumber_(strNumber), reValue_(strBlank + // " *(-*[0-9.]+) *"
					strNumber + strBlank), reMissing_(strBlank + strQMark + strBlank), reFiniteValues_(" *\\{.*\\} *"), // enclosing {} are detected first, then the interior is parsed for the list of values. In the interior, any separator between the numbers will work
			reIntervals_(strLeftPar + // " *\\[ *(-*[0-9.]+) *: *(-*[0-9.]+) *\\] *"
					strNumber + strCentralColon + strNumber + strRightPar), reLuIntervals_(strLeftPar +  // " *\\[ *-inf *: *(-*[0-9.]+) *\\] *"
					strMinusInf + strCentralColon + strNumber + strRightPar), reRuIntervals_(strLeftPar + // " *\\[ *(-*[0-9.]+) *: *\\+inf *\\] *"
					strNumber + strCentralColon + strPlusInf + strRightPar) {
	}

	bool parseStr(const std::string& str, Type& v, MisVal& mv) {
		std::smatch matches;

		if (std::regex_match(str, matches, reValue_)) { // value is present
			v = str2type<Type>(matches[1].str()) + offset_;
			mv.first = present_;
			return true;
		}

		if (std::regex_match(str, matches, reMissing_)) { // value is completely missing
			v = Type(0);
			mv.first = missing_; // in all other cases data is considered completely missing
			return true;
		}

		if (std::regex_match(str, matches, reFiniteValues_)) { // only a finite number of values are acceptable
			v = Type(0);
			std::string::const_iterator start = str.begin();
			std::string::const_iterator end = str.end();
			std::smatch m;

			std::set<Type> setVal; // using a set allows for automatic sorting and duplicates deletion
			while (std::regex_search(start, end, m, reNumber_)) {
				setVal.insert(str2type<Type>(m[0].str()) + offset_);
				start = m[0].second;
			}

			mv.first = missingFiniteValues_;
			for (typename std::set<Type>::const_iterator it = setVal.begin(), itEnd = setVal.end(); it != itEnd; ++it) {
				mv.second.push_back(*it);
			}

			return true;
		}

		if (std::regex_match(str, matches, reIntervals_)) { // acceptable values provided by intervals
			v = Type(0);

			std::set<Type> setVal;
			setVal.insert(str2type<Type>(matches[1].str()) + offset_);
			setVal.insert(str2type<Type>(matches[2].str()) + offset_);

			if (setVal.size() == 2) {
				mv.first = missingIntervals_;
				mv.second.reserve(2);
				for (typename std::set<Type>::const_iterator it = setVal.begin(), itEnd = setVal.end(); it != itEnd; ++it) {
					mv.second.push_back(*it);
				}
				return true;
			} else {
				return false;
			}
		}

		if (std::regex_match(str, matches, reLuIntervals_)) { // data is lower bounded
			v = Type(0);
			mv.first = missingLUIntervals_;
			mv.second.push_back(str2type<Type>(matches[1].str()) + offset_);
			return true;
		}

		if (std::regex_match(str, matches, reRuIntervals_)) { // data is upper bounded
			v = Type(0);
			mv.first = missingRUIntervals_;
			mv.second.push_back(str2type<Type>(matches[1].str()) + offset_);
			return true;
		}

		return false;
	}

private:
	Type offset_;

	std::regex reNumber_;
	std::regex reValue_;
	std::regex reMissing_;
	std::regex reFiniteValues_;
	std::regex reIntervals_;
	std::regex reLuIntervals_;
	std::regex reRuIntervals_;
};

} // namespace mixt

#endif // MISVALPARSER_H
