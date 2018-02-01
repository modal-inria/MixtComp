/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "WeibullLikelihood.h"

#include "Various/mixt_Enum.h"

namespace mixt {

WeibullLikelihood::WeibullLikelihood(const Vector<Real>& param, const AugmentedData<Vector<Real> >& augData, Index nbClass) :
    param_(param),
    augData_(augData) {}

Real WeibullLikelihood::lnCompletedProbability(Index i, Index k) {
  Real kParam = param_(2 * k    );
  Real lambda = param_(2 * k + 1);

  Real logProba = weibull_.lpdf(kParam, lambda, augData_.data_(i));

  return logProba;
}

Real WeibullLikelihood::lnObservedProbability(Index i, Index k) {
  Real kParam = param_(2 * k    );
  Real lambda = param_(2 * k + 1);

  Real logProba;

  switch(augData_.misData_(i).first) { // likelihood for present value
    case present_: {
      logProba = weibull_.lpdf(kParam, lambda, augData_.data_(i));
    }
    break;

    case missing_: { // no contribution to the observed log likelihood
      logProba = 0.;
    }
    break;

    case missingRUIntervals_: {
      Real infBound = augData_.misData_(i).second[0];
      Real infCdf = weibull_.lcdf(kParam, lambda, infBound);
      logProba = std::log(1.0 - infCdf);
    }
    break;

    default: {}
    break;
  }

  return logProba;
}

} /* namespace mixt */
