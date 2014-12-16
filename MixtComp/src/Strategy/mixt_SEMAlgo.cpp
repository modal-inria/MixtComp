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
#include "../Various/mixt_Constants.h"
#include "../Various/mixt_Timer.h"
#include "../Various/mixt_IO.h"

namespace mixt
{

SEMAlgo::SEMAlgo(int nbIterMax, int nbSamplingAttempts) :
    p_model_(0),
    nbIterMax_(nbIterMax),
    nbSamplingAttempts_(nbSamplingAttempts)
{}

SEMAlgo::SEMAlgo(SEMAlgo const& algo) :
    p_model_(algo.p_model_),
    nbIterMax_(algo.nbIterMax_),
    nbSamplingAttempts_(algo.nbSamplingAttempts_)
{}

void SEMAlgo::setModel(mixt::MixtureComposer* p_model)
{
  p_model_ = p_model;
}

std::string SEMAlgo::run()
{
#ifdef MC_DEBUG
  std::cout << "SEMAlgo::run, entering" << std::endl;
#endif

  Timer myTimer;

  if (p_model_->state() == burnIn_)
  {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, initial partition export" << std::endl;
    std::cout << "SEMAlgo::run, p_model_->storeShortRun" << std::endl;
#endif
    myTimer.setName("SEM burn-in");
    p_model_->storeBurnIn(-1,
                          nbIterMax_ - 1); // export of the initial partition
  }
  else if (p_model_->state() == longRun_)
  {
    myTimer.setName("SEM run");
  }

  for (int iter = 0; iter < nbIterMax_; ++iter)
  {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, iter: " << iter << std::endl;
#endif

    myTimer.iteration(iter, nbIterMax_);
    // SE steps
    p_model_->eStep();
    for (int iterSample = 0; iterSample < nbSamplingAttempts_; ++iterSample)
    {
#ifdef MC_DEBUG
      std::cout << "\titerSample: " << iterSample << std::endl;
#endif
      int nbIndPerClass = p_model_->sStep();
      if (nbIndPerClass > minIndPerClass)
      {
        break; // enough individuals in each class to carry on
      }
      else
      {
        return   std::string("SEMAlgo::run(), sStep(): not enough individuals per class: ")
               + type2str(nbIndPerClass)
               + std::string(", while required minimum is: ")
               + type2str(minIndPerClass);
      }
    }
    p_model_->samplingStep();

    if (p_model_->state() == burnIn_)
    {
      if((iter / moduloMisClass > 0) && (iter % moduloMisClass == 0))
    //      if(iter == 10)
      {
#ifdef MC_DEBUG
      std::cout << "SEMAlgo::run, p_model_->misClasStep" << std::endl;
#endif
        p_model_->misClasStep(iter);
      }
    }

    // M steps
    p_model_->mStep();
    p_model_->pStep();

    // storage steps
    if (p_model_->state() == burnIn_)
    {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, p_model_->storeShortRun" << std::endl;
#endif
      p_model_->storeBurnIn(iter,
                            nbIterMax_ - 1);
    }

    if (p_model_->state() == longRun_)
    {
#ifdef MC_DEBUG
      std::cout << "SEMAlgo::run, p_model_->storeLongRun" << std::endl;
#endif
      p_model_->storeParam(iter,
                           nbIterMax_ - 1);
    }
  }

  return ""; // success: return empty string
}

} // namespace mixt
