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
 *  Created on: Nov 11, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#include "mixt_GibbsStrategy.h"
#include "../Various/mixt_Timer.h"
#include "../Various/mixt_Various.h"

namespace mixt {

/** default constructor */
GibbsStrategy::GibbsStrategy(MixtureComposer* p_composer,
                             const StrategyParam& param) :
    p_composer_(p_composer),
    param_(param) {}

std::string GibbsStrategy::run() {
  std::string warnLog;

  p_composer_->gibbsSampling(performInitialization_,
                             doNotSampleData_,
                             param_.nbGibbsBurnInIter_,
                             0, // group
                             1); // groupMax

  p_composer_->gibbsSampling(doNotPerformInitialization_,
                             sampleData_,
                             param_.nbGibbsIter_,
                             1, // group
                             1); // groupMax
  return warnLog;
}

} // namespace mixt
