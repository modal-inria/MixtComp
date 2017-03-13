/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: November 14, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_PARAMEXTRACTORCSV_H
#define MIXT_PARAMEXTRACTORCSV_H

#include "../LinAlg/mixt_LinAlg.h"

namespace mixt
{

class ParamExtractorCsv
{
  public:
    ParamExtractorCsv();
    ~ParamExtractorCsv();
    void exportParam(std::string idName,
                     const Matrix<Real>* p_params);
};

} // namespace mixt

#endif // MIXT_PARAMEXTRACTORCSV_H
