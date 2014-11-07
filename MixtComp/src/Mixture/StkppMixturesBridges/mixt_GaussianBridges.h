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

#include "mixt_Gaussian_sjk.h"
#include "../../Sampler/mixt_GaussianSampler.h"
#include "../../Data/mixt_GaussianDataStat.h"
#include "../../Param/mixt_SimpleParamStat.h"
#include "../../Likelihood/mixt_GaussianLikelihood.h"

namespace mixt
{

// forward declaration

// forward declaration
template<int Id,
         typename DataHandler,
         typename DataExtractor,
         typename ParamExtractor> class MixtureBridge;

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
    /** Type of the DataStat computer */
    typedef GaussianDataStat DataStatComputer;
    /** Type of the DataStat storage */
    typedef std::map<int, std::map<int, STK::Array2DVector<STK::Real> > > DataStatStorage;
    /** Type of the ParamStat */
    typedef SimpleParamStat ParamStat;
    /** Type of the parameter set to be used as output */
    typedef STK::Array2D<STK::Real> Param;
    /** Type of the Data */
    typedef Data::Type Type;
    /** Type of the mixture model */
    typedef Gaussian_sjk Mixture;
    /** Sampler to be used for this specific mixture model */
    typedef GaussianSampler Sampler;
    /** Type of Likelihood */
    typedef GaussianLikelihood Likelihood;
};

template<typename DataHandler,
         typename DataExtractor,
         typename ParamExtractor>
struct GaussianBridge_sjk_m
{
   typedef MixtureBridge<STK::Clust::Gaussian_sjk_,
                         DataHandler,
                         DataExtractor,
                         ParamExtractor> type;
};

} /* namespace mixt */

#endif /* MIXT_GAUSSIANBRIDGE_H */
