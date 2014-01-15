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
 * Project:  stkpp::
 * created on: 7 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Clust_Traits.h
 *  @brief In this file we define the MixtureModelImpl class and the MixtureTraits class.
 **/


#ifndef STK_CLUST_TRAITS_H
#define STK_CLUST_TRAITS_H

namespace STK
{

/** @ingroup Clustering
 *  Main class for the maximization step implementation and for
 *  the random initialization of the parameters.
 *
 *  The MixtureModelImpl struct must be specialized for any
 *  models deriving from the IMixtureModel
 *  interfaces by implementing the following methods:
 *  @code
 *    static void randomInit(Array1D< Component* >& components);
 *    static void mStep(Array1D< Component* >& components, Array2D<Real> const* p_tik);
 *  @endcode
 **/
template<class Array, class Parameter>
struct MixtureModelImpl;

namespace Clust
{
/** Main class for the mixtures traits policy.
 *  The traits struct MixtureTraits must be specialized for any
 *  components deriving from the Interface IMixtureComponents.
 **/
template <class Mixture> struct MixtureTraits;

/** Given the Id of a mixture model, the struct Traits allow to get
 *  type of the input and output data of the parameters.
 **/
template<int id> struct IngredientTraits;

} // namespace hidden


}  // namespace STK
#endif /* STK_MIXTURETRAITS_H */
