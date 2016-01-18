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
 *  Created on: Mar 7, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

#ifndef MIXT_MIXTUREALGO_H
#define MIXT_MIXTUREALGO_H

#include "../Composer/mixt_MixtureComposer.h"
#include "../Various/mixt_Enum.h"

namespace mixt
{

class SEMAlgo
{
  public:
    /** default constructor */
    SEMAlgo(MixtureComposer* p_composer,
            int nbIter);

    /**
     * run the algorithm
     * @return string describing the problem in case of soft degeneracy */
    void run(RunType runType,
             RunProblemType& runPb,
             SamplerType sampler,
             int group,
             int groupMax);

  private:
    /** pointer on the mixture model */
    MixtureComposer* p_composer_;

    /** number of iterations of the algorithm */
    int nbIter_;
};
} // namespace mixt

#endif /* MIXT_MIXTUREALGO_H */
