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

/** Element-wise + between matrix and scalar */
inline const CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                          const Derived>
operator+(const Scalar& scalar) const
{
  return CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                      const Derived>(derived(),
                                     internal::scalar_add_op<Scalar>(scalar));
}

/** Element-wise - between matrix and scalar */
inline const CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                          const Derived>
operator-(const Scalar& scalar) const
{
  return CwiseUnaryOp<internal::scalar_add_op<Scalar>,
                      const Derived>(derived(),
                                     internal::scalar_add_op<Scalar>(-scalar));
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
                           const Derived,
                           const Derived>
operator%(const MatrixBase<OtherDerived>& other) const
{
  return Eigen::CwiseBinaryOp<Eigen::internal::scalar_product_op<Scalar, Scalar>,
                              const Derived,
                              const OtherDerived>(derived(),
                                                  other.derived(),
                                                  Eigen::internal::scalar_product_op<Scalar, Scalar>());
}

/** Element-wise log computation */
const CwiseUnaryOp<internal::scalar_log_op<Scalar>,
                   const Derived>
log() const
{
  return Eigen::CwiseUnaryOp<internal::scalar_log_op<Scalar>,
                             const Derived>(derived(),
                                            internal::scalar_log_op<Scalar>());
}

/** Element-wise exp computation */
const CwiseUnaryOp<internal::scalar_exp_op<Scalar>,
                   const Derived>
exp() const
{
  return Eigen::CwiseUnaryOp<Eigen::internal::scalar_exp_op<Scalar>,
                             const Derived>(derived(),
                                            internal::scalar_exp_op<Scalar>());
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

#endif // MIXT_EIGENMATRIXBASEADDONS_H
