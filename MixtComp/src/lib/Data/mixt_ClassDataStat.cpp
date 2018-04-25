/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: May 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_ClassDataStat.h"

#include "../Various/mixt_Constants.h"

namespace mixt {

ClassDataStat::ClassDataStat(ZClassInd& zClassInd) :
    zClassInd_(zClassInd) {}

void ClassDataStat::setNbIndividual(int nbInd) {
  dataStatStorage_.resize(nbInd,
                          zClassInd_.zi().dataRange_.max_ + 1);
}

void ClassDataStat::sample(int ind) {
  int currMod = zClassInd_.zi().data_(ind);
  dataStatStorage_(ind, currMod) += 1.;
}

void ClassDataStat::sampleVals(int ind,
                               int iteration,
                               int iterationMax) {
  if (zClassInd_.zi().misData_(ind).first != present_) {
    if (iteration == 0) {
      dataStatStorage_.row(ind) = 0.; // initialize storage for individual

      sample(ind); // first sampling
    }
    else if (iteration == iterationMax) { // export the statistics to dataStatStorage
      sample(ind); // last sampling

      dataStatStorage_.row(ind) /= Real(iterationMax + 1); // from count to frequencies
    }
    else { // any other iteration: just store the current value
      sample(ind);
    }
  }
}

void ClassDataStat::imputeData(int ind, Matrix<Real>& tik) {
  if (zClassInd_.zi().misData_(ind).first != present_) { // imputation by the mode
    int mode;
    tik.row(ind) = dataStatStorage_.row(ind); // completed tik from last sampling are replaced by observed tik
    dataStatStorage_.row(ind).maxCoeff(&mode);
    zClassInd_.setZAndClassInd(ind, mode);
  }
}

} // namespace mixt
