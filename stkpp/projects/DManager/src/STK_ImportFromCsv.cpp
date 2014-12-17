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
 * Project:  stkpp::DManager
 * Purpose:  Implementation of the class ImportFromCsv.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 */

/** @file STK_ImportFromCsv.cpp
 *  @brief In this file we implement the classes ImportFromCsv.
 **/

#include "STKernel/include/STK_Real.h"
#include "STKernel/include/STK_Exceptions.h"

#include "../include/STK_ImportFromCsv.h"

namespace STK
{

// Default constructor for ImportCsv
ImportFromCsv::ImportFromCsv( ReadWriteCsv const& import, Import::TypeImport type)
                            : p_dataFrame_(0)
							              , typeImport_(type)
                            , import_(import)
{}

/* Copy Constructor.
 *  @param import the ImportFromCsv to import
 **/
ImportFromCsv::ImportFromCsv( ImportFromCsv const& import)
                            : p_dataFrame_(import.p_dataFrame_)
                            , typeImport_(import.typeImport_)
                            , import_(import.import_)
{}

// virtual dtor
ImportFromCsv::~ImportFromCsv()
{ if (p_dataFrame_) delete p_dataFrame_; }

/* launch the conversion from the ReadWriteCsv to a Frame. */
bool ImportFromCsv::run()
{
  msg_error_ = _T("");
  if (!p_dataFrame_) p_dataFrame_ = new DataFrame;
  switch (typeImport_)
  {
    case Import::numeric_:
      return asNumeric();
      break;
    case Import::only_numeric_:
      return asOnlyNumeric();
      break;
    case Import::string_:
      return asString();
      break;
    default:
      return false;
      break;
  };
  return false; // avoid warning
}

/* launch the conversion from the ReadWriteCsv to a DataFrame. */
bool ImportFromCsv::asNumeric()
{
  try
  {
    // for each field Try a numeric conversion
    for (int j =import_.begin(); j <=import_.lastIdx(); j++)
    {
      Variable<Real>* pvReal = new Variable<Real>();
      // test number of successful conversion
      if (import<Real>(j, *pvReal)==import_.sizeRow(j))
      {  // if no failure add variable to the dataframe
         p_dataFrame_->pushBackVariable(pvReal);
      }
      else
      {
        delete pvReal; // delete varReal
        Variable<String>* pvString = import_.clone(j);
        p_dataFrame_->pushBackVariable(pvString);
      }
    }
  }
  catch(const Exception& error)
  {
    msg_error_  = error.error();
    msg_error_ += _T("\nIn ImportCsv::runNumeric()");
    return false;
  }
  return true;
}

/** launch the conversion from the ReadWriteCsv to a Frame. */
bool ImportFromCsv::asOnlyNumeric()
{
  try
  {
    // for each field Try a numeric conversion
    for (int j =import_.begin(); j <import_.end(); j++)
    {
      Variable<Real>* pvReal = new Variable<Real>;
      // test number of successful conversion
      if (import<Real>(j, *pvReal) == import_.sizeRow(j))
      {  // if no failure add variable to the dataframe
         p_dataFrame_->pushBackVariable(pvReal);
      }
      else delete pvReal;
    }
  }
  catch(const Exception& error)
  {
    msg_error_ += _T("Error in ImportCsv::runOnlyNumeric()\n");
    msg_error_ += error.error();
    return false;
  }
  return true;
}

/* launch the conversion from the ReadWriteCsv to a DataFrame. */
bool ImportFromCsv::asString()
{
  try
  {
    // for each field Try a numeric conversion
    for (int j =import_.begin(); j <=import_.lastIdx(); j++)
    {
      Variable<String>* pvString = import_.clone(j);
      p_dataFrame_->pushBackVariable(pvString);
    }
  }
  catch(const Exception& error)
  {
    msg_error_ += _T("Error in ImportCsv::runString()\n");
    msg_error_ += error.error();
    return false;
  }
  return true;
}


} // namespace STK
