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
 *  Created on: July 24, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_GAUSSIANLIKELIHOOD_H
#define MIXT_GAUSSIANLIKELIHOOD_H

#include "DManager/include/STK_DataHandler.h"
#include "Arrays/include/STK_CArrayPoint.h"
#include "../Data/mixt_AugmentedData.h"

namespace mixt
{

class GaussianLikelihood
{
  public:
    /** Constructor */
    GaussianLikelihood(std::string idName,
                       const STK::IDataHandler* handler,
                       const STK::CArrayPoint<STK::Real>* p_prop,
                       const STK::Array2D<STK::Real>* p_param,
                       const AugmentedData<STK::Array2D<STK::Real> >* augData);
    /** Destructor */
    virtual ~GaussianLikelihood();

    /** Compute the observed log-likelihood */
    STK::Real lnLikelihood();

  private:
    /** Original data table indicating whether a data is present, and not
     * missing nor partially observed */
    STK::Array2D<STK::Real> presentData_;

    /** Pointer to proportions */
    const STK::CArrayPoint<STK::Real>* p_prop_;

    /** Pointer to parameters table */
    const STK::Array2D<STK::Real>* p_param_;

    /** Pointer to AugmentedData, to get the lists of missing and partially observed values */
    const AugmentedData<STK::Array2D<STK::Real> >* p_augData_;
};

} /* namespace mixt */

#endif /* STK_MIXTURECOMPOSER_H */
