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
 *  Created on: 19 feb. 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

// List of fields:
// iv_missing_
// iv_missingFiniteValues
// iv_missingIntervals
// iv_missingLUIntervals
// iv_missingRUIntervals

#include <cmath>
#include "stkpp/projects/STatistiK/include/STK_Law_Exponential.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Uniform.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Normal.h"
#include "mixt_GaussianSamplerIterator.h"

namespace mixt
{

GaussianSamplerIterator::GaussianSamplerIterator(const STK::Array2D<STK::Real>* p_param,
                                                 const STK::CArrayVector<int>* p_zi,
                                                 iv_missing missing,
                                                 iv_missing missingEnd,
                                                 iv_missingIntervals missingIntervals,
                                                 iv_missingIntervals missingIntervalsEnd,
                                                 iv_missingLUIntervals missingLUIntervals,
                                                 iv_missingLUIntervals missingLUIntervalsEnd,
                                                 iv_missingRUIntervals missingRUIntervals,
                                                 iv_missingRUIntervals missingRUIntervalsEnd) :
  p_param_(p_param),
  p_zi_(p_zi),
  iv_missing_(missing),
  iv_missingEnd_(missingEnd),
  iv_missingIntervals_(missingIntervals),
  iv_missingIntervalsEnd_(missingIntervalsEnd),
  iv_missingLUIntervals_(missingLUIntervals),
  iv_missingLUIntervalsEnd_(missingLUIntervalsEnd),
  iv_missingRUIntervals_(missingRUIntervals),
  iv_missingRUIntervalsEnd_(missingRUIntervalsEnd),
  currVec_(0)
{
  findNonEmpty();
}

GaussianSamplerIterator::GaussianSamplerIterator(const GaussianSamplerIterator& mit) :
  p_param_(mit.p_param_),
  p_zi_(mit.p_zi_),
  iv_missing_(mit.iv_missing_),
  iv_missingEnd_(mit.iv_missingEnd_),
  iv_missingIntervals_(mit.iv_missingIntervals_),
  iv_missingIntervalsEnd_(mit.iv_missingIntervalsEnd_),
  iv_missingLUIntervals_(mit.iv_missingLUIntervals_),
  iv_missingLUIntervalsEnd_(mit.iv_missingLUIntervalsEnd_),
  iv_missingRUIntervals_(mit.iv_missingRUIntervals_),
  iv_missingRUIntervalsEnd_(mit.iv_missingRUIntervalsEnd_),
  currVec_(mit.currVec_)
{}

GaussianSamplerIterator& GaussianSamplerIterator::operator++()
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
      ++iv_missingIntervals_;
      findNonEmpty();
    }
    break;

    case 2:
    {
      ++iv_missingLUIntervals_;
      findNonEmpty();
    }
    break;

    case 3:
      ++iv_missingRUIntervals_;
    break;
  }
  return *this;
}

GaussianSamplerIterator GaussianSamplerIterator::operator++(int)
{
  GaussianSamplerIterator tmp(*this);
  operator++();
  return tmp;
}

bool GaussianSamplerIterator::operator==(const GaussianSamplerIterator& rhs) const
{
  if ((iv_missing_             == rhs.iv_missing_            ) &&
      (iv_missingIntervals_    == rhs.iv_missingIntervals_   ) &&
      (iv_missingLUIntervals_  == rhs.iv_missingLUIntervals_ ) &&
      (iv_missingRUIntervals_  == rhs.iv_missingRUIntervals_ ))
    return true;
  return false;
}

bool GaussianSamplerIterator::operator!=(const GaussianSamplerIterator& rhs) const
{
  return ! operator==(rhs);
}

GaussianSamplerIterator::RetValue GaussianSamplerIterator::operator*() const
{
  STK::Real mean, sd, z;
  pos currPos(std::pair<int, int>(0, 0));

  switch(currVec_)
  {
    case 0: // missing
    {
      currPos = *iv_missing_;
      mean  = p_param_->elt(2*p_zi_->elt(currPos.first)    , currPos.second);
      sd    = p_param_->elt(2*p_zi_->elt(currPos.first) + 1, currPos.second);
      z = STK::Law::Normal::rand(0., 1.);
    }
    break;

    case 1: // missingIntervals
    {
      currPos = iv_missingIntervals_->first;
      STK::Real infBound(iv_missingIntervals_->second.first);
      STK::Real supBound(iv_missingIntervals_->second.second);
      mean  = p_param_->elt(2*p_zi_->elt(currPos.first)    , currPos.second);
      sd    = p_param_->elt(2*p_zi_->elt(currPos.first) + 1, currPos.second);
      STK::Real lower = (infBound - mean) / sd;
      STK::Real upper = (supBound - mean) / sd;
      STK::Real alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;

      if (alpha*exp(alpha * lower / 2.) / sqrt(exp(1)) > exp(lower / 2) / (upper - lower))
      {
        do
        {
          z = luSampler(lower, alpha);
        }
        while(upper < z);
      }
      else
      {
        z = lrbSampler(lower, upper);
      }
    }
    break;

    case 2: // missingLUIntervals
    {
      currPos = iv_missingLUIntervals_->first;
      STK::Real supBound(iv_missingLUIntervals_->second);
      mean  = p_param_->elt(2*p_zi_->elt(currPos.first)    , currPos.second);
      sd    = p_param_->elt(2*p_zi_->elt(currPos.first) + 1, currPos.second);
      STK::Real upper = (supBound - mean) / sd;
      STK::Real alpha = (upper + sqrt(pow(upper, 2) + 4.))/2.;

      z = -luSampler(-upper, alpha);
    }
    break;

    case 3: // missingRUIntervals
    {
      currPos = iv_missingRUIntervals_->first;
      STK::Real infBound(iv_missingRUIntervals_->second);
      mean  = p_param_->elt(2*p_zi_->elt(currPos.first)    , currPos.second);
      sd    = p_param_->elt(2*p_zi_->elt(currPos.first) + 1, currPos.second);
      STK::Real lower = (infBound - mean) / sd;
      STK::Real alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
      z = luSampler(lower, alpha);
    }
    break;
  }
  return RetValue(currPos, z * sd + mean);
}

// left unbounded sampler
STK::Real GaussianSamplerIterator::luSampler(STK::Real lower, STK::Real alpha) const
{
  STK::Real z, u, rho;
  if (lower < 0)
  {
    do
    {
      z = STK::Law::Normal::rand(0, 1);
    }
    while (z < lower);
  }
  else
  {
    do
    {
      z = STK::Law::Exponential::rand(1./alpha) + lower;
      rho = exp(-pow((z - alpha), 2) / 2.);
      u = STK::Law::Uniform::rand(0., 1.);
    }
    while (u > rho);
  }
  return z;
}

// left and right bounded sampler
STK::Real GaussianSamplerIterator::lrbSampler(STK::Real lower, STK::Real upper) const
{
  STK::Real z, u, rho;

  do
  {
    z = STK::Law::Uniform::rand(lower, upper);

    if (lower < 0. && 0. < upper)
      rho = exp(-pow(z, 2));
    else if (upper < 0.)
      rho = exp((pow(upper, 2)-pow(z, 2))/2);
    else if (0. < lower)
      rho = exp((pow(lower, 2)-pow(z, 2))/2);

    u = STK::Law::Uniform::rand(0., 1.);
  }
  while(u > rho);

  return z;
}

void GaussianSamplerIterator::findNonEmpty()
{
  bool found = false;
  while(currVec_ < 4 && found == false)
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
        if (iv_missingIntervals_ == iv_missingIntervalsEnd_)
          ++currVec_;
        else
          found = true;
      }
      break;

      case 2:
      {
        if (iv_missingLUIntervals_ == iv_missingLUIntervalsEnd_)
          ++currVec_;
        else
          found = true;
      }
      break;

      case 3:
      {
        if (iv_missingRUIntervals_ == iv_missingRUIntervalsEnd_)
          ++currVec_;
        else
          found = true;
      }
      break;
    }
  }
}

} // namespace mixt
