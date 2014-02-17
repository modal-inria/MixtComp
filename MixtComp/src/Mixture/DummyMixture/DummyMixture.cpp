/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 modal

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
 * created on: Nov 28, 2013
 * Author:   modal
 **/

/** @file DummyCluster.cpp
 *  @brief In this file 
 **/

#include "DummyMixture.h"

namespace mixt
{

DummyCluster::DummyCluster(std::string name, int nbCluster)
                          :IIngredient(name, nbCluster)
{
  // TODO Auto-generated constructor stub

}

DummyCluster::DummyCluster(DummyCluster const& mixture)
                          :IIngredient(mixture)
{
  // TODO Auto-generated constructor stub

}

DummyCluster::~DummyCluster()
{
  // TODO Auto-generated destructor stub
}

DummyCluster* DummyCluster::clone()
{
  return new DummyCluster(*this);
}

DummyCluster* DummyCluster::create()
{
  return new DummyCluster(name(), nbCluster());
}

void DummyCluster::initializeModel()
{

}

void DummyCluster::initializeStep()
{

}


void DummyCluster::randomInit()
{

}

void DummyCluster::paramUpdateStep()
{

}

double DummyCluster::lnComponentProbability(int sample_num, int Cluster_num)
{
  return 0.;
}

int DummyCluster::nbFreeParameter() const
{
  return 0;
}

void DummyCluster::setData()
{

}

} /* namespace mixt */
