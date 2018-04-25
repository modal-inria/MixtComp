/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 9, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_PoissonLikelihood.h"
#include "../Various/mixt_Enum.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

PoissonLikelihood::PoissonLikelihood(const Vector<Real>& param,
                                     const AugmentedData<Vector<int> >& augData,
                                     int nbClass) :
    param_(param),
    augData_(augData)
{}

Real PoissonLikelihood::lnCompletedProbability(int i, int k) const {
  return poisson_.lpdf(augData_.data_(i),
                       param_(k));;
}

Real PoissonLikelihood::lnObservedProbability(int i, int k) const {
  Real logProba;

  switch(augData_.misData_(i).first) {  // likelihood for present value
    case present_: {
      logProba = poisson_.lpdf(augData_.data_(i),
                               param_(k));
    }
    break;

    case missing_: { // no contribution to the observed likelihood
      logProba = 0.;
    }
    break;

    default: {}
    break;
  }

  return logProba;
}

} /* namespace mixt */
