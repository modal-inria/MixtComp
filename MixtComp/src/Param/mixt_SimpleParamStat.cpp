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
#include "../Various/mixt_Constants.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

SimpleParamStat::SimpleParamStat(Vector<Real>* p_param,
                                 Matrix<Real>* p_paramStatStorage,
                                 Matrix<Real>* p_paramlog,
                                 Real confidenceLevel) :
    nbIter_(0),
    nbParam_(0),
    p_param_(p_param),
    p_paramStatStorage_(p_paramStatStorage),
    p_paramlog_(p_paramlog),
    confidenceLevel_(confidenceLevel)
{}

SimpleParamStat::~SimpleParamStat()
{}

void SimpleParamStat::sample(int iteration)
{
  for (int p = 0; p < nbParam_; ++p)
  {
    Real paramVal = (*p_param_)(p, 0);
#ifdef MC_DEBUG
    if (std::abs(paramVal) < epsilon)
    {
      std::cout << "SimpleParamStat::sample" << std::endl;
      std::cout << "std::abs(paramVal)" << std::endl;
    }
#endif
    (*p_paramlog_)(p, iteration) = paramVal;
  }
}

void SimpleParamStat::sampleParam(int iteration,
                                  int iterationMax)
{
  if (iteration == 0)
  {
    nbParam_ = p_param_->rows();

    // resize internal storage
    (*p_paramlog_).resize(nbParam_, iterationMax + 1);

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

      RowVector<Real> currRow = (*p_paramlog_).row(p); // copy to use sortContiguous on contiguous data
      sortContiguous(currRow);
      Real alpha = (1. - confidenceLevel_) / 2.;
      int realIndLow = alpha * iterationMax;
      int realIndHigh = (1. - alpha) * iterationMax;
      Real mean = currRow.mean();
      Real low  =   (1. - (realIndLow  - int(realIndLow ))) * currRow(realIndLow     )
                  + (      realIndLow  - int(realIndLow ) ) * currRow(realIndLow  + 1);
      Real high =   (1. - (realIndHigh - int(realIndHigh))) * currRow(realIndHigh    )
                  + (      realIndHigh - int(realIndHigh) ) * currRow(realIndHigh + 1);

      (*p_paramStatStorage_)(p, 0) = mean;
      (*p_paramStatStorage_)(p, 1) = low;
      (*p_paramStatStorage_)(p, 2) = high;
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
    (*p_param_)(i, 0) = (*p_paramStatStorage_)(i, 0);
  }
#ifdef MC_DEBUG
  std::cout << "(*p_param_): " << (*p_param_) << std::endl;
  std::cout << "(*p_paramStatStorage_): " << (*p_paramStatStorage_) << std::endl;
#endif
}

} // namespace mixt
