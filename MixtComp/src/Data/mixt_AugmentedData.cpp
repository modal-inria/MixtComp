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

#include "STatistiK/include/STK_Law_Uniform.h"
#include "STatistiK/include/STK_Law_Categorical.h"
#include "mixt_AugmentedData.h"

namespace mixt
{

template<>
Range<STK::Real>::Range(STK::Real min,
                        STK::Real max) :
    min_(min),
    max_(max),
    range_(max - min)
{}

template<>
Range<int>::Range(int min,
                  int max) :
    min_(min),
    max_(max),
    range_(max - min + 1) // used to store the number of modalities, for example
{}

template<>
void AugmentedData<STK::Array2D<STK::Real> >::removeMissing()
{
  if (!rangeUpdate_)
  {
    for (int j = 0; j < misData_.cols(); ++j)
    {
      for (int i = 0; i < misData_.rows(); ++i)
      {
        if (misData_(i, j).first != present_)
        {
          data_(i, j) = 0.;
        }
      }
    }
  }
  else
  {
  for (int j = 0; j < misData_.cols(); ++j)
  {
    for (int i = 0; i < misData_.rows(); ++i)
    {
      if (misData_(i, j).first != present_)
      {
        STK::Real sampleVal;
        switch(misData_(i, j).first) // (iterator on map)->(mapped element).(MisType)
        {
          case present_:
          {}
          break;

          case missing_:
          {
            STK::Real min = dataRange_.min_;
            STK::Real max = dataRange_.max_;
            sampleVal = STK::Law::Uniform::rand(min, max);
          }
          break;

          case missingFiniteValues_: // no missing finite values for continuous data
          {}
          break;

          case missingIntervals_:
          {
  #ifdef MC_DEBUG
            std::cout << "AugmentedData<STK::Array2D<STK::Real> >::removeMissing" << std::endl;
            std::cout << "case missingIntervals_" << std::endl;
            std::cout << "misData_(i, j).second.size(): " << misData_(i, j).second.size() << std::endl;
  #endif
            STK::Real infBound = misData_(i, j).second[0]; // (iterator on map)->(mapped element).(vector of parameters)[element]
            STK::Real supBound = misData_(i, j).second[1];
            sampleVal = STK::Law::Uniform::rand(infBound, supBound);
          }
          break;

          case missingLUIntervals_:
          {
            STK::Real min = dataRange_.min_;
            STK::Real supBound = misData_(i, j).second[0];
            sampleVal = STK::Law::Uniform::rand(min, supBound);
          }
          break;

          case missingRUIntervals_:
          {
            STK::Real infBound = misData_(i, j).second[0];
            STK::Real max = dataRange_.max_;
            sampleVal = STK::Law::Uniform::rand(infBound, max);
          }
          break;
        }
        data_(i, j) = sampleVal;
      }
    }
  }
  }
}

template<>
void AugmentedData<STK::Array2D<int> >::removeMissing()
{
#ifdef MC_DEBUG
  std::cout << "AugmentedData<STK::Array2D<int> >::removeMissing" << std::endl;
#endif
  if (!rangeUpdate_)
  {
    for (int j = 0; j < misData_.cols(); ++j)
    {
      for (int i = 0; i < misData_.rows(); ++i)
      {
        if (misData_(i, j).first != present_)
        {
          data_(i, j) = 0.;
        }
      }
    }
  }
  else
  {
  for (int j = 0; j < misData_.cols(); ++j)
  {
    for (int i = 0; i < misData_.rows(); ++i)
    {
      if (misData_(i, j).first != present_)
      {
        int sampleVal;
        int firstModality = dataRange_.min_;
        int nbModalities = dataRange_.range_;
  #ifdef MC_DEBUG
        std::cout << "i: " << i << ", j: " << j << std::endl;
        std::cout << "firstModality: " << firstModality << ", nbModalities: " << nbModalities << std::endl;
  #endif
        switch(misData_(i, j).first) // (iterator on map)->(mapped element).(MisType)
        {
          case present_:
          {}
          break;

          case missing_:
          {
            STK::Array2DVector<STK::Real> modalities(STK::Range(firstModality,
                                                                nbModalities),
                                                     1. / nbModalities);
            sampleVal = STK::Law::Categorical::rand(modalities);
          }
          break;

          case missingFiniteValues_:
          {
            STK::Real proba = 1. / misData_(i, j).second.size(); // (iterator on map)->(mapped element).(vector of parameters)
            STK::Array2DVector<STK::Real> modalities(STK::Range(firstModality,
                                                                nbModalities),
                                                     0.);
            for(std::vector<int>::const_iterator itParam = misData_(i, j).second.begin();
                itParam != misData_(i, j).second.end();
                ++itParam)
            {
  #ifdef MC_DEBUG
            std::cout << "\tproba: " << proba << std::endl;
  #endif
              modalities[*itParam] = proba;
            }
            sampleVal = STK::Law::Categorical::rand(modalities);
          }
          break;

          default: // other types of intervals not present in integer data
          {}
          break;
        }
        data_(i, j) = sampleVal;
      }
    }
  }
  }
}

} // namespace mixt
