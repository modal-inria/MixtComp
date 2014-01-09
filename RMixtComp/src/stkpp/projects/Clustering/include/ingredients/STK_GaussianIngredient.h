/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

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


#ifndef STK_GAUSSIANINGREDIENT_H
#define STK_GAUSSIANINGREDIENT_H

#include "STK_MixtureBridge.h"

#include "../GaussianMixtureModels/STK_Gaussian_sjk.h"
#include "../GaussianMixtureModels/STK_Gaussian_s.h"

namespace STK
{

namespace Clust
{
/** @ingroup Clustering
 *  specialization of the IngredientTraits for the gaussian_sjk_ model
 **/
template<>
struct IngredientTraits<Clust::Gaussian_sjk_>
{
   /** Type of the data set to used as input */
   typedef Array2D<STK::Real> Data;
   /** Type of the parameter set to used as output */
   typedef Array2D<STK::Real> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef Gaussian_sjk<Data> Ingredient;
};


/** @ingroup Clustering
 *  specialization of the IngredientTraits for the gaussian_s_ model
 **/
template<>
struct IngredientTraits<Clust::Gaussian_s_>
{
   /** Type of the data set to used as input */
   typedef Array2D<STK::Real> Data;
   /** Type of the parameter set to used as output */
   typedef Array2D<STK::Real> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef Gaussian_s<Data> Ingredient;
};

} // namespace Clust

typedef MixtureBridge<Clust::Gaussian_sjk_> IngredientGaussian_sjk;
typedef MixtureBridge<Clust::Gaussian_s_> IngredientGaussian_s;

} /* namespace STK */

#endif /* STK_GAUSSIANINGREDIENT_H */
