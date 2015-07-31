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
#include "../IO/mixt_IO.h"
#include "../Various/mixt_Various.h"

namespace mixt
{

SEMAlgo::SEMAlgo(MixtureComposer* p_composer,
                 int nbIterMax,
                 int nbSamplingAttempts) :
    p_composer_(p_composer),
    nbIterMax_(nbIterMax),
    nbSamplingAttempts_(nbSamplingAttempts)
{}

std::string SEMAlgo::run(RunType runType,
                         DegeneracyType& deg,
                         int group,
                         int groupMax)
{
#ifdef MC_DEBUG
  std::cout << "SEMAlgo::run" << std::endl;
#endif

  Timer myTimer;

  if (runType == burnIn_)
  {
    myTimer.setName("SEM: burn-in");
    p_composer_->storeSEMBurnIn(-1,
                                nbIterMax_ - 1); // export of the initial partition
  }
  else if (runType == run_)
  {
    myTimer.setName("SEM: run");
  }

  for (int iter = 0; iter < nbIterMax_; ++iter)
  {
#ifdef MC_DEBUGNEW
    std::cout << "SEMAlgo::run, iter: " << iter << std::endl;
#endif

    myTimer.iteration(iter, nbIterMax_ - 1);
    writeProgress(group,
                  groupMax,
                  iter,
                  nbIterMax_ - 1);

    p_composer_->eStep();

    std::string sWarn;
    for (int n = 0; n < nbSamplingAttempts_; ++n) // sStep, samplingStep and mStep until there is no degeneracy
    {
#ifdef MC_DEBUGNEW
      std::cout << "SEMAlgo::run, n: " << n << std::endl;
#endif

      int currMinIndPerClass = p_composer_->sStep(); // sStep performed

#ifdef MC_DEBUGNEW
      std::cout << "currMinIndPerClass: " << currMinIndPerClass << std::endl;
      std::cout << "*p_composer_->p_zi()_" << std::endl;
      std::cout << *p_composer_->p_zi() << std::endl;
#endif

      if (currMinIndPerClass < minIndPerClass) // min number of individuals per class tested
      {
#ifdef MC_DEBUGNEW
        std::cout << "sStep, not enough individual per class." << std::endl;
#endif

        std::stringstream sstm;
        sstm << "sStep, not enough individual per class." << std::endl;
        sWarn += sstm.str();
        continue;
      }

      p_composer_->samplingStep(); // each mixture samples its partially observed values

      DegeneracyType currDeg;
      std::string mWarn = p_composer_->mStep(currDeg);
      if (currDeg == strongDeg_ && n < nbSamplingAttempts_ - 1) // strong degeneracy, but sampling steps remain: do another one
      {
  #ifdef MC_DEBUGNEW
        std::cout << "degeneracy in p_model_->mStep, currDeg: " << currDeg << std::endl;
  #endif

        sWarn += mWarn;
        continue;
      }
      else if (currDeg == strongDeg_ && n == nbSamplingAttempts_ - 1) // strong degeneracy at last iteration: exit and signal it to the strategy
      {
        deg = strongDeg_;
        return sWarn;
      }
      else // no degeneracy, hence no need for further sampling
      {
        break;
      }
    }

    if (runType == burnIn_)
    {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, p_model_->storeShortRun" << std::endl;
#endif
      p_composer_->storeSEMBurnIn(iter,
                                  nbIterMax_ - 1);
    }

    if (runType == run_)
    {
#ifdef MC_DEBUG
      std::cout << "SEMAlgo::run, p_model_->storeLongRun" << std::endl;
#endif
      p_composer_->storeSEMRun(iter,
                               nbIterMax_ - 1);
    }
  }

  return std::string(); // success: return empty string
}

} // namespace mixt
