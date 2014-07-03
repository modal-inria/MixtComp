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
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_GAUSSIANBRIDGE_H
#define MIXT_GAUSSIANBRIDGE_H

#include "stkpp/projects/Clustering/include/GaussianMixtureModels/STK_Gaussian_sjk.h"
#include "../../Sampler/mixt_GaussianSampler.h"
#include "../../Sampler/mixt_GaussianSamplerIterator.h"
#include "../../Data/mixt_GaussianDataStat.h"

namespace mixt
{

// forward declaration
template<int Id> class MixtureBridge;

/**
 *  Specialization of the MixtureTraits for the Gaussian_sjk_ model
 **/
template<>
struct BridgeTraits<STK::Clust::Gaussian_sjk_>
{
    /** Type of the data set to be used as input */
    typedef STK::Array2D<STK::Real> Data;
    /** Type of the AugmentedData*/
    typedef AugmentedData<Data> AugData;
    /** Type of the DataStat*/
    typedef GaussianDataStat DataStat;
    /** Type of the parameter set to be used as output */
    typedef STK::Array2D<STK::Real> Param;
    /** Type of the Data */
    typedef Data::Type Type;
    /** Type of the mixture model */
    typedef STK::Gaussian_sjk<Data> Mixture;
    /** Sampler to be used for this specific mixture model */
    typedef GaussianSampler Sampler;
    /** Corresponding sampler iterator */
    typedef GaussianSamplerIterator SamplerIterator;
};

typedef MixtureBridge<STK::Clust::Gaussian_sjk_> GaussianBridge_sjk_m;

} /* namespace mixt */

#endif /* MIXT_GAUSSIANBRIDGE_H */
