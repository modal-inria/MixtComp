/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_GaussianLikelihood.h"
#include "../Various/mixt_Enum.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt {

GaussianLikelihood::GaussianLikelihood(const Vector<Real>& param,
                                       const AugmentedData<Vector<Real> >& augData,
                                       int nbClass) :
    param_(param),
    augData_(augData) {}

Real GaussianLikelihood::lnCompletedProbability(int i, int k) {
  Real mean  = param_(2 * k    );
  Real sd    = param_(2 * k + 1);

  Real logProba = normal_.lpdf(augData_.data_(i),
                               mean,
                               sd);

  return logProba;
}

Real GaussianLikelihood::lnObservedProbability(int i, int k) {
  Real mean  = param_(2 * k    );
  Real sd    = param_(2 * k + 1);

  Real logProba;

  switch(augData_.misData_(i).first) { // likelihood for present value
    case present_: {
      Real mean  = param_(2 * k    );
      Real sd    = param_(2 * k + 1);

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

    default: {}
    break;
  }
  return logProba;
}

} /* namespace mixt */
