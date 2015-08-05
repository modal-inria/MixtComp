/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: Mar 13, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_ClassSampler.h"
#include "../Various/mixt_Constants.h"
#include "../Composer/mixt_MixtureComposer.h"

namespace mixt
{

ClassSampler::ClassSampler(const MixtureComposer& composer,
                           AugmentedData<Vector<int> >& zi,
                           const Matrix<Real>& tik,
                           int nbClass) :
    composer_(composer),
    nbClass_(nbClass),
    zi_(zi),
    tik_(tik)
{}

void ClassSampler::sampleIndividual(int i, bool checkSampleCondition)
{
#ifdef MC_DEBUG
  std::cout << "ClassSampler::sampleIndividual" << std::endl;
  std::cout << "i: " << i << std::endl;
#endif

  if (zi_.misData_(i).first != present_)
  {
    int sampleVal = -1; // initialized with dummy value

#ifdef MC_DEBUG
    std::cout << "present_" << std::endl;
#endif

    switch(zi_.misData_(i).first)
    {
      case missing_:
      {
#ifdef MC_DEBUG
        std::cout << "missing_" << std::endl;
#endif
        Vector<Real> modalities(nbClass_);
        for (zi_.data_(i) = 0; zi_.data_(i) < nbClass_; ++zi_.data_(i)) // z_i changed in place to take all possible values
        {
          modalities(zi_.data_(i)) = tik_(i, zi_.data_(i)) * cdp(checkSampleCondition); // cdp value is 0 or 1, taking into account whether conditions on data are verified or not
        }
        modalities = modalities / modalities.sum();
        sampleVal = multi_.sample(modalities);
      }
      break;

      case missingFiniteValues_: // renormalize proba distribution on allowed sampling values
      {
#ifdef MC_DEBUG
        std::cout << "missingFiniteValues_" << std::endl;
#endif
        Vector<Real> modalities(nbClass_);
        modalities = 0.;

        for(std::vector<int>::const_iterator currMod = zi_.misData_(i).second.begin();
            currMod != zi_.misData_(i).second.end();
            ++currMod)
        {
#ifdef MC_DEBUG
          std::cout << "\tcurrMod: " << *currMod << std::endl;
#endif
          zi_.data_(i) = *currMod;
          modalities(*currMod) = tik_(i, *currMod) * cdp(checkSampleCondition);
        }
        modalities = modalities / modalities.sum();
        sampleVal = multi_.sample(modalities);
      }
      break;

      default:
      {
#ifdef MC_DEBUG
          std::cout << "ClassSampler, missing value type unknown" << std::endl;
#endif
      }
      break;
    }
    zi_.data_(i) = sampleVal;
  }
}

Real ClassSampler::cdp(bool checkSampleCondition) const
{
  if (checkSampleCondition == true)
  {
    return composer_.checkSampleCondition();
  }
  else
  {
    return 1.;
  }
}
} // namespace mixt
