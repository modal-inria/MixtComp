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

namespace mixt
{

/** Create a composer, but reinitialize the mixtures parameters. */
MixtureComposer* MixtureComposer::create() const
{
  MixtureComposer* p_composer = new MixtureComposer(nbSample(), nbVariable(), nbCluster());
  p_composer->createComposer(v_mixtures());
  return p_composer;
}

/** Create a clone of the current model, with mixtures parameters preserved. */
MixtureComposer* MixtureComposer::clone() const
{
  return new MixtureComposer(*this);
}

void MixtureComposer::storeIntermediateResults(int iteration)
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->storeIntermediateResults(iteration);
  }
}

} /* namespace mixt */
