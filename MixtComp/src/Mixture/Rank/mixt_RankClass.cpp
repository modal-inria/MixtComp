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

#include "mixt_RankClass.h"

namespace mixt
{
RankClass::RankClass(const Vector<RankIndividual>& data,
                     const std::set<int>& setInd,
                     RankVal& mu,
                     Real& pi) :
    nbInd_(data.size()),
    data_(data),
    setInd_(setInd),
    mu_(mu),
    pi_(pi)
{}

Real RankClass::lnCompletedProbability() const
{
  Real logProba = 0.;
  int a, g; // a and g are only used in the mStep, here they are dummy variables

  for (std::set<int>::const_iterator it = setInd_.begin(), itEnd = setInd_.end();
       it != itEnd;
       ++it)
  {
    logProba += data_(*it).lnCompletedProbability(mu_, pi_, a, g);
  }

  return logProba;
}

Real RankClass::lnCompletedProbability(int& a, int& g) const
{
  Real logProba = 0.;
  a = 0;
  g = 0;

  for (std::set<int>::const_iterator it = setInd_.begin(), itEnd = setInd_.end();
       it != itEnd;
       ++it)
  {
    int currA, currG;
    logProba += data_(*it).lnCompletedProbability(mu_, pi_, currA, currG);
    a += currA;
    g += currG;
  }

  return logProba;
}

Real RankClass::lnCompletedProbabilityInd(int i) const
{
  int a, g; // a and g are only used in the mStep, here they are dummy variables
  return data_(i).lnCompletedProbability(mu_, pi_, a, g);
}

Real RankClass::lnObservedProbability(int i) const {
  Real logProba;

  if (data_(i).allMissing()) {
#ifdef MC_DEBUG
    std::cout << "RankClass::lnObservedProbability, allMissing" << std::endl;
#endif

    logProba = 0.; // using enumCompleted on a completely missing individual might by computationally intractable for a high number of positions
  }
  else {
#ifdef MC_DEBUG
    std::cout << "RankClass::lnObservedProbability, enumCompleted" << std::endl;
#endif

    std::list<RankVal> allCompleted = data_(i).enumCompleted(); // get the list of all possible completions of observation i
    Vector<Real> allCompletedProba(allCompleted.size()); // used to "linearize" the storage of probabilities from allCompleted

    int c = 0;
    for (std::list<RankVal>::const_iterator it = allCompleted.begin(), itE = allCompleted.end();
         it != itE;
         ++c, ++it) {
      std::map<RankVal, Real>::const_iterator itM = observedProbaSampling_.find(*it); // has the current completion been observed in computeObservedProba ?
      if (itM == observedProbaSampling_.end()) { // the current individual has not been observed during sampling
        allCompletedProba(c) = minInf;
      }
      else {
        allCompletedProba(c) = std::log(itM->second);
      }
    }

#ifdef MC_DEBUG
    std::cout << "RankMixture::lnObservedProbability, allCompletedProba.size(): " << allCompletedProba.size() << ", allCompletedProba: " << itString(allCompletedProba) << std::endl;
#endif

    Vector<Real> dummy;
    logProba = dummy.logToMulti(allCompletedProba); // compute the observed logproba by summing all the completed logproba
  }

  return logProba;
}

void RankClass::sampleMu()
{
  Vector<Real, 2> logProba; // first element: current log proba, second element: logProba of permuted state
  Vector<Real, 2> proba   ; // multinomial distribution obtained from the logProba

  logProba(0) = lnCompletedProbability(); // proba of current mu

  for (int p = 0; p < mu_.nbPos() - 1; ++p)
  {
    mu_.permutation(p);
    logProba(1) = lnCompletedProbability();
    proba.logToMulti(logProba);

#ifdef MC_DEBUG
    std::cout << "RankClass::sampleMu, p: " << p << ", logProba: " << itString(logProba) << ", proba: " << itString(proba) << std::endl;
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

void RankClass::mStep()
{
#ifdef MC_DEBUG
    std::cout << "RankClass::mStep" << std::endl;
#endif
  Vector<RankVal> mu(nbGibbsIterRank);
  Vector<Real> pi(nbGibbsIterRank);
  Vector<Real> logProba(nbGibbsIterRank);

  int a, g;
  for (int i = 0; i < nbGibbsIterRank; ++i)
  {
    sampleMu();
    mu(i) = mu_;
    logProba(i) = lnCompletedProbability(a, g);
    pi(i) = Real(g) / Real(a);

#ifdef MC_DEBUG
    std::cout << "Rank::mStep, i: " << i << ", mu: " << mu(i) << ", pi: " << pi(i) << ", logProba: " << logProba(i) << std::endl;
#endif
  }

  int bestTheta;
  logProba.maxCoeff(&bestTheta);

  mu_ = mu(bestTheta);
  pi_ = pi(bestTheta);

  pi_ = std::max(pi_, 0.01);
  pi_ = std::min(pi_, 0.99);

#ifdef MC_DEBUG
  std::cout << "RankClass::mStep, mu_: " << mu_ << ", pi_: " << pi_ << std::endl;
#endif
}

void RankClass::computeObservedProba() {
  RankIndividual ri(mu_.nbPos()); // dummy rank individual used to compute a Vector<std::map<RankVal, Real> > for each class

  ri.observedProba(mu_,
                   pi_,
                   observedProbaSampling_);
}

} // namespace mixt
