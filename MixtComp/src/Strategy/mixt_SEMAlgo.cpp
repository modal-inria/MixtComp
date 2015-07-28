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

SEMAlgo::SEMAlgo(MixtureComposer* p_model,
                 int nbIterMax,
                 int nbSamplingAttempts) :
    p_model_(p_model),
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
    myTimer.setName("SEMAlgo::run(), burn-in");
    p_model_->storeSEMBurnIn(-1,
                             nbIterMax_ - 1); // export of the initial partition
  }
  else if (runType == run_)
  {
    myTimer.setName("SEMAlgo::run(), run");
  }

  for (int iter = 0; iter < nbIterMax_; ++iter)
  {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, iter: " << iter << std::endl;
#endif

    myTimer.iteration(iter, nbIterMax_ - 1);
    writeProgress(group,
                  groupMax,
                  iter,
                  nbIterMax_ - 1);

    p_model_->eStep();

    std::string sWarn = p_model_->sStepNbAttempts(nbSamplingAttempts_,
                                                  deg); // p_model_->sStep() called nbSamplingAttempts_ at most, to get enough individuals per class
    if (deg != noDeg_)
    {
#ifdef MC_DEBUG
      std::cout << "SEMAlgo::run, degeneracy in p_model_->sStepNbAttempts" << std::endl;
#endif
      return sWarn;
    }

    p_model_->samplingStep(); // each mixture samples its partially observed values

    std::string mWarn = p_model_->mStep(deg);
    if (deg != noDeg_)
    {
#ifdef MC_DEBUG
      std::cout << "SEMAlgo::run, degeneracy in p_model_->mStep" << std::endl;
#endif
      return mWarn; // error reported in the mStep, terminate the SEM algo, and report it to the strategy.
    }

    if (runType == burnIn_)
    {
#ifdef MC_DEBUG
    std::cout << "SEMAlgo::run, p_model_->storeShortRun" << std::endl;
#endif
      p_model_->storeSEMBurnIn(iter,
                               nbIterMax_ - 1);
    }

    if (runType == run_)
    {
#ifdef MC_DEBUG
      std::cout << "SEMAlgo::run, p_model_->storeLongRun" << std::endl;
#endif
      p_model_->storeSEMRun(iter,
                            nbIterMax_ - 1);
    }
  }

  return std::string(); // success: return empty string
}

} // namespace mixt
