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
 **/

#include "mixt_ZClassInd.h"

namespace mixt {

void ZClassInd::setNbInd(int nbInd) {
  nbInd_ = nbInd;
  zi_.resizeArrays(nbInd);
}

void ZClassInd::computeRange() {
  zi_.computeRange();
}

void ZClassInd::setRange(int min, int max, int range) {
  zi_.dataRange_.min_   = min; // real range provided by the parameters is enforced
  zi_.dataRange_.max_   = max;
  zi_.dataRange_.range_ = range;
}

void ZClassInd::setAllMissing() {
  zi_.setAllMissing(nbInd_);
}

void ZClassInd::setZAndClassInd(int i, int k) {
  classInd_(zi_.data_(i)).erase(i);
  zi_.data_(i) = k;
  classInd_(zi_.data_(i)).insert(i);
}

} /* namespace mixt */
