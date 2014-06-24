/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  stkpp::DManager
 * created on: 16 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_DataHandler.cpp
 *  @brief In this file we implement the DataHandler class.
 **/

#include "../include/STK_IDataHandler.h"
#include "Arrays/include/STK_Display.h"

namespace STK
{

void IDataHandler::writeInfo(ostream& os) const
{
  // show content
   for (InfoMap::const_iterator it=info_.begin(); it!=info_.end(); ++it)
     os << _T("name: ") << it->first << _T(", model: ") << it->second << _T('\n');
}

bool IDataHandler::addInfo(std::string const& idData, std::string const& idModel)
{
  // parse descriptor file
  std::pair<InfoMap::iterator,bool> ret;
  // check if identifer is already present
  ret = info_.insert(std::pair<std::string,std::string>(idData, idModel));
  // if name already exists, check if there is incoherence
  if (ret.second==false)
  {
     if (ret.first->second != idModel)
     {
#ifdef STK_DMANAGER_VERBOSE
       stk_cerr << _T("Incoherence in addInfo.\n");
#endif
       return false;
     }
  }
  return true;
}



} // namespace STK



