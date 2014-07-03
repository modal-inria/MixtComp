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

#include "mixt_CategoricalDataStat.h"

namespace mixt
{

CategoricalDataStat::CategoricalDataStat(const AugmentedData<STK::Array2D<int> >* pm_augDataij) :
    nbIter_(0),
    nbMissing_(0),
    pm_augDataij_(pm_augDataij)
{
  nbMissing_ =   pm_augDataij_->v_missing_.size()
               + pm_augDataij_->v_missingFiniteValues_.size();
  // second dimension corresponds to the couple (sample position, variable position)
  posMissing_.resize(STK::Range(0, nbMissing_), 2);
  posMissing_ = 0;

  initPos();
  setModalities();

  // second dimension corresponds the modalities
  statMissing_.resize(STK::Range(0, nbMissing_), nbModalities_);
  posMissing_ = 0.;
#ifdef MC_DEBUG
  std::cout << "Initializing statMissing_ and posMissing_" << std::endl;
  std::cout << "statMissing_" <<  std::endl;
  std::cout << statMissing_ << std::endl;
  std::cout << "posMissing_" <<  std::endl;
  std::cout << posMissing_ << std::endl;
#endif
}

void CategoricalDataStat::initPos()
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
  for (iv_missingFiniteValues it = pm_augDataij_ ->v_missingFiniteValues_.begin();
       it != pm_augDataij_ ->v_missingFiniteValues_.end();
       ++it)
  {
    posMissing_(currVal, 0) = it->first.first;
    posMissing_(currVal, 1) = it->first.second;
    ++currVal;
  }
}

void CategoricalDataStat::setModalities()
{
  std::vector<std::pair<int, int> >::const_iterator it   (pm_augDataij_->dataRanges_.begin());
  std::vector<std::pair<int, int> >::const_iterator itEnd(pm_augDataij_->dataRanges_.end  ());

  int min = it->first;
  int max = it->second;

  ++it;

  for (; it != itEnd; ++it)
  {
    int currMin = it->first;
    int currMax = it->second;
    if (currMin < min) min = currMin;
    if (currMax > max) max = currMax;
  }

  nbModalities_ = max;
}

void CategoricalDataStat::sampleVals()
{
  for (int currVal = 0; currVal < nbMissing_; ++currVal)
  {
    statMissing_(currVal, pm_augDataij_->data_(posMissing_(currVal, 0),
                                               posMissing_(currVal, 1))) += 1.;
  }

  ++nbIter_;
}

void CategoricalDataStat::exportVals(STK::Array2D<int>& posMissing, STK::Array2D<STK::Real>& statMissing) const
{
  posMissing = posMissing_;
  statMissing = statMissing_ / STK::Real(nbIter_); // probability of each modality
}

} // namespace mixt
