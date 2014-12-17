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
 * Project:  stkpp::Analysis
 * Purpose:  Define the Interface IFunction base class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_IFunction.h
 *  @brief In this file we define Interface base class for Real functions.
 **/

#ifndef STK_IFUNCTION_H
#define STK_IFUNCTION_H

#include "STKernel/include/STK_Real.h"
#include "Sdk/include/STK_IRecursiveTemplate.h"

namespace STK
{

/** @ingroup Analysis
 *  @brief Interface base class for functions.
 *
 * The IFunction class is the templated base class for all functions.
 *
 * IFunction takes a function template parameter. This parameter
 * is the type of the class which derives from it. For example
 * @code
 * class Function : public IFunction<Function>
 * {...}
 * @endcode
  *
 * The pseudo virtual function defined in this interface and to implement
 * have the following definition:
 * @code
    // return the the value of the function at x
    inline Real fImpl(Real x) const;
 * @endcode
 * Default implementation of the @c xmin() and @c xmax() methods is the
 * interval \f$(-\infty, +\infty)\f$.
 * This interface will mainly be used for minimization, maximization or zero
 * finding algorithms.
 **/
template <class Function>
class IFunction : public IRecursiveTemplate<Function>
{
  protected:
    /** Default Constructor*/
    inline IFunction() {}
    /** destructor.*/
    inline ~IFunction() {}

  public:
    /** @return the value of the function at x
     *  @param x a real value in the range (xmin, xmax).
     **/
    inline Real operator()(Real x) const { return this->asDerived().fImpl(x);}

    /** @return the minimal value of the function at x */
    inline Real xmin() const { return this->asDerived().xminImpl();}
    /** @return the maximal value of the function at x */
    inline Real xmax() const { return this->asDerived().xmaxImpl();}

    /** @return Default implementation of the minimal value */
    inline Real xminImpl() const { return -Arithmetic<Real>::max();}
    /** @return Default implementation of the maximal value */
    inline Real xmaxImpl() const { return Arithmetic<Real>::max();}
};

} // namespace STK

#endif /*STK_IFUNCTION_H*/
