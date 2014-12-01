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
class Range
{
  public:
    Range(DataType min,
          DataType max);
    ~Range(){};
    DataType min_;
    DataType max_;
    DataType range_;
};

template <typename DataType>
class AugmentedData
{
  public:
    /** Base type of the data table, for example, STK::Real */
    typedef typename DataType::Type Type;
    /** Missing value descriptor: type of missing, and list of parameters */
    typedef typename std::pair<MisType, std::vector<Type> > MisVal;

    /** type of the complete structure for missing data */
    typedef typename STK::Array2D<MisVal> MisData;

    AugmentedData() :
      nbMissing_(0),
      globalRange_(Type(0),
                   Type(0))
      {};
    ~AugmentedData() {};

    void resizeArrays(int nbSample, int nbVariable)
    {
      data_.resize(nbSample, nbVariable);
      data_ = Type(0);
      misData_.resize(nbSample, nbVariable);
    }

    void computeRanges()
    {
      Type globMin;
      Type globMax;
      // data range filling
      for (int currVar = 0; currVar < data_.sizeCols(); ++currVar)
      {
        int currMin = STK::Stat::minSafe(data_.col(currVar));
        int currMax = STK::Stat::maxSafe(data_.col(currVar));
        if (currVar == 0)
        {
          globMin = currMin;
          globMax = currMax;
        }
        else
        {
          if (currMin < globMin) globMin = currMin;
          if (currMax > globMax) globMax = currMax;
        }
        dataRanges_.push_back(Range<Type>(currMin,
                                          currMax));
#ifdef MC_DEBUG
        std::cout << "AugmentedData::computeRange()" << std::endl;
        std::cout << "\tcurrVar: " << currVar << std::endl
                  << "\t\tmin: " << dataRanges_.at(currVar).min_ << std::endl
                  << "\t\tmax, " << dataRanges_.at(currVar).max_ << std::endl
                  << "m_augDataij_.data_ before removeMissing: " << std::endl
                  << data_ << std::endl;
#endif
      }
      globalRange_ = Range<Type>(globMin,
                                 globMax);
    }

    void setPresent(int i, int j, Type val)
    {
#ifdef MC_DEBUG
      std::cout << "AugmentedData::setPresent" << std::endl;
      std::cout << "data_.sizeRows(): " << data_.sizeRows() << ", data_.sizeCols(): " << data_.sizeCols() << std::endl;
      std::cout << "misData_.sizeRows(): " << misData_.sizeRows() << ", misData_.sizeCols(): " << misData_.sizeCols() << std::endl;
#endif
      data_(i, j) = val;
      misData_(i, j) = MisVal(present_,
                              std::vector<Type>());
    }

    void setMissing(int i, int j, MisVal& val)
    {
      data_(i, j) = STK::Arithmetic<Type>::NA();
      misData_(i, j) = val;
      ++nbMissing_;
    }

    /** Remove the missing values by uniform samplings */
    void removeMissing();

    /** two dimensional data table, for example a STK::Array2D<STK::Real> */
    DataType data_;

    /** data structure for partially observed values */
    MisData misData_;
    /** total number of partially observed values, used to output the results */
    int nbMissing_;
    /** available data ranges, one pair per data column */
    std::vector<Range<Type> > dataRanges_;

    /** maximum range, for example to get the global number of modalities */
    Range<Type> globalRange_;
};

} // namespace mixt

#endif // MIXT_AUGMENTEDDATA_H
