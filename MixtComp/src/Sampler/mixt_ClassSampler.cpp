/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Mar 13, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_ClassSampler.h"
#include "../Composer/mixt_MixtureComposer.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{

ClassSampler::ClassSampler(ZClassInd& zClassInd,
                           const Matrix<Real>& tik,
                           int nbClass) :
    nbClass_(nbClass),
    zClassInd_(zClassInd),
    tik_(tik)
{}

void ClassSampler::sStepNoCheck(int i) {
	if (zClassInd_.zi().misData_(i).first != present_) {
		int sampleVal = -1; // initialized with dummy value

		switch(zClassInd_.zi().misData_(i).first) {
		case missing_: {
			sampleVal = multi_.sample(tik_.row(i)); // the most simple case
		}
		break;

		case missingFiniteValues_: { // renormalize proba distribution on allowed sampling values
			Vector<Real> modalities(nbClass_, 0.);

			for(std::vector<Index>::const_iterator currMod = zClassInd_.zi().misData_(i).second.begin(),
					endMod  = zClassInd_.zi().misData_(i).second.end();
					currMod != endMod;
					++currMod) {
				modalities(*currMod) = tik_(i, *currMod);
			}
			modalities = modalities / modalities.sum();
			sampleVal = multi_.sample(modalities);
		}
		break;

		default: {}
		break;
		}
		zClassInd_.setZAndClassInd(i, sampleVal);
	}
}
} // namespace mixt
