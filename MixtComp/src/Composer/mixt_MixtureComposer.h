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
 *  Created on: Dec 13, 2013
 *  Author:     Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

/** @file mixt_MixtureComposer.h
 *  @brief In this file 
 **/

#ifndef MIXT_MIXTURECOMPOSER_H_
#define MIXT_MIXTURECOMPOSER_H_

#include "stkpp/projects/Clustering/include/STK_MixtureComposer.h"
#include "stkpp/projects/Clustering/include/STK_IMixture.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Data/mixt_DataHandlerR.h"

namespace mixt
{

class MixtureComposer : public STK::MixtureComposer
{
  public:
    typedef std::vector<STK::IMixture*>::const_iterator ConstMixtIterator;
    typedef std::vector<STK::IMixture*>::iterator MixtIterator;
                                   
    /** Constructor.
     * @param nbSample number of samples
     * @param nbVariable number of variables
     * @param nbCluster number of clusters
     */
    inline MixtureComposer(int nbSample, int nbVariable, int nbCluster) :
        STK::MixtureComposer(nbSample, nbVariable,  nbCluster) {}
                                   
    /** copy constructor.
     *  @param model the model to copy
     */
    inline MixtureComposer(STK::MixtureComposer const& model) :
        STK::MixtureComposer(model) {}
                                   
    /** The individual mixture will be deleted there.*/
    virtual ~MixtureComposer() {}
    
    /** Create a composer, but reinitialize the mixtures parameters. */
    virtual MixtureComposer* create() const;
    
    /** Create a clone of the current model, with mixtures parameters preserved. */
    virtual MixtureComposer* clone() const;
    
    void storeIntermediateResults(int iteration);
};

} /* namespace mixt */

#endif /* MIXT_MIXTURECOMPOSER_H_ */
