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

namespace mixt
{
ClassSampler::ClassSampler(AugmentedData<Vector<int> >& zi,
                           const Matrix<Real>& tik,
                           int nbClass) :
    nbClass_(nbClass),
    zi_(zi),
    tik_(tik)
{}

ClassSampler::~ClassSampler()
{}

void ClassSampler::sampleIndividual(int i)
{
#ifdef MC_DEBUG
  std::cout << "ClassSampler::sampleIndividual" << std::endl;
  std::cout << "i: " << i << std::endl;
#endif

  if (zi_.misData_(i).first != present_)
  {
    int sampleVal;

#ifdef MC_DEBUG
    std::cout << "present_" << std::endl;
#endif

    switch(zi_.misData_(i, 0).first)
    {
      case missing_:
      {
#ifdef MC_DEBUG
        std::cout << "missing_" << std::endl;
#endif
        sampleVal = multi_.sample(tik_.block(i, 0,         // position of first element
                                             1, nbClass_)); // dimension of the vector to extract);
      }
      break;

      case missingFiniteValues_: // renormalize proba distribution on allowed sampling values
      {
#ifdef MC_DEBUG
        std::cout << "missingFiniteValues_" << std::endl;
#endif
        Vector<Real> modalities(nbClass_);
        modalities = 0.;

        Vector<Real> equiModalities(nbClass_);
        equiModalities = 0.;

        for(std::vector<int>::const_iterator currMod = zi_.misData_(i).second.begin();
            currMod != zi_.misData_(i).second.end();
            ++currMod)
        {
#ifdef MC_DEBUG
          std::cout << "\tcurrMod: " << *currMod << std::endl;
#endif
          modalities(*currMod) = tik_(i, *currMod);
          equiModalities(*currMod) = 1.;
        }
        Real modSum = modalities.sum();
        if (modSum < minStat)
        {
          equiModalities = equiModalities / equiModalities.sum();
          sampleVal = multi_.sample(equiModalities);
        }
        else
        {
          modalities = modalities / modalities.sum();
          sampleVal = multi_.sample(modalities);
        }
      }
      break;

      default:
      {}
      break;
    }
    zi_.data_(i) = sampleVal;
  }
}
} // namespace mixt
