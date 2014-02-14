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
 *  Created on: 30 dec. 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file mixt_IMixture.h
 *  @brief In this file .
 **/

#ifndef MIXT_IMIXTURE_H
#define MIXT_IMIXTURE_H

#include "mixt_AugmentedData.h"
#include "mixt_MixtureComposer.h"
#include "stkpp/projects/Clustering/include/STK_IMixture.h"

namespace mixt
{

class IMixture : public STK::IMixture
{
  public:
    /* constructor */
    IMixture( std::string const& idName, int nbCluster)
                            : STK::IMixture(idName, nbCluster)
    {}
    
    /* copy constructor */
    IMixture(IMixture const& mixture)
                            : STK::IMixture(mixture)
    {}
    
    /* Virtual destructor. */
    ~IMixture(){}
    
    template<typename Data>
    inline void getData(Data& data, int& nbVariable)
    {p_composer()->getData<Data>(STK::IMixture::idName(), data, nbVariable);}

    inline MixtureComposer const* p_composer()
    {return static_cast<MixtureComposer const*>(STK::IMixture::p_composer());}
    
    void setMixtureComposer(MixtureComposer const* p_composer)
    {
      STK::IMixture::setMixtureComposer(p_composer);
    }
};

} // namespace mixt

#endif /* MIXT_IMIXTURE_H */
