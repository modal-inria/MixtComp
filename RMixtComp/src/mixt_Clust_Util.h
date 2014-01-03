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
/** utility function for creating an Ingredient.
*  @param id of the ingredient to create
*  @param model ingredient to create
*  @param nbCluster number of cluster
**/
IMixture* createMixtCompIngredient(STK::Clust::Ingredient model, std::string const& id, int nbCluster);
} // namespace Clust
} // namespace mixt

#endif /* MIXT_CLUST_UTIL_H_ */