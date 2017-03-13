/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_GAUSSIANBRIDGE_H
#define MIXT_GAUSSIANBRIDGE_H

#include<set>

#include "mixt_Gaussian_sjk.h"
#include "../../Sampler/mixt_GaussianSampler.h"
#include "../../Data/mixt_ConfIntDataStat.h"
#include "../../Likelihood/mixt_GaussianLikelihood.h"

namespace mixt
{

// forward declaration

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
struct BridgeTraits<Gaussian_sjk_>
{
    /** Type of the data set to be used as input */
    typedef Vector<Real> Data;
    /** Type of the AugmentedData*/
    typedef AugmentedData<Data> AugData;
    /** Type of the DataStat computer */
    typedef ConfIntDataStat<Real> DataStat;
    /** Type of the Data */
    typedef Data::Type Type;
    /** Type of the mixture model */
    typedef Gaussian_sjk Mixture;
    /** Sampler to be used for this specific mixture model */
    typedef GaussianSampler Sampler;
    /** Type of Likelihood */
    typedef GaussianLikelihood Likelihood;
};

template <typename DataHandler,
          typename DataExtractor,
          typename ParamSetter,
          typename ParamExtractor>
using GaussianBridge_sjk_m = MixtureBridge<Gaussian_sjk_,
                                              DataHandler,
                                              DataExtractor,
                                              ParamSetter,
                                              ParamExtractor>;
} /* namespace mixt */

#endif /* MIXT_GAUSSIANBRIDGE_H */
