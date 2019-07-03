/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: August 20, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RANKINDIVIDUAL_H
#define RANKINDIVIDUAL_H

#include <Mixture/Rank/RankVal.h>
#include <list>
#include <map>
#include <set>

#include "../../LinAlg/LinAlg.h"
#include "../../Various/Enum.h"
#include "../../Statistic/Statistic.h"

namespace mixt {

/**
 * A RankIndividual is an object that contains everything needed to describe a particular individual. It contains both the observed and completed
 * values, as well as methods to compute the likelihood or to perform samplings. In contrast, the RankVal is the much more simplest representation
 * of a rank, ignoring all probabilistic notions.
 */
class RankIndividual {
public:

	typedef std::pair<MisType, std::vector<int> > MisVal;

	RankIndividual();

	RankIndividual(int nbPos);

	RankIndividual(const RankVal& rv);

	RankIndividual(const RankIndividual& ri);

	RankIndividual& operator=(const RankIndividual& ri);

	int nbPos() const {
		return nbPos_;
	}

	/** Get the observed x value, for example for debugging purposes */
	const RankVal& x() const {
		return x_;
	}

	/** Get the observed x value, for example for debugging purposes */
	RankVal& xModif() {
		return x_;
	}

	/** Get the presentation order, for example for debugging purposes */
	const Vector<int>& y() const {
		return y_;
	}

	/** Set the number of positions in the rank, used to resize storage */
	void setNbPos(int nbPos);

	template<typename T>
	void setO(const T& data) {
		x_.setO(data);
	}

	template<typename T>
	void setR(const T& data) {
		x_.setR(data);
	}

	void setObsData(const Vector<MisVal>& v);

	const Vector<MisVal>& obsData() {
		return obsData_;
	}

	/**
	 * Perform one round of Gibbs sampling for the partially observed data
	 * @param mu central rank
	 * @param pi precision */
	void sampleX(const RankVal& mu, Real pi);

	/**
	 * Perform one round of Gibbs sampling for the presentation order
	 * @param mu central rank
	 * @param pi precision */
	void sampleY(const RankVal& mu, Real pi);

	/**
	 * Completed log-probability of the individual
	 * @param mu central rank
	 * @param pi precision */
	Real lnCompletedProbability(const RankVal& mu, Real pi, int& a,
			int& g) const;

	/** Uniform sample for partially observed valued and presentation order */
	void removeMissing();

	/** Shuffle the presentation order y_*/
	void yGen();

	/**
	 * Sample an individual from parameters, conditionally to the current presentation order y_
	 * @param mu central rank
	 * @param pi precision
	 * @return log-probability of the sampled value
	 * */
	Real xGen(const RankVal& mu, Real pi);

	void AG(const RankVal& mu, int& a, int& g) const;

	void probaYgX(const RankVal& mu, Real pi, Vector<Vector<int> >& resVec,
			Vector<Real>& resProba);

	/** Compute the probability distribution of Y given X, to check the Gibbs sampling of Y given X.
	 * The Y are stored in resVec, the associated probabilities in resProba. The function is recursive
	 * to easily cover all possible cases and modifies the tail of the argument vec. */
	void recYgX(const RankVal& mu, Real pi, Vector<Vector<int> >& resVec,
			Vector<Real>& resProba, Vector<int>& vec,
			const std::set<int>& remainingMod, int firstElem, int nbElem,
			int currPos, int nbPos);

	/** Compute the observed probability distribution for mu and pi, by generating nbSampleObserved
	 * independent observations and marginalizing over presentation order. This procedure is similar
	 * to what is used in the Ordinal model, and in contrast with the use of the harmonic mean estimator
	 * of the observed probability. */
	void observedProba(const RankVal& mu, Real pi,
			std::map<RankVal, Real>& proba);

	bool checkMissingType(const Vector<bool>& acceptedType) const;

	/** Provide a list of possible values for a position considering the observed value and the set of
	 * possible values remaining. */
	std::list<int> candidateList(int currPos,
			const std::set<int>& remainingMod) const;

	/** Recursive function called by enumAllCompleted.
	 *
	 * @param currPos current position in the rank
	 * @param remainingMod set of modalities that have not yet been seen in the rank
	 * @param completedVec the completed rank from which recEnumComplete was called (complement remainingMod)
	 * @return The list of all the possible RankVal enumerated in sub calls */
	std::list<RankVal> recEnumComplete(int currPos,
			const std::set<int>& remainingMod,
			const Vector<int>& completedVec) const;

	/** Use the obsData_ information to compute all the possible completions for the individual. If there is no
	 * partially observed data, the method still is useful at providing a check of the observed individual. For
	 * a valid individual, the returned list must have at least one element, which is a copy of x_, otherwise,
	 * data is invalid, for example in the case 1,1,3. */
	std::list<RankVal> enumCompleted() const;

	/** Is the individual completely observed ? This is used to determine if statistics for a partially observed
	 * individual have to be computed / exported */
	bool allPresent() const {
		return allPresent_;
	}

	/** Is the individual completely missing ? In that case its observed probability is 1. for example, whatever
	 * the parameters.  */
	bool allMissing() const {
		return allMissing_;
	}

	/** Check if the modalities in positions pos and pos + 1 can be permuted */
	bool checkPermutation(int pos) const;
private:
	/** Permute the elements firstElem and firstElem + 1 in y_ */
	void permutationY(int firstElem);

	/** Is a value authorized for a particular MisVal describing */
	bool isAuthorized(int value, const MisVal& misval) const;

	/** Number of positions in the rank */
	int nbPos_;

	/** log(1 / n!) to normalize completed probability */
	Real lnFacNbPos_;

	/** Description of the observed data, one MisVal per position in the rank. Should
	 * this be implemented in "ordering" presentation ? */
	Vector<MisVal> obsData_;

	/** Completed individual, position -> modality representation */
	RankVal x_;

	/** Presentation order */
	Vector<int> y_;

	/** Sampler for int */
	MultinomialStatistic multi_;

	bool allPresent_;

	bool allMissing_;
};

} // namespace mixt

#endif // RANKINDIVIDUAL_H
