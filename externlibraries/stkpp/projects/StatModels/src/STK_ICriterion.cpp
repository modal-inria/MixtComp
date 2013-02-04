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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::Model
 * created on: 22 juil. 2011
 * Purpose: implement the Interface base class ICriterion.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_ICriterion.h
 *  @brief In this file we implement the Interface base class ICriterion.
 **/

#include "../include/STK_ICriterion.h"

namespace STK
{
//Constructor
ICriterion::ICriterion( IModelBase const& model)
                      : p_model_(&model)
                      , value_(Arithmetic<Real>::NA())
{}

//Destructor
ICriterion::~ICriterion()
{}

} // namespace STK
