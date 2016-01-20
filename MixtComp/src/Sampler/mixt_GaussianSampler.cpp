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
 *  Created on: Feb 18, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_GaussianSampler.h"
#include "../Mixture/mixt_IMixture.h"

namespace mixt
{
GaussianSampler::GaussianSampler(const IMixture& mixture,
                                 AugmentedData<Vector<Real> >& augData,
                                 const Vector<Real>& param,
                                 int nbClass) :
    mixture_(mixture),
    augData_(augData),
    param_(param)
{}

void GaussianSampler::samplingStepCheck(int i,
                                        int z_i)
{
  if (augData_.misData_(i).first != present_) {
    do {
      Real z;
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
        }
        break;
      }

      augData_.data_(i) = z;
    }
    while(mixture_.checkSampleCondition() == 0);
  }
}

void GaussianSampler::samplingStepNoCheck(int i,
                                          int z_i) {
  if (augData_.misData_(i).first != present_) {
    Real z;
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
      }
      break;
    }

    augData_.data_(i) = z;
  }
}

} // namespace mixt
