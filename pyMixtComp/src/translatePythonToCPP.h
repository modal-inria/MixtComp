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
#include <IO/NamedAlgebra.h>
#include <LinAlg/LinAlg.h>
#include <LinAlg/names.h>


namespace mixt {

template <typename OutType>
void translatePythonToCPP(const boost::python::dict& in, OutType& out) {
	out = in["data"];
}

template <typename T>
void translatePythonToCPP(const boost::python::dict& in, NamedVector<T>& out) {
	Index nrow = in["nrow"];
	out.vec_.resize(nrow);

	out.rowNames_ = in["rowNames"];

	std::vector<T> dataRaw = in["data"];
	for (Index i = 0; i < nrow; ++i) {
		out.vec_(i) = dataRaw[i];
	}
}

template <typename T>
void translatePythonToCPP(const boost::python::dict& in, NamedMatrix<T>& out) {
	Index nrow = in["nrow"];
	Index ncol = in["ncol"];
	out.mat_.resize(nrow, ncol);

	out.rowNames_ = in["rowNames"];
	out.colNames_ = in["colNames"];

	std::vector<std::vector<T>> dataRaw = in["data"];
	for (Index j = 0; j < ncol; ++j) {
		for (Index i = 0; i < nrow; ++i) {
			out.mat_(i, j) = dataRaw[i][j];
		}
	}
}

}  // namespace mixt

#endif /* PYMIXTCOMP_SRC_TRANSLATEPYTHONTOCPP_H */
