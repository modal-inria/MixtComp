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
 * Project:  Base
 * Purpose:  Surdefine the std streams for the STK fundamental types.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file  STK_Stream.h
 *  @brief In this file we sur-define the standard streams with the
 *  STK fundamental type Char.
 **/

#ifndef STK_IOSTREAM_H
#define STK_IOSTREAM_H

#include <iostream>
#include <sstream>
#include <fstream>

#include "STK_Char.h"

namespace STK
{
/**  @ingroup Base
  *  @defgroup stream STKernel::Base I/O stream declarations
  *
  *  Nearly all of the I/O classes are parameterized on the type of
  *  characters they read and write (The major exception is ios_base at
  *  the top of the hierarchy).
  *
  *  For ease of use, all of the basic_* I/O-related
  *  classes are given typedef names in the namespace STK. For example:
  * 
  *  @code
  *     typedef basic_ifstream<Char>  ifstream;
  *  @endcode
  *
  *  These declarations in the STK namespace would be very useful if you
  *  modifiy the built-in type representation of Char (say wchar_t).
  *  @{
 */

/** @ingroup stream
 *  ios for Char
 **/
typedef std::basic_ios<Char>           ios;

/** @ingroup stream
 *  streambuf for Char
 **/
typedef std::basic_streambuf<Char>     streambuf;

/** @ingroup stream
 *  istream for Char
 **/
typedef std::basic_istream<Char>       istream;

/** @ingroup stream
 *  ostream for Char
 **/
typedef std::basic_ostream<Char>       ostream;

/** @ingroup stream
 *  iostream for Char
 **/
typedef std::basic_iostream<Char>      iostream;

/** @ingroup stream
 *  stringbuf for Char
 **/
typedef std::basic_stringbuf<Char>     stringbuf;

/** @ingroup stream
 *  istringstream (istringstream) for Char
 **/
typedef std::basic_istringstream<Char> istringstream;

/** @ingroup stream
 *  ostringstream (ostringstream) for Char
 **/
typedef std::basic_ostringstream<Char> ostringstream;

/** @ingroup stream
 *  stringstream (stringstream) for Char
 **/
typedef std::basic_stringstream<Char>  stringstream;

/** @ingroup stream
 *  filebuf for Char
 **/
typedef std::basic_filebuf<Char>       filebuf;

/** @ingroup stream
 *  ifstream for Char
 **/
typedef std::basic_ifstream<Char>      ifstream;

/** @ingroup stream
 *  ofstream for Char
 **/
typedef std::basic_ofstream<Char>      ofstream;

/** @ingroup stream
 *  fstream for Char
 **/
typedef std::basic_fstream<Char>       fstream;

} // namespaceSTK

#endif /* STK_IOSTREAM_H */
