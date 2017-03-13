/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_PARAMSETTERDUMMY_H
#define MIXT_PARAMSETTERDUMMY_H

#include "../Data/mixt_AugmentedData.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Mixture/Rank/mixt_RankIndividual.h"
#include "../Mixture/Rank/mixt_RankStat.h"
#include "../Mixture/Rank/mixt_RankVal.h"

namespace mixt {

/** Dummy IO class for debugging purposes */
class DataHandlerDummy {
  public:
    template<typename DataType>
    std::string getData(std::string const& idData,
                        AugmentedData<DataType>& augData,
                        Index& nbSample,
                        std::string& param,
                        typename AugmentedData<DataType>::Type offset) const {
      return std::string();
    };

    std::string getData(std::string const& idData,
                        Vector<std::string>& dataStr,
                        Index& nbInd,
                        std::string& paramStr) const;
};

/** Dummy IO class for debugging purposes */
class DataExtractorDummy {
  public:
    template<typename DataType,
             typename Type>
    void exportVals(Index indexMixture,
                    std::string idName,
                    const AugmentedData<DataType>& augData,
                    const Vector<RowVector<Type> >& stat) const {};

    /** Export function for Rank model */
    void exportVals(Index indexMixture,
                    std::string idName,
                    const Vector<RankIndividual>& data) const;

    /** Export function for Ordinal and Poisson model */
    void exportVals(
        int indexMixture,
        bool hasModalities,
        std::string idName,
        const AugmentedData<Vector<int> >& augData,
        const Vector<RowVector<int> >& dataStatStorage) const;
};

/** Dummy IO class for debugging purposes */
class ParamExtractorDummy {
  public:
    void exportParam(Index indexMixture,
                     const std::string& idName,
                     const std::string& paramName,
                     const Matrix<Real>& paramStatStorage,
                     const Matrix<Real>& paramLogStorage,
                     const std::vector<std::string>& paramNames,
                     const Real confidenceLevel,
                     const std::string& paramStr) const;

    void exportParam(Index indexMixture,
                     const std::string& idName,
                     const std::string& paramName,
                     const std::vector<RankStat>& paramStat,
                     const std::vector<std::string>& paramNames,
                     Real confidenceLevel) const;
};

class ParamSetterDummy {
  public:
    ParamSetterDummy();

    ~ParamSetterDummy();

    void getParam(const std::string& idName,
                  const std::string& paramName,
                  Vector<Real>& params,
                  const std::string& paramStr) const;

    void getParam(const std::string& idName,
                  const std::string& paramName,
                  Vector<RankVal>& param,
                  const std::string& paramStr) const;
};

} // namespace mixt

#endif // MIXT_PARAMEXTRACTORCSV_H
