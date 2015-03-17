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

#include <limits>
#include <utility>
#include <vector>
#include "../LinAlg/mixt_LinAlg.h"
#include "../Various/mixt_Def.h"
#include "../Various/mixt_Constants.h"
#include "../Statistic/mixt_UniformStatistic.h"
#include "../Statistic/mixt_MultinomialStatistic.h"

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
    /** Base type of the data table, for example, Real */
    typedef typename DataType::Type Type;
    /** Missing value descriptor: type of missing, and list of parameters */
    typedef typename std::pair<MisType, std::vector<Type> > MisVal;

    /** type of the complete structure for missing data */
    typedef Matrix<MisVal> MisData;

    AugmentedData() :
      nbSample_(0),
      nbPresent_(0),
      nbMissing_(0),
      dataRange_(Type(0),
                 Type(0)),
      rangeUpdate_(false)
      {};
    ~AugmentedData() {};

    void resizeArrays(int nbSample)
    {
      data_.resize(nbSample, 1);
      data_ = Type(0);
      misData_.resize(nbSample, 1);
    }

    void computeRange()
    {
      Type min;
      Type max;

      for (int i = 0; i < misData_.rows(); ++i)
      {
        for (int j = 0; j < misData_.cols(); ++j)
        {
          switch (misData_(i, j).first)
          {
            case present_: // data is present, range is updated directly
            {
              rangeUpdate(min,
                          max,
                          data_(i, j));
            }
            break;

            /* Data is missing, range is updated using information on the missing value.
             * If data is completely missing, misData_(i, j).second.size() == 0, and no update on the ranges is
             * carried on.
             */
            default:
            {
              for (typename std::vector<Type>::const_iterator it = misData_(i, j).second.begin();
                   it != misData_(i, j).second.end();
                   ++it)
              {
                rangeUpdate(min,
                            max,
                            *it);
              }
            }
            break;
          }
        }
      }
      if (rangeUpdate_ == true)
      {
        dataRange_ = Range<Type>(min, max);
      }
      else // in prediction, a variable can be completely missing
      {
        dataRange_ = Range<Type>(minModality,
                                 minModality + 0.5); // default range, should allow for initialization in prediction for all types of data
      }
#ifdef MC_DEBUG
      std::cout << "AugmentedData::computeRange" << std::endl;
      std::cout << "min: " << min << ", max: " << max << std::endl;
#endif
    }

    void setPresent(int i, int j, Type val)
    {
#ifdef MC_DEBUG
      std::cout << "AugmentedData::setPresent" << std::endl;
      std::cout << "data_.rows(): " << data_.rows() << ", data_.cols(): " << data_.cols() << std::endl;
      std::cout << "misData_.rows(): " << misData_.rows() << ", misData_.cols(): " << misData_.cols() << std::endl;
#endif
      data_(i, j) = val;
      misData_(i, j) = MisVal(present_,
                              std::vector<Type>());
      ++nbPresent_;
      ++nbSample_;
    }

    void setMissing(int i, int j, const MisVal& val)
    {
      data_(i, j) = std::numeric_limits<int>::quiet_NaN(); // set to quiet nan, for types that supports it. For int, the returned value would be 0 ...
      misData_(i, j) = val;
      ++nbMissing_;
      ++nbSample_;
    }

    /** set all individuals to be completely missing, for example for the latent class in unsupervised learning */
    void setAllMissing(int nbSample)
    {
      resizeArrays(nbSample);
      MisVal misVal;
      misVal.first = missing_; // description of completely missing individual
      for (int i = 0; i < nbSample; ++i)
      {
        setMissing(i, 1, misVal);
      }
    }

    /** Remove the missing values by uniform samplings */
    void removeMissing();

    /** Remove the missing values by uniform samplings, taking into account that classes begin at 0 no matter the minModality value */
    void removeMissingClass();

    /** two dimensional data table, for example a Matrix<Real> */
    DataType data_;

    /** data structure for partially observed values */
    MisData misData_;
    /** total number of values */
    int nbSample_;
    /** total number of present values */
    int nbPresent_;
    /** total number of partially observed values, used to output the results */
    int nbMissing_;
    /** available data ranges, one pair per data column */
    Range<Type> dataRange_;

  private:
    /** boolean to know if range has already been updated */
    bool rangeUpdate_;

    void rangeUpdate(Type& min,
                     Type& max,
                     const Type& val)
    {
      if (!rangeUpdate_)
      {
        min = val;
        max = val;
        rangeUpdate_ = true;
      }
      else
      {
        if (val < min) min = val;
        if (val > max) max = val;
      }
    }

    /** Uniform law*/
    UniformStatistic uniform_;

    /** multinomial law */
    MultinomialStatistic multi_;
};

} // namespace mixt

#endif // MIXT_AUGMENTEDDATA_H
