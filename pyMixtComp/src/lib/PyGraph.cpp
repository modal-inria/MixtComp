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
 *  Created on: January 21, 2019
 *  Authors:    Quentin Grimonprez, Vincent Kubicki
 **/


#include "PyGraph.h"

namespace mixt {

PyGraph::PyGraph(const boost::python::dict &d) :
		d_(d) {
}


/* Setters */
void PyGraph::set(const boost::python::dict &d) {
	d_ = d;
}

/* Getters */
const boost::python::dict &PyGraph::getD() const {
	return d_;
}

std::string PyGraph::get() const {
	std::string out = boost::python::extract<std::string>(boost::python::str(d_));

	return out;
}


/* Graph Functions */
void PyGraph::addSubGraph(const std::vector<std::string> &path, const std::string &name, const PyGraph &p) {
	addSubGraph(path, 0, d_, name, p);
}

void PyGraph::addSubGraph(const std::vector<std::string> &path, Index currDepth,
                          boost::python::dict &currLevel, const std::string &name,
                          const PyGraph &p) {
	if (currDepth == path.size())
		currLevel[name] = p.getD();
	else {
		if (!currLevel.has_key(path[currDepth]))  // if next level does not exist, create it
		{
			boost::python::dict nextLevel;
			addSubGraph(path, currDepth + 1, nextLevel, name, p);
		}
		else {
			boost::python::extract<boost::python::dict> temp(currLevel[path[currDepth]]);

 			if (temp.check()) {
				boost::python::dict nextLevel = temp();
				addSubGraph(path, currDepth + 1, nextLevel, name, p);
			} else { // if it already exists but is not a dict, throw an exception
				std::string askedPath;
				for (Index i = 0; i < currDepth + 1; ++i)
					askedPath += "/" + path[i];
				throw(askedPath + " already exists and is not a python object.");
			}
		}
	}
}

void PyGraph::getSubGraph(const std::vector<std::string> &path, PyGraph &p) const {
	boost::python::dict d;
	go_to(path, d);
	p.set(d);
}



/* Payload Functions */


bool PyGraph::exist_payload(const std::vector<std::string> &path,
                            const std::string &name) const {
	boost::python::dict d;
	go_to(path, d);
	return d.has_key(name);
}

void PyGraph::name_payload(const std::vector<std::string> &path,
                           std::list<std::string> &l) const {
	boost::python::dict d;
	go_to(path, d);

	boost::python::list keys = d.keys();
	for (int i = 0; i < len(keys); ++i) {
		std::string key = boost::python::extract<std::string>(keys[i]);
		l.push_back(key);
	}
}



/* Go To Functions */
void PyGraph::go_to(const std::vector<std::string> &path, boost::python::dict &d) const {
	go_to(path, 0, d_, d);
}

void PyGraph::go_to(const std::vector<std::string> &path, Index currDepth,
                    const boost::python::dict &currLevel, boost::python::dict &d) const {
	if (currDepth == path.size())
		d = currLevel;
	else {
		if (!currLevel.has_key(path[currDepth])) {
			std::string askedPath;
			for (Index i = 0; i < currDepth + 1; ++i)
				askedPath += "/" + path[i];
			throw(askedPath + " path does not exist.");
		}
		const boost::python::dict &nextLevel = boost::python::extract<boost::python::dict>(currLevel[path[currDepth]]);
		go_to(path, currDepth + 1, nextLevel, d);
	}
}




}  // namespace mixt
