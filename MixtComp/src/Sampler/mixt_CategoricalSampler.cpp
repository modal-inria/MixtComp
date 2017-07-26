/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
