/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "WeibullSampler.h"
#include "Mixture/mixt_IMixture.h"

namespace mixt {

WeibullSampler::WeibullSampler(AugmentedData<Vector<Real> >& augData, const Vector<Real>& param, Index nbClass) :
		augData_(augData),
		param_(param)
{}

void WeibullSampler::samplingStepNoCheck(Index i, Index z_i) {
	if (augData_.misData_(i).first != present_) {
		Real x = 0;
	    Real k      = param_(2 * z_i    );
	    Real lambda = param_(2 * z_i + 1);

		switch(augData_.misData_(i).first) {
		  case missing_: {
		    x = weibull_.sample(k, lambda);
		  }
		  break;

		  case missingRUIntervals_: {
		    x = weibull_.sampleIB(k, lambda, augData_.misData_(i).second[0]);
		  }
		  break;

		  default: {
			  throw("WeibullSampler::samplingStepNoCheck, error in missing data handling, please report to the maintainer.");
		  }
		  break;
		}

		augData_.data_(i) = x;
	}
}

} // namespace mixt
