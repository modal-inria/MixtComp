/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: May 29, 2015
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_CLASSDATASTAT_H
#define MIXT_CLASSDATASTAT_H


#include "../LinAlg/mixt_LinAlg.h"
#include "../Composer/mixt_ZClassInd.h"
#include "mixt_AugmentedData.h"

namespace mixt {

class ClassDataStat {
  public:
    ClassDataStat(ZClassInd& zClassInd);

    void setNbIndividual(int nbInd);

    void sampleVals(int sample,
                    int iteration,
                    int iterationMax);

    void imputeData(int ind, Matrix<Real>& tik);

    const Matrix<Real>& getDataStatStorage() const {return dataStatStorage_;}

  private:
    /** Reference to augmented data */
    ZClassInd& zClassInd_;

    /** Dense description of the missing values */
    Matrix<Real> dataStatStorage_;

    void sample(int ind);
};

} // namespace mixt

#endif // MIXT_CLASSDATASTAT_H
