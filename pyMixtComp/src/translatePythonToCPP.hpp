//
//  translatePythonToCPP.hpp
//  MixtComp
//
//  Created by Leo Perard on 09/01/2019.
//
//

#ifndef PYMIXT_TANSLATEPYTHONTOCPP_H
#define PYMIXT_TANSLATEPYTHONTOCPP_H

#include <boost/python.hpp>

using namespace boost::python;

namespace mixt {

template <typename OutType>
void translatePythonToCPP(const dict& in, OutType& out) {
  out = in["data"];
}

template <typename T>
void translatePythonToCPP(const dict& in, NamedVector<T>& out) {
  Index nrow = in["nrow"];
  out.vec_.resize(nrow);

  out.rowNames_ = in["rowNames"];

  std::vector<T> dataRaw = in["data"];
  for (Index i = 0; i < nrow; ++i) {
    out.vec_(i) = dataRaw[i];
  }
}

template <typename T>
void translatePythonToCPP(const dict& in, NamedMatrix<T>& out) {
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

#endif /* PYMIXT_TANSLATEPYTHONTOCPP_H */
