/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "Dummy.h"

namespace mixt {

std::string DataHandlerDummy::getData(std::string const& idName,
                                      Vector<std::string>& dataStr,
                                      Index& nbInd,
                                      std::string& paramStr) const {
  return std::string();
}

Index DataHandlerDummy::nbSample() const {
	return 12;
}

/** Export function for Rank model */
void DataExtractorDummy::exportVals(Index indexMixture,
                                    std::string idName,
                                    const Vector<RankIndividual>& data) const {}

/** Export function for Ordinal and Poisson model */
void DataExtractorDummy::exportVals(
    int indexMixture,
    bool hasModalities,
    std::string idName,
    const AugmentedData<Vector<int> >& augData,
    const Vector<RowVector<int> >& dataStatStorage) const
{}

void ParamExtractorDummy::exportParam(Index indexMixture,
                                      const std::string& idName,
                                      const std::string& paramName,
                                      const Matrix<Real>& paramStat,
                                      const Matrix<Real>& paramsLog,
                                      const std::vector<std::string>& paramNames,
                                      const Real confidenceLevel,
                                      const std::string& paramStr) const {}

void ParamExtractorDummy::exportParam(Index indexMixture,
                                      const std::string& idName,
                                      const std::string& paramName,
                                      const std::vector<RankStat>& paramStat,
                                      const std::vector<std::string>& paramNames,
                                      Real confidenceLevel) const {}

ParamSetterDummy::ParamSetterDummy() {}

ParamSetterDummy::~ParamSetterDummy() {}

void ParamSetterDummy::getParam(const std::string& idName,
                                const std::string& paramName,
                                Vector<Real>& params,
                                const std::string& paramStr) const {}

void  ParamSetterDummy::getParam(const std::string& idName,
                                 const std::string& paramName,
                                 Vector<RankVal>& param,
                                 const std::string& paramStr) const {}

} // namespace mixt
