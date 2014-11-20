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

namespace mixt
{

/** default constructor */
GibbsStrategy::GibbsStrategy(mixt::MixtureComposer* p_composer,
                             int nbBurnInIterGibbs,
                             int nbIterGibbs) :
    p_composer_(p_composer),
    nbBurnInIterGibbs_(nbBurnInIterGibbs),
    nbIterGibbs_(nbIterGibbs)
{}

/** copy constructor */
GibbsStrategy::GibbsStrategy(GibbsStrategy const& strategy) :
    p_composer_(strategy.p_composer_),
    nbBurnInIterGibbs_(strategy.nbBurnInIterGibbs_),
    nbIterGibbs_(strategy.nbIterGibbs_)
{}

/** destructor */
GibbsStrategy::~GibbsStrategy()
{}

bool GibbsStrategy::run()
{
  for (int iterBurnInGibbs = 0; iterBurnInGibbs < nbBurnInIterGibbs_; ++iterBurnInGibbs)
  {
#ifdef MC_DEBUG
    std::cout << "GibbsStrategy::run(), iterBurnInGibbs: " << iterBurnInGibbs << std::endl;
#endif
    p_composer_->sStep();
    p_composer_->samplingStep();
    p_composer_->eStep();
  }

  p_composer_->gibbsSampling(nbIterGibbs_);

  p_composer_->finalizeStep();

  return true;
}

} // namespace mixt
