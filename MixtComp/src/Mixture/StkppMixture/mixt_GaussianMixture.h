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

#ifndef MIXT_GAUSSIANINGREDIENT_H
#define MIXT_GAUSSIANINGREDIENT_H

#include "../Sampler/mixt_GaussianSampler.h"
#include "../Sampler/mixt_GaussianSamplerIterator.h"
#include "mixt_MixtureBridge.h"
#include "stkpp/projects/Clustering/include/Mixtures/STK_GaussianMixtures.h"

namespace mixt
{

/**
 *  Specialization of the SamplerTraits for the Gaussian_sjk_ model
 **/
template<>
struct SamplerTraits<STK::Clust::Gaussian_sjk_>
{
   /** Sampler to be used for this specific mixture model */
   typedef GaussianSampler Sampler;
   /** Corresponding sampler iterator */
   typedef GaussianSamplerIterator SamplerIterator;
};

typedef MixtureBridge<STK::Clust::Gaussian_sjk_> MixtureGaussian_sjk_m;

} /* namespace mixt */

#endif /* STK_GAUSSIANINGREDIENT_H */
