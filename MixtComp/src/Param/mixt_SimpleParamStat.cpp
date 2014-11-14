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

namespace mixt
{

SimpleParamStat::SimpleParamStat(STK::Array2D<STK::Real>* p_param,
                                 STK::Array2D<STK::Real>* p_paramStatStorage,
                                 STK::Real confidenceLevel) :
    nbIter_(0),
    nbParam_(0),
    nbVar_(0),
    p_param_(p_param),
    p_paramStatStorage_(p_paramStatStorage),
    confidenceLevel_(confidenceLevel)
{}

SimpleParamStat::~SimpleParamStat()
{}

void SimpleParamStat::sampleParam(int iteration,
                                  int iterationMax)
{
  if (iteration == 0)
  {
    nbParam_ = p_param_->sizeRows();
    nbVar_ = p_param_->sizeCols();

    // resize internal storage
    stat_.resize(nbVar_ * nbParam_);
    for (int j = 0; j < nbVar_ * nbParam_; ++j)
    {
      stat_[j] = STK::Array2DVector<STK::Real>(iterationMax, 0.);
    }

    // resize export storage
    p_paramStatStorage_->resize(nbParam_, 3 * nbVar_);

    // first sampling, on each variable and each parameter
    for (int j = 0; j < nbVar_; ++j)
    {
      for (int p = 0; p < nbParam_; ++p)
      {
        stat_[j * nbParam_ + p][iteration] = p_param_->elt(p, j);
      }
    }
  }
  else if (iteration == iterationMax)
  {
    for (int j = 0; j < nbVar_; ++j)
    {
      for (int p = 0; p < nbParam_; ++p)
      {
        STK::Array2DVector<STK::Real>& currParam = stat_[j * nbParam_ + p];

#ifdef MC_DEBUG
      std::cout << "GaussianDataStat::sampleVals, last iteration: " << std::endl;
      std::cout << "j: " << j << ", p: " << p << std::endl;
      std::cout << "currParam: " << std::endl;
      std::cout << currParam << std::endl;
#endif

        STK::Array2DVector<int> indOrder; // to store indices of ascending order
        STK::heapSort(indOrder, currParam);
        STK::Real alpha = (1. - confidenceLevel_) / 2.;
        STK::Real realIndLow = alpha * iterationMax;
        STK::Real realIndHigh = (1. - alpha) * iterationMax;

        STK::Real mean = currParam.mean();
        STK::Real low  =  (1. - (realIndLow  - int(realIndLow ))) * currParam[indOrder[int(realIndLow )    ]]
                        + (      realIndLow  - int(realIndLow ) ) * currParam[indOrder[int(realIndLow ) + 1]];
        STK::Real high =  (1. - (realIndHigh - int(realIndHigh))) * currParam[indOrder[int(realIndHigh)    ]]
                        + (      realIndHigh - int(realIndHigh) ) * currParam[indOrder[int(realIndHigh) + 1]];

        p_paramStatStorage_->elt(p, j * nbParam_ + 0) = mean;
        p_paramStatStorage_->elt(p, j * nbParam_ + 1) = low;
        p_paramStatStorage_->elt(p, j * nbParam_ + 2) = high;
      }
    }
  }
  else
  {
    for (int j = 0; j < nbVar_; ++j)
    {
      for (int p = 0; p < nbParam_; ++p)
      {
        stat_[j * nbParam_ + p][iteration] = p_param_->elt(p, j);
      }
    }
  }
}

void SimpleParamStat::setExpectationParam()
{
  for (int i = 0; i < nbParam_; ++i)
  {
    for(int j = 0; j < nbVar_; ++j)
    {
      p_param_->elt(i, j) = p_paramStatStorage_->elt(i, 3 * j);
    }
  }
}

} // namespace mixt
