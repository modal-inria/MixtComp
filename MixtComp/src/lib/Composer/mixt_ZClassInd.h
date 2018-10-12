/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: February 24, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *
 **/

#ifndef LIB_COMPOSER_MIXT_ZCLASSIND_H
#define LIB_COMPOSER_MIXT_ZCLASSIND_H

#include <set>
#include <regex>
#include <Data/mixt_AugmentedData.h>
#include <IO/IOFunctions.h>

namespace mixt {

/** This class combines the information on z and on the list of observation per class. The two are correlated,
 * hence it is better that a single class be in charge of keeping those two aspects in sync. */
class ZClassInd {
public:
	void setIndClass(Index nbInd, Index nbClass);

	/** Ask the zi_ member to compute its range */
	void computeRange();

	/** Set the range of the data, useful in prediction. */
	void setRange(int min, int max, int range);

	void setAllMissing();

	std::string checkMissingType();

	/** The DataHandler initializes zi_, and classInd_ is updated. */
	template<typename Graph>
	std::string setZi(Graph& g) {
		std::string warnLog;
		std::string paramStr;

		std::vector<std::string> data;
		g.get_payload({}, "z_class", data);

		warnLog += StringToAugmentedData("z_class", data, zi_, -minModality);

		for (int k = 0; k < nbClass_; ++k) {
			classInd_(k).clear();
		}

		for (int i = 0; i < nbInd_; ++i) {
			classInd_(zi_.data_(i)).insert(i);
		}

		return warnLog;
	}

	/** The class of a particular individual is modified */
	void setZAndClassInd(Index i, Index k);

	const AugmentedData<Vector<Index> >& zi() const {
		return zi_;
	}
	const Vector<std::set<Index> >& classInd() const {
		return classInd_;
	}

	void printState() const;

private:
	Index nbInd_;
	Index nbClass_;

	/** The zik class label */
	AugmentedData<Vector<Index> > zi_;

	/** A vector containing in each element a vector of the indices of individuals that
	 * belong to this class. Can be passed as an alternative to zi_ to a subtype of IMixture. */
	Vector<std::set<Index> > classInd_;
};

} /* namespace mixt */

#endif /* MIXT_ZCLASSIND_H */
