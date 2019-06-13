#include <boost/python.hpp>

#include "pmc.h"



BOOST_PYTHON_MODULE(pyMixtComp)
{
	boost::python::def("greet", greet);
	boost::python::def("pmc", pmc);
}
