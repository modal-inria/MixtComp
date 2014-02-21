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

#include <cmath>
#include "stkpp/projects/STatistiK/include/STK_Law_Exponential.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Uniform.h"
#include "stkpp/projects/STatistiK/include/STK_Law_Normal.h"

#include "mixt_GaussianSamplerIterator.h"

namespace mixt
{

GaussianSamplerIterator::GaussianSamplerIterator(iv_missing missing,
                                                 iv_missing missingEnd,
                                                 iv_missingFiniteValues missingFiniteValues,
                                                 iv_missingFiniteValues missingFiniteValuesEnd,
                                                 iv_missingIntervals missingIntervals,
                                                 iv_missingIntervals missingIntervalsEnd,
                                                 iv_missingLUIntervals missingLUIntervals,
                                                 iv_missingLUIntervals missingLUIntervalsEnd,
                                                 iv_missingRUIntervals missingRUIntervals,
                                                 iv_missingRUIntervals missingRUIntervalsEnd) :
  iv_missing_(missing),
  iv_missingEnd_(missingEnd),
  iv_missingFiniteValues_(missingFiniteValues),
  iv_missingFiniteValuesEnd_(missingFiniteValuesEnd),
  iv_missingIntervals_(missingIntervals),
  iv_missingIntervalsEnd_(missingIntervalsEnd),
  iv_missingLUIntervals_(missingLUIntervals),
  iv_missingLUIntervalsEnd_(missingLUIntervalsEnd),
  iv_missingRUIntervals_(missingRUIntervals),
  iv_missingRUIntervalsEnd_(missingRUIntervalsEnd)
{}

GaussianSamplerIterator::GaussianSamplerIterator(const GaussianSamplerIterator& mit) :
  iv_missing_(mit.iv_missing_),
  iv_missingEnd_(mit.iv_missingEnd_),
  iv_missingFiniteValues_(mit.iv_missingFiniteValues_),
  iv_missingFiniteValuesEnd_(mit.iv_missingFiniteValuesEnd_),
  iv_missingIntervals_(mit.iv_missingIntervals_),
  iv_missingIntervalsEnd_(mit.iv_missingIntervalsEnd_),
  iv_missingLUIntervals_(mit.iv_missingLUIntervals_),
  iv_missingLUIntervalsEnd_(mit.iv_missingLUIntervalsEnd_),
  iv_missingRUIntervals_(mit.iv_missingRUIntervals_),
  iv_missingRUIntervalsEnd_(mit.iv_missingRUIntervalsEnd_)
{}

GaussianSamplerIterator& GaussianSamplerIterator::operator++()
{
  switch(currVec_)
  {
    case 0:
      ++iv_missing_;
      if (iv_missing_ == iv_missingEnd_)
        ++currVec_;
      break;
    case 1:
      ++iv_missingFiniteValues_;
      if (iv_missingFiniteValues_ == iv_missingFiniteValuesEnd_)
        ++currVec_;
      break;
    case 2:
      ++iv_missingIntervals_;
      if (iv_missingIntervals_ == iv_missingIntervalsEnd_)
        ++currVec_;
      break;
    case 3:
      ++iv_missingLUIntervals_;
      if (iv_missingLUIntervals_ == iv_missingLUIntervalsEnd_)
        ++currVec_;
      break;
    case 4:
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

bool GaussianSamplerIterator::operator==(const GaussianSamplerIterator& rhs)
{
  switch(currVec_)
  {
    case 0:
      return (iv_missing_ == rhs.iv_missing_);
      break;
    case 1:
      return (iv_missingFiniteValues_ == rhs.iv_missingFiniteValues_);
      break;
    case 2:
      return (iv_missingIntervals_ == rhs.iv_missingIntervals_);
      break;
    case 3:
      return (iv_missingLUIntervals_ == rhs.iv_missingLUIntervals_);
      break;
    case 4:
      return (iv_missingRUIntervals_ == rhs.iv_missingRUIntervals_);
      break;
  }
  return 0;
}

bool GaussianSamplerIterator::operator!=(const GaussianSamplerIterator& rhs)
{
  return ! operator==(rhs);
}

int GaussianSamplerIterator::operator*()
{
  switch(currVec_)
  {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
  }
  return 0;
}

// left unbounded sampler
double GaussianSamplerIterator::luSampler(double lower, double alpha)
{
  double z, u, rho;
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
double GaussianSamplerIterator::lrbSampler(double lower, double upper)
{
  double z, u, rho;

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

} // namespace mixt
