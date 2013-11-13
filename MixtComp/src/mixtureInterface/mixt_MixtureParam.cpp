/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

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

*/

/*
 * Project:  MixtComp
 * created on: Nov 13, 2013
 * Author:   Vincent KUBICKI
 **/

/** @file mixt_MixtureParam.cpp
 *  @brief In this file 
 **/

# include "mixt_MixtureParam.h"

namespace mixt
{

Mixture StringToTypeReduction( std::string const& type)
{
  if (type == "rankCluster")
    return rankCluster_;
  if (type == "Gamma_ajk_bjk")
    return Gamma_ajk_bjk_;
  if (type == "Gamma_ajk_bj")
    return Gamma_ajk_bj_;
  if (type == "Gaussian_sjk")
    return Gaussian_sjk_;
  if (type == "Gaussian_s")
    return Gaussian_s_;
  return unknown_mixture_;
}

std::string TypeReductionToString( Mixture const& type)
{
  if (type == rankCluster_)
    return "rankCluster";
  if (type == Gamma_ajk_bjk_)
    return "Gamma_ajk_bjk";
  if (type == Gamma_ajk_bj_)
    return "Gamma_ajk_bj";
  if (type == Gaussian_sjk_)
    return "Gaussian_sjk";
  if (type == Gaussian_s_)
    return "Gaussian_s";
  return "unknown_mixture";
}

} // namespace mixt
