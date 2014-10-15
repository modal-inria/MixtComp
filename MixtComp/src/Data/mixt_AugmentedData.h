/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

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
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_AUGMENTEDDATA_H
#define MIXT_AUGMENTEDDATA_H

#include<utility>
#include<vector>
#include "Arrays/include/STK_Array2D.h"
#include "../Various/mixt_Def.h"

namespace mixt
{

template <class DataType>
struct Range
{
    Range(DataType min,
          DataType max)
    {
      min_ = min;
      max_ = max;
      range_ = max - min;
    }
    DataType min_;
    DataType max_;
    DataType range_;
};

template <>
struct Range<int>
{
    Range(int min,
          int max)
    {
      min_ = min;
      max_ = max;
      range_ = max - min + 1;
    }
    int min_;
    int max_;
    int range_;
};

template <class DataType>
struct AugmentedData
{
  /** base type of the table */
  typedef typename DataType::Type Type;
  /** type of a missing value descriptor */
  typedef typename std::pair<MisType, std::vector<Type> > MisVal;
  /** type of data structure for partially observed values */
  typedef typename std::map<int, std::map<int, MisVal> > MisData;

  /** iterator on individuals */
  typedef typename std::map<int, std::map<int, MisVal> >::const_iterator ConstIt_MisInd;
  /** iterator on variables for a given individual */
  typedef typename std::map<int, MisVal>::const_iterator ConstIt_MisVar;

  /** two dimensional data table, for example a STK::Array2D<STK::Real> */
  DataType data_;
  /** two dimensionnal array of booleans, to indicate presence or absence of data */
  STK::Array2D<bool> present_;
  /** data structure for partially observed values */
  MisData misData_;
  /** available data ranges, one pair per data column */
  std::vector<Range<Type> > dataRanges_;
};

} // namespace mixt

#endif // MIXT_AUGMENTEDDATA_H
