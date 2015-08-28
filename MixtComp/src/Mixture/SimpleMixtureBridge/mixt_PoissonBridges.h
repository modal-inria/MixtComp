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
 **/

#ifndef MIXT_POISSONBRIDGES_H
#define MIXT_POISSONBRIDGES_H

#include "mixt_Poisson_k.h"
#include "../../Sampler/mixt_PoissonSampler.h"
#include "../../Data/mixt_ConfIntDataStat.h"
#include "../../Likelihood/mixt_PoissonLikelihood.h"
#include "Eigen/Dense"

namespace mixt
{

// forward declaration
template<int Id,
         typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor> class MixtureBridge;

/**
 *  Specialization of the MixtureTraits for the Gaussian_sjk_ model
 **/
template<>
struct BridgeTraits<Poisson_k_>
{
    /** Type of the data set to be used as input */
    typedef Vector<int> Data;
    /** Type of the AugmentedData */
    typedef AugmentedData<Data> AugData;
    /** Type of the DataStat */
    typedef ConfIntDataStat<int> DataStat;
    /** Type of the Data */
    typedef Data::Type Type;
    /** Type of the mixture model */
    typedef Poisson_k Mixture;
    /** Sampler to be used for this specific mixture model */
    typedef PoissonSampler Sampler;
    /** Type of Likelihood */
    typedef PoissonLikelihood Likelihood;
};

template <typename DataHandler,
          typename DataExtractor,
          typename ParamSetter,
          typename ParamExtractor>
using PoissonBridge_k_m = MixtureBridge<Poisson_k_,
                                              DataHandler,
                                              DataExtractor,
                                              ParamSetter,
                                              ParamExtractor>;

} /* namespace mixt */

#endif // MIXT_POISSONBRIDGES_H
