/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

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
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_CLUST_UTIL_H
#define MIXT_CLUST_UTIL_H

#include "STKernel/include/STK_String.h"
#include "STKernel/include/STK_Real.h"

namespace mixt
{

enum modelState
{
  modelCreated_, ///< the model has been created but is not initialized
  modelInitialized_, ///< the model is initialized, but its parameters are not initialized
  initialization_, ///< The parameters of the model have been initialized
  burnIn_,      ///< A short run has been done
  longRun_,   ///< a long run has been done
  finalized_ ///< the model is finalized
};

enum Mixture
{
  Categorical_pjk_,
  Gaussian_sjk_,
  Poisson_k_,
  unknown_mixture_
};

Mixture stringToMixture(std::string const& type);

std::string mixtureToString(Mixture const& type);

}  // namespace mixt

#endif /* STK_CLUST_UTIL_H */
