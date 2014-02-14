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
 *  Created on: Dec 31, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_Clust_Util.cpp
 *  @brief In this file 
 **/

#include "mixt_Clust_Util.h"
#include "mixt_GaussianMixture.h"
#include "mixt_GammaMixture.h"
#include "mixt_IMixture.h"

namespace mixt
{
namespace Clust
{
IMixture* createMixtCompMixture(STK::Clust::Mixture model, std::string const& id, int nbCluster)
{
  switch (model)
  {
//    case STK::Clust::Gamma_ajk_bjk_:
//      return new MixtureGamma_ajk_bjk_m(id, nbCluster);
//      break;
    case STK::Clust::Gaussian_sjk_:
      return new MixtureGaussian_sjk_m(id, nbCluster);
      break;
    default:
      return 0;
      break;
  }
  // avoid warning
  return 0;
}
} // namespace Clust
} // namespace mixt
