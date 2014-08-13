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
 *  Created on: Mar 27, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_CATEGORICALBRIDGES_H
#define MIXT_CATEGORICALBRIDGES_H

#include "stkpp/projects/Clustering/include/CategoricalMixtureModels/STK_Categorical_pjk.h"
#include "../../Sampler/mixt_CategoricalSampler.h"
#include "../../Sampler/mixt_CategoricalSamplerIterator.h"
#include "../../Data/mixt_CategoricalDataStat.h"
#include "../../Param/mixt_SimpleParamStat.h"
#include "../../Likelihood/mixt_CategoricalLikelihood.h"

namespace mixt
{

// forward declaration
template<int Id,
         typename DataHandler,
         typename DataExtractor,
         typename ParamExtractor> class MixtureBridge;

/**
 *  Specialization of the MixtureTraits for the Gaussian_sjk_ model
 **/
template<>
struct BridgeTraits<STK::Clust::Categorical_pjk_>
{
    /** Type of the data set to be used as input */
    typedef STK::Array2D<int> Data;
    /** Type of the AugmentedData */
    typedef AugmentedData<Data> AugData;
    /** Type of the DataStat */
    typedef CategoricalDataStat DataStat;
    /** Type of the ParamStat */
    typedef SimpleParamStat ParamStat;
    /** Type of the parameter set to be used as output */
    typedef STK::Array2D<STK::Real> Param;
    /** Type of the Data */
    typedef Data::Type Type;
    /** Type of the mixture model */
    typedef STK::Categorical_pjk<Data> Mixture;
    /** Sampler to be used for this specific mixture model */
    typedef CategoricalSampler Sampler;
    /** Corresponding sampler iterator */
    typedef CategoricalSamplerIterator SamplerIterator;
    /** Type of Likelihood */
    typedef CategoricalLikelihood Likelihood;
};

template<typename DataHandler,
         typename DataExtractor,
         typename ParamExtractor>
struct CategoricalBridge_pjk_m
{
   typedef MixtureBridge<STK::Clust::Categorical_pjk_,
                         DataHandler,
                         DataExtractor,
                         ParamExtractor> type;
};

} /* namespace mixt */

#endif /* MIXT_CATEGORICALBRIDGES_H */
