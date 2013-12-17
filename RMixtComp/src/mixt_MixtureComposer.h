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
 * created on: Dec 13, 2013
 * Author:   Vincent KUBICKI
 **/

/** @file mixt_MixtureComposer.h
 *  @brief In this file 
 **/

#ifndef MIXT_MIXTURECOMPOSER_H_
#define MIXT_MIXTURECOMPOSER_H_

#include "stkpp/include/Clustering.h"
#include "mixt_DataHandlerR.h"

namespace mixt
{
  
class MixtureComposer : public STK::MixtureComposer
{
  public:    
    /** Constructor.
     * @param nbCluster number of cluster
     */
    inline MixtureComposer(int nbCluster)
                                   : STK::MixtureComposer(nbCluster) {}
                                   
    /** Constructor.
     * @param nbCluster number of cluster
     * @param nbSample number of sample
     */
    inline MixtureComposer(int nbCluster, int nbSample)
                                   : STK::MixtureComposer(nbCluster, nbSample) {}
                                   
    /** copy constructor.
     *  @param model the model to copy
     */
    inline MixtureComposer(STK::MixtureComposer const& model)
                                   : STK::MixtureComposer(model) {}
                                   
    /** The individual ingredient will be deleted there.*/
    virtual ~MixtureComposer() {}
    
    /** Create a composer, but reinitialize the ingredients parameters. */
    virtual MixtureComposer* create() const;
    
    /** Create a clone of the current model, with ingredients parameters preserved. */
    virtual MixtureComposer* clone() const;
    
    inline void getAugmentedData(std::string const& idData, AugmentedData<STK::Array2D<STK::Real> >& augData, int& nbVariable) const
              {p_DataHandlerR()->getAugmentedData(idData, augData, nbVariable);};
    
  private:
    inline DataHandlerR const* p_DataHandlerR() const
              {return static_cast<DataHandlerR const*>(this->p_handler());};
};

} /* namespace mixt */

#endif /* MIXT_MIXTURECOMPOSER_H_ */