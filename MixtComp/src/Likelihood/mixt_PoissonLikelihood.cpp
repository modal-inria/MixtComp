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
 *  Created on: December 9, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_PoissonLikelihood.h"
#include "../Various/mixt_Def.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

PoissonLikelihood::PoissonLikelihood(const Vector<Real>& param,
                                     const AugmentedData<Vector<int> >& augData,
                                     int nbClass) :
    param_(param),
    augData_(augData)
{}

Real PoissonLikelihood::lnCompletedProbability(int i, int k)
{
#ifdef MC_DEBUG
   std::cout << "PoissonLikelihood::lnCompletedLikelihood" << std::endl;
   std::cout << "\t(*p_param_): " << param_ << std::endl;
#endif

  Real lambda = param_(k);
  Real proba;
  proba = poisson_.pdf(augData_.data_(i),
                       lambda);

  return std::log(proba);
}

Real PoissonLikelihood::lnObservedProbability(int i, int k)
{
#ifdef MC_DEBUG
  std::cout << "PoissonLikelihood::lnObservedLikelihood" << std::endl;
  std::cout << "\t(*p_param_): " << param_ << std::endl;
#endif
  Real lambda = param_(k);
  Real proba = minInf;

  switch(augData_.misData_(i).first)   // likelihood for present value
  {
    case present_:
    {
      proba = poisson_.pdf(augData_.data_(i),
                           lambda);
    }
    break;

    case missing_: // no contribution to the observed likelihood
    {
#ifdef MC_DEBUG
      std::cout << "\tmissing" << std::endl;
#endif
      proba = 1.;
    }
    break;

    default:
    {}
    break;
  }

  return std::log(proba);

#ifdef MC_DEBUG
  std::cout << "\tproba: " << proba << std::endl;
#endif
}

} /* namespace mixt */
