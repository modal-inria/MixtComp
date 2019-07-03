/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: August 28, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RANKCLASS_H
#define RANKCLASS_H

#include <Mixture/IMixture.h>
#include <Mixture/Rank/RankIndividual.h>
#include <Mixture/Rank/RankVal.h>
#include <set>


namespace mixt {

/** Class used to perform EM on a single variable with a single class. RankMixture contain an array of RankClass */
class RankClass {
public:
	typedef std::pair<MisType, std::vector<int> > MisVal;
//    /** Default constructor */
//    RankClass(int nbClass);

	/** Constructor with data and parameters provided. useful for unit-testing. */
	RankClass(const Vector<RankIndividual>& data, RankVal& mu, Real& pi);

	Real lnCompletedProbability(const std::set<Index>& setInd) const;

	Real lnCompletedProbability(const std::set<Index>& setInd, int& a, int& g) const;

	Real lnCompletedProbabilityInd(int i) const;

	Real lnObservedProbability(int i) const;

	/** Perform one round of Gibbs sampling for the central rank */
	void sampleMu(const std::set<Index>& setInd);

	/** */
	void mStep(const std::set<Index>& setInd);

	void computeObservedProba();
private:
	int nbInd_;

	/** Data */
	const Vector<RankIndividual>& data_;

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

#endif // RANKCLASS_H
