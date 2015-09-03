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
 *  Created on: September 3, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKPARAMSTAT_H
#define MIXT_RANKPARAMSTAT_H

#include <list>

#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_RankVal.h"

namespace mixt
{

/** Compute statistics for a single class. Use a vector of those for a mixture. */
class RankParamStat
{
  public:
    RankParamStat(RankVal& mu,
                  Real& pi,
                  Real confidenceLevel);

    /** This tuple is dedicated to Quentin Grimonprez, proud forefather of its usage among us, mere Modal Team member.
     * May he be blessed for that. */
    const std::list<std::tuple<RankVal,
                               Real,
                               Vector<Real,
                                      3> > >& getStatStorageMuPi() const {return statStorageMuPi_;}

    const Vector<RankVal>& getLogStorageMu() const {return logStorageMu_;}
    const Vector<Real>&    getLogStoragePi() const {return logStoragePi_;}

    void sampleParam(int iteration,
                     int iterationMax);

    /** Set mu and pi with the estimated value, at at the end of the SEM */
    void setExpectationParam();
  private:
    void sample(int iteration);

    RankVal& mu_;
    Real& pi_;

    Real confidenceLevel_;

    /** Each element in the list contains a triplet <mu, frequency of mu, Vector (median, left quantile, right quantile) of pi>.
     * The confidenceLevel parameter act as a cutoff both for mu and pi. */
    std::list<std::tuple<RankVal, Real, Vector<Real, 3> > > statStorageMuPi_;

    /** Storage for iterations results, 1 value per iteration */
    Vector<RankVal> logStorageMu_;

    /** Storage for iterations results, 1 value per iteration */
    Vector<Real> logStoragePi_;
};

} // namespace mixt

#endif // MIXT_RANKPARAMSTAT_H
