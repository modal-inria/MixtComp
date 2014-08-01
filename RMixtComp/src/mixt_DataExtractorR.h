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
 *  Created on: July 7, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_DATAEXTRACTOR_H
#define MIXT_DATAEXTRACTOR_H

#include "Arrays/include/STK_Array2D.h"
#include "Rcpp.h"

namespace mixt
{

class DataExtractorR
{
  public:
    DataExtractorR();
    ~DataExtractorR();
    void exportVals(std::string idName,
                    const STK::Array2D<STK::Real>* p_data,
                    const STK::Array2D<int>* p_posMissing,
                    const STK::Array2D<STK::Real>* p_statMissing);
    void exportVals(std::string idName,
                    const STK::Array2D<int>* p_data,
                    const STK::Array2D<int>* p_posMissing,
                    const STK::Array2D<STK::Real>* p_statMissing);
    Rcpp::List rcppReturnVals() const;
  private:
    Rcpp::List data_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTOR_H
