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
 *  Created on: August 21, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "RGraph.h"

namespace mixt {

RGraph::RGraph(const Rcpp::List& l) :
		l_(l) {
}

void RGraph::set(const Rcpp::List& j) {
	l_ = j;
}

void RGraph::getSubGraph(const std::vector<std::string>& path, RGraph& j) const {
	Rcpp::List l;
	go_to(path, l);

	j.set(l);
}

bool RGraph::exist_payload(const std::vector<std::string>& path, const std::string& name) const {
	Rcpp::List l;
	go_to(path, l);

	if (!l.containsElementNamed(name.c_str())) {
		return false;
	}

	return true;
}

void RGraph::go_to(const std::vector<std::string>& path, Rcpp::List& l) const {
	go_to(path, 0, l_, l);
}

void RGraph::go_to(const std::vector<std::string>& path, Index currDepth, const Rcpp::List& currLevel, Rcpp::List& l) const {
	if (currDepth == path.size()) {
		l = currLevel;
	} else {
		if (!currLevel.containsElementNamed(path[currDepth].c_str())) {
			std::string askedPath;
			for (Index i = 0; i < currDepth + 1; ++i) {
				askedPath += +"/" + path[i];
			}
			throw(askedPath + " path does not exist.");
		}

		go_to(path, currDepth + 1, currLevel[path[currDepth]], l);
	}
}

void RGraph::name_payload(const std::vector<std::string>& path, std::list<std::string>& l) const {
	Rcpp::List j;
	go_to(path, j);

	std::vector<std::string> v = j.names();
	std::copy(v.begin(), v.end(), std::back_inserter(l));
}

void RGraph::addSubGraph(const std::vector<std::string>& path, const std::string& name, const RGraph& p) {
	l_ = addSubGraph(path, 0, l_, name, p);
}

Rcpp::List RGraph::addSubGraph(const std::vector<std::string>& path, Index currDepth, Rcpp::List currLevel, const std::string& name, const RGraph& p) const {
	if (currDepth == path.size()) { // currLevel is the right element in path, add the payload
		currLevel[name] = p.getL();
		return currLevel;
	} else {
		if (!currLevel.containsElementNamed(path[currDepth].c_str())) { // if next level does not exist, create it
			currLevel[path[currDepth]] = Rcpp::List::create();
		} else if (TYPEOF(currLevel[path[currDepth]]) != VECSXP) { // if it already exists but is not a json object, throw an exception
			std::string askedPath;
			for (Index i = 0; i < currDepth + 1; ++i) {
				askedPath + "/" + path[i];
			}
			throw(askedPath + " already exists and is not an R list.");
		}

		Rcpp::List nextLevel = currLevel[path[currDepth]];
		currLevel[path[currDepth]] = addSubGraph(path, currDepth + 1, nextLevel, name, p);
		return currLevel;
	}
}

}
