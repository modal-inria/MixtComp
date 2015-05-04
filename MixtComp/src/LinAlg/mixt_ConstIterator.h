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
 *  Created on: May 15, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CONSTITERATOR_H
#define MIXT_CONSTITERATOR_H

// reference on what needs to be implemented for STL iterators: http://stackoverflow.com/questions/7758580/writing-your-own-stl-container/7759622#7759622

//#include <iostream>
//#include <iterator>

class ConstIterator : public std::iterator<std::random_access_iterator_tag,
                                          Scalar,
                                          int,
                                          Scalar*,
                                          Scalar&>
{
  public:
    ConstIterator(int pos, const Derived& mat) :
      pos_(pos),
      rows_(mat.rows()),
      cols_(mat.cols()),
      p_mat_(&mat)
    {}

    ConstIterator(const ConstIterator& it) :
      pos_(it.pos_),
      rows_(it.rows_),
      cols_(it.cols_),
      p_mat_(it.p_mat_)
    {}

    ConstIterator(const Iterator& it) :
      pos_(it.pos_),
      rows_(it.rows_),
      cols_(it.cols_),
      p_mat_(it.p_mat_)
    {}

    ~ConstIterator() {}

    ConstIterator operator+(int i)
    {
      return ConstIterator(pos_ + i, *p_mat_);
    }

    ConstIterator operator-(int i)
    {
      return ConstIterator(pos_ - i, *p_mat_);
    }

    int operator-(const ConstIterator& it)
    {
      return pos_ - it.pos_;
    }

    bool operator<(const ConstIterator& it)
    {
      return pos_ < it.pos_;
    }

    bool operator==(const ConstIterator& it)
    {
      if (pos_ == it.pos_)
        return true;
      else
        return false;
    }

    bool operator!=(const ConstIterator& it)
    {
      if (pos_ != it.pos_)
        return true;
      else
        return false;
    }

    const Scalar& operator*() const
    {
      int i;
      int j;
      posToIn(i, j);
      return (*p_mat_)(i, j);
    }

    const ConstIterator& operator++()
    {
      ++pos_;
      return *this;
    }

    const ConstIterator& operator--()
    {
      --pos_;
      return *this;
    }

    int pos_;
    int rows_;
    int cols_;
    const Derived* p_mat_;

    /** updates indices from linear position */
    void posToIn(int& i, int& j) const
    {
      i = pos_ % rows_;
      j = pos_ / rows_;
    }
};

#endif // MIXT_ITERATOR_H
