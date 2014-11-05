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

#include <utility>
#include <vector>
#include "Arrays/include/STK_Array2D.h"
#include "STatistiK/include/STK_Stat_Functors.h"
#include "../Various/mixt_Def.h"

namespace mixt
{

template <typename DataType>
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

template <typename DataType>
class AugmentedData
{
  public:
    AugmentedData() :
      nbMissing_(0)
      {};
    ~AugmentedData() {};

    /** Base type of the data table, for example, STK::Real */
    typedef typename DataType::Type Type;
    /** Missing value descriptor: type of missing, and list of parameters */
    typedef typename std::pair<MisType, std::vector<Type> > MisVal;

    /** type of the complete structure for missing data */
    typedef typename std::map<int, std::map<int, MisVal> > MisData;
    /** type of an individual, as returned by getInd(). Map variable index -> (type of missing, list of parameters) */
    typedef typename std::map<int, MisVal> IndType;

    /** iterator on individuals */
    typedef typename MisData::const_iterator ConstIt_MisInd;
    /** iterator on variables for a given individual */
    typedef typename std::map<int, MisVal>::const_iterator ConstIt_MisVar;

    /** Individual to be retrieved */
    const IndType& getInd(int i) const
    {
      if (misData_.find(i) == misData_.end())
        return emptyInd_;
      else
        return misData_.find(i)->second;
    }

    void resizeArrays(int nbSample, int nbVariable)
    {
      data_.resize(nbSample, nbVariable);
      present_.resize(nbSample, nbVariable);
      data_ = Type(0);
      present_ = false;
    }

    void computeRanges()
    {
      // data range filling
      for (int currVar = 0; currVar < data_.sizeCols(); ++currVar)
      {
        dataRanges_.push_back(Range<Type>(STK::Stat::minSafe(data_.col(currVar)),
                                          STK::Stat::maxSafe(data_.col(currVar))));
#ifdef MC_DEBUG
        std::cout << "AugmentedData::computeRange()" << std::endl;
        std::cout << "\tcurrVar: " << currVar << std::endl
                  << "\t\tmin: " << dataRanges_.at(currVar).min_ << std::endl
                  << "\t\tmax, " << dataRanges_.at(currVar).max_ << std::endl
                  << "m_augDataij_.data_ before removeMissing: " << std::endl
                  << data_ << std::endl;
#endif
      }
    }

    void setPresent(int i, int j, Type val)
    {
      data_(i, j) = val;
      present_(i, j) = true;
    }

    void setMissing(int i, int j, MisVal& val)
    {
      misData_[i][j] = val;
      data_(i, j) = STK::Arithmetic<Type>::NA();
      ++nbMissing_;
      present_(i, j) = false;
    }

    /** Remove the missing values by uniform samplings */
    void removeMissing();

    /** two dimensional data table, for example a STK::Array2D<STK::Real> */
    DataType data_;
    /** two dimensional array of booleans, to indicate presence or absence of data */
    STK::Array2D<bool> present_;
    /** data structure for partially observed values */
    MisData misData_;
    /** total number of partially observed values, used to output the results */
    int nbMissing_;
    /** available data ranges, one pair per data column */
    std::vector<Range<Type> > dataRanges_;

  private:
    const IndType emptyInd_; // empty map, to be returned for empty individual
};

} // namespace mixt

#endif // MIXT_AUGMENTEDDATA_H
