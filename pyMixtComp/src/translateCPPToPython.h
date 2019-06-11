//
//  translateCPPtoPython.hpp
//  MixtComp
//
//  Created by Leo Perard on 09/01/2019.
//
//

#ifndef PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H
#define PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H

#include <boost/python.hpp>

namespace mixt {

template <typename InType>
void translateCPPToPython(const InType& in, boost::python::dict& out) {
	out["data"] = in;
}

template <typename T>
void translateCPPToPython(const NamedVector<T>& in, boost::python::dict& out) {
	Index nrow = in.vec_.size();
	const T* rawData = in.vec_.data();
	std::vector<T> data(rawData, rawData + in.vec_.size());

	out["ctype"] = "Vector";
	out["data"] = data;
	out["dtype"] = names<T>::name;
	out["rowNames"] = in.rowNames_;
	out["nrow"] = nrow;
}

template <typename T>
void translateCPPToPython(const NamedMatrix<T>& in, boost::python::dict& out) {
	Index nrow = in.mat_.rows();
	Index ncol = in.mat_.cols();
	std::vector<std::vector<T>> data(nrow, std::vector<T>(ncol));

	for (Index i = 0; i < nrow; ++i) {
		for (Index j = 0; j < ncol; ++j) {
			data[i][j] = in.mat_(i, j);
		}
	}

	out["colNames"] = in.colNames_;
	out["ctype"] = "Matrix";
	out["data"] = data;
	out["dtype"] = names<T>::name;
	out["rowNames"] = in.rowNames_;
	out["ncol"] = ncol;
	out["nrow"] = nrow;
}

}  // namespace mixt

#endif /* PYMIXTCOMP_SRC_TRANSLATECPPTOPYTHON_H */
