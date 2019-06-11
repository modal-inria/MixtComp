/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "GaussianLikelihood.h"
#include "../../../Various/Enum.h"
#include "../../../LinAlg/LinAlg.h"

namespace mixt {

GaussianLikelihood::GaussianLikelihood(const Vector<Real>& param,
                                       const AugmentedData<Vector<Real> >& augData,
                                       int nbClass) :
    param_(param),
    augData_(augData) {}

Real GaussianLikelihood::lnCompletedProbability(int i, int k) const {
  Real mean  = param_(2 * k    );
  Real sd    = param_(2 * k + 1);

  Real logProba = normal_.lpdf(augData_.data_(i),
                               mean,
                               sd);

  return logProba;
}

Real GaussianLikelihood::lnObservedProbability(int i, int k) const {
  Real mean  = param_(2 * k    );
  Real sd    = param_(2 * k + 1);

  Real logProba = 0.;

  switch(augData_.misData_(i).first) { // likelihood for present value
    case present_: {
      logProba = normal_.lpdf(augData_.data_(i),
                              mean,
                              sd);
    }
    break;

    case missing_: { // no contribution to the observed log likelihood
      logProba = 0.;
    }
    break;

    case missingIntervals_: {
      Real infBound  = augData_.misData_(i).second[0];
      Real supBound  = augData_.misData_(i).second[1];
      Real infCdf = normal_.cdf(infBound,
                                mean,
                                sd);
      Real supCdf = normal_.cdf(supBound,
                                mean,
                                sd);
      logProba = std::log(supCdf - infCdf);
    }
    break;

    case missingLUIntervals_: {
      Real supBound = augData_.misData_(i).second[0];
      Real supCdf = normal_.cdf(supBound,
                                mean,
                                sd);
      logProba = std::log(supCdf);
    }
    break;

    case missingRUIntervals_: {
      Real infBound = augData_.misData_(i).second[0];
      Real infCdf = normal_.cdf(infBound,
                                mean,
                                sd);
      logProba = std::log(1. - infCdf);
    }
    break;

    default: {
    	throw("GaussianLikelihood::lnObservedProbability, error in missing data handling, please report to the maintainer.");
    }
    break;
  }
  return logProba;
}

} /* namespace mixt */
