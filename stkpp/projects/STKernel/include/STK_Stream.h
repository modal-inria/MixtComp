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
 * Purpose:  Surdefine the std streams for the STK fundamental types.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file  STK_Stream.h
 *  @brief In this file we sur-define the standard streams with the
 *  STK fundamental type Char.
 **/

#ifndef STK_STREAM_H
#define STK_STREAM_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "STK_Char.h"

namespace STK
{
/**@ingroup iostream
*  @{
**/

/** ios for Char */
typedef std::basic_ios<Char>           ios;
/** streambuf for Char*/
typedef std::basic_streambuf<Char>     streambuf;
/** istream for Char*/
typedef std::basic_istream<Char>       istream;
/** ostream for Char*/
typedef std::basic_ostream<Char>       ostream;
/** iostream for Char*/
typedef std::basic_iostream<Char>      iostream;
/** stringbuf for Char */
typedef std::basic_stringbuf<Char>     stringbuf;
/** istringstream for Char */
typedef std::basic_istringstream<Char> istringstream;
/** ostringstream (ostringstream) for Char */
typedef std::basic_ostringstream<Char> ostringstream;
/** stringstream for Char */
typedef std::basic_stringstream<Char>  stringstream;
/** filebuf for Char */
typedef std::basic_filebuf<Char>       filebuf;
/** ifstream for Char */
typedef std::basic_ifstream<Char>      ifstream;
/** ofstream for Char */
typedef std::basic_ofstream<Char>      ofstream;
/***/
typedef std::basic_fstream<Char>       fstream;

/** @} */
} // namespace STK

#endif /* STK_STREAM_H */
