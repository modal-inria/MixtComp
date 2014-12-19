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

namespace mixt
{
GaussianSampler::GaussianSampler(AugmentedData<STK::Array2D<STK::Real> >* p_augData,
                                 const STK::Array2D<STK::Real>* p_param,
                                 int nbClass) :
    p_augData_(p_augData),
    p_param_(p_param)
{}

GaussianSampler::~GaussianSampler()
{}

void GaussianSampler::sampleIndividual(int i, int z_i)
{
#ifdef MC_DEBUG
  std::cout << "GaussianSampler::sampleIndividual" << std::endl;
  std::cout << "\ti: " << i << ", z_i: " << z_i << std::endl;
#endif

  for (int j = 0; j < p_augData_->misData_.cols(); ++j)
  {
    if (p_augData_->misData_(i, j).first != present_)
    {
      STK::Real z;
      STK::Real mean  = p_param_->elt(2 * z_i    , j);
      STK::Real sd    = p_param_->elt(2 * z_i + 1, j);

#ifdef MC_DEBUG
      std::cout << "\tmean: " << mean << ", sd: " << sd << std::endl;
#endif

      switch(p_augData_->misData_(i, j).first)
      {
        case missing_:
        {
#ifdef MC_DEBUG
          std::cout << "\tmissing_" << std::endl;
#endif
          z = normal_.sample(0.,
                             1.);
        }
        break;

        case missingIntervals_:
        {
#ifdef MC_DEBUG
          std::cout << "\tmissingIntervals_" << std::endl;
#endif
          STK::Real infBound = p_augData_->misData_(i, j).second[0];
          STK::Real supBound = p_augData_->misData_(i, j).second[1];

          STK::Real lower = (infBound - mean) / sd;
          STK::Real upper = (supBound - mean) / sd;
          STK::Real alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;

#ifdef MC_DEBUG
          std::cout << "\tmissingIntervals_" << std::endl;
          std::cout << "\tinfBound " << infBound << ", supBound: " << supBound << std::endl;
          std::cout << "\tlower: " << lower << ", upper: " << upper << std::endl;
          std::cout << "\talpha: " << alpha << std::endl;
#endif

          if (alpha*exp(alpha * lower / 2.) / sqrt(exp(1)) > exp(pow(lower, 2) / 2) / (upper - lower))
          {
            do
            {
              z = lbSampler(lower);
            }
            while(upper < z);
          }
          else
          {
            z = lrbSampler(lower, upper);
          }
        }
        break;

        case missingLUIntervals_: // missingLUIntervals
        {
#ifdef MC_DEBUG
          std::cout << "\tmissingLUIntervals_" << std::endl;
#endif
          STK::Real supBound = p_augData_->misData_(i, j).second[0];
          STK::Real upper = (supBound - mean) / sd;
          z = -lbSampler(-upper);
        }
        break;

        case missingRUIntervals_: // missingRUIntervals
        {
#ifdef MC_DEBUG
          std::cout << "\tmissingRUIntervals_" << std::endl;
#endif
          STK::Real infBound = p_augData_->misData_(i, j).second[0];
          STK::Real lower = (infBound - mean) / sd;
          z = lbSampler(lower);
        }
        break;

        default:
        {}
        break;
      }

#ifdef MC_DEBUG
      std::cout << "\tsampled val: " << z * sd + mean << std::endl;
#endif
      p_augData_->data_(i, j) = z * sd + mean;
    }
  }
}

STK::Real GaussianSampler::lbSampler(STK::Real lower)
{
  STK::Real alpha = (lower + sqrt(pow(lower, 2) + 4.))/2.;
  STK::Real z, u, rho;
  if (lower < 0)
  {
    do
    {
      z = normal_.sample(0.,
                         1.);
    }
    while (z < lower);
  }
  else
  {
    do
    {
      z = exponential_.sample(1. / alpha) + lower;
      rho = exp(-pow((z - alpha), 2) / 2.);
      u = uniform_.sample(0., 1.);
    }
    while (u > rho);
  }
  return z;
}

STK::Real GaussianSampler::lrbSampler(STK::Real lower, STK::Real upper)
{
  STK::Real z, u, rho;

  do
  {
    z = uniform_.sample(lower, upper);

    if (lower < 0. && 0. < upper)
      rho = exp(-pow(z, 2));
    else if (upper < 0.)
      rho = exp((pow(upper, 2)-pow(z, 2))/2);
    else if (0. < lower)
      rho = exp((pow(lower, 2)-pow(z, 2))/2);

    u = uniform_.sample(0., 1.);
  }
  while(u > rho);

  return z;
}
} // namespace mixt
