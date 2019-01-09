/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "WeibullLikelihood.h"
#include "Various/Enum.h"

namespace mixt {

WeibullLikelihood::WeibullLikelihood(const Vector<Real>& param, const AugmentedData<Vector<Real> >& augData, Index nbClass) :
    param_(param),
    augData_(augData) {}

Real WeibullLikelihood::lnCompletedProbability(Index i, Index k) const {
  Real kParam = param_(2 * k    );
  Real lambda = param_(2 * k + 1);

  Real logProba = weibull_.lpdf(augData_.data_(i), kParam, lambda);

  return logProba;
}

Real WeibullLikelihood::lnObservedProbability(Index i, Index k) const {
  Real kParam = param_(2 * k    );
  Real lambda = param_(2 * k + 1);

  Real logProba = 0;

  switch(augData_.misData_(i).first) { // likelihood for present value
    case present_: {
      logProba = weibull_.lpdf(augData_.data_(i), kParam, lambda);
    }
    break;

    case missing_: { // no contribution to the observed log likelihood
      logProba = 0.;
    }
    break;

    case missingRUIntervals_: {
      Real infBound = augData_.misData_(i).second[0];
      Real infCdf = weibull_.cdf(infBound, kParam, lambda);
      logProba = std::log(1.0 - infCdf);
    }
    break;

    case missingIntervals_: {
        Real infBound  = augData_.misData_(i).second[0];
        Real supBound  = augData_.misData_(i).second[1];
        Real infCdf = weibull_.cdf(infBound, kParam, lambda);
        Real supCdf = weibull_.cdf(supBound, kParam, lambda);
        logProba = std::log(supCdf - infCdf);
     }
     break;

    default: {
    	throw("WeibullLikelihood::lnObservedProbability, error in missing data handling, please report to the maintainer.");
    }
    break;
  }

  return logProba;
}

} /* namespace mixt */
