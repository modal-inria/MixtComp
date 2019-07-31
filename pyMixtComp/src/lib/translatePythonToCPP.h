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
 *  Created on: June, 2019
 *  Authors:    Quentin Grimonprez
 **/

#ifndef PYMIXTCOMP_SRC_TRANSLATEPYTHONTOCPP_H
#define PYMIXTCOMP_SRC_TRANSLATEPYTHONTOCPP_H

#include <iostream>
#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <IO/NamedAlgebra.h>
#include <LinAlg/LinAlg.h>
#include <LinAlg/names.h>


namespace mixt {

template <typename OutType>
void translatePythonToCPP(const boost::python::api::object_item& in, OutType& out) {
	out = boost::python::extract<OutType>(in);
}


template <typename T>
void translatePythonToCPP(const boost::python::api::object_item& in, std::vector<T>& out) {
    out = std::vector<T>(boost::python::stl_input_iterator<T>(in),
	                     boost::python::stl_input_iterator<T>());
}


template <typename T>
void translatePythonToCPP(const boost::python::api::object_item& in, std::vector<std::vector<T> >& out) {
	for(int i = 0; i < boost::python::len(in); ++i){
	    out.push_back(std::vector<T>(boost::python::stl_input_iterator<T>(in[i]),
		              boost::python::stl_input_iterator<T>()));
	}
}


template <typename T>
void translatePythonToCPP(const boost::python::api::object_item& in, NamedVector<T>& out) {
	Index nrow = boost::python::extract<Index>(in["nrow"]);

	out.vec_.resize(nrow);
	out.rowNames_.resize(nrow);

	for(Index i = 0; i < nrow; ++i) {
		out.vec_(i) = boost::python::extract<T>(in["data"][i]);
	}

	out.rowNames_ = std::vector<std::string>(boost::python::stl_input_iterator<std::string>(in["rowNames"]),
	                                         boost::python::stl_input_iterator<std::string>());
}


template <typename T>
void translatePythonToCPP(const boost::python::api::object_item& in, NamedMatrix<T>& out) {
	Index nrow = boost::python::extract<Index>(in["nrow"]);
	Index ncol = boost::python::extract<Index>(in["ncol"]);

	out.mat_.resize(nrow, ncol);
	out.rowNames_.resize(nrow);
	out.colNames_.resize(ncol);

	for (Index j = 0; j < ncol; ++j) {
		for (Index i = 0; i < nrow; ++i) {
			out.mat_(i, j) = boost::python::extract<T>(in["data"][i][j]);
		}
	}

	out.rowNames_ = std::vector<std::string>(boost::python::stl_input_iterator<std::string>(in["rowNames"]),
	                                         boost::python::stl_input_iterator<std::string>());
	out.colNames_ = std::vector<std::string>(boost::python::stl_input_iterator<std::string>(in["colNames"]),
	                                         boost::python::stl_input_iterator<std::string>());
}

}  // namespace mixt

#endif /* PYMIXTCOMP_SRC_TRANSLATEPYTHONTOCPP_H */
