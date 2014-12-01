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

#include "mixt_Categorical_pjk.h"
#include "../../Sampler/mixt_CategoricalSampler.h"
#include "../../Data/mixt_CategoricalDataStat.h"
#include "../../Param/mixt_SimpleParamStat.h"
#include "../../Likelihood/mixt_CategoricalLikelihood.h"
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
struct BridgeTraits<STK::Clust::Categorical_pjk_>
{
    /** Type of the data set to be used as input */
    typedef STK::Array2D<int> Data;
    /** Type of the AugmentedData */
    typedef AugmentedData<Data> AugData;
    /** Type of the DataStat */
    typedef CategoricalDataStat DataStatComputer;
    /** Type of the DataStat */
    typedef Eigen::Matrix<std::vector<std::pair<int, STK::Real> >,
                          Eigen::Dynamic,
                          Eigen::Dynamic> DataStatStorage;
    /** Type of the Data */
    typedef Data::Type Type;
    /** Type of the mixture model */
    typedef Categorical_pjk Mixture;
    /** Sampler to be used for this specific mixture model */
    typedef CategoricalSampler Sampler;
    /** Type of Likelihood */
    typedef CategoricalLikelihood Likelihood;
};

template<typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>
struct CategoricalBridge_pjk_m
{
   typedef MixtureBridge<STK::Clust::Categorical_pjk_,
                         DataHandler,
                         DataExtractor,
                         ParamSetter,
                         ParamExtractor> type;
};

} /* namespace mixt */

#endif /* MIXT_CATEGORICALBRIDGES_H */
