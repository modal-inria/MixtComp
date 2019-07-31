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

#ifndef PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H
#define PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H

#include <iostream>
#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <IO/NamedAlgebra.h>
#include <LinAlg/LinAlg.h>
#include <LinAlg/names.h>
#include <boost/python/numpy.hpp>


namespace mixt {

template <typename InType>
void translateCPPToPython(const InType& in, const std::string& name, boost::python::dict& out) {
	out[name] = in;
}

template <typename T>
void translateCPPToPython(const std::vector<T>& in, const std::string& name, boost::python::dict& out) {
	boost::python::list pyList;
	for (Index i = 0; i < in.size(); ++i) {
		pyList.append(in[i]);
	}
	out[name] = pyList;
}


template <typename T>
void translateCPPToPython(const std::vector<std::vector<T> >& in, const std::string& name, boost::python::dict& out) {
	boost::python::list pyList;
	for (Index i = 0; i < in.size(); ++i) {
		boost::python::list pyList2;
		for (Index j = 0; j < in[i].size(); ++j) {
			pyList2.append(in[i][j]);
		}
		pyList.append(pyList2);
	}
	out[name] = pyList;
}



template <typename T>
void translateCPPToPython(const NamedVector<T>& in, const std::string& name, boost::python::dict& out) {
	out[name] = boost::python::dict();

	Index nrow = in.vec_.size();

	boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<T>();
	boost::python::tuple shape = boost::python::make_tuple(nrow);
	boost::python::tuple stride = boost::python::make_tuple(sizeof(T));
	boost::python::object own;
	boost::python::numpy::ndarray vec = boost::python::numpy::from_data(in.vec_.data(), dt, shape, stride, own);
	out[name]["data"] = vec.copy();// otherwise data are lost

	boost::python::list rowNames;
	for(auto& s: in.rowNames_){
		rowNames.append(s);
	}
	out[name]["rowNames"] = rowNames;

	out[name]["nrow"] = nrow;
	out[name]["dtype"] = names<T>::name;
	out[name]["ctype"] = "Vector";
}


template <typename T>
void translateCPPToPython(const NamedMatrix<T>& in, const std::string& name, boost::python::dict& out) {
	out[name] = boost::python::dict();

	Index nrow = in.mat_.rows();
	Index ncol = in.mat_.cols();

	boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<T>();
	boost::python::tuple shape = boost::python::make_tuple(nrow, ncol);
	boost::python::tuple stride = boost::python::make_tuple(sizeof(T), sizeof(T)*nrow);
	boost::python::object own;
	boost::python::numpy::ndarray mat = boost::python::numpy::from_data(in.mat_.data(), dt, shape, stride, own);
	out[name]["data"] = mat.copy();// otherwise data are lost

	boost::python::list rowNames;
	for(auto& s: in.rowNames_){
		rowNames.append(s);
	}
	out[name]["rowNames"] = rowNames;

	boost::python::list colNames;
	for(auto& s: in.colNames_){
		colNames.append(s);
	}
	out[name]["colNames"] = colNames;

	out[name]["ctype"] = "Matrix";
	out[name]["dtype"] = names<T>::name;
	out[name]["ncol"] = ncol;
	out[name]["nrow"] = nrow;
}


}  // namespace mixt

#endif /* PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H */
