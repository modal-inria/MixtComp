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
 *  Created on: February the 1st, 2018
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef WEIBULLLIKELIHOOD_H
#define WEIBULLLIKELIHOOD_H

#include <Data/AugmentedData.h>
#include <LinAlg/LinAlg.h>
#include "WeibullStatistic.h"

namespace mixt {

class WeibullLikelihood {
  public:
    /** Constructor */
    WeibullLikelihood(const Vector<Real>& param, const AugmentedData<Vector<Real> >& augData, Index nbClass);

    /** Compute the completed log-likelihood */
    Real lnCompletedProbability(Index i, Index k) const;

    /** Compute the observed log-likelihood */
    Real lnObservedProbability(Index i, Index k) const;

  private:

    /** Pointer to parameters table */
    const Vector<Real>& param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<Real> >& augData_;

    WeibullStatistic weibull_;
};

} /* namespace mixt */

#endif /* WEIBULLLIKELIHOOD_H */
