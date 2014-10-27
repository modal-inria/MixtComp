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
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#include "mixt_RemoveMissing.h"

namespace mixt
{

/** Utility function to lookup the data set and remove missing values
 *  coordinates. */
void removeMissing(AugmentedData<STK::Array2D<STK::Real> >* p_augData_)
{
  // loop on missing individuals
  for (AugmentedData<STK::Array2D<STK::Real> >::ConstIt_MisInd itInd = p_augData_->misData_.begin();
       itInd != p_augData_->misData_.end();
       ++itInd)
  {
    // loop on missing variables
    for (AugmentedData<STK::Array2D<STK::Real> >::ConstIt_MisVar itVar = itInd->second.begin();
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

          STK::Real min = p_augData_->dataRanges_[j].min_;
          STK::Real max = p_augData_->dataRanges_[j].max_;
          sampleVal = STK::Law::Uniform::rand(min, max);
        }
        break;

        case missingIntervals_:
        {
          STK::Real infBound = itVar->second.second[0]; // (iterator on map)->(mapped element).(vector of parameters)[element]
          STK::Real supBound = itVar->second.second[1];
          STK::Real sampleVal = STK::Law::Uniform::rand(infBound, supBound);
        }
        break;

        case missingLUIntervals_:
        {
          STK::Real min = p_augData_->dataRanges_[j].min_;
          STK::Real supBound = itVar->second.second[0];
          STK::Real sampleVal = STK::Law::Uniform::rand(min, supBound);
        }
        break;

        case missingRUIntervals_:
        {
          STK::Real infBound = itVar->second.second[0];
          STK::Real max = p_augData_->dataRanges_[j].max_;
          STK::Real sampleVal = STK::Law::Uniform::rand(infBound, max);
        }
        break;
      }
      p_augData_->data_(i, j) = sampleVal;
    }
  }
}

void removeMissing(AugmentedData<STK::Array2D<int> >* p_augData_)
{
  // loop on missing individuals
  for (AugmentedData<STK::Array2D<int> >::ConstIt_MisInd itInd = p_augData_->misData_.begin();
       itInd != p_augData_->misData_.end();
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
      int firstModality = p_augData_->dataRanges_[j].min_;
      int nbModalities = p_augData_->dataRanges_[j].range_;
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
            modalities[*itParam] = proba;
          }
          sampleVal = STK::Law::Categorical::rand(modalities);
        }
        break;
      }
      p_augData_->data_(i, j) = sampleVal;
    }
  }
}

} /* namespace mixt */
