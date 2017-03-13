/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: May 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CONFINTDATASTAT_H
#define MIXT_CONFINTDATASTAT_H

#include "../LinAlg/mixt_LinAlg.h"
#include "mixt_AugmentedData.h"

namespace mixt {

template <typename Type>
class ConfIntDataStat {
  public:
    ConfIntDataStat(AugmentedData<Vector<Type> >& augData,
                    Real confidenceLevel)  :
      augData_(augData),
      confidenceLevel_(confidenceLevel)
    {};

    void setNbIndividual(int nbInd) {
      stat_.resize(nbInd);
      dataStatStorage_.resize(nbInd);
    }

    void sampleVals(int ind,
                    int iteration,
                    int iterationMax) {
      if (augData_.misData_(ind).first != present_) {
        if (iteration == 0) { // clear the temporary statistical object
          stat_(ind).resize(iterationMax + 1); // temporary storage for the individual
          dataStatStorage_(ind).resize(3); // export storage

          sample(ind, iteration); // first sampling
        }
        else if (iteration == iterationMax) { // export the statistics to the p_dataStatStorage object
          sample(ind, iteration); // last sampling

          stat_(ind).sort();
          Real alpha = (1. - confidenceLevel_) / 2.;
          int realIndLow =        alpha  * iterationMax;
          int realIndHigh = (1. - alpha) * iterationMax;

          dataStatStorage_(ind)(0) = stat_(ind)(iterationMax / 2);
          dataStatStorage_(ind)(1) = stat_(ind)(realIndLow      );
          dataStatStorage_(ind)(2) = stat_(ind)(realIndHigh + 1 );

          stat_(ind).resize(0); // clear the memory for current individual
        }
        else {
          sample(ind, iteration); // standard sampling
        }
      }
    }

    void imputeData(int ind) {
      if (augData_.misData_(ind).first != present_) {
        augData_.data_(ind) = dataStatStorage_(ind)(0); // imputation by the mode
      }
    }

    const Vector<RowVector<Type> >& getDataStatStorage() const {return dataStatStorage_;}

  private:
    void sample(int ind,
                int iteration) {
      stat_(ind)(iteration) = augData_.data_(ind);
    }

    /** pointer to data array */
    AugmentedData<Vector<Type> >& augData_;

    /** Description of the missing values */
    Vector<RowVector<Type> > dataStatStorage_;

    /** Array to store sampled values across iterations, for the current individual, access: tempStat_[i]
     * i: iteration */
    Vector<Vector<Type> > stat_;

    /** Confidence level */
    Real confidenceLevel_;
};

} // namespace mixt

#endif // MIXT_CONFINTDATASTAT_H
