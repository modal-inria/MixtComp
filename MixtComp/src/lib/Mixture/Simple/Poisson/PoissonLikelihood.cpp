/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 9, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "PoissonLikelihood.h"
#include "../../../Various/mixt_Enum.h"
#include "../../../LinAlg/mixt_LinAlg.h"

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
  Real logProba = 0.;

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

    case missingRUIntervals_: {
      Real infBound = augData_.misData_(i).second[0];
      Real infCdf = poisson_.cdf(infBound, param_(k));
      logProba = std::log(1.0 - infCdf);
    }
    break;

    case missingLUIntervals_: {
        Real supBound  = augData_.misData_(i).second[0];
        Real supCdf = poisson_.cdf(supBound, param_(k));
        logProba = std::log(supCdf);
     }
     break;

    case missingIntervals_: {
        Real infBound  = augData_.misData_(i).second[0];
        Real supBound  = augData_.misData_(i).second[1];
        Real infCdf = poisson_.cdf(infBound, param_(k));
        Real supCdf = poisson_.cdf(supBound, param_(k));
        logProba = std::log(supCdf - infCdf);
     }
     break;

    default: {
    	throw("PoissonLikelihood::PoissonLikelihood, error in missing data handling, please report to the maintainer.");
    }
    break;
  }

  return logProba;
}

} /* namespace mixt */
