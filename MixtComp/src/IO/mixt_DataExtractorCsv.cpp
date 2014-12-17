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

#include "mixt_DataExtractorCsv.h"

namespace mixt
{

DataExtractorCsv::DataExtractorCsv()
{}

DataExtractorCsv::~DataExtractorCsv()
{}

void DataExtractorCsv::exportVals(std::string idName,
                                  const AugmentedData<STK::Array2D<int> >* p_augData,
                                  const std::map<int, std::map<int, std::vector< std::pair<int, STK::Real> > > >* p_dataStatStorage)
{}

void DataExtractorCsv::exportVals(std::string idName,
                                  const AugmentedData<STK::Array2D<STK::Real> >* p_augData,
                                  const std::map<int, std::map<int, STK::Array2DVector<STK::Real> > >* p_dataStatStorage)
{}

} // namespace mixt