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

namespace mixt
{

/** Dummy IO class for debugging purposes */
class DataHandlerDummy
{
  public:
    template<typename DataType>
    std::string getData(std::string const& idData,
                        AugmentedData<DataType>& augData,
                        int& nbSample,
                        std::string& param,
                        typename AugmentedData<DataType>::Type offset) const
    {
      return std::string();
    };

    std::string getData(std::string const& idData,
                        Vector<std::string>& dataStr,
                        int& nbInd,
                        std::string& paramStr) const;
};

/** Dummy IO class for debugging purposes */
class DataExtractorDummy
{
  public:
    template<typename DataType,
             typename Type>
    void exportVals(int indexMixture,
                    std::string idName,
                    const AugmentedData<DataType>& augData,
                    const Vector<RowVector<Type> >& stat) const {};

    /** Export function for Rank model */
    void exportVals(int indexMixture,
                    std::string idName,
                    const Vector<RankIndividual>& data) const;
};

/** Dummy IO class for debugging purposes */
class ParamExtractorDummy
{
  public:
    void exportParam(int indexMixture,
                     const std::string& idName,
                     const std::string& paramName,
                     const Matrix<Real>& paramStatStorage,
                     const Matrix<Real>& paramLogStorage,
                     const std::vector<std::string>& paramNames,
                     const Real confidenceLevel) const;

    void exportParam(int indexMixture,
                     const std::string& idName,
                     const std::string& paramName,
                     const std::vector<RankStat>& paramStat,
                     const std::vector<std::string>& paramNames,
                     Real confidenceLevel) const;
};

class ParamSetterDummy
{
  public:
    ParamSetterDummy();

    ~ParamSetterDummy();

    void getParam(const std::string& idName,
                  const std::string& paramName,
                  Vector<Real>& params) const;

    void getParam(const std::string& idName,
                  const std::string& paramName,
                  Vector<RankVal>& param) const;
};

} // namespace mixt

#endif // MIXT_PARAMEXTRACTORCSV_H
