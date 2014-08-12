/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: Mar 27, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

// List of fields:
// iv_missing_
// iv_missingFiniteValues

#include "STatistiK/include/STK_Law_Categorical.h"
#include "mixt_CategoricalSamplerIterator.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{

CategoricalSamplerIterator::CategoricalSamplerIterator(const STK::Array2D<STK::Real>* p_param,
                                                       const STK::CArrayVector<int>* p_zi,
                                                       const std::vector<Range<int> >& dataRange,
                                                       iv_missing missing,
                                                       iv_missing missingEnd,
                                                       iv_missingFiniteValues missingFiniteValues,
                                                       iv_missingFiniteValues missingFiniteValuesEnd) :
  p_param_(p_param),
  p_zi_(p_zi),
  iv_missing_(missing),
  iv_missingEnd_(missingEnd),
  iv_missingFiniteValues_(missingFiniteValues),
  iv_missingFiniteValuesEnd_(missingFiniteValuesEnd),
  currVec_(0),
  dataRange_(dataRange)
{
  findNonEmpty();
}

CategoricalSamplerIterator::CategoricalSamplerIterator(const CategoricalSamplerIterator& mit) :
  p_param_(mit.p_param_),
  p_zi_(mit.p_zi_),
  iv_missing_(mit.iv_missing_),
  iv_missingEnd_(mit.iv_missingEnd_),
  iv_missingFiniteValues_(mit.iv_missingFiniteValues_),
  iv_missingFiniteValuesEnd_(mit.iv_missingFiniteValuesEnd_),
  currVec_(mit.currVec_),
  dataRange_(mit.dataRange_)
{}

CategoricalSamplerIterator& CategoricalSamplerIterator::operator++()
{
  switch(currVec_)
  {
    case 0:
    {
      ++iv_missing_;
      findNonEmpty();
    }
    break;

    case 1:
    {
      ++iv_missingFiniteValues_;
      findNonEmpty();
    }
    break;
  }
  return *this;
}

CategoricalSamplerIterator CategoricalSamplerIterator::operator++(int)
{
  CategoricalSamplerIterator tmp(*this);
  operator++();
  return tmp;
}

bool CategoricalSamplerIterator::operator==(const CategoricalSamplerIterator& rhs) const
{
  if ((iv_missing_             == rhs.iv_missing_            ) &&
      (iv_missingFiniteValues_ == rhs.iv_missingFiniteValues_))
    return true;
  return false;
}

bool CategoricalSamplerIterator::operator!=(const CategoricalSamplerIterator& rhs) const
{
  return ! operator==(rhs);
}

CategoricalSamplerIterator::RetValue CategoricalSamplerIterator::operator*() const
{
  int sampleVal;
  pos currPos(std::pair<int, int>(0, 0));

#ifdef MC_DEBUG
  std::cout << std::endl;
  std::cout << "CategoricalSamplerIterator::operator*" << std::endl;
  std::cout << "\tp_zi_: " << p_zi_ << std::endl;
  std::cout << "\tp_zi_->sizeRows(): " << p_zi_->sizeRows() << std::endl;
#endif

  switch(currVec_)
  {
    case 0: // missing
    {
      currPos = *iv_missing_;
      int minModality = dataRange_[currPos.second].min_;
      int nbModalities = dataRange_[currPos.second].range_;
      int z_i = p_zi_->elt(currPos.first);
      STK::Array2DVector<STK::Real> modalities = (*p_param_)(STK::Range(z_i * nbModalities,
                                                                        nbModalities),
                                                             currPos.second);
#ifdef MC_DEBUG
      std::cout << std::endl;
      std::cout << "CategoricalSamplerIterator::operator*, missing" << std::endl
                << "\tsample: " << currPos.first << " var: " << currPos.second << std::endl
                << "\tz_i: " << z_i << std::endl
                << "\tnbModalities: " << nbModalities << std::endl
                << "\tModalities: " << modalities;
#endif
      sampleVal = STK::Law::Categorical::rand(modalities) - z_i * nbModalities + minModality;
    }
    break;

    case 1: // missingFiniteValues
    {
      currPos = iv_missingFiniteValues_->first;
      int minModality = dataRange_[currPos.second].min_;
      int nbModalities = dataRange_[currPos.second].range_;
      int z_i = p_zi_->elt(currPos.first);
      STK::Array2DVector<STK::Real> modalities(STK::Range(minModality,
                                                          nbModalities),
                                               0.);
      STK::Array2DVector<STK::Real> equiModalities(STK::Range(minModality,
                                                              nbModalities),
                                                   0.);
      for(std::vector<int>::const_iterator currMod = iv_missingFiniteValues_->second.begin();
          currMod != iv_missingFiniteValues_->second.end();
          ++currMod)
      {
        modalities.elt(*currMod) = (*p_param_)(z_i * nbModalities + *currMod,
                                               currPos.second);
        equiModalities.elt(*currMod) = 1.;
      }
      STK::Real modSum = modalities.sum();
      if (modSum < minStat)
      {
        equiModalities = equiModalities / equiModalities.sum();
        sampleVal = STK::Law::Categorical::rand(equiModalities);
#ifdef MC_DEBUG
      std::cout << std::endl;
      std::cout << "CategoricalSamplerIterator::operator*, missingFiniteValues" << std::endl
                << "\tsample: " << currPos.first << " var: " << currPos.second << std::endl
                << "\tz_i: " << z_i << std::endl
                << "\tnbModalities: " << nbModalities << std::endl
                << "\tequiModalities: " << equiModalities;
#endif
      }
      else
      {
        modalities = modalities / modalities.sum();
        sampleVal = STK::Law::Categorical::rand(modalities);
#ifdef MC_DEBUG
      std::cout << std::endl;
      std::cout << "CategoricalSamplerIterator::operator*, missingFiniteValues" << std::endl
                << "\tsample: " << currPos.first << " var: " << currPos.second << std::endl
                << "\tz_i: " << z_i << std::endl
                << "\tnbModalities: " << nbModalities << std::endl
                << "\tmodalities: " << modalities;
#endif
      }
    }
    break;
  }
#ifdef MC_DEBUG
  std::cout << "\tSampled Val: " << sampleVal << std::endl;
#endif
  return RetValue(currPos, sampleVal);
}

void CategoricalSamplerIterator::findNonEmpty()
{
  bool found = false;
  while(currVec_ < 2 && found == false)
  {
    switch(currVec_)
    {
      case 0:
      {
        if (iv_missing_ == iv_missingEnd_)
          ++currVec_;
        else
          found = true;
      }
      break;

      case 1:
      {
        if (iv_missingFiniteValues_ == iv_missingFiniteValuesEnd_)
          ++currVec_;
        else
          found = true;
      }
      break;
    }
  }
}

} // namespace mixt
