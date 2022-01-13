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
 *  Created on: Feb 18, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Mixture/IMixture.h>
#include "GaussianSampler.h"

namespace mixt
{
GaussianSampler::GaussianSampler(AugmentedData<Vector<Real> >& augData,
                                 const Vector<Real>& param,
                                 int nbClass) :
    augData_(augData),
    param_(param)
{}

void GaussianSampler::samplingStepNoCheck(int i,
                                          int z_i) {
  if (augData_.misData_(i).first != present_) {
    Real z = 0.;
    Real mean  = param_(2 * z_i    );
    Real sd    = param_(2 * z_i + 1);

    switch(augData_.misData_(i).first) {
      case missing_: {
        z = normal_.sample(mean,
                           sd);
      }
      break;

      case missingIntervals_: {
        Real infBound = augData_.misData_(i).second[0];
        Real supBound = augData_.misData_(i).second[1];

        z = normal_.sampleI(mean,
                            sd,
                            infBound,
                            supBound);
      }
      break;

      case missingLUIntervals_: { // missingLUIntervals
        Real supBound = augData_.misData_(i).second[0];
        z = normal_.sampleSB(mean,
                             sd,
                             supBound);
      }
      break;

      case missingRUIntervals_: { // missingRUIntervals
        Real infBound = augData_.misData_(i).second[0];
        z = normal_.sampleIB(mean,
                             sd,
                             infBound);
      }
      break;

      default: {
    	  throw("GaussianSampler::samplingStepNoCheck, error in missing data handling, please report to the maintainer.");
      }
      break;
    }

    augData_.data_(i) = z;
  }
}

} // namespace mixt
