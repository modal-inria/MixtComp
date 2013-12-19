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
 * Project:  MixtComp
 * created on: 7 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file mixt_IngredientTraits.h
 *  @brief In this file
 **/


#ifndef MIXT_INGREDIENT_TRAITS_H
#define MIXT_INGREDIENT_TRAITS_H

namespace mixt
{

enum Ingredient
{
  Gamma_ajk_bjk_,
  Gamma_ajk_bj_,
  Gaussian_sjk_,
  Gaussian_s_,
  unknown_mixture_
};

/** Given the Id of a mixture model, the struct Traits allow to get
 *  type of the input and output data of the parameters.
 **/
template<int Id> struct IngredientTraits;

}  // namespace mixt

#endif // MIXT_INGREDIENT_TRAITS_H
