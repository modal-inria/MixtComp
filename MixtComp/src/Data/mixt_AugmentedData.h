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

namespace mixt
{

template <class DataType>
struct AugmentedData
{
  typedef typename DataType::Type Type;
  /** combination of a sample number and a column number to identify a value */
  typedef std::pair<int, int> pos;
  /** two dimensionnal data table, for example a STK::Array2D<STK::Real> */
  DataType data_;
  /** available data ranges, one pair per data column */
  std::vector<std::pair<Type, Type> > dataRanges_;
  /** vector of completely unknown values */
  std::vector<          pos                          > v_missing_;
  /** vector of values to be selected among a finite number of possibilities */
  std::vector<std::pair<pos, std::vector<Type>     > > v_missingFiniteValues_;
  /** vector of values restricted to an interval */
  std::vector<std::pair<pos, std::pair<Type, Type> > > v_missingIntervals_;
  /** vector of values restricted to a left unbounded interval [- inf, a] */
  std::vector<std::pair<pos,           Type        > > v_missingLUIntervals_;
  /** vector of values restricted to a right unbounded interval [a, +inf] */
  std::vector<std::pair<pos,           Type        > > v_missingRUIntervals_;
};

} // namespace mixt

#endif // MIXT_AUGMENTEDDATA_H
