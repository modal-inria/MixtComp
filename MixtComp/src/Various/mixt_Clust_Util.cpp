/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: November 6, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#include "STKernel/include/STK_String_Util.h"
#include "mixt_Clust_Util.h"

namespace mixt
{

/* @ingroup Clustering
 *  convert a String to an Mixture.
 *  @param type the String we want to convert
 *  @return the Mixture represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_mixture_ type is returned.
 **/
Mixture stringToMixture(std::string const& type)
{
  if (type == "Gaussian_sjk") return Gaussian_sjk_;
  if (type == "Categorical_pjk") return Categorical_pjk_;
  return unknown_mixture_;
}

/* @ingroup Clustering
 *  convert a Mixture to a String.
 *  @param type the type of Mixture we want to convert
 *  @return the string associated to this type.
 **/
std::string mixtureToString(Mixture const& type)
{
  if (type == Gaussian_sjk_) return "Gaussian_sjk";
  if (type == Categorical_pjk_) return "Categorical_pjk";
  return "unknown";
}

} // namespace mixt


