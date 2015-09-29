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

#ifndef MIXT_RANKINDIVIDUAL_H
#define MIXT_RANKINDIVIDUAL_H

#include <map>
#include <set>

#include "../Various/mixt_Def.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Statistic/mixt_Statistic.h"
#include "mixt_RankVal.h"

namespace mixt
{

/**
 * A rank is an object that contains everything needed to describe a particular individual. It contains both the observed and completed
 * values, as well as methods to compute the likelihood or to perform samplings.
 */
class RankIndividual
{
  public:
    /* Each element of the vector is a position in a rank. The MisType describe the type of observation (or lack of)
     * while the Vector<int> describes the possible values, when needed. */
    typedef std::pair<MisType, std::vector<int> > MisVal;

    RankIndividual();

    RankIndividual(int nbPos);

    RankIndividual(const RankVal& rv);

    RankIndividual(const RankIndividual& ri);

    RankIndividual& operator=(const RankIndividual& ri);

    /** Get the observed x value, for example for debugging purposes */
    const RankVal& x() const {return x_;}

    /** Get the presentation order, for example for debugging purposes */
    const Vector<int>& y() const {return y_;}

    /** Set the number of positions in the rank, used to resize storage */
    void setNbPos(int nbPos);

    template<typename T>
    void setO(const T& data){x_.setO(data);}

    template<typename T>
    void setR(const T& data){x_.setR(data);}

    void setObsData(const Vector<MisVal>& v) {obsData_ = v;}

    const Vector<MisVal>& obsData() {return obsData_;}

    /**
     * Perform one round of Gibbs sampling for the partially observed data
     * @param mu central rank
     * @param pi precision */
    void sampleX(const RankVal& mu,
                 Real pi);

    /**
     * Perform one round of Gibbs sampling for the presentation order
     * @param mu central rank
     * @param pi precision */
    void sampleY(const RankVal& mu,
                 Real pi);

    /**
     * Completed log-probability of the individual
     * @param mu central rank
     * @param pi precision */
    Real lnCompletedProbability(const RankVal& mu,
                                Real pi,
                                int& a,
                                int& g) const;

    /** Uniform sample for partially observed valued and presentation order */
    void removeMissing();

    /** Shuffle the presentation order y_*/
    void yGen();

    /**
     * Sample an individual from parameters, conditionaly to the current presentation order y_
     * @param mu central rank
     * @param pi precision
     * @return log-probability of the sampled value
     * */
    Real xGen(const RankVal& mu,
              Real pi);

    void AG(const RankVal& mu,
            int& a,
            int& g) const;

    void probaYgX(const RankVal& mu,
                  Real pi,
                  Vector<Vector<int> >& resVec,
                  Vector<Real>& resProba);

    /** Compute the probability distribution of Y given X, to check the Gibbs sampling of Y given X.
     * The Y are stored in resVec, the associated probabilities in resProba. The function is recursive
     * to easily cover all possible cases and modifies the tail of the argument vec. */
    void recYgX(const RankVal& mu,
                Real pi,
                Vector<Vector<int> >& resVec,
                Vector<Real>& resProba,
                Vector<int>& vec,
                const std::set<int>& remainingMod,
                int firstElem,
                int nbElem,
                int currPos,
                int nbPos);

    /** Compute the observed probability distribution for mu and pi, by generating nbSampleObserved
     * Independent observations and marginalizing over presentation order. This procedure is similar
     * to what is used in the Ordinal model, and in contrast with the use of the harmonic mean estimator
     * of the observed probability. */
    void observedProba(const RankVal& mu,
                       Real pi,
                       std::map<RankVal, Real>& proba);

  private:
    /** Permute the elements firstElem and firstElem + 1 in y_ */
    void permutationY(int firstElem);

    /** Number of positions in the rank */
    int nbPos_;

    /** log(1 / n!) to normalize completed probability */
    Real lnFacNbPos_;

    /** Description of the observed data, one MisVal per position in the rank */
    Vector<MisVal> obsData_;

    /** Completed individual, position -> modality representation */
    RankVal x_;

    /** Presentation order */
    Vector<int> y_;

    /** Sampler for int */
    MultinomialStatistic multi_;
};

} // namespace mixt

#endif // MIXT_RANKINDIVIDUAL_H
