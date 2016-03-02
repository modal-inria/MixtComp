/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: July 1, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_CategoricalDataStat.h"
#include "../Various/mixt_Constants.h"

namespace mixt {

CategoricalDataStat::CategoricalDataStat(AugmentedData<Vector<int> >& augData,
                                         Real confidenceLevel) :
    augData_(augData),
    confidenceLevel_(confidenceLevel) {}

void CategoricalDataStat::setNbIndividual(int nbInd) {
  stat_.resize(nbInd);
  dataStatStorage_.resize(nbInd);
}

void CategoricalDataStat::sample(int ind) {
  int currMod = augData_.data_(ind);
  stat_(ind)(currMod) += 1.;
}

void CategoricalDataStat::sampleVals(int ind,
                                     int iteration,
                                     int iterationMax) {
  if (augData_.misData_(ind).first != present_) {
    if (iteration == 0) { // clear the temporary statistical object
      stat_(ind).resize(augData_.dataRange_.max_ + 1); // initialize internal storage
      stat_(ind) = 0.;

      dataStatStorage_(ind) = std::vector<std::pair<int, Real> >(); // clear output storage for current individual, a vector of <modality, proba>, ordered by decreasing probability up to a cut-off defined by confidenceLevel

      sample(ind); // first sampling, on each missing variables
    }
    else if (iteration == iterationMax) { // export the statistics to the p_dataStatStorage object
      sample(ind); // last sampling

      stat_(ind) /= Real(iterationMax + 1); // from count to probabilities
      Vector<int> indOrder; // to store indices of ascending order
      stat_(ind).sortIndex(indOrder);
      Real cumProb = 0.;

      for (int i = augData_.dataRange_.max_; // from the most probable modality ...
           i > -1; // ... to the least probable modality
           --i) {
        int currMod = indOrder(i);
        Real currProba = stat_(ind)(currMod);
        dataStatStorage_(ind).push_back(std::pair<int, Real>(currMod, currProba));
        cumProb += currProba;

        if (cumProb > confidenceLevel_) {
          break;
        }
      }

      stat_(ind).resize(0); // clear memory for current individual
    }
    else { // any other iteration: just store the current value
      sample(ind);
    }
  }
}

void CategoricalDataStat::imputeData(int ind) {
  if (augData_.misData_(ind).first != present_) {
    augData_.data_(ind) = dataStatStorage_(ind)[0].first; // imputation by the mode
  }
}

} // namespace mixt
