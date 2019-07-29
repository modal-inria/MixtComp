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
 *  Created on: July 19, 2018
 *  Authors:    Quentin GRIMONPREZ <quentin.grimonprez@inria.fr>
 **/

#ifndef NEGATIVEBINOMIALLIKELIHOOD_H
#define NEGATIVEBINOMIALLIKELIHOOD_H

#include <LinAlg/LinAlg.h>
#include "../../../Data/AugmentedData.h"
#include "NegativeBinomialStatistic.h"

namespace mixt
{

class NegativeBinomialLikelihood
{
  public:
    /** Constructor */
	NegativeBinomialLikelihood(const Vector<Real>& param,
                      const AugmentedData<Vector<int> >& augData,
                      int nbClass);

    /** Compute the completed log-likelihood */
    Real lnCompletedProbability(int i, int k) const;

    /** Compute the observed log-likelihood */
    Real lnObservedProbability(int i, int k) const;

  private:
    /** Pointer to parameters table */
    const Vector<Real>& param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<int> >& augData_;

    /** Statistic object to describe Negative Binomial law */
    NegativeBinomialStatistic negativeBinomial_;
};

} /* namespace mixt */

#endif // NEGATIVEBINOMIALLIKELIHOOD_H
