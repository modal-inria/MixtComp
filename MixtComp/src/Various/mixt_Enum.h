/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: October 10, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 **/

#ifndef MIXT_DEFS_H_
#define MIXT_DEFS_H_

namespace mixt {

/** Do not force member values with = assignment, otherwise NB_ENUM_MISTYPE will
 * not contain the number of elements in MisType */
enum MisType {
  present_,
  missing_,
  missingFiniteValues_,
  missingIntervals_,
  missingLUIntervals_,
  missingRUIntervals_,
  nb_enum_MisType_
};

enum LikelihoodType {
  lnCompletedLikelihood_,
  lnObservedLikelihood_
};

enum RunMode {
  learning_,
  prediction_
};

enum SimpleMixtureID {
  Categorical_pjk_,
  Gaussian_sjk_,
  Poisson_k_
};

enum RunType {
  burnIn_,
  run_,
};

enum RunProblemType {
  noProblem_,
  invalidSampler_
};

enum SamplerType {
  rejectSampler_,
  GibbsSampler_
};

} // namespace mixt

#endif /* MIXT_CONSTANTS_H_ */
