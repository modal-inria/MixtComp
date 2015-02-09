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

// reference on what needs to be implemented for STL iterators: http://stackoverflow.com/questions/7758580/writing-your-own-stl-container/7759622#7759622

#include <iostream>
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
    iterator(int pos, T& mat) :
      pos_(pos),
      rows_(mat.rows()),
      cols_(mat.cols()),
      p_mat_(&mat)
    {
#ifdef MC_DEBUG
      std::cout << "iterator(int pos, T& mat)" << std::endl;
#endif
    }

    iterator(const iterator& it) :
      pos_(it.pos_),
      rows_(it.rows_),
      cols_(it.cols_),
      p_mat_(it.p_mat_)
    {
#ifdef MC_DEBUG
      std::cout << "iterator(const iterator& it)" << std::endl;
#endif
    }

    ~iterator() {}

    iterator operator+(int i)
    {
#ifdef MC_DEBUG
      std::cout << "iterator operator+(int i)" << std::endl;
#endif
      return iterator(pos_ + i, *p_mat_);
    }

    iterator operator-(int i)
    {
#ifdef MC_DEBUG
      std::cout << "iterator operator-(int i)" << std::endl;
#endif
      return iterator(pos_ - i, *p_mat_);
    }

    int operator-(const iterator& it)
    {
#ifdef MC_DEBUG
      std::cout << "int operator-(const iterator& it)" << std::endl;
#endif
      return pos_ - it.pos_;
    }

    bool operator<(const iterator& it)
    {
#ifdef MC_DEBUG
      std::cout << "bool operator<(const iterator& it)" << std::endl;
#endif
      return pos_ < it.pos_;
    }

    bool operator==(const iterator& it)
    {
#ifdef MC_DEBUG
      std::cout << "bool operator==(const iterator& it)" << std::endl;
#endif
      if (pos_ == it.pos_)
        return true;
      else
        return false;
    }

    bool operator!=(const iterator& it)
    {
#ifdef MC_DEBUG
      std::cout << "bool operator!=(const iterator& it)" << std::endl;
#endif
      std::cout << "!=, pos_: " << pos_ << ", it.pos_: " << it.pos_ << std::endl;
      if (pos_ != it.pos_)
        return true;
      else
        return false;
    }

    typename T::Type& operator*() const
    {
#ifdef MC_DEBUG
      std::cout << "typename T::Type& operator*() const" << std::endl;
#endif
      int i;
      int j;
      posToIn(i, j);
      std::cout << "*, pos_: " << pos_ << ", i: " << i << ", j: " << j << std::endl;
      return (*p_mat_)(i, j);
    }

    const iterator& operator++()
    {
#ifdef MC_DEBUG
      std::cout << "const iterator& operator++()" << std::endl;
#endif
      ++pos_;
      return *this;
    }

    const iterator& operator--()
    {
#ifdef MC_DEBUG
      std::cout << "const iterator& operator--()" << std::endl;
#endif
      --pos_;
      return *this;
    }

  protected:
    int pos_;
    int rows_;
    int cols_;
    T* p_mat_;

    /** updates indices from linear position */
    void posToIn(int& i, int& j) const
    {
      i = pos_ % rows_;
      j = pos_ / rows_;
    }
};

} // namespace mixt

#endif // MIXT_LINALG_H
