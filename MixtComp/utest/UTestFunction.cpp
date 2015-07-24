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
 *  Created on: July 8, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "UTestFunction.h"

using namespace mixt;

void mStepBOS(const Vector<BOSPath>& path,
              int& mu,
              Real& pi,
              int nbModalities)
{
#ifdef MC_DEBUG
  std::cout << "mStepBOS" << std::endl;
#endif
  pi = 0.; // pi_ parameter is reinitialized
  int nbInd = path.size();

  for (int i = 0; i < nbInd; ++i)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << ", path(i).nbZ(): " << path(i).nbZ() << std::endl;
#endif
    pi += path(i).nbZ();
  }

  pi /= (nbInd * (nbModalities - 1.)); // from accounts to frequencies of z

#ifdef MC_DEBUG
  std::cout << "pi: " << pi << std::endl;
#endif

  Vector<Real> logLik(nbModalities);
  logLik = 0.;

  for (mu = 0; mu < nbModalities; ++mu) // mu obtained from maximization over all possible values
  {
    for (int i = 0; i < nbInd; ++i)
    {
      logLik(mu) += path(i).computeLogProba(mu, pi);
#ifdef MC_DEBUG
      std::cout << "mu: " << mu << ", pi: " << pi << std::endl;
      std::cout << "i: " << i << ", path(i).computeLogProba(mu, pi): " << path(i).computeLogProba(mu, pi) << std::endl;
#endif
    }
  }

#ifdef MC_DEBUG
  std::cout << "logLik" << std::endl;
  std::cout << logLik << std::endl;
#endif

  logLik.maxCoeff(&mu);
}
