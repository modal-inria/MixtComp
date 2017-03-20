/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: Mar 27, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_CATEGORICALBRIDGES_H
#define MIXT_CATEGORICALBRIDGES_H

#include "../../Various/mixt_Enum.h"
#include "mixt_Categorical_pjk.h"
#include "../../Sampler/mixt_CategoricalSampler.h"
#include "../../Data/mixt_CategoricalDataStat.h"
#include "../../Likelihood/mixt_CategoricalLikelihood.h"
#include "../../LinAlg/mixt_LinAlg.h"

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
struct BridgeTraits<Categorical_pjk_>
{
    /** Type of the data set to be used as input */
    typedef Vector<int> Data;
    /** Type of the AugmentedData */
    typedef AugmentedData<Data> AugData;
    /** Type of the DataStat */
    typedef CategoricalDataStat DataStat;
    /** Type of the Data */
    typedef Data::Type Type;
    /** Type of the mixture model */
    typedef Categorical_pjk Mixture;
    /** Sampler to be used for this specific mixture model */
    typedef CategoricalSampler Sampler;
    /** Type of Likelihood */
    typedef CategoricalLikelihood<int> Likelihood;
};

template <typename DataHandler,
          typename DataExtractor,
          typename ParamSetter,
          typename ParamExtractor>
using CategoricalBridge_pjk_m = MixtureBridge<Categorical_pjk_,
                                              DataHandler,
                                              DataExtractor,
                                              ParamSetter,
                                              ParamExtractor>;
} /* namespace mixt */

#endif /* MIXT_CATEGORICALBRIDGES_H */
