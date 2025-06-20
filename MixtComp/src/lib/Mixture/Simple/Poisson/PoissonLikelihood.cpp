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
 *  Created on: December 9, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "PoissonLikelihood.h"
#include <Various/Enum.h>
#include <LinAlg/LinAlg.h>

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
    	int infBound = augData_.misData_(i).second[0];
    	Real infCdf = poisson_.cdf(infBound, param_(k));
    	logProba = std::log(1.0 - infCdf);
    }
    break;

    case missingIntervals_: {
        int infBound  = augData_.misData_(i).second[0];
        int supBound  = augData_.misData_(i).second[1];
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
