/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: December 16, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_LINALG_H
#define MIXT_LINALG_H

#include <Eigen/Dense>

namespace mixt
{

// Eigen storage is column-major by default, which suits the main way of accessing data, by looping over individuals instead of variables

typedef double Real;

/** General sort of data that is contiguous in memory (use of pointer to data and size).
 * Be advised against using it against subparts (i.e. blocks) of matrices which elements are
 * not stored contiguously */
template <typename T>
void sortContiguous(T& ref)
{
  std::sort(ref.data(),
            ref.data() + ref.size());
}

template<typename T, int _Rows = Eigen::Dynamic, int _Cols = Eigen::Dynamic>
class Matrix : public Eigen::Matrix<T, _Rows, _Cols>
{
  public:
    Matrix() :
      Eigen::Matrix<T, _Rows, _Cols>()
    {};

    Matrix(int nrow, int ncol) :
      Eigen::Matrix<T, _Rows, _Cols>(nrow, ncol)
    {};

    /** This constructor allows to construct Matrix from Eigen expressions */
    template<typename OtherDerived>
    Matrix(const Eigen::MatrixBase<OtherDerived>& other) :
      Eigen::Matrix<T, _Rows, _Cols>(other)
    {};

    /** This method allows to assign Eigen expressions to Matrix */
    template<typename OtherDerived>
    Matrix& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
      this->Eigen::Matrix<T, _Rows, _Cols>::operator=(other);
      return *this;
    }

    /** Element-wise operation between matrix and basic type */
    Matrix& operator=(const T& other)
    {
      (*this) = Eigen::Matrix<T, _Rows, _Cols>::Constant(this->rows(), this->cols(), other);
      return *this;
    }

    /** Element-wise += between matrix and basic type */
    Matrix& operator+=(const T& other)
    {
      (*this) = this->Eigen::Matrix<T, _Rows, _Cols>::operator+(Eigen::Matrix<T, _Rows, _Cols>::Constant(this->rows(), this->cols(), other));
      return (*this);
    }

    /** Element-wise - between matrix and basic type */
    Matrix& operator-=(const T& other)
    {
      (*this) = this->Eigen::Matrix<T, _Rows, _Cols>::operator-(Eigen::Matrix<T, _Rows, _Cols>::Constant(this->rows(), this->cols(), other));
      return (*this);
    }

    /** Element-wise + between matrix and basic type */
    const Eigen::CwiseUnaryOp<Eigen::internal::scalar_add_op<T>, const Eigen::ArrayWrapper<Eigen::Matrix<T, _Rows, _Cols> > > operator+(const T& other)
    {
      return this->Eigen::Matrix<T, _Rows, _Cols>::array().operator+(other);
    }

    /** Element-wise - between matrix and basic type */
    const Eigen::CwiseUnaryOp<Eigen::internal::scalar_add_op<T>, const Eigen::ArrayWrapper<Eigen::Matrix<T, _Rows, _Cols> > > operator-(const T& other)
    {
      return this->Eigen::Matrix<T, _Rows, _Cols>::array().operator-(other);
    }

    /** Component-wise product */
    template<typename OtherDerived>
    const Eigen::CwiseBinaryOp<Eigen::internal::scalar_product_op<T, T>, const Eigen::Matrix<T, _Rows, _Cols>, const Eigen::Matrix<double, _Rows, _Cols> > operator%(const Eigen::MatrixBase<OtherDerived>& other)
    {
      return this->Eigen::Matrix<T, _Rows, _Cols>::cwiseProduct(other);
    }

    /** Element-wise log computation */
    const Eigen::CwiseUnaryOp<Eigen::internal::scalar_log_op<T>, const Eigen::ArrayWrapper<Eigen::Matrix<T,  _Rows, _Cols> > > log()
    {
      return this->Eigen::Matrix<T, _Rows, _Cols>::array().log();
    }

    /** Element-wise exp computation */
    const Eigen::CwiseUnaryOp<Eigen::internal::scalar_log_op<T>, const Eigen::ArrayWrapper<Eigen::Matrix<T,  _Rows, _Cols> > > exp()
    {
      return this->Eigen::Matrix<T, _Rows, _Cols>::array().exp();
    }
};

template<typename T, int _Rows = Eigen::Dynamic>
class Vector : public Matrix<T, _Rows, 1>
{
  public:
    Vector() :
      Matrix<T, _Rows, 1>()
    {};

    Vector(int nrow) :
      Matrix<T, _Rows, 1>(nrow, 1)
    {};

    /** This constructor allows to construct Vector from Eigen expressions */
    template<typename OtherDerived>
    Vector(const Eigen::MatrixBase<OtherDerived>& other) :
      Matrix<T, _Rows, 1>(other)
    {};

    /** This method allows to assign Eigen expressions to Vector */
    template<typename OtherDerived>
    Vector& operator=(const Eigen::MatrixBase<OtherDerived>& other)
    {
      this->Matrix<T, _Rows, 1>::operator=(other);
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
};

} // namespace mixt

#endif // MIXT_LINALG_H
