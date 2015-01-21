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

#ifndef MIXT_DATAEXTRACTORCSV_H
#define MIXT_DATAEXTRACTORCSV_H

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
