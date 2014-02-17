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
 * created on: 7 nov. 2009
 * Purpose:  implement the Tool class.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Tool.cpp
 *  @brief In this file we implement the Tool class.
 **/

#include "../include/STK_Tool.h"

namespace STK
{

Tool::Tool( String const& aname,
            String const& xml,
            String const& icon_name,
            Type atype)
          : name_(aname), xml_(xml), icon_name_(icon_name), type_(atype)
{ }

Tool::~Tool()
{ }

}
