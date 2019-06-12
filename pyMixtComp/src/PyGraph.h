#ifndef PYMIXTCOMP_SRC_PYGRAPH_H
#define PYMIXTCOMP_SRC_PYGRAPH_H

#include <list>

#include <Python.h>
#include <boost/python.hpp>

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

	/* Graph Functions */
	void addSubGraph(const std::vector<std::string> &path, const std::string &name,
			const PyGraph &p);
	void getSubGraph(const std::vector<std::string> &path, PyGraph &p) const;

	/* Payload Functions */
	template <typename Type>
	void add_payload(const std::vector<std::string> &path, const std::string &name,
			const Type &p);

	template <typename Type>
	Type get_payload(const std::vector<std::string> &path, const std::string &name) const;

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
			const std::string &name, const Type &p);

	template <typename Type>
	void get_payload(const std::vector<std::string> &path, const std::string &name, Type &p) const;

	/* Graph Functions */
	void addSubGraph(const std::vector<std::string> &path, Index currDepth, boost::python::dict &currLevel,
			const std::string &name, const PyGraph &p);

	boost::python::dict d_;
};
}  // namespace mixt

#endif
