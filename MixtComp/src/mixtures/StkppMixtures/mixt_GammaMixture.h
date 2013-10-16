/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

#ifndef MIXT_GAMMAMIXTURE_H
#define MIXT_GAMMAMIXTURE_H

#include "mixt_MixtureBridge.h"

namespace mixt
{

typedef MixtureBridge<STK::Gamma_pk_ajk_bjk<STK::Array2D<STK::Real> > > Gamma_pk_ajk_bjk;
typedef MixtureBridge<STK::Gamma_p_ajk_bjk<STK::Array2D<STK::Real> > > Gamma_p_ajk_bjk;
typedef MixtureBridge<STK::Gamma_pk_ajk_bj<STK::Array2D<STK::Real> > > Gamma_pk_ajk_bj;
typedef MixtureBridge<STK::Gamma_p_ajk_bj<STK::Array2D<STK::Real> > > Gamma_p_ajk_bj;

} /* namespace mixt */

#endif /* MIXT_GAMMAMIXTURE_H_ */
