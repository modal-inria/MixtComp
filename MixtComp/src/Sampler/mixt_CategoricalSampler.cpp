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
 *  Created on: Mar 27, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_CategoricalSampler.h"
#include "../Various/mixt_Constants.h"
#include "../Mixture/mixt_IMixture.h"

namespace mixt
{
CategoricalSampler::CategoricalSampler(const IMixture& mixture,
                                       AugmentedData<Vector<int> >& augData,
                                       const Vector<Real>& param,
                                       int nbClass) :
    mixture_(mixture),
    nbClass_(nbClass),
    augData_(augData),
    param_(param)
{}

void CategoricalSampler::samplingStepCheck(int i,
                                           int z_i)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalSampler::sampleIndividual" << std::endl;
  std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (augData_.misData_(i).first != present_)
  {
    int sampleVal;
    int nbModalities = param_.rows() / nbClass_;

#ifdef MC_DEBUG
    std::cout << "CategoricalSampler::sampleIndividual" << std::endl;
    std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

    switch(augData_.misData_(i).first)
    {
      case missing_:
      {
        Vector<Real> modalities(nbModalities);
        for (augData_.data_(i) = 0; augData_.data_(i) < nbModalities; ++augData_.data_(i)) // augData_.data_(i) changed in place to take all possible values
        {
          modalities(augData_.data_(i)) = param_(z_i * nbModalities + augData_.data_(i)) * mixture_.checkSampleCondition(); // checkSampleCondition value is 1 or 0, reflecting the fact that conditions on data are verified or not
        }
        modalities = modalities / modalities.sum();
        sampleVal = multi_.sample(modalities);
      }
      break;

      case missingFiniteValues_: // renormalize proba distribution on allowed sampling values
      {
        Vector<Real> modalities(nbModalities, 0.);

        for(std::vector<int>::const_iterator currMod = augData_.misData_(i).second.begin();
            currMod != augData_.misData_(i).second.end();
            ++currMod)
        {
#ifdef MC_DEBUG
          std::cout << "\tcurrMod: " << *currMod << std::endl;
          std::cout << "z_i * nbModalities + *currMod: " << z_i * nbModalities + *currMod << ", param_.size(): " << param_.size() << std::endl;
          std::cout << "*currMod: " << *currMod << ", modalities.size(): " << modalities.size() << std::endl;
#endif
          augData_.data_(i) = *currMod;
          modalities(*currMod) = param_(z_i * nbModalities + *currMod) * mixture_.checkSampleCondition();
        }
        modalities = modalities / modalities.sum();
        sampleVal = multi_.sample(modalities);

      }
      break;

      default:
      {
#ifdef MC_DEBUG
          std::cout << "CategoricalSampler, missing value type unknown" << std::endl;
#endif
      }
      break;
    }
    augData_.data_(i) = sampleVal;
  }
}

void CategoricalSampler::samplingStepNoCheck(int i,
                                             int z_i)
{
#ifdef MC_DEBUG
  std::cout << "CategoricalSampler::sampleIndividual" << std::endl;
  std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (augData_.misData_(i).first != present_)
  {
    int sampleVal;
    int nbModalities = param_.rows() / nbClass_;

#ifdef MC_DEBUG
    std::cout << "CategoricalSampler::sampleIndividual" << std::endl;
    std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

    switch(augData_.misData_(i).first)
    {
      case missing_:
      {
        sampleVal = multi_.sample(param_.block(z_i * nbModalities, 0,  // position of first element
                                               nbModalities      , 1)); // dimension of the vector to extract);
      }
      break;

      case missingFiniteValues_: // renormalize proba distribution on allowed sampling values
      {
        Vector<Real> modalities(nbModalities);
        modalities = 0.;

        for(std::vector<int>::const_iterator currMod = augData_.misData_(i).second.begin();
            currMod != augData_.misData_(i).second.end();
            ++currMod)
        {
#ifdef MC_DEBUG
          std::cout << "\tcurrMod: " << *currMod << std::endl;
          std::cout << "z_i * nbModalities + *currMod: " << z_i * nbModalities + *currMod << ", param_.size(): " << param_.size() << std::endl;
          std::cout << "*currMod: " << *currMod << ", modalities.size(): " << modalities.size() << std::endl;
#endif
          modalities(*currMod) = param_(z_i * nbModalities + *currMod);
        }
        modalities = modalities / modalities.sum();
        sampleVal = multi_.sample(modalities);

      }
      break;

      default:
      {
#ifdef MC_DEBUG
          std::cout << "CategoricalSampler, missing value type unknown" << std::endl;
#endif
      }
      break;
    }
    augData_.data_(i) = sampleVal;
  }
}
} // namespace mixt
