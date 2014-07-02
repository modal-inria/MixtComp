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

#include "mixt_SEMAlgo.h"

namespace mixt
{

SEMAlgo::SEMAlgo(int nbIterMax, int zMin, int nbSamplingAttempts) :
    p_model_(0),
    nbIterMax_(nbIterMax),
    zMin_(zMin),
    nbSamplingAttempts_(nbSamplingAttempts)
{}

SEMAlgo::SEMAlgo(SEMAlgo const& algo) :
    p_model_(algo.p_model_),
    nbIterMax_(algo.nbIterMax_),
    zMin_(algo.zMin_),
    nbSamplingAttempts_(algo.nbSamplingAttempts_)
{}

void SEMAlgo::setModel(mixt::MixtureComposer* p_model)
{
  p_model_ = p_model;
}

bool SEMAlgo::run()
{
    for (int iter = 0; iter < this->nbIterMax_; ++iter)
    {
      for (int iterSample = 0; iterSample < nbSamplingAttempts_; ++iterSample)
        if (p_model_->sStep() > zMin_)
          break;
        else
          return false;

      p_model_->pStep();
      p_model_->samplingStep();
      p_model_->mStep();
      p_model_->eStep();
      if (p_model_->state() == STK::Clust::longRun_)
        p_model_->storeIntermediateResults(iter);
    }

  return true;
}

} // namespace mixt
