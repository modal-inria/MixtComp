/*
 * pmc.h
 *
 *  Created on: 13 juin 2019
 *      Author: Quentin Grimonprez
 */

#ifndef SRC_PMC_H_
#define SRC_PMC_H_

#include <boost/python.hpp>

boost::python::dict pmc(boost::python::dict algoPy, boost::python::dict dataPy, boost::python::dict descPy, boost::python::dict resLearnPy);
char const* greet();


#endif /* SRC_PMC_H_ */
