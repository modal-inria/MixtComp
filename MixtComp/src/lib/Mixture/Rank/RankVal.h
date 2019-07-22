/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 **/

/*
 *  Project:    MixtComp
 *  Created on: August 26, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef RANKVAL_H
#define RANKVAL_H

#include <LinAlg/LinAlg.h>
#include <iostream>

namespace mixt {

/**
 * Both ordering (position -> modality) and ranking (modality -> position) are used in the RankCluster
 * algorithm. RankVal stores and update both representations, allowing easy operations on rank values. It
 * could be used to store both completed values of observation or central rank (mu) in the parameters.
 */
class RankVal {
public:
	RankVal();

	RankVal(int nbPos);

	/** Set ordering via initializer list */
	RankVal(const std::initializer_list<int>& il);

	void operator=(const RankVal& rv);

	bool operator==(const RankVal& rv) const;

	bool operator<(const RankVal& rv) const;

	void setNbPos(int nbPos);

	int nbPos() const {
		return nbPos_;
	}

	/**
	 * Set ordering: position -> modality (the more intuitive way to represent a rank).
	 * Note that no check is performed on sizes to increase speed
	 * */
	template<typename T>
	void setO(const T& data) {
		typename T::const_iterator itD = data.begin();
		typename T::const_iterator itDE = data.end();
		Vector<int>::iterator itV = ordering_.begin();

		for (; itD != itDE; ++itD, ++itV) {
			*itV = *itD;
		}

		switchRepresentation(ordering_, ranking_);
	}

	/**
	 * Set ranking: modality -> position
	 * Note that no check is performed on sizes to increase speed
	 * */
	template<typename T>
	void setR(const T& data) {
		typename T::const_iterator itD = data.begin();
		typename T::const_iterator itDE = data.end();
		Vector<int>::iterator itV = ranking_.begin();

		for (; itD != itDE; ++itD, ++itV) {
			*itV = *itD;
		}

		switchRepresentation(ranking_, ordering_);
	}

	void switchRepresentation(const Vector<int>& mu, Vector<int>& muP) const;

	/**
	 * Permutation of contiguous elements
	 * @param firstElem index of the first element in ordering representation */
	void permutation(int firstElem);

	const Vector<int>& o() const {
		return ordering_;
	}

	const Vector<int>& r() const {
		return ranking_;
	}

	friend std::ostream& operator<<(std::ostream& os, const RankVal& rv);

	std::string str() const;
private:
	/** */
	int nbPos_;

	/** ordering representation */
	Vector<int> ordering_;

	/** ranking representation */
	Vector<int> ranking_;
};

std::ostream& operator<<(std::ostream& os, const RankVal& rv);

} // namespace mixt

#endif // RANKVAL_H
