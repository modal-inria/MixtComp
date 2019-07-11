//
//  translatePythonToCPP.hpp
//  MixtComp
//
//  Created by Leo Perard on 09/01/2019.
//
//

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
void translatePythonToCPP(const boost::python::api::object_item& in, NamedVector<T>& out) {
	Index nrow = boost::python::extract<Index>(in["nrow"]);

	out.vec_.resize(nrow);
	out.rowNames_.resize(nrow);

	for(Index i = 0; i < nrow; ++i) {
		out.rowNames_[i] =  boost::python::extract<std::string>(in["rowNames"][i]);
		out.vec_(i) = boost::python::extract<T>(in["data"][i]);
	}
}


template <typename T>
void translatePythonToCPP(const boost::python::api::object_item& in, NamedMatrix<T>& out) {
	Index nrow = boost::python::extract<Index>(in["nrow"]);
	Index ncol = boost::python::extract<Index>(in["ncol"]);

	out.mat_.resize(nrow, ncol);
	out.rowNames_.resize(nrow);
	out.colNames_.resize(ncol);

	for (Index j = 0; j < ncol; ++j) {
		out.colNames_[j] =  boost::python::extract<std::string>(in["colNames"][j]);

		for (Index i = 0; i < nrow; ++i) {
			out.mat_(i, j) = boost::python::extract<T>(in["data"][i][j]);
		}
	}

	for (Index i = 0; i < nrow; ++i) {
		out.rowNames_[i] = boost::python::extract<std::string>(in["rowNames"][i]);
	}
}

}  // namespace mixt

#endif /* PYMIXTCOMP_SRC_TRANSLATEPYTHONTOCPP_H */
