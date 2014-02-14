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

/** @file mixt_Clust_Util.h
 *  @brief In this file 
 **/

#ifndef MIXT_CLUST_UTIL_H_
#define MIXT_CLUST_UTIL_H_

#include "stkpp/projects/Clustering/include/STK_Clust_Util.h"

namespace mixt
{
  
class IMixture;

namespace Clust
{
/** utility function for creating a Mixture.
*  @param id of the mixture to create
*  @param model mixture to create
*  @param nbCluster number of cluster
**/
IMixture* createMixtCompMixture(STK::Clust::Mixture model, std::string const& id, int nbCluster);
} // namespace Clust
} // namespace mixt

#endif /* MIXT_CLUST_UTIL_H_ */
