/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: April 29, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_BOSNODE
#define MIXT_BOSNODE

#include "../LinAlg/mixt_LinAlg.h"

namespace mixt {

/**
 * Structure containing the values of an iteration of the BOS algorithm.
 * A node contains information on:
 * - breaking point y
 * - blindness of comparison z
 * - final segment e
 */
class BOSNode {
public:
	int y_; // breaking point
	Vector<Vector<int, 2> > part_; // partition is uniquely defined by e_ from previous iteration and by y_
	int partSize_; // number of elements in the partition
	int z_; // blindness of comparison
	Vector<int, 2> e_; // final segment for current iteration, as an index of the partition

	/**
	 * Compute the probability of a given segment conditionally to the partition and y of the node
	 *
	 * @param mu localization parameter (mode) of the distribution
	 * @param pi precision parameter of the distribution
	 * @param seg external segment
	 */
	void partition(const Vector<int, 2>& e);

	/**
	 * Check if a segment is a member of the partition
	 *
	 * @param e segment which presence should be checked
	 */
	bool isInPart(const Vector<int, 2>& e) const;

	Real yLogProba(const Vector<int, 2>& e) const;
	Real zLogProba(Real pi) const;

	/**
	 * Compute the probability of the current segment conditionally to the partition and y of the node
	 *
	 * @param mu localization parameter (mode) of the distribution
	 * @param pi precision parameter of the distribution
	 */
	Real eLogProba(int mu, Real pi) const;
};

} // namespace mixt

#endif // MIXT_BOSNODE
