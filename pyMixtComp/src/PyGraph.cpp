/*
 *  Project:    MixtComp
 *  Created on: Jaunary 21, 2019
 *  Authors:    Leo Perard, Viencent Kubicki, Quentin Grimonprez
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
const boost::python::dict &PyGraph::get() const {
	return d_;
}

/* Graph Functions */
void PyGraph::addSubGraph(const std::vector<std::string> &path, const std::string &name, const PyGraph &p) {
	addSubGraph(path, 0, d_, name, p);
}

void PyGraph::addSubGraph(const std::vector<std::string> &path, Index currDepth,
                          boost::python::dict &currLevel, const std::string &name,
                          const PyGraph &p) {
	if (currDepth == path.size())
		currLevel[name] = p.get();
	else {
		boost::python::extract<boost::python::dict &> nextLevel(currLevel[path[currDepth]]);
		if (!currLevel.has_key(path[currDepth]))
			nextLevel = boost::python::dict();
		else if (nextLevel.check()) {
			std::string askedPath;
			for (Index i = 0; i < currDepth + 1; ++i)
				askedPath += "/" + path[i];
			throw(askedPath + " already exists and is not a python object.");
		}
		addSubGraph(path, currDepth + 1, nextLevel(), name, p);
	}
}

void PyGraph::getSubGraph(const std::vector<std::string> &path, PyGraph &p) const {
	boost::python::dict d;
	go_to(path, d);
	p.set(d);
}



/* Payload Functions */
template <typename Type>
void PyGraph::add_payload(const std::vector<std::string> &path, const std::string &name,
                          const Type &p) {
	add_payload(path, 0, d_, name, p);
}

template <typename Type>
void PyGraph::add_payload(const std::vector<std::string> &path, Index currDepth,
                          boost::python::dict &currLevel, const std::string &name, const Type &p) {
	if (currDepth == path.size())
		tranlateCPPToPython(p, currLevel[name]);
	else {
		boost::python::extract<boost::python::dict &> nextLevel(currLevel[path[currDepth]]);
		if (!currLevel.has_key(path[currDepth]))
			nextLevel = boost::python::dict();
		else if (nextLevel.check()) {
			std::string askedPath;
			for (Index i = 0; i < currDepth + 1; ++i)
				askedPath += "/" + path[i];
			throw(askedPath + " path does not exist.");
		}
		add_payload(path, currDepth + 1, nextLevel(), name, p);
	}
}


template <typename Type>
Type PyGraph::get_payload(const std::vector<std::string> &path,
                          const std::string &name) const {
	Type val;
	get_payload(path, name, val);
	return val;
}

template <typename Type>
void PyGraph::get_payload(const std::vector<std::string> &path, const std::string &name,
                          Type &p) const {
	boost::python::dict d;
	go_to(path, d);
	if (!d.has_key(name)) {
		std::string cPath;
		completePath(path, name, cPath);
		throw(cPath + " object does not exist.");
	}
	translatePythonToCPP(d[name], p);
}


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
	for (Index i = 0; i < len(d); ++i) l.push_back(boost::python::extract<std::string>(d[i]));
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
