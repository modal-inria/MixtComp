/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::regress
 * created on: 11 juil. 2011
 * Purpose:  implement the utilities for the project regess.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Regress_Util.cpp
 *  @brief In this file we implement the utilities method of the project regress.
 **/

#include "../include/STK_Regress_Util.h"
#include "STKernel/include/STK_String.h"

namespace STK
{

namespace Regress
{
/* convert a String to a TypeReduction.
 *  @param type the type of reduction we want to define
 *  @return the TypeReduction represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
KnotsPosition stringToKnotsPosition( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("uniform")))  return uniform_;
  if (toUpperString(type) == toUpperString(_T("periodic"))) return periodic_;
  if (toUpperString(type) == toUpperString(_T("density"))) return density_;
  return unknownKnotsPosition_;
}

/* convert a TypeReduction to a String.
 *  @param type the type of reduction we want to convert
 *  @return the string associated to this type.
 **/
String knotsPositionToString( KnotsPosition const& type)
{
  if (type == uniform_)  return String(_T("uniform"));
  if (type == periodic_) return String(_T("periodic"));
  if (type == density_) return String(_T("density"));
  return String(_T("unknown"));
}

/* convert a String to a TypeRegression.
 *  @param type the type of regression we want to define
 *  @return the TypeRegression represented by the String @c type. if the string
 *  does not match any known name, the @c unknown_ type is returned.
 **/
TypeRegression stringToTypeRegression( String const& type)
{
  if (toUpperString(type) == toUpperString(_T("unknown"))) return unknown_;
  if (toUpperString(type) == toUpperString(_T("linear")))  return linear_;
  if (toUpperString(type) == toUpperString(_T("additiveBSpline"))) return additiveBSpline_;
  if (toUpperString(type) == toUpperString(_T("adaptiveBSpline"))) return adaptiveBSpline_;
  return unknown_;
}

/* convert a TypeRegression to a String.
 *  @param type the type of regression we want to convert
 *  @return the string associated to this type.
 **/
String TypeRegressionToString( TypeRegression const& type)
{
  if (type == linear_)  return String(_T("linear"));
  if (type == additiveBSpline_) return String(_T("additiveBSpline"));
  if (type == adaptiveBSpline_) return String(_T("adaptiveBSpline"));
  return String(_T("unknown"));
}

} // namespace Regress

} // namespace STK
