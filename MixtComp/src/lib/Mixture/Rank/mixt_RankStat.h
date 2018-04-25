/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: September 3, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKPARAMSTAT_H
#define MIXT_RANKPARAMSTAT_H

#include <list>

#include "../../LinAlg/mixt_LinAlg.h"
#include "mixt_RankVal.h"

namespace mixt
{

/** Compute statistics for a single class. Use a vector of those for a mixture. */
class RankStat
{
  public:
    RankStat(RankVal& mu,
             Real confidenceLevel);

    /** There used to be a tuple here, dedicated to Quentin Grimonprez, proud forefather of its usage among us, mere Modal Team member.
     * But it was deemed unnecessary leading to its demise and ultimate obliteration with extreme prejudice.
     * Of the tuple. Not of Quentin. */
    const std::list<std::pair<RankVal, Real> >& statStorageMu() const {return statStorageMu_;}

    const Vector<RankVal>& logStorageMu() const {return logStorageMu_;}

    void sampleValue(int iteration,
                     int iterationMax);

    /** Set mu and pi with the estimated value, at the end of the SEM. The estimated value of mu is the mode, and the
     * estimated value of pi is the mode among the values of pi sampled for the estimated mu. */
    void setExpectationParam();

    void setParamStorage();

    int nbPos() const {return mu_.nbPos();}
  private:
    void sample(int iteration);

    RankVal& mu_;

    Real confidenceLevel_;

    /** Each element in the list contains a triplet <mu, frequency of mu, Vector (median, left quantile, right quantile) of pi>.
     * The confidenceLevel parameter act as a cutoff both for mu and pi. */
    std::list<std::pair<RankVal, Real> > statStorageMu_;

    /** Storage for iterations results, 1 value per iteration */
    Vector<RankVal> logStorageMu_;
};

} // namespace mixt

#endif // MIXT_RANKPARAMSTAT_H
