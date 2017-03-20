/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
