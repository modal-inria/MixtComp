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

#ifndef MIXT_GIBBSSTRATEGY_H
#define MIXT_GIBBSSTRATEGY_H

#include "../Composer/mixt_MixtureComposer.h"

namespace mixt
{
/**
 *  A SemStrategy is based on the following paradigm:
 *  - perform nbBurnIn iterations of the algo
 *  - perform a long run
 **/
class GibbsStrategy
{
  public:
    /** default constructor.
     *  @param p_composer the model to estimate
     **/
    GibbsStrategy(mixt::MixtureComposer* p_composer,
                    int nbBurnInIter,
                    int nbIter);

    /** copy constructor */
    GibbsStrategy(GibbsStrategy const& strategy);

    /** destructor */
    virtual ~GibbsStrategy();
    
    /** run the strategy */
    void run();
    
  protected:
     /** reference on the main model */
    mixt::MixtureComposer* p_composer_;

    int nbBurnInIterGibbs_;
    int nbIterGibbs_;
};

}  // namespace mixt

#endif /* MIXT_GIBBSSTRATEGY_H */
