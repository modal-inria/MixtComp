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

#ifndef MIXT_ENUM_H
#define MIXT_ENUM_H

#include<iostream>

namespace mixt
{

enum Mixture
{
  rankCluster_,
  Gamma_ajk_bjk_,
  Gamma_ajk_bj_,
  Gaussian_sjk_,
  Gaussian_s_,
  unknown_mixture_
};

struct MixtureParam
{
    Mixture mixtureType;
    int firstIndex;
    int lastIndex;
};

/** @ingroup Reduct
 *  convert a String to a TypeReduction.
 *  @param type the String we want to convert
 *  @return the TypeReduction represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
Mixture StringToTypeReduction( std::string const& type);

/** @ingroup Reduct
 *  convert a TypeReduction to a String.
 *  @param type the type of reduction we want to convert
 *  @return the string associated to this type.
 **/
std::string TypeReductionToString( Mixture const& type);

} // namespace mixt

#endif /* MIXT_ENUM_H */
