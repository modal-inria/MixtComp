/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_DataExtractorCsv.h"

namespace mixt
{

DataExtractorCsv::DataExtractorCsv()
{}

DataExtractorCsv::~DataExtractorCsv()
{}

void DataExtractorCsv::exportVals(std::string idName,
                                  const AugmentedData<Matrix<int> >* p_augData,
                                  const std::map<int, std::map<int, std::vector< std::pair<int, Real> > > >* p_dataStatStorage)
{}

void DataExtractorCsv::exportVals(std::string idName,
                                  const AugmentedData<Matrix<Real> >* p_augData,
                                  const std::map<int, std::map<int, Vector<Real> > >* p_dataStatStorage)
{}

} // namespace mixt
