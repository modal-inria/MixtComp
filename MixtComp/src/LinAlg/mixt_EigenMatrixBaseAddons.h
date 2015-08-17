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
 *  Created on: January 27, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_EIGENMATRIXBASEADDONS_H
#define MIXT_EIGENMATRIXBASEADDONS_H

#include "mixt_Iterator.h"
#include "mixt_ConstIterator.h"

typedef Iterator iterator;
typedef ConstIterator const_iterator;

/** Element-wise comparison to a scalar */
inline bool operator==(const Scalar& scalar) const
{
  for(int j = 0; j < cols(); ++j)
    for(int i = 0; i < rows(); ++i)
      if(derived().coeff(i, j) != scalar)
        return false;
  return true;
}

/** Element-wise + between matrix and scalar */
inline const CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                          Derived>
operator+(const Scalar& scalar) const
{
  return CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                      Derived>(derived(),
                               internal::scalar_add_op<Scalar>(scalar));
}

/** Element-wise + between a scalar and a matrix */
friend inline const CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                                 Derived>
operator+(const Scalar& scalar,
          const MatrixBase<Derived>& mat)
{
  return CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                      Derived>(mat.derived(),
                               internal::scalar_add_op<Scalar>(scalar));
}

/** Element-wise - between matrix and scalar */
inline const CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                          Derived>
operator-(const Scalar& scalar) const
{
  return CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                      Derived>(derived(),
                               internal::scalar_add_op<Scalar>(-scalar));
}

/** Element-wise - between a scalar and a matrix */
friend inline const CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                                 Derived>
operator-(const Scalar& scalar,
          const MatrixBase<Derived>& mat)
{
  return CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                      Derived>(- mat.derived(),
                               internal::scalar_add_op<Scalar>(scalar));
}

/** Element-wise assignment of a scalar */
inline MatrixBase<Derived>&
operator=(const Scalar& scalar)
{
  (*this) = CwiseNullaryOp<Eigen::internal::scalar_constant_op<Scalar>,
                           Derived >(derived().rows(),
                                     derived().cols(),
                                     internal::scalar_constant_op<Scalar>(scalar));
  return *this;
}

/** Element-wise += between matrix and scalar */
inline MatrixBase<Derived>&
operator+=(const Scalar& scalar)
{
  (*this) = derived() + scalar;
  return *this;
}

/** Element-wise -= between matrix and scalar */
inline MatrixBase<Derived>&
operator-=(const Scalar& scalar)
{
  (*this) = derived() - scalar;
  return *this;
}

/** Component-wise product */
template<typename OtherDerived>
inline const CwiseBinaryOp<Eigen::internal::scalar_product_op<Scalar, Scalar>,
                           Derived,
                           Derived>
operator%(const MatrixBase<OtherDerived>& other) const
{
  return CwiseBinaryOp<internal::scalar_product_op<Scalar, Scalar>,
                       Derived,
                       OtherDerived>(derived(),
                                     other.derived(),
                                     internal::scalar_product_op<Scalar, Scalar>());
}

/** Component-wise quotient */
template<typename OtherDerived>
inline const CwiseBinaryOp<internal::scalar_quotient_op<Scalar, Scalar>,
                           Derived,
                           Derived>
operator/(const MatrixBase<OtherDerived>& other) const
{
  return CwiseBinaryOp<internal::scalar_quotient_op<Scalar, Scalar>,
                       Derived,
                       OtherDerived>(derived(),
                                     other.derived(),
                                     internal::scalar_quotient_op<Scalar, Scalar>());
}

/** Element-wise %= between matrices */
template<typename OtherDerived>
inline MatrixBase<Derived>&
operator%=(const MatrixBase<OtherDerived>& other)
{
  (*this) = derived() % other;
  return *this;
}

/** Element-wise /= between matrices */
template<typename OtherDerived>
inline MatrixBase<Derived>&
operator/=(const MatrixBase<OtherDerived>& other)
{
  (*this) = derived() / other;
  return *this;
}

/** Element-wise log computation */
inline const CwiseUnaryOp<internal::scalar_log_op<Scalar>,
                          Derived>
log() const
{
  return CwiseUnaryOp<internal::scalar_log_op<Scalar>,
                      Derived>(derived(),
                               internal::scalar_log_op<Scalar>());
}

/** Element-wise exp computation */
inline const CwiseUnaryOp<internal::scalar_exp_op<Scalar>,
                          Derived>
exp() const
{
  return CwiseUnaryOp<internal::scalar_exp_op<Scalar>,
                      Derived>(derived(),
                               internal::scalar_exp_op<Scalar>());
}

/** Element-wise abs computation */
inline const CwiseUnaryOp<internal::scalar_abs_op<Scalar>,
                          Derived>
abs() const
{
  return CwiseUnaryOp<internal::scalar_abs_op<Scalar>,
                      Derived>(derived(),
                               internal::scalar_abs_op<Scalar>());
}

/** Element-wise inverse computation */
inline const CwiseUnaryOp<internal::scalar_inverse_op<Scalar>,
                          Derived>
cInv() const
{
  return CwiseUnaryOp<internal::scalar_inverse_op<Scalar>,
                      Derived>(derived(),
                               internal::scalar_inverse_op<Scalar>());
}

/** Element-wise + between a scalar and a matrix */
friend inline const CwiseUnaryOp<internal::scalar_multiple_op<Scalar>,
                                 const CwiseUnaryOp<internal::scalar_inverse_op<Scalar>,
                                                    Derived> >
operator/(const Scalar& scalar,
          const MatrixBase<Derived>& mat)
{
  return scalar * mat.cInv();
}

iterator begin()
{
  return Iterator(0,
                  derived());
}

const_iterator begin() const
{
  return ConstIterator(0,
                       derived());
}

iterator end()
{
  return Iterator(derived().rows() * derived().cols(),
                  derived());
}

const_iterator end() const
{
  return ConstIterator(derived().rows() * derived().cols(),
                       derived());
}

/** General sort of data that is contiguous in memory (use of pointer to data and size).
 * Be advised against using it against subparts (i.e. blocks) of matrices which elements are
 * not stored contiguously */
void sortContiguous()
{
  std::sort(derived().data(),
            derived().data() + derived().size());
}

/** Sort function for non contiguous data, for example block. Slower than sortContiguous */
void sort()
{
  std::sort(derived().begin(),
            derived().end());
}

/**
 * Computation of a multinomial distribution from log values of weights.
 * A common example of usage is the computations of the proportions t_ik
 * from the logProbabilities. A const-cast is used to allow usage of temporary object !
 *
 * @param log vector of log values
 * @param[out] multi multinomial distribution
 * @value log of the sum of all components
 */
template<typename OtherDerived>
Scalar logToMulti(const MatrixBase<OtherDerived>& multi) const
{
  const_cast<MatrixBase<OtherDerived>& >(multi) = derived();
  Scalar max = multi.maxCoeff();
  const_cast<MatrixBase<OtherDerived>& >(multi) -= max;
  const_cast<MatrixBase<OtherDerived>& >(multi) = multi.exp();
  Scalar sum = multi.sum();
  const_cast<MatrixBase<OtherDerived>& >(multi) = multi / sum;

  return max + std::log(sum);
}

#endif // MIXT_EIGENMATRIXBASEADDONS_H
