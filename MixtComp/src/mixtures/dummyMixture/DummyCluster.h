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
 * created on: Nov 28, 2013
 * Author:   Vincent KUBICKI
 **/

/** @file DummyCluster.h
 *  @brief In this file 
 **/

#ifndef DUMMYCLUSTER_H_
#define DUMMYCLUSTER_H_

#include "Clustering.h"
// #include "/home/modal/workspace/stk/stkpp/projects/Clustering/include/STK_IIngredient.h"

namespace mixt
{

class DummyCluster : public STK::IIngredient
{
  public:
    DummyCluster( std::string name, int nbCluster );
    DummyCluster( DummyCluster const& mixture );

    virtual ~DummyCluster();

    virtual DummyCluster* clone();
    virtual DummyCluster* create();
    virtual void initializeModel();
    virtual void initializeStep();
    virtual void randomInit();
    virtual void paramUpdateStep();
    virtual double lnComponentProbability(int sample_num, int Cluster_num);
    virtual int nbFreeParameter() const;
    virtual void setData();
};

} /* namespace mixt */
#endif /* DUMMYCLUSTER_H_ */
