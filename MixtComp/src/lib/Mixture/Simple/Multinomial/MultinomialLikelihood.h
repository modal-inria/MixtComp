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
 *  Created on: July 28, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MULTINOMIALLIKELIHOOD_H
#define MULTINOMIALLIKELIHOOD_H

#include "../../../Data/AugmentedData.h"
#include "../../../LinAlg/LinAlg.h"

namespace mixt {

template<typename Type>
class MultinomialLikelihood {
  public:
    /** Constructor */
    MultinomialLikelihood(const Vector<Real>& param,
                          const AugmentedData<Vector<Type> >& augData,
                          Index nbClass) :
        nbClass_(nbClass),
        param_(param),
        augData_(augData){}

    /** Compute the completed log probability of individual i */
    Real lnCompletedProbability(Index i, Index k) const {
      Index nbModalities = param_.rows() / nbClass_;
      Index ind = k * nbModalities + augData_.data_(i);

      Real proba = param_(ind);

      return std::log(proba);
    }

    /** Compute the observed log probability of individual i */
    Real lnObservedProbability(Index i, Index k) const {
      Index nbModalities = param_.rows() / nbClass_;
      Real proba = 0.;

      switch (augData_.misData_(i).first) {
        case present_: { // likelihood for present data
          proba = param_(k * nbModalities + augData_.data_(i));
        }
        break;

        case missing_: { // no contribution to the observed likelihood
          proba = 1.;
        }
        break;

        case missingFiniteValues_: { // adding the contributions of the various modalities
          proba = 0.;

          for (typename std::vector<Type>::const_iterator itMiss = augData_.misData_(i).second.begin();
               itMiss != augData_.misData_(i).second.end();
               ++itMiss) {
            proba += param_(k * nbModalities + *itMiss);
          }
        }
        break;

        default: {
        	throw("MultinomialLikelihood::lnObservedProbability, error in missing data handling, please report to the maintainer.");
        }
        break;
      }
      return std::log(proba);
    }

  private:
    Index nbClass_;

    /** Pointer to parameters table */
    const Vector<Real>& param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<Vector<Type> >& augData_;
};

} /* namespace mixt */

#endif /* MULTINOMIALLIKELIHOOD_H */
