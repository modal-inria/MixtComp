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
/*--------------------------------------------------------------------*/
/* $Id: STK_Category.cpp,v 1.1 2010/02/22 14:31:37 siovleff Exp $
 *
 * Project:  stkpp::gui::model
 * created on: 1 nov. 2009
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 * $Log: STK_Category.cpp,v $
 * Revision 1.1  2010/02/22 14:31:37  siovleff
 * Adding Tool and Category class to the model
 *
 **/
/*--------------------------------------------------------------------*/
/** @file STK_Category.cpp
 *  @brief In this file we implement the Category class.
 **/

#include "../include/STK_Category.h"

namespace STK
{

Category::Category( String const& aname, Type atype)
                  : name_(aname), type_(atype)
{}

Category::~Category()
{ }

} // namespace STK
