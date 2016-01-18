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
                 int nbIter) :
    p_composer_(p_composer),
    nbIter_(nbIter)
{}

void SEMAlgo::run(RunType runType,
                  RunProblemType& runPb,
                  SamplerType sampler,
                  int group,
                  int groupMax)
{
  std::string warn;
  Timer myTimer;

  if (runType == burnIn_) {
    myTimer.setName("SEM: burn-in");
  }
  else if (runType == run_) {
    myTimer.setName("SEM: run");
  }

  for (int iter = 0; iter < nbIter_; ++iter) {
    myTimer.iteration(iter, nbIter_ - 1);
    writeProgress(group,
                  groupMax,
                  iter,
                  nbIter_ - 1);

    p_composer_->eStep();

    if (sampler == rejectSampler_) { // use reject sampling
      p_composer_->sStepNoCheck(); // no checkSampleCondition performed, to increase speed of sampling
      p_composer_->samplingStepNoCheck();
      int sampleCond = p_composer_->checkSampleCondition(); // since we are not in initialization, no need for log

      if (sampleCond == 0) { // sampled value rejected, switch to Gibbs sampler
        runPb = invalidSampler_;
        return;
      }
    }
    else { // use Gibbs sampling
      p_composer_->sStepCheck(); // checkSampleCondition is performed at each sampling, hence no need to call p_composer_->checkSampleCondition()
      p_composer_->samplingStepCheck();
    }

    p_composer_->mStep();

    if (runType == run_) {
      p_composer_->storeSEMRun(iter,
                               nbIter_ - 1);
    }
  }

  runPb = noProblem_;
}

} // namespace mixt
