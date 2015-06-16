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
    Range(DataType min = std::numeric_limits<int>::quiet_NaN(),
          DataType max = std::numeric_limits<int>::quiet_NaN(),
          bool hasRange = false);
    ~Range(){};
    DataType min_;
    DataType max_;
    DataType range_;
    bool hasRange_;
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
    typedef Vector<MisVal> MisData;

    AugmentedData() :
      nbSample_(0),
      misCount_(nb_enum_MisType_),
      dataRange_(),
      rangeUpdate_(false)
      {
        for (int i = 0; i < nb_enum_MisType_; ++i) // initialize counter for each type of missing value to 0
        {
          misCount_(i) = 0;
        }
      };
    ~AugmentedData() {};

    /**
     * Return the correct expression according to the number of individuals
     * @param  Number of individuals
     * @return "individual(s) has/have"
    */
    std::string indExpression(int nbInd)
    {
      std::string num;
      if (misCount_(missing_) == 1)
      {
        num = " individual has a value ";
      }
      else
      {
        num = " individuals have values ";
      }
      return num;
    }

    /**
     * Check the types of missing value.
     * @param  A vector containing true or false according to the mixture support of a a given missing value type
     * @return Description of the eventual error, otherwise empty string
    */
    std::string checkMissingType(Vector<bool> listType)
    {
      std::string warnLog;
      if (listType(missing_) == false && misCount_(missing_) > 0)
      {
        std::stringstream sstm;
        sstm << "Non observed values are not supported for this model, yet "
             << misCount_(missing_)
             << indExpression(misCount_(missing_))
             << "completely missing." << std::endl;
        warnLog += sstm.str();
      }

      if (listType(missingFiniteValues_) == false && misCount_(missingFiniteValues_) > 0)
      {
        std::stringstream sstm;
        sstm << "Partially observed values defined by list of possible values, {a, b, c, ... }, are not supported for this model, yet "
             << misCount_(missingFiniteValues_)
             << indExpression(misCount_(missingFiniteValues_))
             << "defined by list of possible values." << std::endl;
        warnLog += sstm.str();
      }

      if (listType(missingIntervals_) == false && misCount_(missingIntervals_) > 0)
      {
        std::stringstream sstm;
        sstm << "Partially observed values defined by interval, [a:b], are not supported for this model, yet "
             << misCount_(missingIntervals_)
             << indExpression(misCount_(missingIntervals_))
             << "defined by interval." << std::endl;
        warnLog += sstm.str();
      }

      if (listType(missingLUIntervals_) == false && misCount_(missingLUIntervals_) > 0)
      {
        std::stringstream sstm;
        sstm << "Partially observed values defined by upper-bounded semi-interval, [-inf:a], are not supported for this model, yet "
             << misCount_(missingLUIntervals_)
             << indExpression(misCount_(missingLUIntervals_))
             << "defined by upper-bounded semi-interval." << std::endl;
        warnLog += sstm.str();
      }

      if (listType(missingRUIntervals_) == false && misCount_(missingRUIntervals_) > 0)
      {
        std::stringstream sstm;
        sstm << "Partially observed values defined by lower-bounded semi-interval, [a:+inf], are not supported for this model, yet "
             << misCount_(missingRUIntervals_)
             << indExpression(misCount_(missingRUIntervals_))
             << "defined by lower-bounded semi-interval." << std::endl;
        warnLog += sstm.str();
      }
      return warnLog;
    }

    void resizeArrays(int nbSample)
    {
      data_.resize(nbSample);
      misData_.resize(nbSample);
    }

    void computeRange()
    {
      Type min;
      Type max;

      for (int i = 0; i < misData_.rows(); ++i)
      {
        switch (misData_(i).first)
        {
          case present_: // data is present, range is updated directly
          {
            rangeUpdate(min,
                        max,
                        data_(i));
          }
          break;

          /* Data is missing, range is updated using information on the missing value.
           * If data is completely missing, misData_(i, j).second.size() == 0, and no update on the ranges is
           * carried on.
           */
          default:
          {
            for (typename std::vector<Type>::const_iterator it = misData_(i).second.begin();
                 it != misData_(i).second.end();
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
      if (rangeUpdate_ == true)
      {
        dataRange_ = Range<Type>(min, max);
      }
#ifdef MC_DEBUG
      std::cout << "AugmentedData::computeRange" << std::endl;
      std::cout << "min: " << min << ", max: " << max << std::endl;
#endif
    }

    void setPresent(int i, Type val)
    {
#ifdef MC_DEBUG
      std::cout << "AugmentedData::setPresent" << std::endl;
      std::cout << "data_.rows(): " << data_.rows() << std::endl;
      std::cout << "misData_.rows(): " << misData_.rows() << std::endl;
#endif
      data_(i) = val;
      misData_(i) = MisVal(present_,
                           std::vector<Type>());
      ++misCount_(present_);
      ++nbSample_;
    }

    void setMissing(int i, const MisVal& val)
    {
      data_(i) = std::numeric_limits<int>::quiet_NaN(); // set to quiet nan, for types that supports it. For int, the returned value would be 0 ...
      misData_(i) = val;
      ++misCount_(val.first);
      ++nbSample_;
    }

    /** set all individuals to be completely missing, for example for the latent class in unsupervised learning */
    void setAllMissing(int nbInd)
    {
      resizeArrays(nbInd);
      MisVal misVal;
      misVal.first = missing_; // description of completely missing individual
      for (int i = 0; i < nbInd; ++i)
      {
        setMissing(i, misVal);
      }
    }

    /** Remove the missing values by uniform samplings */
    void removeMissing();

    /**
     * In-place sort every descriptor of missing values, and check for duplicates, which are currently forbidden
     * @return Description of the eventual error, otherwise empty string
    */
    std::string sortAndCheckMissing()
    {
      std::string warnLog;

      for (int i = 0; i < nbSample_; ++i)
      {
        std::vector<Type>& currVec = misData_(i).second;
        if (currVec.size() > 1)
        {
          std::sort(currVec.begin(),
                    currVec.end());
          for (unsigned int v = 0; v < currVec.size() - 1; ++ v)
          {
            if (currVec[v] == currVec[v + 1])
            {
              std::stringstream sstm;
              sstm << "Individual " << i
                   << " has duplicate value "
                   << currVec[v]
                   << " in its missing value description. This is never necessary and forbidden." << std::endl;
              warnLog += sstm.str();
            }
          }
        }
      }

      return warnLog;
    }

    /** two dimensional data table, for example a Matrix<Real> */
    DataType data_;

    /** data structure for partially observed values */
    MisData misData_;
    /** total number of values */
    int nbSample_;
    /** Number of each type of missing data
     * Order of indices */
    Vector<int> misCount_;
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
