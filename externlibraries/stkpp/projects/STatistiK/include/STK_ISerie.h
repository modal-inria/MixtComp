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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  Analysis
 * Purpose:  Define the Interface Serie base class.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_ISerie.h
 *  @brief In this file we define Interface base class for Real Series.
 **/

#ifndef STK_ISERIE_H
#define STK_ISERIE_H

#include "../../STKernel/include/STK_Real.h"

namespace STK
{

/** @ingroup Analysis
 *  @brief Interface base class for Series.
 *
 * The ISerie class is the templated base class for all
 * Series.
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
    inline Real firstIdx() const;
    // return the next coefficient: firstIdx() have to be called before
    //  any use of next()
    inline Real next() const;
 * @endcode
 **/
template <class Serie>
class ISerie
{
  protected:
    /** Default Constructor
     **/
    ISerie() { ;}

  public:
    /** Virtual destructor.
     **/
    virtual ~ISerie() { ;}

    /** static cast : return a reference of this with a cast to the
     * derived class.
     * This allow to delegate public methods to derived classes.
     **/
    inline Serie& asDerived()
    { return static_cast<Serie&>(*this); }

    /** static cast : return a const reference of this with a cast to
     * the derived class.
     * This allow to delegate public method to derived classes.
     **/
    inline const Serie& asDerived() const
    { return static_cast<Serie const &>(*this); }

    /** return the first coefficient
     **/
    inline Real firstIdx() const
    { return asDerived().firstIdx();}

    /** return the next coefficient: firstIdx() have to be called before
     *  any use of next()
     **/
    inline Real next() const
    { return asDerived().next();}
};

} // namespace STK

#endif /*STK_ISERIE_H*/
