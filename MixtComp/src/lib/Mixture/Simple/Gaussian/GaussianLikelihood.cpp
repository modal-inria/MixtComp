/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

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

      Real proba = supCdf - infCdf;
      if(proba < epsilonProba)
    	  proba = epsilonProba;

      logProba = std::log(proba);
    }
    break;

    case missingLUIntervals_: {
      Real supBound = augData_.misData_(i).second[0];
      Real supCdf = normal_.cdf(supBound,
                                mean,
                                sd);

      if(supCdf < epsilonProba)
    	  supCdf = epsilonProba;

      logProba = std::log(supCdf);
    }
    break;

    case missingRUIntervals_: {
      Real infBound = augData_.misData_(i).second[0];
      Real infCdf = normal_.cdf(infBound,
                                mean,
                                sd);
      Real proba = 1. - infCdf;
      if(proba < epsilonProba)
    	  proba = epsilonProba;

      logProba = std::log(proba);
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
