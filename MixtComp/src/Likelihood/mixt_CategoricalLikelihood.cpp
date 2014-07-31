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
 *  Created on: July 28, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_CategoricalLikelihood.h"

namespace mixt
{

CategoricalLikelihood::CategoricalLikelihood(const STK::CArrayPoint<STK::Real>* p_prop,
                                             const STK::Array2D<STK::Real>* p_param,
                                             const AugmentedData<STK::Array2D<int> >* augData) :
    p_prop_(p_prop),
    p_param_(p_param),
    p_augData_(augData)
{}

CategoricalLikelihood::~CategoricalLikelihood()
{}

STK::Real CategoricalLikelihood::lnLikelihood()
{
  STK::Real lnLikelihood = 0.;
  STK::Array2D<STK::Real> lnComp(presentData_.sizeRows(),
                                 p_prop_->sizeCols(),
                                 0.);

  // likelihood for present data
  for (int i = 0; i < presentData_.sizeRows(); ++i)
  {
    for (int k = 0; k < p_prop_->sizeCols(); ++k)
    {
      for (int j = 0; j < presentData_.sizeCols(); ++j)
      {
        int nbModalities = p_augData_->dataRanges_[j].second + 1;

        if (presentData_(i, j) != STK::Arithmetic<STK::Real>::NA())   // likelihood for present value
        {
          p_param_->elt(k * nbModalities + presentData_(i, j),
                        j);
        }
      }
    }
  }

  // Compute the likelihood for each sample, using the mixture model
  for (int i = 0; i < presentData_.sizeRows(); ++i)
  {
    STK::Real max = lnComp.row(i).maxElt();
    STK::Real sum = (lnComp.row(i) -= max).exp().dot(*p_prop_);
    lnLikelihood += max + std::log(sum);
  }

  return lnLikelihood;
}

} /* namespace mixt */
