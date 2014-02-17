/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Project:  stkpp::gui::model
 * created on: 6 oct. 2009
 * Purpose:  implementation of the PropertyDescriptor class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 * $Log: STK_PropertyDescriptor.cpp,v $
 * Revision 1.1  2009/10/13 20:50:57  siovleff
 * Initial import of the model project
 *
 **/

/** @file STK_PropertyDescriptor.cpp
 *  @brief In this file we implement the PropertyDescriptor Class
 **/

#include "../include/STK_PropertyDescriptor.h"

namespace STK
{

PropertyDescriptor::PropertyDescriptor( String const& id
                                      , String const& displayName
                                      , String const& category
                                      , String const& description
                                      , String const& helpId
                                      )
                                      : id_(id)
                                      , display_(displayName)
                                      , category_(category)
                                      , description_(description)
                                      , helpId_(helpId)
{ ;}

PropertyDescriptor::~PropertyDescriptor()
{ ;}

}
