/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * created on: 7 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Clust_Traits.h
 *  @brief In this file we define the MixtureModelImpl, the MixtureModelTraits
 *  and the BridgeTraits classes.
 **/


#ifndef STK_CLUST_TRAITS_H
#define STK_CLUST_TRAITS_H

namespace STK
{

/** @ingroup Clustering
 *  Main class for the maximization step implementation and for
 *  the random initialization of the parameters.
 *
 *  The MixtureModelImpl struct can be specialized for any
 *  models deriving from the IMixtureModel. Typically, it can be used
 *  for implementing the following method
 *  @code
 *    static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik);
 *  @endcode
 **/
template<class Array, class Parameter>
struct MixtureModelImpl;

namespace Clust
{
/** Main class for the mixtures traits policy.
 *  The traits struct MixtureModelTraits must be specialized for any
 *  Mixture deriving from the Interface IMixtureModel.
 **/
template <class Mixture> struct MixtureModelTraits;

/** Given the Id of a mixture model, the struct Traits allow to get
 *  type of the input and output data of the parameters. This Traits class is
 *  used by the Bridge classes.
 *  @sa MixtureBridge
 *   **/
template<int id> struct BridgeTraits;

} // namespace Clust


}  // namespace STK
#endif /* STK_BridgeTraits_H */
