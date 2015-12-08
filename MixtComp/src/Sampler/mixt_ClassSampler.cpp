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

void ClassSampler::sStepCheck(int i)
{
#ifdef MC_DEBUG
    std::cout << "ClassSampler::sStepCheck, i: " << i << std::endl;
#endif
  if (zi_.misData_(i).first != present_)
  {
    int sampleVal = -1; // initialized with dummy value

    switch(zi_.misData_(i).first)
    {
      case missing_:
      {
        RowVector<Real> modalities(nbClass_, 0.);
        RowVector<int> condition(nbClass_, 0);

        for (int k = 0; k < nbClass_; ++k) // z_i changed in place to take all possible values
        {
          zi_.data_(i) = k;
          condition(k) = composer_.checkSampleCondition();
          modalities(k) = tik_(i, k) * condition(k); // checkSampleCondition value is 1 or 0, reflecting the fact that conditions on data are verified or not
        }

        if (modalities.maxCoeff() < epsilon) // either tik or condition is null, for every class
        {
          condition.maxCoeff(&sampleVal);
        }
        else
        {
          modalities = modalities / modalities.sum();
          sampleVal = multi_.sample(modalities);
        }
      }
      break;

      case missingFiniteValues_: // renormalize proba distribution on allowed sampling values
      {
        RowVector<Real> modalities(nbClass_, 0.);
        RowVector<int> condition(nbClass_, 0);

        for(std::vector<int>::const_iterator currMod = zi_.misData_(i).second.begin();
            currMod != zi_.misData_(i).second.end();
            ++currMod)
        {
          int k = *currMod;
          zi_.data_(i) = k;
          condition(k) = composer_.checkSampleCondition();
          modalities(k) = tik_(i, k) * condition(k);
        }

        if (modalities.maxCoeff() < epsilon) // either tik or condition is null, for every class
        {
          condition.maxCoeff(&sampleVal);
        }
        else
        {
          modalities = modalities / modalities.sum();
          sampleVal = multi_.sample(modalities);
        }
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

void ClassSampler::sStepNoCheck(int i)
{
#ifdef MC_DEBUG
    std::cout << "ClassSampler::sStepNoCheck, i: " << i << std::endl;
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
        std::cout << "missing_, i:" << i << ", tik_.row(i): " << tik_.row(i) <<  std::endl;
#endif
        sampleVal = multi_.sample(tik_.row(i)); // the most simple case
      }
      break;

      case missingFiniteValues_: // renormalize proba distribution on allowed sampling values
      {
#ifdef MC_DEBUG
        std::cout << "missingFiniteValues_" << std::endl;
#endif
        Vector<Real> modalities(nbClass_, 0.);

        for(std::vector<int>::const_iterator currMod = zi_.misData_(i).second.begin();
            currMod != zi_.misData_(i).second.end();
            ++currMod)
        {
#ifdef MC_DEBUG
          std::cout << "\tcurrMod: " << *currMod << std::endl;
#endif
          modalities(*currMod) = tik_(i, *currMod);
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
} // namespace mixt
