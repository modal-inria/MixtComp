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
 *  Created on: July 28, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CATEGORICALLIKELIHOOD_H
#define MIXT_CATEGORICALLIKELIHOOD_H

#include "../Data/mixt_AugmentedData.h"
#include "../LinAlg/mixt_LinAlg.h"

namespace mixt {

template<typename Type>
class CategoricalLikelihood {
  public:
    /** Constructor */
    CategoricalLikelihood(const Vector<Real>& param,
                          const AugmentedData<Vector<Type> >& augData,
                          Index nbClass) :
        nbClass_(nbClass),
        param_(param),
        augData_(augData){}

    /** Compute the completed log probability of individual i */
    Real lnCompletedProbability(Index i, Index k) {
      Index nbModalities = param_.rows() / nbClass_;
      Index ind = k * nbModalities + augData_.data_(i);

      Real proba = param_(ind);

      return std::log(proba);
    }

    /** Compute the observed log probability of individual i */
    Real lnObservedProbability(Index i, Index k) {
      Index nbModalities = param_.rows() / nbClass_;
      Real proba;

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

        default: {}
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

#endif /* MIXT_CATEGORICALLIKELIHOOD_H */
