/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016

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
 *  Created on: February 24, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *
 **/

#ifndef MIXT_ZCLASSIND_H
#define MIXT_ZCLASSIND_H

#include <set>
#include "../Data/mixt_AugmentedData.h"

namespace mixt {

/** This class combines the information on z and on the list of observation per class. The two are correlated,
 * hence it is better that a single class be in charge of keeping those two aspects in sync. */
class ZClassInd {
  public:
    /** Ask the zi_ member to compute its range */
    void computeRange();

    /** Set the range of the data, useful in prediction. */
    void setRange(int min, int max, int range);

    void setAllMissing();


    /** The DataHandler initializes zi_, and classInd_ is updated. */
    template<typename DataHandler>
    void setZi(const DataHandler& dataHandler);

    /** The class of a particular individual is modified */
    void setZAndClassInd(int i, int k);

    const AugmentedData<Vector<int> >& zi() {return zi_;}
    const Vector<std::set<int> >& classInd() {return classInd_;}

  private:
    /** The zik class label */
    AugmentedData<Vector<int> > zi_;

    /** A vector containing in each element a vector of the indices of individuals that
     * belong to this class. Can be passed as an alternative to zi_ to a subtype of IMixture. */
    Vector<std::set<int> > classInd_;
};

} /* namespace mixt */

#endif /* MIXT_ZCLASSIND_H */