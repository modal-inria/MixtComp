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

template<typename T>
class Matrix : public Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>
{
  public:
    Matrix(int nrow, int ncol) :
      Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(nrow, ncol)
    {};
};

template<typename T>
class Vector : public Eigen::Matrix<T, Eigen::Dynamic, 1>
{
  public:
    Vector(int nrow) :
      Eigen::Matrix<T, Eigen::Dynamic, 1>(nrow)
    {};

    /** Copy from a lightweight block object representing a subpart of a matrix */
    Vector(typename Eigen::Block<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>, Eigen::Dynamic, Eigen::Dynamic> block) :
      Eigen::Matrix<T, Eigen::Dynamic, 1>(block)
    {};
};

template<typename T>
class RowVector : public Eigen::Matrix<T, 1, Eigen::Dynamic>
{
  public:
    RowVector(int ncol) :
      Eigen::Matrix<T, 1, Eigen::Dynamic>(ncol)
    {};

    /** Copy from a lightweight block object representing a subpart of a matrix */
    RowVector(typename Eigen::Block<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>, Eigen::Dynamic, Eigen::Dynamic> block) :
      Eigen::Matrix<T, 1, Eigen::Dynamic>(block)
    {};
};

} // namespace mixt

#endif // MIXT_LINALG_H
