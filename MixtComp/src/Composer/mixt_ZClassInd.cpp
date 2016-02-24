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

void ZClassInd::setIndClass(int nbInd, int nbClass) {
  nbInd_ = nbInd;
  nbClass_ = nbClass;
  zi_.resizeArrays(nbInd);
  classInd_.resize(nbClass);
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

std::string ZClassInd::checkMissingType() {
  Vector<bool> at(nb_enum_MisType_); // authorized missing values, should mimic what is found in categorical mixtures
  at(0) = true; // present_,
  at(1) = true;// missing_,
  at(2) = true;// missingFiniteValues_,
  at(3) = false;// missingIntervals_,
  at(4) = false;// missingLUIntervals_,
  at(5) = false;// missingRUIntervals_,

  return zi_.checkMissingType(at);
}

void ZClassInd::setZAndClassInd(int i, int k) {
  classInd_(zi_.data_(i)).erase(i);
  classInd_(k           ).insert(i);
  zi_.data_(i) = k;
}

void ZClassInd::printState() const {
  std::cout << "zi_: " << itString(zi_.data_) << std::endl;

  for (int k = 0; k < nbClass_; ++k) {
    std::cout << "k: " << k << ",";
    for (std::set<int>::const_iterator it    = classInd_(k).begin(),
                                       itEnd = classInd_(k).end();
         it != itEnd;
         ++it) {
      std::cout << " " << *it;
    }
    std::cout << std::endl;
  }
}

} /* namespace mixt */
