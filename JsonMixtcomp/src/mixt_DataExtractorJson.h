/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016
*/

/*
 *  Project:    JsonMixtComp
 *  Created on: Dec 01, 2016
 *  Author:     Etienne GOFFINET  <etienne.goffinet@inria.fr>
 **/

#ifndef MIXT_DATAEXTRACTORJSON_H
#define MIXT_DATAEXTRACTORJSON_H

#include "json.hpp"
#include "../../MixtComp/src/Data/mixt_AugmentedData.h"
#include "../../MixtComp/src/Mixture/Rank/mixt_RankIndividual.h"
#include "../../MixtComp/src/Mixture/Rank/mixt_RankStat.h"
#include "../../MixtComp/src/Mixture/Functional/mixt_Function.h"

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
    void exportVals(const Index&,
    				const std::string&,
					const Vector<mixt::Function>&);

    nlohmann::json jsonReturnVal() const;
  private:
    std::vector<std::string> mixtureName_;
    std::vector<nlohmann::json> data_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORR_H
