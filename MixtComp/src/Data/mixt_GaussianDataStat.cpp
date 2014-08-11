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
 *  Created on: July 1, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_GaussianDataStat.h"

namespace mixt
{

GaussianDataStat::GaussianDataStat(const AugmentedData<STK::Array2D<STK::Real> >* pm_augDataij) :
    nbIter_(0),
    nbMissing_(0),
    pm_augDataij_(pm_augDataij)
{}

GaussianDataStat::~GaussianDataStat() {};

void GaussianDataStat::initPos()
{
  int currVal = 0;
  for (iv_missing it = pm_augDataij_ ->v_missing_.begin();
       it != pm_augDataij_ ->v_missing_.end();
       ++it)
  {
    posMissing_(currVal, 0) = it->first;
    posMissing_(currVal, 1) = it->second;
    ++currVal;
  }
  for (iv_missingIntervals it = pm_augDataij_ ->v_missingIntervals_.begin();
       it != pm_augDataij_ ->v_missingIntervals_.end();
       ++it)
  {
    posMissing_(currVal, 0) = it->first.first;
    posMissing_(currVal, 1) = it->first.second;
    ++currVal;
  }
  for (iv_missingLUIntervals it = pm_augDataij_ ->v_missingLUIntervals_.begin();
       it != pm_augDataij_ ->v_missingLUIntervals_.end();
       ++it)
  {
    posMissing_(currVal, 0) = it->first.first;
    posMissing_(currVal, 1) = it->first.second;
    ++currVal;
  }
  for (iv_missingRUIntervals it = pm_augDataij_ ->v_missingRUIntervals_.begin();
       it != pm_augDataij_ ->v_missingRUIntervals_.end();
       ++it)
  {
    posMissing_(currVal, 0) = it->first.first;
    posMissing_(currVal, 1) = it->first.second;
    ++currVal;
  }
}

void GaussianDataStat::initialize()
{
  nbMissing_ =   pm_augDataij_->v_missing_.size()
               + pm_augDataij_->v_missingIntervals_.size()
               + pm_augDataij_->v_missingLUIntervals_.size()
               + pm_augDataij_->v_missingRUIntervals_.size();
  // second dimension corresponds to the couple (sample position, variables position)
  posMissing_.resize(nbMissing_, 2);
  initPos();
  // second dimension corresponds to mean and variance
  statMissing_.resize(nbMissing_, 2);
  statMissing_ = 0.;
#ifdef MC_DEBUG
  std::cout << "GaussianDataStat, initializing statMissing_ and posMissing_" << std::endl;
/*  std::cout << "statMissing_" <<  std::endl;
  std::cout << statMissing_ << std::endl;
  std::cout << "posMissing_" <<  std::endl;
  std::cout << posMissing_ << std::endl; */
#endif
};

void GaussianDataStat::sampleVals()
{
  for (int currVal = 0; currVal < nbMissing_; ++currVal)
  {
#ifdef MC_DEBUG
    std::cout << "GaussianDataStat::sampleVals"
              << ", nbIter_: " << nbIter_
              << ", sample: " << posMissing_(currVal, 0)
              << ", var: " << posMissing_(currVal, 1)
              << ", value: " << pm_augDataij_->data_(posMissing_(currVal, 0),
                                                     posMissing_(currVal, 1))
              << std::endl;
#endif
    // sum of values for mean computation
    statMissing_(currVal, 0) += pm_augDataij_->data_(posMissing_(currVal, 0),
                                                     posMissing_(currVal, 1));
    // sum of squares for variance computation
    statMissing_(currVal, 1) += pow(pm_augDataij_->data_(posMissing_(currVal, 0),
                                                         posMissing_(currVal, 1)),
                                    2.);
  }

  ++nbIter_;
}

void GaussianDataStat::exportVals(STK::Array2D<int>& posMissing, STK::Array2D<STK::Real>& statMissing) const
{
  posMissing = posMissing_;
  statMissing.resize(nbMissing_, 2);
  statMissing.col(0) = statMissing_.col(0) / nbIter_; // mean
  statMissing.col(1) = (statMissing_.col(1) / nbIter_ - statMissing.col(0) * statMissing.col(0)).sqrt(); // standard deviation
#ifdef MC_DEBUG
  std::cout << "GaussianDataStat::exportVals, nbIter_: " << nbIter_ << std::endl
            << "posMissing_: " << std::endl
            << posMissing_ << std::endl
            << "statMissing_.col(1) / nbIter_: " << std::endl
            << statMissing_.col(1) / nbIter_ << std::endl
            << "statMissing.col(0) * statMissing.col(0)" << std::endl
            << statMissing.col(0) * statMissing.col(0)
            << "statMissing_: " << std::endl
            << statMissing_ << std::endl
            << "statMissing: " << std::endl
            << statMissing << std::endl;
#endif
}

} // namespace mixt
