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
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_GaussianLikelihood.h"
#include "../Various/mixt_Def.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

GaussianLikelihood::GaussianLikelihood(const Vector<Real>& param,
                                       const AugmentedData<Vector<Real> >& augData,
                                       int nbClass) :
    nbClass_(nbClass),
    param_(param),
    augData_(augData)
{}

Real GaussianLikelihood::lnCompletedProbability(int i, int k)
{
#ifdef MC_DEBUG
      std::cout << "GaussianLikelihood::lnCompletedLikelihood" << std::endl;
#endif
  Real mean  = param_(2 * k    );
  Real sd    = param_(2 * k + 1);

  Real logProba = normal_.lpdf(augData_.data_(i),
                               mean,
                               sd);

  return logProba;
}

Real GaussianLikelihood::lnObservedProbability(int i, int k)
{
#ifdef MC_DEBUG
  std::cout << "GaussianLikelihood::lnObservedLikelihood" << std::endl;
#endif
  Real mean  = param_(2 * k    );
  Real sd    = param_(2 * k + 1);

#ifdef MC_DEBUG
  std::cout << "i: " << i << std::endl;
  std::cout << "\tmean: " << mean << std::endl;
  std::cout << "\tsd: " << sd << std::endl;
#endif
  Real logProba;

  switch(augData_.misData_(i).first)   // likelihood for present value
  {
    case present_:
    {
      Real mean  = param_(2 * k    );
      Real sd    = param_(2 * k + 1);

      logProba = normal_.lpdf(augData_.data_(i),
                              mean,
                              sd);
    }
    break;

    case missing_: // no contribution to the observed likelihood
    {
#ifdef MC_DEBUG
      std::cout << "\tmissing" << std::endl;
#endif
      logProba = 0.;
    }
    break;

    case missingIntervals_:
    {
      Real infBound  = augData_.misData_(i).second[0];
      Real supBound  = augData_.misData_(i).second[1];
      Real infCdf = normal_.cdf(infBound,
                                mean,
                                sd);
      Real supCdf = normal_.cdf(supBound,
                                mean,
                                sd);
#ifdef MC_DEBUG
      std::cout << "\tmissingIntervals_" << std::endl;
      std::cout << "\tinfBound: " << infBound << "\tsupBound: " << supBound << std::endl;
      std::cout << "\tinfCdf: " << infCdf << "\tsupCdf: " << supCdf << std::endl;
#endif
      logProba = std::log(supCdf - infCdf);
    }
    break;

    case missingLUIntervals_:
    {
      Real supBound = augData_.misData_(i).second[0];
      Real supCdf = normal_.cdf(supBound,
                                mean,
                                sd);
#ifdef MC_DEBUG
      std::cout << "\tmissingLUIntervals_" << std::endl;
      std::cout << "\tsupBound: " << supBound << std::endl;
      std::cout << "\tsupCdf: " << supCdf << std::endl;
#endif
      logProba = std::log(supCdf);
    }
    break;

    case missingRUIntervals_:
    {
      Real infBound = augData_.misData_(i).second[0];
      Real infCdf = normal_.cdf(infBound,
                                mean,
                                sd);
#ifdef MC_DEBUG
      std::cout << "\tmissingRUIntervals_" << std::endl;
      std::cout << "\tinfBound: " << infBound << std::endl;
      std::cout << "\tinfCdf: " << infCdf << std::endl;
#endif
      logProba = std::log(1. - infCdf);
    }
    break;

    default:
    {}
    break;
  }
#ifdef MC_DEBUG
  std::cout << "\tlogProba: " << logProba << std::endl;
#endif
  return logProba;
}

} /* namespace mixt */
