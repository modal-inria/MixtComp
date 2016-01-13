/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: August 07, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_PARAMEXTRACTORR_H
#define MIXT_PARAMEXTRACTORR_H

#include "MixtComp/src/Mixture/Rank/mixt_RankStat.h"
#include "MixtComp/src/LinAlg/mixt_LinAlg.h"
#include "MixtComp/src/Mixture/Rank/mixt_RankVal.h"
#include "Rcpp.h"

namespace mixt
{

class ParamExtractorR
{
  public:
    ParamExtractorR();
    ~ParamExtractorR();

    /** Extractor for ConfIntParamStat */
    void exportParam(const std::string& idName,
                     const std::string& paramName,
                     const Matrix<Real>& paramStat,
                     const Matrix<Real>& paramsLog,
                     const std::vector<std::string>& paramNames,
                     Real confidenceLevel);

    /** Extractor for Rank data type */
    void exportParam(const std::string& idName,
                     const std::string& paramName,
                     const std::vector<RankStat>& paramStat,
                     const std::vector<std::string>& paramNames,
                     Real confidenceLevel);

    Rcpp::List rcppReturnParam() const;
  private:
    std::map<std::string, std::map<std::string, Rcpp::List> > param_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORR_H
