/* MixtComp version 4  - july 2019
 * Copyright (C) Inria - Université de Lille - CNRS*/

/*
 *  Project:    MixtComp
 *  Created on: July 28, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef CATEGORICALLIKELIHOOD_H
#define CATEGORICALLIKELIHOOD_H

#include "../../../Data/AugmentedData.h"
#include "../../../LinAlg/LinAlg.h"

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
        	throw("CategoricalLikelihood::lnObservedProbability, error in missing data handling, please report to the maintainer.");
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

#endif /* CATEGORICALLIKELIHOOD_H */
