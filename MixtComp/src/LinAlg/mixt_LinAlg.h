/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: January 19, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_LINALG_H
#define MIXT_LINALG_H

#include <Eigen/Dense>
#include <vector>
#include "mixt_Iterator.h"

namespace mixt
{

/** Eigen storage is column-major by default, which suits the main way of accessing data,
 * by looping over individuals instead of variables */
typedef double Real;

template<typename T,
         int _Rows = Eigen::Dynamic,
         int _Cols = Eigen::Dynamic>
class Matrix : public Eigen::Matrix<T, _Rows, _Cols>
{
  public:
    typedef T Type;

    Matrix() :
      Eigen::Matrix<T, _Rows, _Cols>()
    {}

    Matrix(int nrow, int ncol) :
      Eigen::Matrix<T, _Rows, _Cols>(nrow, ncol)
    {}

    /** Constructor from Eigen expressions */
    template<typename OtherDerived>
    Matrix(const Eigen::MatrixBase<OtherDerived>& other) :
      Eigen::Matrix<T, _Rows, _Cols>(other)
    {}

    /** Assign Eigen expressions to Matrix */
    template<typename OtherDerived>
    Matrix& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
      this->Eigen::Matrix<T, _Rows, _Cols>::operator=(other);
      return *this;
    }

    /** Element-wise assignment of a scalar */
    Matrix& operator=(const T& scalar)
    {
      (*this) = Eigen::CwiseNullaryOp<Eigen::internal::scalar_constant_op<T>,
                                      const Eigen::Matrix<T, _Rows, _Cols> >(this->rows(),
                                                                             this->cols(),
                                                                             Eigen::internal::scalar_constant_op<T>(scalar));
      return *this;
    }
};

template<typename T, int _Rows = Eigen::Dynamic>
class Vector : public Matrix<T, _Rows, 1>
{
  public:
    Vector() :
      Matrix<T, _Rows, 1>()
    {}

    Vector(int nrow) :
      Matrix<T, _Rows, 1>(nrow, 1)
    {}

    /** This constructor allows to construct Vector from Eigen expressions */
    template<typename OtherDerived>
    Vector(const Eigen::MatrixBase<OtherDerived>& other) :
      Matrix<T, _Rows, 1>(other)
    {}

    /** This method allows to assign Eigen expressions to Vector */
    template<typename OtherDerived>
    Vector& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
      this->Matrix<T, _Rows, 1>::operator=(other);
      return *this;
    }

    /** Element-wise assignment of a scalar */
    Vector& operator=(const T& scalar)
    {
      (*this) = Eigen::CwiseNullaryOp<Eigen::internal::scalar_constant_op<T>,
                                      const Eigen::Matrix<T, _Rows, 1> >(this->rows(),
                                                                         1,
                                                                         Eigen::internal::scalar_constant_op<T>(scalar));
      return *this;
    }
};

template<typename T, int _Cols = Eigen::Dynamic>
class RowVector : public Matrix<T, 1, _Cols>
{
  public:
    RowVector() :
      Matrix<T, 1, _Cols>()
    {};

    RowVector(int ncol) :
      Matrix<T, 1, _Cols>(1, ncol)
    {};

    /** This constructor allows to construct RowVector from Eigen expressions */
    template<typename OtherDerived>
    RowVector(const Eigen::MatrixBase<OtherDerived>& other) :
      Matrix<T, 1, _Cols>(other)
    {};

    /** This method allows to assign Eigen expressions to RowVector */
    template<typename OtherDerived>
    RowVector& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
      this->Matrix<T, 1, _Cols>::operator=(other);
      return *this;
    }

    /** Element-wise assignment of a scalar */
    RowVector& operator=(const T& scalar)
    {
      (*this) = Eigen::CwiseNullaryOp<Eigen::internal::scalar_constant_op<T>,
                                      const Eigen::Matrix<T, 1, _Cols> >(1,
                                                                         this->cols(),
                                                                         Eigen::internal::scalar_constant_op<T>(scalar));
      return *this;
    }
};

/** Constant operator, could be used to fix an object to a constant value if the assignment to a scalar has not been
 * defined. For example m = Constant(3, 3, 12.) to assign 12 to a 3x3 m object. */
template<typename T>
const Eigen::CwiseNullaryOp<Eigen::internal::scalar_constant_op<T>,
                            const Eigen::Matrix<T,
                                                Eigen::Dynamic,
                                                Eigen::Dynamic> >
Constant (int nrow, int ncol, const T& scalar)
{
  return Eigen::CwiseNullaryOp<Eigen::internal::scalar_constant_op<T>,
                               const Eigen::Matrix<T,
                                                   Eigen::Dynamic,
                                                   Eigen::Dynamic> >(nrow,
                                                                     ncol,
                                                                     Eigen::internal::scalar_constant_op<T>(scalar));
}

/** General sort of data that is contiguous in memory (use of pointer to data and size).
 * Be advised against using it against subparts (i.e. blocks) of matrices which elements are
 * not stored contiguously */
template <typename Container>
void sortContiguous(Container& ref)
{
  std::sort(ref.data(),
            ref.data() + ref.size());
}

/** Sort function for non contiguous data, for example block. Slower than sortContiguous */
template <typename Container>
void sort(Container& ref)
{
  std::sort(iterator<Container>(0, ref),
            iterator<Container>(ref.rows() * ref.cols(), ref));
}

/** Comparator for indexed pairs */
template <typename pair>
bool comparator (const pair& l, const pair& r)
{
  return l.first < r.first;
};

/** Contiguous data sort, keeping track of the indices, introduces overhead, copy input data, copy indexes */
 template <typename Container>
void sortContiguousIndex(Container& in, Vector<int>& out)
{
  typedef typename std::pair<typename Container::Type, int> IntPair;

  int nbElem = in.size();
  out.resize(nbElem); // resizing the indices vector
  std::vector<IntPair> sortVec(nbElem);
  int n = 0;

  for (int i = 0; i < in.rows(); ++i)
  {
    for (int j = 0; j < in.cols(); ++j)
    {
      sortVec[n] = IntPair(in(i, j), n);
      ++n;
    }
  }

  std::sort(sortVec.begin(), sortVec.end(), comparator<IntPair>);

  for(int i = 0; i < nbElem; ++i)
  {
    out(i) = sortVec[i].second;
  }
}

} // namespace mixt

#endif // MIXT_LINALG_H
