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
  // loop on missing individuals
  for (ConstIt_MisInd itInd = misData_.begin();
       itInd != misData_.end();
       ++itInd)
  {
    // loop on missing variables
    for (ConstIt_MisVar itVar = itInd->second.begin();
        itVar != itInd->second.end();
        ++itVar)
    {
      int i = itInd->first;
      int j = itVar->first;
      STK::Real sampleVal;
      switch(itVar->second.first) // (iterator on map)->(mapped element).(MisType)
      {
        case missing_:
        {
          STK::Real min = dataRanges_[j].min_;
          STK::Real max = dataRanges_[j].max_;
          sampleVal = STK::Law::Uniform::rand(min, max);
        }
        break;

        case missingIntervals_:
        {
#ifdef MC_DEBUG
          std::cout << "AugmentedData<STK::Array2D<STK::Real> >::removeMissing" << std::endl;
          std::cout << "case missingIntervals_" << std::endl;
          std::cout << "itVar->second.second.size(): " << itVar->second.second.size() << std::endl;
#endif
          STK::Real infBound = itVar->second.second[0]; // (iterator on map)->(mapped element).(vector of parameters)[element]
          STK::Real supBound = itVar->second.second[1];
          sampleVal = STK::Law::Uniform::rand(infBound, supBound);
        }
        break;

        case missingLUIntervals_:
        {
          STK::Real min = dataRanges_[j].min_;
          STK::Real supBound = itVar->second.second[0];
          sampleVal = STK::Law::Uniform::rand(min, supBound);
        }
        break;

        case missingRUIntervals_:
        {
          STK::Real infBound = itVar->second.second[0];
          STK::Real max = dataRanges_[j].max_;
          sampleVal = STK::Law::Uniform::rand(infBound, max);
        }
        break;
      }
      data_(i, j) = sampleVal;
    }
  }
}

template<>
void AugmentedData<STK::Array2D<int> >::removeMissing()
{
#ifdef MC_DEBUG
  std::cout << "AugmentedData<STK::Array2D<int> >::removeMissing" << std::endl;
#endif
  // loop on missing individuals
  for (ConstIt_MisInd itInd = misData_.begin();
       itInd != misData_.end();
       ++itInd)
  {
    // loop on missing variables
    for (AugmentedData<STK::Array2D<int> >::ConstIt_MisVar itVar = itInd->second.begin();
        itVar != itInd->second.end();
        ++itVar)
    {
      int i = itInd->first;
      int j = itVar->first;
      int sampleVal;
      int firstModality = dataRanges_[j].min_;
      int nbModalities = dataRanges_[j].range_;
#ifdef MC_DEBUG
      std::cout << "i: " << i << ", j: " << j << std::endl;
      std::cout << "firstModality: " << firstModality << ", nbModalities: " << nbModalities << std::endl;
#endif
      switch(itVar->second.first) // (iterator on map)->(mapped element).(MisType)
      {
        case missing_:
        {
          STK::Array2DVector<STK::Real> modalities(STK::Range(firstModality, nbModalities), 1. / nbModalities);
          sampleVal = STK::Law::Categorical::rand(modalities);
        }
        break;

        case missingFiniteValues_:
        {
          STK::Real proba = 1. / itVar->second.second.size(); // (iterator on map)->(mapped element).(vector of parameters)
          STK::Array2DVector<STK::Real> modalities(STK::Range(firstModality,
                                                              nbModalities),
                                                   0.);
          for(std::vector<int>::const_iterator itParam = itVar->second.second.begin();
              itParam != itVar->second.second.end();
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
      }
      data_(i, j) = sampleVal;
    }
  }
}

} // namespace mixt
