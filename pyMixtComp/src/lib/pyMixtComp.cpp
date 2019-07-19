#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "pmc.h"



BOOST_PYTHON_MODULE(pyMixtComp)
{
	Py_Initialize();
	boost::python::numpy::initialize();
	boost::python::def("pmc", pmc);
}
