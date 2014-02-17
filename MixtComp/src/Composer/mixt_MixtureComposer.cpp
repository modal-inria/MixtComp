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
 *  Created on: Jan 3, 2014
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_MixtureComposer.cpp
 *  @brief In this file 
 **/

#include "mixt_MixtureComposer.h"
#include "../Mixture/mixt_IMixture.h"

namespace mixt
{

/** Create a composer, but reinitialize the mixtures parameters. */
MixtureComposer* MixtureComposer::create() const
{
  if (!p_DataHandlerR()) {STKRUNTIME_ERROR_NO_ARG(MixtureComposer::create,data handler is not set);};
 
  // set dimensions
  MixtureComposer* p_composer = new MixtureComposer(nbCluster_);
  p_composer->createComposer(p_DataHandlerR(), v_mixtures());
  return p_composer;
}

/** Create a clone of the current model, with mixtures parameters preserved. */
MixtureComposer* MixtureComposer::clone() const
{
  if (!p_DataHandlerR()) {STKRUNTIME_ERROR_NO_ARG(MixtureComposer::createmixtures,data handler is not set);};
  return new MixtureComposer(*this);
}

/* create mixtures using info from p_dataHandler */
void MixtureComposer::createMixtCompMixtures()
{
  if (!p_DataHandlerR()) {STKRUNTIME_ERROR_NO_ARG(MixtureComposer::createmixtures,data handler is not set);};
  for (InfoMap::const_iterator it=p_DataHandlerR()->info().begin(); it!=p_DataHandlerR()->info().end(); ++it)
  {
    std::string name = it->first;
    std::string model= it->second;
    STK::Clust::Mixture idModel = STK::Clust::stringToMixture(model);
    if (idModel != STK::Clust::unknown_mixture_)
    {
      registerMixture(Clust::createMixtCompMixture(idModel, name, nbCluster_));
    }
  }
}

void MixtureComposer::registerMixture(IMixture* mixture)
{
  v_mixtures_.push_back(mixture);
  v_mixtures_.back()->setMixtureComposer(this);
}

void MixtureComposer::setDataHandler(DataHandlerR const* p_handler)
{
  STK::MixtureComposer::setDataHandler(p_handler);
}

} /* namespace mixt */
