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
 *  Created on: August 12, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_SimpleParamStat.h"
#include "DManager/include/STK_HeapSort.h"
#include "Arrays/include/STK_Display.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{

SimpleParamStat::SimpleParamStat(STK::Array2D<STK::Real>* p_param,
                                 STK::Array2D<STK::Real>* p_paramStatStorage,
                                 STK::Real confidenceLevel) :
    nbIter_(0),
    nbParam_(0),
    p_param_(p_param),
    p_paramStatStorage_(p_paramStatStorage),
    confidenceLevel_(confidenceLevel)
{}

SimpleParamStat::~SimpleParamStat()
{}

void SimpleParamStat::sample(int iteration)
{
  for (int p = 0; p < nbParam_; ++p)
  {
    STK::Real paramVal = p_param_->elt(p, 0);
#ifdef MC_DEBUG
    if (std::abs(paramVal) < epsilon)
    {
      std::cout << "SimpleParamStat::sample" << std::endl;
      std::cout << "std::abs(paramVal)" << std::endl;
    }
#endif
    stat_(p, iteration) = paramVal;
  }
}

void SimpleParamStat::sampleParam(int iteration,
                                  int iterationMax)
{
  if (iteration == 0)
  {
    nbParam_ = p_param_->sizeRows();

    // resize internal storage
    stat_.resize(nbParam_, iterationMax + 1);

    // resize export storage
    p_paramStatStorage_->resize(nbParam_, 3);

    // first sampling, on each variable and each parameter
    sample(0);
  }
  else if (iteration == iterationMax)
  {
    // last sampling
    sample(iterationMax);

    for (int p = 0; p < nbParam_; ++p)
    {
#ifdef MC_DEBUG
    std::cout << "SimpleParamStat::sampleParam: " << std::endl;
    std::cout << "\tp: " << p << std::endl;
    std::cout << "\tcurrParam: " << std::endl;
    std::cout << currParam << std::endl;
#endif

      STK::Array2DVector<int> indOrder; // to store indices of ascending order
      STK::heapSort(indOrder, stat_.row(p));
      STK::Real alpha = (1. - confidenceLevel_) / 2.;
      STK::Real realIndLow = alpha * iterationMax;
      STK::Real realIndHigh = (1. - alpha) * iterationMax;

      STK::Real mean = stat_.row(p).mean();
      STK::Real low  =  (1. - (realIndLow  - int(realIndLow ))) * stat_(p, indOrder[int(realIndLow )    ])
                      + (      realIndLow  - int(realIndLow ) ) * stat_(p, indOrder[int(realIndLow ) + 1]);
      STK::Real high =  (1. - (realIndHigh - int(realIndHigh))) * stat_(p, indOrder[int(realIndHigh)    ])
                      + (      realIndHigh - int(realIndHigh) ) * stat_(p, indOrder[int(realIndHigh) + 1]);

      p_paramStatStorage_->elt(p, 0) = mean;
      p_paramStatStorage_->elt(p, 1) = low;
      p_paramStatStorage_->elt(p, 2) = high;
    }
  }
  else
  {
    // standard sampling
    sample(iteration);
  }
}

void SimpleParamStat::setExpectationParam()
{
#ifdef MC_DEBUG
  std::cout << "SimpleParamStat::setExpectationParam" << std::endl;
#endif
  for (int i = 0; i < nbParam_; ++i)
  {
    p_param_->elt(i, 0) = p_paramStatStorage_->elt(i, 0);
  }
#ifdef MC_DEBUG
  std::cout << "(*p_param_): " << (*p_param_) << std::endl;
  std::cout << "(*p_paramStatStorage_): " << (*p_paramStatStorage_) << std::endl;
#endif
}

} // namespace mixt
