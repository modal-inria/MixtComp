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
 *  Created on: December 12, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include <Mixture/IMixture.h>
#include "WeibullSampler.h"

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

		  case missingIntervals_: {
		    x = weibull_.sampleI(k, lambda, augData_.misData_(i).second[0], augData_.misData_(i).second[1]);
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
