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
 *  Created on: December 12, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_PoissonSampler.h"

namespace mixt
{
PoissonSampler::PoissonSampler(AugmentedData<STK::Array2D<int> >* p_augData,
                               const STK::Array2D<STK::Real>* p_param,
                               int nbClass) :
    p_augData_(p_augData),
    p_param_(p_param)
{}

PoissonSampler::~PoissonSampler()
{}

void PoissonSampler::sampleIndividual(int i, int z_i)
{
#ifdef MC_DEBUG
  std::cout << "PoissonSampler::sampleIndividual" << std::endl;
  std::cout << "\ti: " << i << ", z_i: " << z_i << std::endl;
#endif

  if (p_augData_->misData_(i, 0).first != present_)
  {
    int x;
    STK::Real lambda = p_param_->elt(z_i, 0);

#ifdef MC_DEBUG
    std::cout << "\tlambda: " << lambda << std::endl;
#endif

    switch(p_augData_->misData_(i, 0).first)
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
    p_augData_->data_(i, 0) = x;
  }
}

} // namespace mixt
