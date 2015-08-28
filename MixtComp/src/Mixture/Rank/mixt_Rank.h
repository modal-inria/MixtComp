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

#ifndef MIXT_RANK_H
#define MIXT_RANK_H

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
class Rank
{
  public:
    /* Each element of the vector is a position in a rank. The MisType describe the type of observation (or lack of)
     * while the Vector<int> describes the possible values, when needed. */
    typedef std::pair<MisType, Vector<int> > MisVal;

    Rank();

    Rank(int nbPos);

    /** Get the observed x value, for example for debugging purposes */
    const RankVal& getX() const {return x_;}

    /** Get the presentation order, for example for debugging purposes */
    const Vector<int>& getY() const {return y_;}

    /** Set the number of positions in the rank, used to resize storage */
    void setNbPos(int nbPos);

    template<typename T>
    void setO(const T& data){x_.setO(data);}

    template<typename T>
    void setR(const T& data){x_.setR(data);}

    /**
     * Perform one round of Gibbs sampling for the partially observed data
     * @param mu central rank
     * @param pi precision */
    void samplingX(const RankVal& mu,
                   Real pi);

    /**
     * Perform one round of Gibbs sampling for the presentation order
     * @param mu central rank
     * @param pi precision */
    void samplingY(const RankVal& mu,
                   Real pi);

    /**
     * Completed log-probability of the individual
     * @param mu central rank
     * @param pi precision */
    Real lnCompletedProbability(const RankVal& mu,
                                Real pi) const;

    /** Uniform sample for partially observed valued and presentation order */
    void removeMissing();

    /**
     * Sample an individual from parameters, a presentation order
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

  private:
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

#endif // MIXT_RANK_H
