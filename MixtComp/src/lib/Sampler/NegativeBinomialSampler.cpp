/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include "NegativeBinomialSampler.h"
#include "../Mixture/mixt_IMixture.h"

namespace mixt
{
NegativeBinomialSampler::NegativeBinomialSampler(AugmentedData<Vector<int> >& augData,
                               const Vector<Real>& param,
                               int nbClass) :
    augData_(augData),
    param_(param) {}

void NegativeBinomialSampler::samplingStepNoCheck(int i, int z_i) {
  if (augData_.misData_(i).first != present_) {
    int x = 0;
    size_t n = param_(2*z_i);
    Real p = param_(2*z_i+1);

    switch(augData_.misData_(i).first) {
      case missing_: {
        x = negativeBinomial_.sample(n, p);
      }
      break;

      default: {
    	  throw("NegativeBinomialSampler::samplingStepNoCheck, error in missing data handling, please report to the maintainer.");
      }
      break;
    }

    augData_.data_(i) = x;
  }
}

} // namespace mixt
