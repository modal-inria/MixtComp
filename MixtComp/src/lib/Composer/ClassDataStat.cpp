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
 *  Created on: May 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "ClassDataStat.h"

#include <Various/Constants.h>

namespace mixt {

ClassDataStat::ClassDataStat(ZClassInd& zClassInd) :
		zClassInd_(zClassInd) {
}

void ClassDataStat::setNbIndividual(int nbInd) {
	dataStatStorage_.resize(nbInd, zClassInd_.zi().dataRange_.max_ + 1);
	dataStatStorage_ = 0.;
}

void ClassDataStat::sample(int ind) {
	int currMod = zClassInd_.zi().data_(ind);
	dataStatStorage_(ind, currMod) += 1.;
}

void ClassDataStat::sampleVals(int ind, int iteration, int iterationMax) {
	if (zClassInd_.zi().misData_(ind).first != present_) {
		if (iteration == 0) {
			dataStatStorage_.row(ind) = 0.; // initialize storage for individual

			sample(ind); // first sampling
		} else if (iteration == iterationMax) { // export the statistics to dataStatStorage
			sample(ind); // last sampling

			dataStatStorage_.row(ind) /= Real(iterationMax + 1); // from count to frequencies
		} else { // any other iteration: just store the current value
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
