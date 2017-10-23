/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_GAUSSIAN_SJK_H
#define MIXT_GAUSSIAN_SJK_H

#include <vector>
#include <set>

#include "../../LinAlg/mixt_LinAlg.h"
#include "../../Statistic/mixt_NormalStatistic.h"
#include "../../Various/mixt_Enum.h"
#include "../../Mixture/mixt_IMixture.h"

namespace mixt {

class Gaussian_sjk {
  public:
    Gaussian_sjk(const std::string& idName,
                 int nbClass,
                 Vector<Real>& param,
                 const Vector<std::set<Index> >& classInd);

    Vector<bool> acceptedType() const;

    int computeNbFreeParameters() const;

    bool hasModalities() const;

    std::string setData(const std::string& paramStr,
                        AugmentedData<Vector<Real> >& augData);

    /** Algorithm based on http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Incremental_algorithm
     * using the biased estimator which corresponds to the maximum likelihood estimator */
    void mStep();

    std::vector<std::string> paramNames() const;

    void writeParameters() const;

    int checkSampleCondition(std::string* warnLog = NULL) const;

    std::string initParam(const Vector<Index>& initObs);
  private:
    std::string idName_;
    int nbClass_;

    Vector<Real>& param_;

    Vector<Real>* p_data_;

    const Vector<std::set<Index> >& classInd_;

    /** Statistic object to describe Poisson law */
    NormalStatistic normal_;
};

} // namespace mixt

#endif
