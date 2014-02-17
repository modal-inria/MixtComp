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
 * Project:  Base
 * Purpose:  Utility for computing elapsed time.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Chrono.h
 *  @brief In this file we define the Chrono namespace and it's
 *  utilities functions.
 **/


#ifndef STK_CHRONO_H
#define STK_CHRONO_H

#include "STK_Real.h"

namespace STK
{
/** @ingroup Base
 *  @brief The namespace Chrono is an utility namespace for computing
 *  elapsed time during the execution of a program.
 **/
namespace Chrono
{
  /** @ingroup Base
   *  @brief Start the Chrono. */
  void start();

  /** @ingroup Base
   *  @brief Compute the elapsed time (in second) since last @c start() call.
   *  @return the elapsed time
   **/
  Real elapsed();
} // namespace Chrono

} // namespace STK

#endif /*STK_CHRONO_H*/
