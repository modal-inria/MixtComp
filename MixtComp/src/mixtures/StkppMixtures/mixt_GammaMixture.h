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

#include "../../mixtureInterface/mixt_Traits.h"
#include "mixt_MixtureBridge.h"
#include "../../stkpp/projects/Clustering/include/GammaMixtureModels/STK_Gamma_ajk_bjk.h"

namespace mixt
{

/** Given the Id of a mixture model, the struct Traits allow to get
 *  type of the input data and of the parameters.
 **/
template<>
struct Traits<Gamma_ajk_bjk_>
{
   /** Type of the data set to used as input */
   typedef STK::Array2D<STK::Real> Data;
   /** Type of the parameter set to used as output */
   typedef STK::Array2D<STK::Real> Param;
   /** Type of the Data */
   typedef typename Data::Type Type;
   /** Type of the mixture model */
   typedef STK::Gamma_pk_ajk_bjk<Data> MixtureModel;
};

typedef typename Traits<Gamma_ajk_bjk_>::Data Array;
typedef MixtureBridge<Gamma_ajk_bjk_> Gamma_ajk_bjk;

} /* namespace mixt */

#endif /* MIXT_GAMMAMIXTURE_H_ */
