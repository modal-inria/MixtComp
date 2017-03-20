/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: July 7, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_DATAEXTRACTORR_H
#define MIXT_DATAEXTRACTORR_H

#include "MixtComp/src/Data/mixt_AugmentedData.h"
#include "Eigen/Dense"
#include <Rcpp.h>

#include "MixtComp/src/Mixture/Rank/mixt_RankIndividual.h"
#include "MixtComp/src/Mixture/Rank/mixt_RankStat.h"
#include "MixtComp/src/Mixture/Functional/mixt_FunctionalClass.h"

namespace mixt
{

class DataExtractorR
{
  public:
    void setNbMixture(int nbMixture);

    /** Export function for categorical model */
    void exportVals(int indexMixture,
                    bool hasModalities,
                    std::string idName,
                    const AugmentedData<Vector<int> >& augData,
                    const Vector<std::vector<std::pair<int, Real> > >& dataStatStorage);

    /** Export function for classes (called from the composer) */
    void exportVals(int indexMixture,
                    std::string idName,
                    const AugmentedData<Vector<Index> >& augData,
                    const Matrix<Real>& tik);

    /** Export function for gaussian model */
    void exportVals(int indexMixture,
                    bool hasModalities,
                    std::string idName,
                    const AugmentedData<Vector<Real> >& augData,
                    const Vector<RowVector<Real> >& dataStatStorage);

    /** Export function for Ordinal and Poisson model */
    void exportVals(int indexMixture,
                    bool hasModalities,
                    std::string idName,
                    const AugmentedData<Vector<int> >& augData,
                    const Vector<RowVector<int> >& dataStatStorage);

    /** Export function for Rank model */
    void exportVals(int indexMixture,
                    std::string idName,
                    const Vector<RankIndividual>& data,
                    const std::vector<RankStat>& dataStat);

    /** Export function for Functional model */
    void exportVals(int indexMixture,
                    std::string idName,
                    const Vector<Function>& data);

    Rcpp::List rcppReturnVal() const;
  private:
    std::vector<std::string> mixtureName_;
    std::vector<Rcpp::List> data_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORR_H
