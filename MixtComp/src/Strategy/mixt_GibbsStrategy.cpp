/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
GibbsStrategy::GibbsStrategy(
    MixtureComposer* p_composer,
    const StrategyParam& param) :
        p_composer_(p_composer),
        param_(param) {}

std::string GibbsStrategy::run() {
  std::string warnLog;

  /**
   * Even if they are known at that point, it is not possible to use the parameters for complete initialization,
   * because the class has to be sampled first. And, without completion, no eStep can be performed to get the proper
   * t_ik (consistently computing the observed tik is not possible for all variables). So, the order of initialization is:
   * - call initData to complete all observations, including z values
   * - perform eStep
   * - perform sStep
   * - perform samplingStep
   * */

  p_composer_->initData();

  p_composer_->gibbsSampling(
      doNotSampleData_,
      param_.nbGibbsBurnInIter_,
      0, // group
      1); // groupMax

  p_composer_->gibbsSampling(
      sampleData_,
      param_.nbGibbsIter_,
      1, // group
      1); // groupMax

  return warnLog;
}

} // namespace mixt
