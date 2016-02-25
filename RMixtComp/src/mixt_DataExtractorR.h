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

#include "MixtComp/src/Mixture/Rank/mixt_RankIndividual.h"
#include "MixtComp/src/Mixture/Rank/mixt_RankStat.h"

namespace mixt
{

class DataExtractorR
{
  public:
    void setNbMixture(int nbMixture);

    /** Export function for categorical model */
    void exportVals(int indexMixture,
                    std::string idName,
                    const AugmentedData<Vector<int> >& augData,
                    const Vector<std::vector<std::pair<int, Real> > >& dataStatStorage);

    /** Export function for classes (called from the composer) */
    void exportVals(int indexMixture,
                    std::string idName,
                    const AugmentedData<Vector<int> >& augData,
                    const Matrix<Real>& tik);

    /** Export function for gaussian model */
    void exportVals(int indexMixture,
                    std::string idName,
                    const AugmentedData<Vector<Real> >& augData,
                    const Vector<RowVector<Real> >& dataStatStorage);

    /** Export function for Poisson model */
    void exportVals(int indexMixture,
                    std::string idName,
                    const AugmentedData<Vector<int> >& augData,
                    const Vector<RowVector<int> >& dataStatStorage);

    /** Export function for Rank model */
    void exportVals(int indexMixture,
                    std::string idName,
                    const Vector<RankIndividual>& data,
                    const std::vector<RankStat>& dataStat);

    Rcpp::List rcppReturnVal() const;
  private:
    std::vector<std::string> mixtureName_;
    std::vector<Rcpp::List> data_;
};

} // namespace mixt

#endif // MIXT_DATAEXTRACTORR_H
