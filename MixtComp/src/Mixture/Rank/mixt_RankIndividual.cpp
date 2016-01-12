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
 *  Created on: August 20, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include<set>

#include "mixt_RankIndividual.h"
#include "../LinAlg/mixt_Math.h"

namespace mixt
{

RankIndividual::RankIndividual() :
  nbPos_(0),
  lnFacNbPos_(0)
{}

RankIndividual::RankIndividual(int nbPos) :
    nbPos_(nbPos),
    lnFacNbPos_(- std::log(fac(nbPos))),
    x_(nbPos)
{
  obsData_.resize(nbPos);
  y_.resize(nbPos);
}

RankIndividual::RankIndividual(const RankVal& rv) :
    nbPos_(rv.nbPos()),
    lnFacNbPos_(- std::log(fac(nbPos_))),
    x_(rv)
{
  obsData_.resize(nbPos_);
  y_.resize(nbPos_);
}

RankIndividual::RankIndividual(const RankIndividual& ri) :
  nbPos_(ri.nbPos_),
  lnFacNbPos_(ri.lnFacNbPos_),
  obsData_(ri.obsData_),
  x_(ri.x_),
  y_(ri.y_)
{} // note that the state of multi_ is not copied and a new rng is created

RankIndividual& RankIndividual::operator=(const RankIndividual& ri)
{
  nbPos_ = ri.nbPos_;
  lnFacNbPos_ = ri.lnFacNbPos_;
  obsData_ = ri.obsData_;
  x_ = ri.x_;
  y_ = ri.y_;

  return *this;
} // note that the state of multi_ is not copied and a new rng is created

void RankIndividual::setNbPos(int nbPos)
{
  nbPos_ = nbPos;
  obsData_.resize(nbPos);
  x_ .setNbPos(nbPos);

  y_.resize(nbPos);
  std::iota(y_.begin(), y_.end(), 0); // presentation order initialized, ready to be shuffled by removeMissing

  lnFacNbPos_ = - std::log(fac(nbPos_));
}

void RankIndividual::removeMissing()
{
#ifdef MC_DEBUGNEW
    std::cout << "RankIndividual::removeMissing, x_: " << x_ << ", y_: " << itString(y_) << std::endl;
#endif

  std::iota(y_.begin(), y_.end(), 0);
  yGen();

  if (allMissing() == true) { // no need for enumeration if everything is missing, shuffling will do the job faster
    Vector<int> xVec(nbPos_);
    std::iota(xVec.begin(), xVec.end(), 0);
    x_.setO(xVec);
  }
  else { // uniform sampling on all the possible completions
    std::list<RankVal> rankList = enumCompleted();

    int sampledIndex = multi_.sampleInt(0, rankList.size() - 1);
    std::list<RankVal>::const_iterator it = rankList.begin();
    std::advance(it, sampledIndex);

    x_ = *it;
  }
}

void RankIndividual::yGen()
{
  multi_.shuffle(y_);
}

Real RankIndividual::xGen(const RankVal& mu,
                          Real pi)
{
#ifdef MC_DEBUG
  int a = 0;
  int g = 0;
#endif

#ifdef MC_DEBUG
  std::cout << "RankIndividual::xGen, mu: " << mu << ", pi: " << pi << ", y_: " << itString(y_) << std::endl;
#endif

  Real logProba = 0.;

  Real goodlp = std::log(     pi);
  Real badlp  = std::log(1. - pi);

  std::vector<int> x(1); // vector is suboptimal for insertion, but provides contiguous memory storage which will fit in CPU cache. std::list on the contrary does not guarantee contiguity.
  x.reserve(nbPos_);

  x[0] = y_(0);

  for (int j = 1; j < nbPos_; ++j) // current element in the presentation order, or current size of the x vector
  {
    int currY = y_(j);
    bool yPlaced = false; // has currY been placed correctly ?
    for (int i = 0; i < j; ++i)
    {
      bool comparison = mu.r()(currY) < mu.r()(x[i]); // true if curr elem is correctly ordered

      if (multi_.sampleBinomial(pi) == 1) // is the comparison correct ?
      {
        logProba += goodlp;

#ifdef MC_DEBUG
        ++a;
        ++g;
#endif
      }
      else
      {
        comparison = !comparison;
        logProba += badlp;

#ifdef MC_DEBUG
        ++a;
#endif
      }

      if (comparison) // element j must be placed here
      {
        x.insert(x.begin() + i, currY);
        yPlaced = true;
        break; // no need to test further position for j element
      }
    }
    if (!yPlaced)
    {
      x.push_back(currY); // if element j has not been placed yet, it goes at the end of x
    }
  }

  x_.setO(x);

#ifdef MC_DEBUG
  std::cout << "Rank::xGen, a: " << a << ", g:" << g << std::endl;
#endif

  return lnFacNbPos_ + logProba;
}

Real RankIndividual::lnCompletedProbability(const RankVal& mu,
                                            Real pi,
                                            int& a,
                                            int& g) const
{
  AG(mu, a, g);

#ifdef MC_DEBUG
  std::cout << "Rank::lnCompletedProbability, a: " << a << ", g:" << g << ", y_: " << itString(y_) << std::endl;
#endif

  return lnFacNbPos_ + g * std::log(pi) + (a - g) * std::log(1. - pi);
}

void RankIndividual::AG(const RankVal& mu,
                        int& a,
                        int& g) const {
#ifdef MC_DEBUG
  std::cout << "Rank::AG, y_: " << itString(y_) << ", mu.r(): " << itString(mu.r()) << ", x_.r(): " << itString(x_.r()) << std::endl;
#endif

  a = 0;
  g = 0;

  std::vector<int> x(1); // vector is suboptimal for insertion, but provides contiguous memory storage which will fit in CPU cache
  x.reserve(nbPos_);

  x[0] = y_(0);

  for (int j = 1; j < nbPos_; ++j) // current element in the presentation order, or current size of the x vector
  {
    int currY = y_(j);
    bool yPlaced = false; // y not considered at correct position until proven so

    for (int i = 0; i < j; ++i) {
      yPlaced = (x_.r()(currY) < x_.r()(x[i]));

      if (yPlaced == (mu.r()(currY) < mu.r()(x[i]))) { // is the comparison correct, according to the order provided in mu ?
        ++a;
        ++g;
      }
      else {
        ++a;
      }

      if (yPlaced) {
        x.insert(x.begin() + i, currY);
        break;
      }
    }
    if (!yPlaced) {
      x.push_back(currY); // if element j has not been placed yet, it goes at the end of x
    }
  }
}

void RankIndividual::sampleX(const RankVal& mu,
                             Real pi) {

}

/**
 * Perform one round of Gibbs sampling for the presentation order
 * @param mu central rank
 * @param pi precision */
void RankIndividual::sampleY(const RankVal& mu,
                             Real pi)
{
  int a, g; // dummy variables
  Vector<Real, 2> logProba; // first element: current log proba, second element: logProba of permuted state
  Vector<Real, 2> proba   ; // multinomial distribution obtained from the logProba

  logProba(0) = lnCompletedProbability(mu, pi, a, g); // proba of current y

  for (int p = 0; p < nbPos_ - 1; ++p)
  {
    permutationY(p);
    logProba(1) = lnCompletedProbability(mu, pi, a, g);
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
      permutationY(p); // revert to previous state
    }
  }
}

void RankIndividual::permutationY(int firstElem)
{
  int dummy = y_(firstElem);
  y_(firstElem    ) = y_(firstElem + 1);
  y_(firstElem + 1) = dummy;
}

void RankIndividual::probaYgX(const RankVal& mu,
                              Real pi,
                              Vector<Vector<int> >& resVec,
                              Vector<Real>& resProba)
{
  int nbInd = fac(nbPos_);

  std::set<int> remainingMod;
  for (int m = 0; m < nbPos_; ++m)
  {
    remainingMod.insert(m);
  }

  Vector<int> vec(nbPos_);
  Vector<Real> logProba(nbInd);

  recYgX(mu,
         pi,
         resVec,
         logProba,
         vec,
         remainingMod,
         0,
         nbInd,
         0,
         nbPos_);

  resProba.logToMulti(logProba); // from log of joint distribution to conditional distribution
}

void RankIndividual::recYgX(const RankVal& mu,
                            Real pi,
                            Vector<Vector<int> >& resVec,
                            Vector<Real>& resProba,
                            Vector<int>& vec,
                            const std::set<int>& remainingMod,
                            int firstElem,
                            int nbElem,
                            int currPos,
                            int nbPos)
{
  int a, g; // dummy variables
  if (currPos == nbPos) // no more modalities to add in the vector
  {
    y_ = vec; // assignment to compute
    resVec(firstElem) = vec;
    resProba(firstElem) = lnCompletedProbability(mu, pi, a, g); // register current vector and its value
  }
  else
  {
    int nextNbElem = nbElem / (nbPos - currPos);
    int indexMod = 0;
    for (std::set<int>::const_iterator it = remainingMod.begin();
         it != remainingMod.end();
         ++it, ++indexMod) // iteration over the modality that have not yet been included in the vector
    {
      std::set<int> remainingModNew = remainingMod; // computation of the modalities available for the remaining of vec
      for (std::set<int>::iterator itNew = remainingModNew.begin();
           itNew != remainingModNew.end();
           ++itNew)
      {
        if (*itNew == *it) // the current modality will not be available further down the vector
        {
          remainingModNew.erase(itNew);
          break;
        }
      }

      vec(currPos) = *it;
      recYgX(mu,
             pi,
             resVec,
             resProba,
             vec,
             remainingModNew,
             firstElem + indexMod * nextNbElem,
             nextNbElem,
             currPos + 1,
             nbPos);
    }
  }
}

void RankIndividual::observedProba(const RankVal& mu,
                                   Real pi,
                                   std::map<RankVal, Real>& proba) {
  proba.clear();
  for (int i = 0; i < nbSampleObserved; ++i) {
    yGen();
    xGen(mu, pi);
    proba[x_] += 1.;

#ifdef MC_DEBUG
    std::cout << "RankIndividual::observedProba, i: " << i << ", x_: " << x_ << std::endl;
#endif
  }

  for (std::map<RankVal, Real>::iterator it = proba.begin(), itEnd = proba.end();
       it != itEnd;
       ++it) {
    it->second /= Real(nbSampleObserved);
  }
}

bool RankIndividual::checkMissingType(const Vector<bool>& acceptedType) const
{
  for (int p = 0; p < nbPos_; ++p)
  {
    if (acceptedType(obsData_(p).first) == false)
    {
      return false;
    }
  }

  return true;
}

std::list<int> RankIndividual::candidateList(int currPos,
                                             const std::set<int>& remainingMod) const {
  std::list<int> candidateList;

  if (obsData_(currPos).first == present_) { // if data is present
    if (remainingMod.find(x_.o()(currPos)) != remainingMod.end()) { // only add the observed value if it is still available, to avoid duplication of value in completed individual
#ifdef MC_DEBUG
      std::cout << "RankIndividual::candidateList, present_, x_.o(): " << itString(x_.o()) << std::endl;
#endif

      candidateList.push_back(x_.o()(currPos));
    }
  }
  else if (obsData_(currPos).first == missing_) { // since value is unobserved, all candidates can be considered
    for (std::set<int>::const_iterator it = remainingMod.begin(), itE = remainingMod.end();
         it != itE;
         ++it) {
      candidateList.push_back(*it);
    }
  }
  else if (obsData_(currPos).first == missingFiniteValues_) { // only add the intersection between what is possible and what is available
#ifdef MC_DEBUG
    std::cout << "RankIndividual::candidateList, missingFiniteValues_, obsData_(currPos).second: " << itString(obsData_(currPos).second) << std::endl;
#endif

    std::set_intersection(remainingMod.begin(), // compute the intersection between the remainingMod and the currently possible missing value
                          remainingMod.end(),
                          obsData_(currPos).second.begin(),
                          obsData_(currPos).second.end(),
                          std::back_inserter(candidateList));
  }

  return candidateList;
}

std::list<RankVal> RankIndividual::recEnumComplete(int currPos,
                                                   const std::set<int>& remainingMod,
                                                   const Vector<int>& completedVec) const {
#ifdef MC_DEBUG
  std::cout << "RankIndividual::recEnumComplete, currPos: " << currPos << ", "
            << "remainingMod: " << itString(remainingMod) << ", "
            << "completedVec: " << itString(completedVec) << std::endl;
#endif

  std::list<RankVal> rankList;

  if (currPos != nbPos_) { // recursive calls remain
    std::list<int> cl = candidateList(currPos, // compute the list of possible modalities for the current position, considering the observation
                                      remainingMod);

#ifdef MC_DEBUG
    std::cout << "cl.size(): " << cl.size() << std::endl;
#endif

    for (std::list<int>::const_iterator it = cl.begin(), itE = cl.end();
         it != itE;
         ++it) {
#ifdef MC_DEBUG
      std::cout << "cl, *it: " << *it << std::endl;
#endif

      std::set<int> subRemainingMod = remainingMod;
      subRemainingMod.erase(*it);
      Vector<int> subCompletedVec = completedVec;
      subCompletedVec(currPos) = *it;

      rankList.merge(recEnumComplete(currPos + 1,
                                     subRemainingMod,
                                     subCompletedVec));
    }
  }
  else { // termination condition of the recursion
#ifdef MC_DEBUG
    std::cout << "RankIndividual::recEnumComplete, termination" << std::endl;
#endif

    RankVal rv(nbPos_);
    rv.setO(completedVec);
    rankList.push_back(rv);
  }

  return rankList;
}

std::list<RankVal> RankIndividual::enumCompleted() const {
  std::list<RankVal> rankList;
  std::set<int> remainingMod;
  Vector<int> completedVec(nbPos_, -1);

  for (int i = 0; i < nbPos_; ++i) {
    remainingMod.insert(i);
  }

  rankList = recEnumComplete(0, // currPos
                             remainingMod, // remaining mod
                             completedVec); // completedVec

  return rankList;
}

bool RankIndividual::allMissing() const {
  for (int p = 0; p < nbPos_; ++p) {
    if (obsData_(p).first != missing_) {
      return false;
    }
  }

  return true;
}

} // namespace mixt
