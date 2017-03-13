/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: August 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKCLASS_H
#define MIXT_RANKCLASS_H

#include <set>

#include "mixt_RankVal.h"
#include "mixt_RankIndividual.h"
#include "../mixt_IMixture.h"

namespace mixt
{

/** Class used to perform EM on a single variable with a single class. RankMixture contain an array of RankClass */
class RankClass
{
  public:
//    /** Default constructor */
//    RankClass(int nbClass);

    /** Constructor with data and parameters provided. useful for unit-testing. */
    RankClass(const Vector<RankIndividual>& data,
              const std::set<Index>& setInd,
              RankVal& mu,
              Real& pi);

    Real lnCompletedProbability() const;

    Real lnCompletedProbability(int& a, int& g) const;

    Real lnCompletedProbabilityInd(int i) const;

    Real lnObservedProbability(int i) const;

    /** Perform one round of Gibbs sampling for the central rank */
    void sampleMu();

    /** */
    void mStep(EstimatorType bias);

    void computeObservedProba();
  private:
    int nbInd_;

    /** Data */
    const Vector<RankIndividual>& data_;

    /** List of individuals among the data that belong to the class corresponding to this RankClass */
    const std::set<Index>& setInd_;

    /** Parameter mu */
    RankVal& mu_;

    /** Parameter pi */
    Real& pi_;

    MultinomialStatistic multi_;

    /** Vector of map containing observed probability distribution, used in independent sampling estimation.
     * Each element of the vector corresponds to a class */
    std::map<RankVal, Real> observedProbaSampling_;
};

} // namespace mixt

#endif // MIXT_RANKCLASS_H
