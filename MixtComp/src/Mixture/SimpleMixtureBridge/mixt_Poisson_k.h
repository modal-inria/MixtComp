/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: December 12, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_POISSON_K_H
#define MIXT_POISSON_K_H

#include <vector>
#include <set>

#include "../mixt_IMixture.h"
#include "Data/mixt_AugmentedData.h"
#include "LinAlg/mixt_LinAlg.h"
#include "Statistic/mixt_PoissonStatistic.h"
#include "Various/mixt_Enum.h"

namespace mixt {

class Poisson_k {
  public:
    Poisson_k(const std::string& idName,
              int nbClass,
              Vector<Real>& param,
              const Vector<std::set<Index> >& classInd);

    Vector<bool> acceptedType() const;

    int computeNbFreeParameters() const;

    bool hasModalities() const;

    std::string setData(const std::string& paramStr,
                        AugmentedData<Vector<int> >& augData);

    void mStep(EstimatorType bias);

    std::vector<std::string> paramNames() const;

    void writeParameters() const;

    int checkSampleCondition(std::string* warnLog = NULL) const;

    void initParam(const Vector<Index>& initObs) {};
  private:
    std::string idName_;
    int nbClass_;
    Vector<Real>& param_;
    Vector<int>* p_data_;
    const Vector<std::set<Index> >& classInd_;

    /** Statistic object to describe Poisson law */
    PoissonStatistic poisson_;
};

} // namespace mixt

#endif // MIXT_POISSON_K_H
