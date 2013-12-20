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
 * Project:  MixtComp
 * created on: 15 nov. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

#ifndef MIXT_GAMMAINGREDIENT_H
#define MIXT_GAMMAINGREDIENT_H

#include "mixt_MixtureBridge.h"

namespace mixt
{

typedef MixtureBridge<STK::Clust::Gamma_ajk_bjk_> IngredientGamma_ajk_bjk;
typedef MixtureBridge<STK::Clust::Gamma_ajk_bj_> IngredientGamma_ajk_bj;

} /* namespace STK */

#endif /* MIXT_GAMMAINGREDIENT_H */
