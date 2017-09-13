/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_RankClass.h"

namespace mixt
{
RankClass::RankClass(const Vector<RankIndividual>& data,
                     const std::set<Index>& setInd,
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

  for (std::set<Index>::const_iterator it = setInd_.begin(), itEnd = setInd_.end();
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

  for (std::set<Index>::const_iterator it = setInd_.begin(), itEnd = setInd_.end();
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

  for (int p = 0; p < mu_.nbPos() - 1; ++p) {
    mu_.permutation(p);
    logProba(1) = lnCompletedProbability();
    proba.logToMulti(logProba);

#ifdef MC_DEBUG
    std::cout << "RankClass::sampleMu, p: " << p << ", logProba: " << itString(logProba) << ", proba: " << itString(proba) << std::endl;
#endif

    if (multi_.sample(proba) == 1) { // switch to permuted state ?
      logProba(0) = logProba(1); // accept permutation
    }
    else {
      mu_.permutation(p); // revert to previous state
    }
  }
}

void RankClass::mStep() {
  Vector<RankVal> mu(nbGibbsIterRankMStep);
  Vector<Real> pi(nbGibbsIterRankMStep);
  Vector<Real> logProba(nbGibbsIterRankMStep);

  int a, g;
  int i = 0;
  while (i < nbGibbsIterRankMStep) {
    sampleMu();
    mu(i) = mu_;
    logProba(i) = lnCompletedProbability(a, g);
    pi(i) = Real(g) / Real(a);

    if (0 < g && g < a) {
      ++i; // only register current mu and pi if they are valid
    }
  }

  int bestTheta;
  logProba.maxCoeff(&bestTheta);

  mu_ = mu(bestTheta);
  pi_ = pi(bestTheta);
}

void RankClass::computeObservedProba() {
  RankIndividual ri(mu_.nbPos()); // dummy rank individual used to compute a Vector<std::map<RankVal, Real> > for each class
  Vector<MisVal> obsData(mu_.nbPos(), MisVal(missing_, {})); // individual is completely missing, so that remove missing will reinitialize everything upon call
  ri.setObsData(obsData);
  ri.removeMissing();
  ri.observedProba(mu_,
                   pi_,
                   observedProbaSampling_);

#ifdef MC_DEBUG
  std::cout << "RankClass::computeObservedProba, observedProbaSampling_.size(): " << observedProbaSampling_.size() << std::endl;
#endif
}

} // namespace mixt
