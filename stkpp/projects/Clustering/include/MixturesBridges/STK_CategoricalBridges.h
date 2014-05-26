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

/** @file STK_CategoricalBridges.h
 *  @brief In this file we define the Gaussian bridges of the Clustering project.
 **/


#ifndef STK_CATEGORICALBRIDGES_H
#define STK_CATEGORICALBRIDGES_H


#include "../CategoricalMixtureModels/STK_Categorical_pjk.h"
#include "../CategoricalMixtureModels/STK_Categorical_pk.h"

namespace STK
{

// forward declaration
template<int Id> class MixtureBridge;

namespace Clust
{
/** @ingroup Clustering
 *  specialization of the BridgeTraits for the Categorical_pjk model
 **/
template<>
struct BridgeTraits<Clust::Categorical_pjk_>
{
   /** Type of the data set to used as input */
   typedef Array2D<int> Data;
   /** Type of the parameter set to used as output */
   typedef Array2D<int> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef Categorical_pjk<Data> Mixture;
};
/** @ingroup Clustering
 *  specialization of the BridgeTraits for the Categorical_pk model
 **/
template<>
struct BridgeTraits<Clust::Categorical_pk_>
{
   /** Type of the data set to used as input */
   typedef Array2D<int> Data;
   /** Type of the parameter set to used as output */
   typedef Array2D<int> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef Categorical_pk<Data> Mixture;
};

} // namespace Clust

typedef MixtureBridge<Clust::Categorical_pjk_> CategoricalBridge_pjk;
typedef MixtureBridge<Clust::Categorical_pk_> CategoricalBridge_pk;


} /* namespace STK */

#endif /* STK_CATEGORICALBRIDGES_H */
