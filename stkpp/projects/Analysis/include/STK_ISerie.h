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
 * Project:  Analysis
 * Purpose:  Define the Interface Serie base class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ISerie.h
 *  @brief In this file we define Interface base class for Real Series.
 **/

#ifndef STK_ISERIE_H
#define STK_ISERIE_H

#include "STKernel/include/STK_Real.h"
#include "Sdk/include/STK_IRecursiveTemplate.h"

namespace STK
{

/** @ingroup Analysis
 *  @brief Interface base class for Series.
 *
 * The ISerie class is the templated base class for all Series.
 * 
 * ISerie takes a Serie template parameter. This parameter
 * is the type of the class which derives from it. For exemple
 * @code
 * class Serie : public ISerie<Serie>
 * {...}
 * @endcode
  * 
 * The pseudo virtual function defined in this interface have the
 * following definition:
 * @code
    // return the first coefficient
    inline Real firstImpl() const;
    // return the next coefficient: first() have to be called before
    //  any use of nextImpl()
    inline Real next() const;
 * @endcode
 **/
template <class Serie>
class ISerie : public IRecursiveTemplate<Serie>
{
  protected:
    /** Default Constructor*/
    ISerie() {}
    /** destructor.*/
    ~ISerie() {}

  public:
    /** @return the first coefficient of the serie */
    inline Real first() const { return this->asDerived().firstImpl();}
    /** @return the next coefficient: first() have to be called before
     *  any use of next()
     **/
    inline Real next() const { return this->asDerived().nextImpl();}
};

} // namespace STK

#endif /*STK_ISERIE_H*/
