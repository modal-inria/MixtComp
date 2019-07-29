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
 *  Created on: January 27, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef EIGENMATRIXBASEADDONS_H
#define EIGENMATRIXBASEADDONS_H

//#include <numeric>

#include <LinAlg/Iterator.h>
#include "ConstIterator.h"

typedef Iterator iterator;
typedef ConstIterator const_iterator;

/**
 * VECTOR OPERATIONS
 * Note that E is added as a suffix to design elementwise operation, because exp() and log() are partially defined for matrix,
 * but not completely implemented in Eigen.
 */

/** log(v) */
inline const CwiseUnaryOp<internal::scalar_log_op<Scalar>, Derived> logE() const {
	return CwiseUnaryOp<internal::scalar_log_op<Scalar>, Derived>(derived(), internal::scalar_log_op<Scalar>());
}

/** exp(v) */
inline const CwiseUnaryOp<internal::scalar_exp_op<Scalar>, Derived> expE() const {
	return CwiseUnaryOp<internal::scalar_exp_op<Scalar>, Derived>(derived(), internal::scalar_exp_op<Scalar>());
}

/** abs(v) */
inline const CwiseUnaryOp<internal::scalar_abs_op<Scalar>, Derived> absE() const {
	return CwiseUnaryOp<internal::scalar_abs_op<Scalar>, Derived>(derived(), internal::scalar_abs_op<Scalar>());
}

/** v^-1 */
inline const CwiseUnaryOp<internal::scalar_inverse_op<Scalar>, Derived> cInvE() const {
	return CwiseUnaryOp<internal::scalar_inverse_op<Scalar>, Derived>(derived(), internal::scalar_inverse_op<Scalar>());
}

/**
 * VECTOR SCALAR OPERATIONS
 */

/** v == s */
inline bool operator==(const Scalar& scalar) const {
	for (int j = 0; j < cols(); ++j)
		for (int i = 0; i < rows(); ++i)
			if (derived().coeff(i, j) != scalar)
				return false;
	return true;
}

/** v != s */
inline bool operator!=(const Scalar& scalar) const {
	for (int j = 0; j < cols(); ++j)
		for (int i = 0; i < rows(); ++i)
			if (derived().coeff(i, j) != scalar)
				return true;
	return false;
}

/** v > s */
inline bool operator>(Scalar rhs) const {
	typename Derived::const_iterator lhsIt = derived().begin();
	for (; lhsIt != derived().end(); ++lhsIt) {
		if (*lhsIt <= rhs) {
			return false; // exit as soon as sufficient conditions are met
		}
	}
	return true;
}

/** v < s */
inline bool operator<(Scalar rhs) const {
	typename Derived::const_iterator lhsIt = derived().begin();
	for (; lhsIt != derived().end(); ++lhsIt) {
		if (*lhsIt >= rhs) {
			return false;
		}
	}
	return true;
}

/** v + s */
const CwiseBinaryOp<internal::scalar_sum_op<Scalar>, const Derived, const ConstantReturnType> operator+(const Scalar& scalar) const {
	return CwiseBinaryOp<internal::scalar_sum_op<Scalar>, const Derived, const ConstantReturnType>(derived(), MatrixBase < Derived > ::Constant(rows(), cols(), scalar));
}

/** v - s */
const CwiseBinaryOp<internal::scalar_difference_op<Scalar>, const Derived, const ConstantReturnType> operator-(const Scalar& scalar) const {
	return CwiseBinaryOp<internal::scalar_difference_op<Scalar>, const Derived, const ConstantReturnType>(derived(), MatrixBase < Derived > ::Constant(rows(), cols(), scalar));
}

/** v = s */
inline MatrixBase<Derived>&
operator=(const Scalar& scalar) {
	(*this) = CwiseNullaryOp<Eigen::internal::scalar_constant_op<Scalar>, Derived>(derived().rows(), derived().cols(), internal::scalar_constant_op < Scalar > (scalar));
	return *this;
}

/** v += s */
inline MatrixBase<Derived>&
operator+=(const Scalar& scalar) {
	(*this) = derived() + scalar;
	return *this;
}

/** v -= s */
inline MatrixBase<Derived>&
operator-=(const Scalar& scalar) {
	(*this) = derived() - scalar;
	return *this;
}

/**
 * SCALAR VECTOR OPERATIONS
 */

/** s + v */
friend const CwiseBinaryOp<internal::scalar_sum_op<Scalar>, const ConstantReturnType, Derived> operator+(const Scalar& scalar, const MatrixBase<Derived>& mat) {
	return CwiseBinaryOp<internal::scalar_sum_op<Scalar>, const ConstantReturnType, Derived>(MatrixBase < Derived > ::Constant(mat.rows(), mat.cols(), scalar), mat.derived());
}

/** s - v */
friend const CwiseBinaryOp<internal::scalar_difference_op<Scalar>, const ConstantReturnType, Derived> operator-(const Scalar& scalar, const MatrixBase<Derived>& mat) {
	return CwiseBinaryOp<internal::scalar_difference_op<Scalar>, const ConstantReturnType, Derived>(MatrixBase < Derived > ::Constant(mat.rows(), mat.cols(), scalar), mat.derived());
}

/** s / v */
friend const CwiseBinaryOp<internal::scalar_quotient_op<Scalar>, const ConstantReturnType, Derived> operator/(const Scalar& scalar, const MatrixBase<Derived>& mat) {
	return CwiseBinaryOp<internal::scalar_quotient_op<Scalar>, const ConstantReturnType, Derived>(MatrixBase < Derived > ::Constant(mat.rows(), mat.cols(), scalar), mat.derived());
}

/**
 * VECTOR VECTOR OPERATIONS
 * Note that since we use the matrix API, most of the matrix / matrix operations are already accessible directly via the API
 */

/** v < v */
inline bool operator<(const Derived& rhs) const {
	typename Derived::const_iterator lhsIt = derived().begin();
	typename Derived::const_iterator rhsIt = rhs.begin();
	for (; lhsIt != derived().end(); ++lhsIt, ++rhsIt) {
		if (*lhsIt < *rhsIt) {
			return true;
		} else if (*lhsIt > *rhsIt) {
			return false;
		}
	}

	return false; // equality of all terms
}

/** v > v */
inline bool operator>(const Derived& rhs) const {
	typename Derived::const_iterator lhsIt = derived().begin();
	typename Derived::const_iterator rhsIt = rhs.begin();
	for (; lhsIt != derived().end(); ++lhsIt, ++rhsIt) {
		if (*lhsIt > *rhsIt) {
			return true;
		} else if (*lhsIt < *rhsIt) {
			return false;
		}
	}

	return false;
}

/** v * v */
template<typename OtherDerived>
inline const CwiseBinaryOp<Eigen::internal::scalar_product_op<Scalar, Scalar>, Derived, Derived> operator%(const MatrixBase<OtherDerived>& other) const {
	return CwiseBinaryOp<internal::scalar_product_op<Scalar, Scalar>, Derived, OtherDerived>(derived(), other.derived(), internal::scalar_product_op<Scalar, Scalar>());
}

/** v / v */
template<typename OtherDerived>
inline const CwiseBinaryOp<internal::scalar_quotient_op<Scalar, Scalar>, Derived, Derived> operator/(const MatrixBase<OtherDerived>& other) const {
	return CwiseBinaryOp<internal::scalar_quotient_op<Scalar, Scalar>, Derived, OtherDerived>(derived(), other.derived(), internal::scalar_quotient_op<Scalar, Scalar>());
}

/** v %= v */
template<typename OtherDerived>
inline MatrixBase<Derived>&
operator%=(const MatrixBase<OtherDerived>& other) {
	(*this) = derived() % other;
	return *this;
}

/** v /= v */
template<typename OtherDerived>
inline MatrixBase<Derived>&
operator/=(const MatrixBase<OtherDerived>& other) {
	(*this) = derived() / other;
	return *this;
}

/**
 * ITERATORS, SORT, LOGTOMULTI
 */

iterator begin() {
	return Iterator(0, 0, derived());
}

const_iterator begin() const {
	return ConstIterator(0, 0, derived());
}

iterator end() {
	return Iterator(0, derived().cols(), derived());
}

const_iterator end() const {
	return ConstIterator(0, derived().cols(), derived());
}

/** Sort function for non contiguous data, for example block. Slower than sortContiguous */
void sort() {
	std::sort(derived().begin(), derived().end());
}

/** Sorted indices of the container */
template<typename Container>
void sortIndex(Container& out) const {
	out.resize(derived().size());
	for (int i = 0, ie = out.size(); i < ie; ++i) {
		out(i) = i;
	}
	std::sort(out.begin(), out.end(), [this](int left, int right) {return (*this)(left) < (*this)(right);});
}

/**
 * Computation of a multinomial distribution from log values of weights.
 * A common example of usage is the computations of the proportions t_ik
 * from the logProbabilities.
 *
 * This method modifies directly the calling object. This is why there are
 * no return value nor reference output. derived() is the calling object being
 * modified.
 *
 * @param log vector of log values
 * @param[out] multi multinomial distribution
 * @value log of the marginal probability
 */
template<typename OtherDerived>
Scalar logToMulti(const MatrixBase<OtherDerived>& multi) {
	derived() = multi;
	Scalar max = derived().maxCoeff();
	derived() -= max;
	derived() = derived().expE();
	Scalar sum = derived().sum();
	derived() = derived() / sum;

	return max + std::log(sum);
}

/**
 * Similar to logToMulti, except that the vector of log probability is exported.
 * It is useful for cases where the proba is needed in a computation, rather
 * than for sampling.
 *
 * Note that the calling object must be of the same type as the argument. It did not
 * compile easily using MatrixBase<OtherDerived> as the type of exp.
 */
Scalar logProbaToLogMulti(const Derived& multi) {
	derived() = multi;
	Scalar max = derived().maxCoeff();
	derived() -= max;

	Derived exp(derived().expE());
	Scalar logSum = std::log(exp.sum());
	derived() -= logSum;

	return max + logSum;
}

template<typename Type>
void copyIterable(const Type& other) {
	derived().resize(other.size());

	int currCoeff = 0;
	for (typename Type::const_iterator it = other.begin(), itE = other.end(); it != itE; ++it, ++currCoeff) {
		derived()(currCoeff) = *it;
	}
}

#endif // EIGENMATRIXBASEADDONS_H
