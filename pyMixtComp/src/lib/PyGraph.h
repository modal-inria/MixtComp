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

#ifndef PYMIXTCOMP_SRC_PYGRAPH_H
#define PYMIXTCOMP_SRC_PYGRAPH_H

#include <list>

#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include "translateCPPToPython.h"
#include "translatePythonToCPP.h"
#include <IO/IOFunctions.h>
#include <IO/NamedAlgebra.h>
#include <LinAlg/LinAlg.h>


namespace mixt {

class PyGraph {
public:
	/* Constructors */
	PyGraph(){};
	PyGraph(const boost::python::dict &d);

	/* Setters */
	void set(const boost::python::dict &d);

	/* Getters */
	const boost::python::dict &getD() const;

	std::string get() const;

	/* Graph Functions */
	void addSubGraph(const std::vector<std::string> &path, const std::string &name,
			const PyGraph &p);
	void getSubGraph(const std::vector<std::string> &path, PyGraph &p) const;

	/* Payload Functions */
	template <typename Type>
	void add_payload(const std::vector<std::string> &path, const std::string &name,
			const Type &p) {
		add_payload(path, 0, d_, name, p);
	}

	template <typename Type>
	Type get_payload(const std::vector<std::string> &path, const std::string &name) const{
		Type val;
		get_payload(path, name, val);
		return val;
	}


	template <typename Type>
	void get_payload(const std::vector<std::string> &path, const std::string &name, Type &p) const{
		boost::python::dict d;
		go_to(path, d);
		if (!d.has_key(name)) {
			std::string cPath;
			completePath(path, name, cPath);
			throw(cPath + " object does not exist.");
		}
		translatePythonToCPP(d[name], p);
	}


	bool exist_payload(const std::vector<std::string> &path, const std::string &name) const;

	void name_payload(const std::vector<std::string> &path, std::list<std::string> &l) const;


private:
	/* Go To Functions */
	void go_to(const std::vector<std::string> &path, boost::python::dict &d) const;
	void go_to(const std::vector<std::string> &path, Index currDepth, const boost::python::dict &currLevel,
			boost::python::dict &d) const;

	/* Payload functions */
	template <typename Type>
	void add_payload(const std::vector<std::string> &path, Index currDepth, boost::python::dict &currLevel,
			const std::string &name, const Type &p) {
		if (currDepth == path.size()){
			translateCPPToPython(p, name, currLevel);
		}else {
			if (!currLevel.has_key(path[currDepth]))// if next level does not exist, create it
			{
				boost::python::dict nextLevel;
				add_payload(path, currDepth + 1, nextLevel, name, p);
				currLevel[path[currDepth]] = nextLevel;
			}else{
//				boost::python::extract<boost::python::dict &> nextLevel(currLevel[path[currDepth]]);
				boost::python::dict nextLevel(currLevel[path[currDepth]]);
//				if(nextLevel.check())// check() works with extract
//				{
					add_payload(path, currDepth + 1, nextLevel, name, p);
					currLevel[path[currDepth]] = nextLevel;
//				}else{
//					std::cout<<"pas check"<<std::endl;
//					std::string askedPath;
//					for (Index i = 0; i < currDepth + 1; ++i) {
//						askedPath + "/" + path[i];
//					}
//					throw(askedPath + " already exists and is not a python object.");
//				}
			}
		}
	}

	/* Graph Functions */
	void addSubGraph(const std::vector<std::string> &path, Index currDepth, boost::python::dict &currLevel,
			const std::string &name, const PyGraph &p);

	boost::python::dict d_;
};
}  // namespace mixt

#endif
