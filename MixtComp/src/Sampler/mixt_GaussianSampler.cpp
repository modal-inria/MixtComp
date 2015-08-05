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

void GaussianSampler::sampleIndividual(int i,
                                       int z_i,
                                       bool checkSampleCondition)
{
#ifdef MC_DEBUG
  std::cout << "GaussianSampler::sampleIndividual" << std::endl;
  std::cout << "\ti: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (augData_.misData_(i).first != present_)
  {
    Real z;
    Real mean  = param_(2 * z_i    );
    Real sd    = param_(2 * z_i + 1);

#ifdef MC_DEBUG
    std::cout << "\tmean: " << mean << ", sd: " << sd << std::endl;
#endif

    switch(augData_.misData_(i).first)
    {
      case missing_:
      {
#ifdef MC_DEBUG
        std::cout << "\tmissing_" << std::endl;
#endif
        z = normal_.sample(mean,
                           sd);
      }
      break;

      case missingIntervals_:
      {
#ifdef MC_DEBUG
        std::cout << "\tmissingIntervals_" << std::endl;
#endif
        Real infBound = augData_.misData_(i).second[0];
        Real supBound = augData_.misData_(i).second[1];

        z = normal_.sampleI(mean,
                            sd,
                            infBound,
                            supBound);
#ifdef MC_DEBUG
        if (!(infBound < z < supBound))
        {
          std::cout << "\tmissingIntervals, sampling error" << std::endl;
          std::cout << "\tinfBound: " << infBound << std::endl;
          std::cout << "\tsupBound: " << supBound << std::endl;
          std::cout << "\tz: " << z << std::endl;
        }
#endif
      }
      break;

      case missingLUIntervals_: // missingLUIntervals
      {
#ifdef MC_DEBUG
        std::cout << "\tmissingLUIntervals_" << std::endl;
#endif
        Real supBound = augData_.misData_(i).second[0];
        z = normal_.sampleSB(mean,
                             sd,
                             supBound);
#ifdef MC_DEBUG
        if (!(z < supBound))
        {
          std::cout << "\tmissingLUIntervals_, sampling error" << std::endl;
          std::cout << "\tsupBound: " << supBound << std::endl;
          std::cout << "\tz: " << z << std::endl;
        }
#endif
      }
      break;

      case missingRUIntervals_: // missingRUIntervals
      {
#ifdef MC_DEBUG
        std::cout << "\tmissingRUIntervals_" << std::endl;
#endif
        Real infBound = augData_.misData_(i).second[0];
        z = normal_.sampleIB(mean,
                             sd,
                             infBound);
#ifdef MC_DEBUG
        if (!(infBound < z))
        {
          std::cout << "\tmissingRUIntervals_, sampling error" << std::endl;
          std::cout << "\tinfBound: " << infBound << std::endl;
          std::cout << "\tz: " << z << std::endl;
        }
#endif
      }
      break;

      default:
      {
#ifdef MC_DEBUG
        std::cout << "\tunsupported missing value type" << std::endl;
#endif
      }
      break;
    }

#ifdef MC_DEBUG
    std::cout << "\tsampled val: " << z * sd + mean << std::endl;
#endif
    augData_.data_(i) = z;
  }
}
} // namespace mixt
