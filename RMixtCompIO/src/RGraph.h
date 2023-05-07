/* MixtComp version 4 - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS */

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
 *  Created on: August 23, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RMIXTCOMP_SRC_RGRAPH_H
#define RMIXTCOMP_SRC_RGRAPH_H

#include <iostream>
#include <list>
#include <string>
#include "Rcpp.h"
#include <IO/NamedAlgebra.h>
#include <IO/IOFunctions.h>
#include <LinAlg/LinAlg.h>

#include "translateCPPToR.h"
#include "translateRToCPP.h"

namespace mixt {

class RGraph {
public:
	RGraph() {
		l_ = Rcpp::List::create();
		l_.attr("names") = std::vector<std::string>();
	}
	;

	RGraph(const Rcpp::List& l);

	void set(const Rcpp::List& s);

	const Rcpp::List& getL() const {
		return l_;
	}

	void getSubGraph(const std::vector<std::string>& path, RGraph& j) const;

	/**
	 * Add the payload, and create the complete path to it if it does not exist yet.
	 */
	template<typename Type>
	void add_payload(const std::vector<std::string>& path, const std::string& name, const Type& p) {
		l_ = add_payload(path, 0, l_, name, p);
	}

	void addSubGraph(const std::vector<std::string>& path, const std::string& name, const RGraph& p);

	/**
	 * Get the payload and write it in the destination argument.
	 */
	template<typename Type>
	void get_payload(const std::vector<std::string>& path, const std::string& name, Type& p) const {
		Rcpp::List l;
		go_to(path, l);

		if (!l.containsElementNamed(name.c_str())) {
			std::string cPath;
			completePath(path, name, cPath);
			throw(cPath + " object does not exist.");
		}
		translateRToCPP(l[name], p);
	}

	/**
	 * Get the payload as a return value. Useful in a few cases like filling values in a constructor.
	 */
	template<typename Type>
	Type get_payload(const std::vector<std::string>& path, const std::string& name) const {
		Type val;
		get_payload(path, name, val);
		return val;
	}

	bool exist_payload(const std::vector<std::string>& path, const std::string& name) const;

	void name_payload(const std::vector<std::string>& path, std::list<std::string>& l) const;

private:
	void go_to(const std::vector<std::string>& path, Rcpp::List& l) const;

	void go_to(const std::vector<std::string>& path, Index currDepth, const Rcpp::List& currLevel, Rcpp::List& l) const;

	/**
	 * Note that currLevel is a value and not a reference. This is different from what occurs in JSONGraph for example. The reason is detailed
	 * in this post: https://stackoverflow.com/questions/52006424/modifying-a-subsection-of-an-rcpplist-in-a-separate-function-by-reference
	 */
	template<typename Type>
	Rcpp::List add_payload(const std::vector<std::string>& path, Index currDepth, Rcpp::List currLevel, const std::string& name, const Type& p) const {
		if (currDepth == path.size()) { // currLevel is the right element in path, add the payload
			SEXP temp;
			translateCPPToR(p, temp);
			currLevel[name] = temp;

//			CPPToRTranslate(p, currLevel[name]);
//			currLevel[name] = p;
			return currLevel;
		} else {
			if (!currLevel.containsElementNamed(path[currDepth].c_str())) { // if next level does not exist, create it
				currLevel[path[currDepth]] = Rcpp::List::create();
			} else if (TYPEOF(currLevel[path[currDepth]]) != VECSXP) { // if it already exists but is not a json object, throw an exception
				std::string askedPath;
				for (Index i = 0; i < currDepth + 1; ++i) {
				  askedPath = askedPath + "/" + path[i];
				}
				throw(askedPath + " already exists and is not an R list.");
			}

			Rcpp::List nextLevel = currLevel[path[currDepth]];
			currLevel[path[currDepth]] = add_payload(path, currDepth + 1, nextLevel, name, p);
			return currLevel;
		}
	}

	Rcpp::List addSubGraph(const std::vector<std::string>& path, Index currDepth, Rcpp::List currLevel, const std::string& name, const RGraph& p) const;

	Rcpp::List l_;
};

}

#endif
