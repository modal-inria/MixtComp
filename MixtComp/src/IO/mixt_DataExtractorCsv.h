/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_DATAEXTRACTORCSV_H
#define MIXT_DATAEXTRACTORCSV_H

#include <map>
#include "../Data/mixt_AugmentedData.h"

namespace mixt
{

class DataExtractorCsv
{
  public:
    DataExtractorCsv();
    ~DataExtractorCsv();

    /** Export function for categorical model */
    void exportVals(std::string idName,
                    const AugmentedData<Matrix<int> >* p_augData,
                    const std::map<int, std::map<int, std::vector< std::pair<int, Real> > > >* p_dataStatStorage);
    /** Export function for gaussian model */
    void exportVals(std::string idName,
                    const AugmentedData<Matrix<Real> >* p_augData,
                    const std::map<int, std::map<int, Vector<Real> > >* p_dataStatStorage);
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORCSV_H
