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

namespace STK
{

void IDataHandler::writeInfo(ostream& os) const
{
  // show content
  for (InfoMap::const_iterator it=info_.begin(); it!=info_.end(); ++it)
  os << _T("IdData: ") << it->first << _T(", IdModel: ") << it->second << _T('\n');
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
#ifdef STK_MIXTURE_DEBUG
       stk_cerr << _T("In IDataHandler::addInfo, There exists an idData with a different idModel.\n");
#endif
       return false;
     }
  }
  return true;
}

/* @brief Giving a the Id of a dataset, find the Id of the model.
 *  @param idData can be any string given by the user.
 *  @param idModel The Id of the model associated with the data
 *  (not modified if idData is not present in the map).
 *  @return @c false if there exists already an idData matched with an other
 *  idModel, @c true otherwise.
 **/
bool IDataHandler::getIdModel(std::string const& idData, std::string& idModel) const
{
  bool res = false;
  // show content
  for (InfoMap::const_iterator it=info_.begin(); it!=info_.end(); ++it)
  { if (it->first == idData) { idModel = it->second; res = true; break;}}
 return res;
}

} // namespace STK



