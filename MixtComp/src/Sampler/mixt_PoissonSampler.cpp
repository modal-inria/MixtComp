/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_PoissonSampler.h"
#include "../Mixture/mixt_IMixture.h"

namespace mixt
{
PoissonSampler::PoissonSampler(AugmentedData<Vector<int> >& augData,
                               const Vector<Real>& param,
                               int nbClass) :
    augData_(augData),
    param_(param)
{}

void PoissonSampler::samplingStepNoCheck(int i,
                                         int z_i)
{
#ifdef MC_DEBUG
  std::cout << "PoissonSampler::sampleIndividual" << std::endl;
  std::cout << "\ti: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (augData_.misData_(i).first != present_)
  {
    int x;
    Real lambda = param_(z_i);

#ifdef MC_DEBUG
    std::cout << "\tlambda: " << lambda << std::endl;
#endif

    switch(augData_.misData_(i).first)
    {
      case missing_:
      {
#ifdef MC_DEBUG
        std::cout << "\tmissing_" << std::endl;
#endif
        x = poisson_.sample(lambda);
      }
      break;

      default:
      {}
      break;
    }

#ifdef MC_DEBUG
    std::cout << "\tsampled val: " << x << std::endl;
#endif
    augData_.data_(i) = x;
  }
}

} // namespace mixt
