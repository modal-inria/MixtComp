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

#ifndef MIXT_ITERATOR_H
#define MIXT_ITERATOR_H

#include <iterator>

namespace mixt
{

template <typename T>
class iterator : public std::iterator<std::random_access_iterator_tag,
                                      typename T::Type,
                                      int,
                                      typename T::Type*,
                                      typename T::Type&>
{
  public:
    iterator(int i, int j, T& mat) :
      i_(i),
      j_(j),
      rows_(mat.rows()),
      cols_(mat.cols()),
      mat_(mat)
    {}

    iterator(const iterator& it) :
      i_(it.i_),
      j_(it.j_),
      rows_(it.rows_),
      cols_(it.cols_),
      mat_(it.mat_)
    {}

    ~iterator() {}

    int operator-(const iterator& it)
    {
      return (j_ - it.j_) * rows_ + (i_ - it.i_);
    }

    bool operator==(const iterator& it)
    {
      if (i_ == it.i_ && j_ == it.j_ && &mat_ == &it.mat_)
        return true;
      else
        return false;
    }

    bool operator!=(const iterator& it)
    {
      std::cout << "!=, i_: " << i_ << ", j_: " << j_ << std::endl;
      std::cout << "!=, it.i_: " << it.i_ << ", it.j_: " << it.j_ << std::endl;
      if (i_ != it.i_ || j_ != it.j_ || &mat_ != &it.mat_)
        return true;
      else
        return false;
    }

    typename T::Type& operator*()
    {
      std::cout << "*, i_: " << i_ << ", j_: " << j_ << std::endl;
      return mat_(i_, j_);
    }

    const iterator& operator++()
    {
      if (i_ < rows_ - 1)
      {
        ++i_;
      }
      else
      {
        i_ = 0;
        ++j_;
      }
      std::cout << "++, i_: " << i_ << ", j_: " << j_ << std::endl;
      return *this;
    }

    int i_;
    int j_;
    int rows_;
    int cols_;
    T& mat_;
};

} // namespace mixt

#endif // MIXT_LINALG_H
