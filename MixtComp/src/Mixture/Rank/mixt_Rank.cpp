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
 *  Created on: August 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_Rank.h"

namespace mixt
{
Rank::Rank(int nbClass) :
    nbClass_(nbClass),
    nbInd_(0),
    nbPos_(0),
    pi_(0.)
{}

Rank::Rank(int nbClass,
           int nbInd,
           Vector<Vector<int> >& data,
           const RankVal& mu,
           Real pi) :
    nbClass_(nbClass),
    nbInd_(nbInd),
    nbPos_(mu.getNbPos()),
    mu_(mu),
    pi_(pi)
{
  data_.resize(nbInd);
  for (int i = 0; i < nbInd; ++i)
  {
    data_(i).setNbPos(nbPos_);
    data_(i).setO(data(i));
  }
}

void Rank::removeMissing()
{
  for (int i = 0; i < nbInd_; ++i)
  {
    data_(i).removeMissing();
  }
}

Real Rank::lnCompletedProbability() const
{
  Real logProba = 0.;

  for (int i = 0; i < nbInd_; ++i)
  {
    logProba += data_(i).lnCompletedProbability(mu_, pi_);
  }

  return logProba;
}

/**
 * Perform one round of Gibbs sampling for the central rank */
void Rank::sampleMu()
{
  Vector<Real, 2> logProba; // first element: current log proba, second element: logProba of permuted state
  Vector<Real, 2> proba   ; // multinomial distribution obtained from the logProba

  logProba(0) = lnCompletedProbability(); // proba of current mu

  for (int p = 0; p < nbPos_ - 1; ++p)
  {
    mu_.permutation(p);
    logProba(1) = lnCompletedProbability();
    proba.logToMulti(logProba);

#ifdef MC_DEBUG
    std::cout << "p: " << p << ", logProba: " << logProba.transpose() << ", proba: " << proba.transpose() << std::endl;
#endif

    if (multi_.sample(proba) == 1) // switch to permuted state ?
    {
      logProba(0) = logProba(1); // accept permutation
    }
    else
    {
      mu_.permutation(p); // revert to previous state
    }
  }
}
} // namespace mixt
