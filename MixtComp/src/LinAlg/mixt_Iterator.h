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

#ifndef MIXT_ITERATOR_H
#define MIXT_ITERATOR_H

// reference on what needs to be implemented for STL iterators: http://stackoverflow.com/questions/7758580/writing-your-own-stl-container/7759622#7759622

//#include <iostream>
//#include <iterator>

/**
 * Iterator class for Eigen::Matrix. This file is included from mixt_EigenMatrixBaseAddons.h and the Iterator class is therefor not a member of the namespace mixt::.
 * It is accessible in MixtComp through the derived class mixt::Matrix.
 */

class Iterator : public std::iterator<std::random_access_iterator_tag,
                                      Scalar,
                                      int,
                                      Scalar*,
                                      Scalar&>
{
  public:
    Iterator(int i,
             int j,
             Derived& mat) :
      i_(i),
      j_(j),
      rows_(mat.rows()),
      p_mat_(&mat)
    {}

    Iterator operator+(int i)
    {
      int posP, iP, jP;
      posP = pos();
      posP += i;
      posToIn(posP, iP, jP);
      return Iterator(iP, jP, *p_mat_);
    }

    Iterator operator-(int i)
    {
      int posP, iP, jP;
      posP = pos();
      posP -= i;
      posToIn(posP, iP, jP);
      return Iterator(iP, jP, *p_mat_);
    }

    int operator-(const Iterator& it)
    {
      return pos() - it.pos();
    }

    bool operator<(const Iterator& it)
    {
      if (j_ < it.j_)
      {
        return true;
      }
      else if (j_ > it.j_)
      {
        return false;
      }
      else if (i_ < it.i_) // in this case, j_ == it.j_, hence comparison must be done on i_
      {
        return true;
      }

      return false;
    }

    bool operator==(const Iterator& it)
    {
      if (i_ == it.i_ && j_ == it.j_)
        return true;
      else
        return false;
    }

    bool operator!=(const Iterator& it)
    {
      if (i_ != it.i_ || j_ != it.j_)
        return true;
      else
        return false;
    }

    Scalar& operator*() const
    {
      return (*p_mat_)(i_, j_);
    }

    Scalar* operator->() const
    {
      return &(*p_mat_)(i_, j_);
    }

    const Iterator& operator++()
    {
      if (i_ < rows_ - 1) // row increment
      {
        ++i_;
      }
      else // column increment
      {
        i_ = 0;
        ++j_;
      }
      return *this;
    }

    const Iterator& operator--()
    {
      if (i_ > 0)
      {
        --i_;
      }
      else
      {
        i_ = rows_ - 1;
        --j_;
      }
      return *this;
    }

    int i_;
    int j_;
    int rows_;
    Derived* p_mat_;

    void posToIn(int pos,
                 int& i,
                 int& j) const
    {
      std::div_t divresult;
      divresult = std::div(pos, rows_);

      i = divresult.rem;
      j = divresult.quot;
    }

    int pos() const
    {
      return j_ * rows_ + i_;
    }
};

#endif // MIXT_ITERATOR_H
