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
 * Project:  stkpp::Sdk
 * created on: 14 déc. 2011
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Traits.h
 *  @brief In this file we define the main traits class we use for the
 *  STK++ Containers.
 **/


#ifndef STK_TRAITS_H
#define STK_TRAITS_H

namespace STK
{

namespace hidden
{
/** @ingroup hidden
 *  The traits struct Traits must be specialized for any 2D
 *  container derived from the interface classes ITContainer. For example:
 *  @code
 *  template<class TYPE>
 *  struct hidden::Traits< Array2D<TYPE> >
 *  {
 *    typedef TYPE          Type;
 *    typedef Array2DPoint<TYPE> Row;
 *    typedef Array1D<TYPE> Column;
 *  };
 *  @endcode
 */
template <typename Derived> struct Traits;

} // Sdk

} // namespace STK

#endif /* STK_SDK_TRAITS_H */
