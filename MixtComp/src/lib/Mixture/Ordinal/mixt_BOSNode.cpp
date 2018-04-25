/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: April 29, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_BOSNode.h"

namespace mixt {

void BOSNode::partition(const Vector<int, 2>& e) {
	if (y_ < e(0) || y_ > e(1)) { // if y is not in the interval, the partition is empty
		partSize_ = 0;
		part_.resize(partSize_);
	} else {
		partSize_ = 3; // maximum size of the partition
		int yPos; // position of the "breaking point" segment
		if (e(0) == y_) // is the left segment empty ?
				{
			partSize_ -= 1;
		}
		if (e(1) == y_) { // is the right segment empty ?
			partSize_ -= 1;
		}
		part_.resize(partSize_);

		if (e(0) != y_) { // is the left interval non-empty ? If not, partition element will be an empty vector
			part_(0)(0) = e(0);
			part_(0)(1) = y_ - 1;
			yPos = 1;
		} else
			yPos = 0;

		if (e(1) != y_) { // is the right interval non-empty ? If not, partition element will be an empty vector
			part_(partSize_ - 1)(0) = y_ + 1;
			part_(partSize_ - 1)(1) = e(1);
			yPos = partSize_ - 2;
		} else
			yPos = partSize_ - 1;

		part_(yPos)(0) = y_; // center interval always contains the center element
		part_(yPos)(1) = y_;
	}
}

bool BOSNode::isInPart(const Vector<int, 2>& e) const {
	bool isIn = false;
	for (int i = 0; i < partSize_; ++i) {
		if (e == part_(i))
			isIn = true;
	}
	return isIn;
}

Real BOSNode::yLogProba(const Vector<int, 2>& e) const {
	Real yProba;
	if (e(0) <= y_ && y_ <= e(1)) { // y is among the last segment values
		yProba = 1. / Real(e(1) - e(0) + 1.);
	} else {
		yProba = 0.;
	}

	return std::log(yProba); // conditional probability of y, which only depends of the size of the interval
}

Real BOSNode::zLogProba(Real pi) const {
	Real zProba;
	if (z_ == 1) { // comparison is perfect
		zProba = pi;
	} else { // comparison is blind
		zProba = (1. - pi);
	}
	return std::log(zProba);
}

Real BOSNode::eLogProba(int mu, Real pi) const {
	Real eProba;

	if (z_ == 1) { // comparison is perfect, and only the best segment has a nonzero probability
		int closestSegment = -1; // index in partition of the closest segment
		Real disClosestSegment; // distance between mu and closest segment
		for (int s = 0; s < partSize_; ++s) { // loop on all segments of the partition
			Real disCurrSegment = std::min(std::abs(mu - part_(s)(0)), // distance of current segment to the mode
			std::abs(mu - part_(s)(1)));
			if (s == 0 || disCurrSegment < disClosestSegment) // for the first segment, or if a new closest segment has been detected
					{
				closestSegment = s;
				disClosestSegment = disCurrSegment;
			}
		}

		if (closestSegment > -1 && e_ == part_(closestSegment)) { // a closest segment exists, and e points to it
			eProba = 1.;
		} else {
			eProba = 0.;
		}
	} else { // comparison is blind, and proba is based on sizes of segments
		int sizePart = 0; // total size of the partition
		eProba = 0.; // by default the segment is assumed absent from the partition, and hence having a null probability
		for (int s = 0; s < partSize_; ++s) // loop on all segments of the partition
				{
			sizePart += part_(s)(1) - part_(s)(0) + 1;
			if (e_ == part_(s)) { // computation of probability for current e_

				eProba = Real(part_(s)(1) - part_(s)(0) + 1);
			}
		}
		if (sizePart > 0) { // the case sizePart = 0 means that y was not in the segment e of the previous iteration. Case is possible during a Gibbs sampling.
			eProba /= Real(sizePart);
		} else {
			eProba = 0.;
		}
	}

	return std::log(eProba);
}

} // namespace mixt
