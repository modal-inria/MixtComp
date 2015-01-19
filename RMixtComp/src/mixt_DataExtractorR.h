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

#ifndef MIXT_DATAEXTRACTORR_H
#define MIXT_DATAEXTRACTORR_H

#include "MixtComp/src/Data/mixt_AugmentedData.h"
#include "Rcpp.h"
#include "Eigen/Dense"

namespace mixt
{

class DataExtractorR
{
  public:
    DataExtractorR();
    ~DataExtractorR();

    /** Export function for categorical model */
    void exportVals(std::string idName,
                    const AugmentedData<STK::Array2D<int> >* p_augData,
                    const Eigen::Matrix<std::vector<std::pair<int, Real> >,
                                        Eigen::Dynamic,
                                        Eigen::Dynamic>* p_dataStatStorage);
    /** Export function for gaussian model */
    void exportVals(std::string idName,
                    const AugmentedData<STK::Array2D<Real> >* p_augData,
                    const STK::Array2D<STK::Array2DPoint<Real> >* p_dataStatStorage);
    /** Export function for Poisson model */
    void exportVals(std::string idName,
                    const AugmentedData<STK::Array2D<int> >* p_augData,
                    const STK::Array2D<STK::Array2DPoint<int> >* p_dataStatStorage);

    Rcpp::List rcppReturnVal() const;
  private:
    Rcpp::List data_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORR_H
