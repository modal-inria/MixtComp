/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: January 19, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_LINALG_H
#define MIXT_LINALG_H

#include <vector>
#include <Eigen/Dense>
#include "mixt_Typedef.h"

namespace mixt {

template<typename T, int _Rows = Eigen::Dynamic, int _Cols = Eigen::Dynamic>
class Matrix: public Eigen::Matrix<T, _Rows, _Cols> {
public:
	typedef T Type;

	Matrix() :
			Eigen::Matrix<T, _Rows, _Cols>() {
	}

	Matrix(int nrow, int ncol) :
			Eigen::Matrix<T, _Rows, _Cols>(nrow, ncol) {
	}

	Matrix(int nrow, int ncol, T val) :
			Eigen::Matrix<T, _Rows, _Cols>(nrow, ncol) {
		*this = val;
	}

	/** Constructor from Eigen expressions */
	template<typename OtherDerived>
	Matrix(const Eigen::MatrixBase<OtherDerived>& other) :
			Eigen::Matrix<T, _Rows, _Cols>(other) {
	}

	/** Assign Eigen expressions to Matrix */
	template<typename OtherDerived>
	Matrix& operator=(const Eigen::MatrixBase<OtherDerived>& other) {
		this->Eigen::Matrix<T, _Rows, _Cols>::operator=(other);
		return *this;
	}

	/** Element-wise assignment of a scalar */
	Matrix& operator=(const T& scalar) {
		(*this) = Eigen::CwiseNullaryOp<Eigen::internal::scalar_constant_op<T>,
				const Eigen::Matrix<T, _Rows, _Cols> >(this->rows(),
				this->cols(), Eigen::internal::scalar_constant_op<T>(scalar));
		return *this;
	}
};

template<typename T, int _Rows = Eigen::Dynamic>
class Vector: public Matrix<T, _Rows, 1> {
public:
	Vector() :
			Matrix<T, _Rows, 1>() {
	}

	Vector(int nrow) :
			Matrix<T, _Rows, 1>(nrow, 1) {
	}

	Vector(int nrow, T val) :
			Matrix<T, _Rows, 1>(nrow, 1) {
		*this = val;
	}

	/** This constructor allows to construct Vector from Eigen expressions */
	template<typename OtherDerived>
	Vector(const Eigen::MatrixBase<OtherDerived>& other) :
			Matrix<T, _Rows, 1>(other) {
	}

	/** This method allows to assign Eigen expressions to Vector */
	template<typename OtherDerived>
	Vector& operator=(const Eigen::MatrixBase<OtherDerived>& other) {
		this->Matrix<T, _Rows, 1>::operator=(other);
		return *this;
	}

	/** Element-wise assignment of a scalar */
	Vector& operator=(const T& scalar) {
		(*this) = Eigen::CwiseNullaryOp<Eigen::internal::scalar_constant_op<T>,
				const Eigen::Matrix<T, _Rows, 1> >(this->rows(), 1,
				Eigen::internal::scalar_constant_op<T>(scalar));
		return *this;
	}

	/** Initializer list assignment */
	Vector& operator=(const std::initializer_list<T>& list) {
		int size = list.size();
		this->resize(size);
		typename std::initializer_list<T>::const_iterator it = list.begin();
		for (int i = 0, ie = size; i < ie; ++i) {
			(*this)(i) = *it;
			++it;
		}
		return *this;
	}
};

template<typename T, int _Cols = Eigen::Dynamic>
class RowVector: public Matrix<T, 1, _Cols> {
public:
	RowVector() :
			Matrix<T, 1, _Cols>() {
	}
	;

	RowVector(int ncol) :
			Matrix<T, 1, _Cols>(1, ncol) {
	}
	;

	RowVector(int ncol, T val) :
			Matrix<T, 1, _Cols>(1, ncol) {
		*this = val;
	}
	;

	/** This constructor allows to construct RowVector from Eigen expressions */
	template<typename OtherDerived>
	RowVector(const Eigen::MatrixBase<OtherDerived>& other) :
			Matrix<T, 1, _Cols>(other) {
	}
	;

	/** This method allows to assign Eigen expressions to RowVector */
	template<typename OtherDerived>
	RowVector& operator=(const Eigen::MatrixBase<OtherDerived>& other) {
		this->Matrix<T, 1, _Cols>::operator=(other);
		return *this;
	}

	/** Element-wise assignment of a scalar */
	RowVector& operator=(const T& scalar) {
		(*this) = Eigen::CwiseNullaryOp<Eigen::internal::scalar_constant_op<T>,
				const Eigen::Matrix<T, 1, _Cols> >(1, this->cols(),
				Eigen::internal::scalar_constant_op<T>(scalar));
		return *this;
	}

	/** Initializer list assignment */
	RowVector& operator=(const std::initializer_list<T>& list) {
		int size = list.size();
		this->resize(size);
		typename std::initializer_list<T>::const_iterator it = list.begin();
		for (int i = 0, ie = size; i < ie; ++i) {
			(*this)(i) = *it;
			++it;
		}
		return *this;
	}
};

}

#endif // MIXT_LINALG_H
