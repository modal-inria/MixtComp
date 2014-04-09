/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 15 nov. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_GaussianBridges.h
 *  @brief In this file we define the Gaussian bridges of the Clustering project.
 **/


#ifndef STK_GAUSSIANBRIDGES_H
#define STK_GAUSSIANBRIDGES_H

#include "../GaussianMixtureModels/STK_Gaussian_sjk.h"
#include "../GaussianMixtureModels/STK_Gaussian_sk.h"
#include "../GaussianMixtureModels/STK_Gaussian_sj.h"
#include "../GaussianMixtureModels/STK_Gaussian_s.h"

namespace STK
{

// forward declaration
template<int Id> class MixtureBridge;

namespace Clust
{
/** @ingroup Clustering
 *  specialization of the BridgeTraits for the Gaussian_sjk model
 **/
template<>
struct BridgeTraits<Clust::Gaussian_sjk_>
{
   /** Type of the data set to used as input */
   typedef Array2D<Real> Data;
   /** Type of the parameter set to used as output */
   typedef Array2D<Real> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef Gaussian_sjk<Data> Mixture;
};
/** @ingroup Clustering
 *  specialization of the BridgeTraits for the Gaussian_sk model
 **/
template<>
struct BridgeTraits<Clust::Gaussian_sk_>
{
   /** Type of the data set to used as input */
   typedef Array2D<Real> Data;
   /** Type of the parameter set to used as output */
   typedef Array2D<Real> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef Gaussian_sk<Data> Mixture;
};
/** @ingroup Clustering
 *  specialization of the BridgeTraits for the Gaussian_sj model
 **/
template<>
struct BridgeTraits<Clust::Gaussian_sj_>
{
   /** Type of the data set to used as input */
   typedef Array2D<Real> Data;
   /** Type of the parameter set to used as output */
   typedef Array2D<Real> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef Gaussian_sj<Data> Mixture;
};
/** @ingroup Clustering
 *  specialization of the BridgeTraits for the Gaussian_s model
 **/
template<>
struct BridgeTraits<Clust::Gaussian_s_>
{
   /** Type of the data set to used as input */
   typedef Array2D<Real> Data;
   /** Type of the parameter set to used as output */
   typedef Array2D<Real> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef Gaussian_s<Data> Mixture;
};

} // namespace Clust

typedef MixtureBridge<Clust::Gaussian_sjk_> GaussianBridge_sjk;
typedef MixtureBridge<Clust::Gaussian_sk_> GaussianBridge_sk;
typedef MixtureBridge<Clust::Gaussian_sj_> GaussianBridge_sj;
typedef MixtureBridge<Clust::Gaussian_s_> GaussianBridge_s;

} /* namespace STK */

#endif /* STK_GAUSSIANBRIDGES_H */
