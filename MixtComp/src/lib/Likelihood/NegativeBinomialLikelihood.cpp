/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#include "NegativeBinomialLikelihood.h"
#include "../Various/mixt_Enum.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

NegativeBinomialLikelihood::NegativeBinomialLikelihood(const Vector<Real>& param,
                                     const AugmentedData<Vector<int> >& augData,
                                     int nbClass) :
    param_(param),
    augData_(augData)
{}

Real NegativeBinomialLikelihood::lnCompletedProbability(int i, int k) const {
  return negativeBinomial_.lpdf(augData_.data_(i), param_(2 * k), param_(2 * k + 1));
}

Real NegativeBinomialLikelihood::lnObservedProbability(int i, int k) const {
  Real logProba = 0.;

  switch(augData_.misData_(i).first) {  // likelihood for present value
    case present_: {
      logProba = negativeBinomial_.lpdf(augData_.data_(i), param_(2 * k), param_(2 * k + 1));
    }
    break;

    case missing_: { // no contribution to the observed likelihood
      logProba = 0.;
    }
    break;

    default: {
    	throw("NegativeBinomialLikelihood::NegativeBinomialLikelihood, error in missing data handling, please report to the maintainer.");
    }
    break;
  }

  return logProba;
}

} /* namespace mixt */
