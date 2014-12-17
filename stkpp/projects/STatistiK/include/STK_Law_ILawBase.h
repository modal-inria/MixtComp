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
 * Project:  stkpp::STatistiK::Law
 * Purpose:  Interface base class for all multivariate probabilities laws.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Law_ILawBase.h
 *  @brief In this file we define the interface base class for all
 *  multivariate probabilities laws ILawBase.
 **/

#ifndef STK_LAW_ILAWBASE_H
#define STK_LAW_ILAWBASE_H

#include "STKernel/include/STK_String.h"

namespace STK
{

namespace Law
{
/** @ingroup Laws
 *  @brief Interface base class for all the (univariate/multivariate)
 *  probabilities laws.
 *
 *  A general probability law possess a name. The parameters are specific
 *  to the law and will be set by reference.
 **/
class ILawBase
{
  protected:
    /** Constructor.
     *  @param name the name of the law
     **/
    inline ILawBase(String const& name) : name_(name) {}
    /** destructor. **/
    inline ~ILawBase() {}

  public:
    /** @return the name of the distribution */
    inline String const& name() const { return name_;}

  protected:
    /** Name of the Law. **/
    String name_;
};

} // namespace Law

} //  namespace STK

#endif /*STK_LAW_ILAWBASE_H*/
