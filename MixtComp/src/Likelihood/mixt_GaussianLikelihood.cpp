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

#include "mixt_GaussianLikelihood.h"
#include "STatistiK/include/STK_Law_Normal.h"

namespace mixt
{

GaussianLikelihood::GaussianLikelihood(std::string idName,
                                       const STK::IDataHandler* handler,
                                       const STK::CArrayPoint<STK::Real>* p_prop,
                                       const STK::Array2D<STK::Real>* p_param,
                                       const AugmentedData<STK::Array2D<STK::Real> >* augData) :
    p_prop_(p_prop),
    p_param_(p_param),
    p_augData_(augData)
{
  int dummyVar;
  // fill presentData_ with the original data set containing NA information
  handler->getData(idName,
                   presentData_,
                   dummyVar);
}

GaussianLikelihood::~GaussianLikelihood()
{

}

STK::Real GaussianLikelihood::lnLikelihood()
{
  STK::Real lnLikelihood = 0.;

  // likelihood for present value
  for (int i = 0; i < presentData_.sizeRows(); ++i)
  {
    STK::Array2DPoint<STK::Real> lnComp(p_prop_->sizeCols());
    lnComp = 0.;
    for (int k = 0; k < p_prop_->sizeCols(); ++k)
    {
      for (int j = 0; j < presentData_.sizeCols(); ++j)
      {
        if (presentData_(i, j) != STK::Arithmetic<STK::Real>::NA())
        {
          STK::Real mean  = p_param_->elt(2*k    , j);
          STK::Real sd    = p_param_->elt(2*k + 1, j);
          lnComp[k] += STK::Law::Normal::lpdf(presentData_(i, j),
                                              mean,
                                              sd);
        }
      }
    }
    STK::Real max = lnComp.maxElt();
    STK::Real sum = (lnComp -= max).exp().dot(*p_prop_);
    lnLikelihood += max + std::log(sum);
  }
  return lnLikelihood;
}

} /* namespace mixt */
