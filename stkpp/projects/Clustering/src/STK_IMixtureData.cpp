/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

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
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_IMixtureData.cpp
 *  @brief In this file we implement the interface class IMixtureData.
 **/

#include "../include/STK_IMixtureData.h"

namespace STK
{
/* default constructor. */
IMixtureData::IMixtureData(std::string const& idData): nbVariable_(0), v_missing_(), idData_(idData) {}
/* copy constructor
 *  @param manager the IMixtureData to copy
 **/
IMixtureData::IMixtureData( IMixtureData const& manager)
                          : nbVariable_(manager.nbVariable_)
                          , v_missing_(manager.v_missing_)
                          , idData_(manager.idData_)
{}

} // namespace STK

