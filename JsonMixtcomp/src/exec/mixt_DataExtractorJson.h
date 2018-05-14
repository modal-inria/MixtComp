/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/

#ifndef MIXT_DATAEXTRACTORJSON_H
#define MIXT_DATAEXTRACTORJSON_H

#include "json.hpp"
#include "Data/mixt_AugmentedData.h"
#include "Mixture/Rank/mixt_RankIndividual.h"
#include "Mixture/Rank/mixt_RankStat.h"
#include "Mixture/Functional/mixt_Function.h"

namespace mixt
{

class DataExtractorJson
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

    /** Export function for Poisson model */
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

    nlohmann::json jsonReturnVal() const;
  private:
    std::vector<std::string> mixtureName_;
    std::vector<nlohmann::json> data_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORR_H
