/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public
 License along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place,
 Suite 330,
 Boston, MA 02111-1307
 USA

 */

/*
 * Project:  MixtComp
 * created on: Dec 31, 2013
 * Author:   Vincent KUBICKI
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
    case STK::Clust::Gamma_ajk_bjk_:
      return new MixtureGamma_ajk_bjk_m(id, nbCluster);
      break;
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