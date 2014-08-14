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

namespace mixt
{

SimpleParamStat::SimpleParamStat(const STK::Array2D<STK::Real>* p_param) :
    nbIter_(0),
    nbVar_(0),
    p_param_(p_param)
{}

SimpleParamStat::~SimpleParamStat()
{}

void SimpleParamStat::initialize()
{
  nbParam_ = p_param_->sizeRows();
  nbVar_ = p_param_->sizeCols();
  stat_.resize(nbParam_,
               nbVar_ * 2);
  stat_ = 0.;

#ifdef MC_DEBUG
  std::cout << "SimpleParamStat::initialize()" << std::endl;
  std::cout <<  "\tp_param_->sizeRows(): " << p_param_->sizeRows() << std::endl;
  std::cout <<  "\tp_param_->sizeCols() * 2: " << p_param_->sizeCols() * 2 << std::endl;
#endif
}

void SimpleParamStat::sampleParam()
{
  ++nbIter_;
#ifdef MC_DEBUG
  std::cout << "SimpleParamStat::sampleParam()" << std::endl;
#endif
  for (int j = 0; j < nbVar_; ++j)
  {
    /* here x is p_param_->col(j)
     * mean is stat_.col(2 * j)
     * m2 is stat_.col(2 * j + 1)
     */
    STK::Array2DVector<STK::Real> delta;
    delta = p_param_->col(j) - stat_.col(2 * j);
    stat_.col(2 * j    ) += delta / nbIter_;
    stat_.col(2 * j + 1) += delta * (p_param_->col(j) - stat_.col(2 * j));
#ifdef MC_DEBUG
    std::cout << "\tj: " << j << std::endl;
    std::cout << "\tdelta" << std::endl;
    std::cout << delta << std::endl;
    std::cout << "p_param_->col(j)" << std::endl;
    std::cout << p_param_->col(j) << std::endl;
    std::cout << "stat_.col(2 * j)" << std::endl;
    std::cout << stat_.col(2 * j) << std::endl;
    std::cout << "p_param_->col(j) - stat_.col(2 * j)" << std::endl;
    std::cout << p_param_->col(j) - stat_.col(2 * j) << std::endl;
    std::cout << "delta * (p_param_->col(j) - stat_.col(2 * j))" << std::endl;
    std::cout << delta * (p_param_->col(j) - stat_.col(2 * j)) << std::endl;
#endif
  }
#ifdef MC_DEBUG
  std::cout << "\t*p_param_" << std::endl;
  std::cout << *p_param_ << std::endl;
  std::cout << "\tstat_" << std::endl;
  std::cout << stat_ << std::endl;
#endif
}

void SimpleParamStat::exportParam(STK::Array2D<STK::Real>* stat) const
{
  stat->resize(nbParam_, nbVar_ * 2);
  for (int j = 0; j < nbVar_; ++j)
  {
    stat->col(2 * j) = stat_.col(j); // mean
    if (nbIter_ < 2)
      stat->col(2 * j + 1) = 0.; // standard deviation
    else
      stat->col(2 * j + 1) = (stat_.col(2 * j + 1) / (nbIter_ - 1.)).sqrt(); // standard deviation
  }
}

} // namespace mixt
