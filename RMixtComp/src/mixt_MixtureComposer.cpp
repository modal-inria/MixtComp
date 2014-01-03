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
 * created on: Jan 3, 2014
 * Author:   Vincent KUBICKI
 **/

/** @file mixt_MixtureComposer.cpp
 *  @brief In this file 
 **/

#include "mixt_MixtureComposer.h"
#include "mixt_IMixture.h"

namespace mixt
{

/* create ingredients using info from p_dataHandler */
void MixtureComposer::createMixtCompIngredients()
{
  if (!p_DataHandlerR())
//        STKRUNTIME_ERROR_NO_ARG(MixtureComposer::createIngredients,data handler is not set);
  for (InfoMap::const_iterator it=p_DataHandlerR()->info().begin(); it!=p_DataHandlerR()->info().end(); ++it)
  {
    std::string name = it->first;
    std::string model= it->second;
    STK::Clust::Ingredient idModel = STK::Clust::stringToIngredient(model);
#ifdef DSTK_MIXTURE_DEBUG
    stk_cout << _T("model = ") << model << _T("\n");
    stk_cout << _T("name = ") << name << _T("\n");
    stk_cout << _T("Ingredient = ") << idModel << _T("\n");
#endif
    if (idModel != STK::Clust::unknown_mixture_)
    {
      registerIngredient(Clust::createMixtCompIngredient(idModel, name, nbCluster_));
    }
  }
}

} /* namespace mixt */