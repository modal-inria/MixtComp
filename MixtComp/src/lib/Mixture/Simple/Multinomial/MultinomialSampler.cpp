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
 *  Created on: Mar 27, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Mixture/IMixture.h>
#include <Mixture/Simple/Multinomial/MultinomialSampler.h>
#include <Various/Constants.h>

namespace mixt
{
MultinomialSampler::MultinomialSampler(AugmentedData<Vector<int> >& augData,
                                       const Vector<Real>& param,
                                       int nbClass) :
    nbClass_(nbClass),
    augData_(augData),
    param_(param)
{}

void MultinomialSampler::samplingStepNoCheck(int i,
                                             int z_i)
{
#ifdef MC_DEBUG
  std::cout << "MultinomialSampler::sampleIndividual" << std::endl;
  std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (augData_.misData_(i).first != present_)
  {
    int sampleVal = 0;
    int nbModalities = param_.rows() / nbClass_;

#ifdef MC_DEBUG
    std::cout << "MultinomialSampler::sampleIndividual" << std::endl;
    std::cout << "i: " << i << ", z_i: " << z_i << std::endl;
#endif

    switch(augData_.misData_(i).first)
    {
      case missing_:
      {
        Vector<Real> paramTemp = param_.block(z_i * nbModalities, 0,  // position of first element
                                              nbModalities      , 1);
        sampleVal = multi_.sample(paramTemp); // dimension of the vector to extract);
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
    	  throw("MultinomialSampler::samplingStepNoCheck, error in missing data handling, please report to the maintainer.");
      }
      break;
    }
    augData_.data_(i) = sampleVal;
  }
}
} // namespace mixt
