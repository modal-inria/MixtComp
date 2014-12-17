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
 * Project:  STKernel::Base
 * Purpose:  Define the fundamental type Char.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Char.h
 *  @brief In this file we define the fundamental type Char.
 **/

#ifndef STK_CHAR_H
#define STK_CHAR_H

#include <limits>
#include "STK_Base_Util.h"

namespace STK
{
// forward declarations
template<typename Type> struct Arithmetic;
template<typename Type> struct IdTypeImpl;

/** @ingroup Base
 *  @brief STK fundamental type of a Char.
 *
 *  The type Char is defined for the internal representation
 *  of the characters. Note that if you change the representation,
 *  you don't have to modify the stream classes defined in
 *  the file STK_StreamBase.h, but you have to modify the
 *  global stream objects std::cout, std::cin, std::cerr, std::clog
 *  in order to access to the standard I/O channels.
 **/
#ifdef STK_UNICODE

typedef wchar_t Char;

/** Transform x to wchar_t
 **/
#define _T(x) L ## x

/** Standard stk output stream
 **/
#define stk_cout std::wcout

/** Standard stk input stream
 **/
#define stk_cin  std::wcin

/** Standard stk error stream
 **/
#define stk_cerr std::wcerr

/** Standard stk log stream
 **/
#define stk_clog std::wclog

#else // !Unicode

typedef char Char;

/** Let x unmodified
 **/
#define _T(x) x

/** Standard stk output stream
 **/
#define stk_cout std::cout

/** Standard stk input stream
 **/
#define stk_cin  std::cin

/** Standard stk error stream
 **/
#define stk_cerr std::cerr

/** Standard stk log stream
 **/
#define stk_log std::clog

#endif // STK_UNICODE

/** @ingroup Arithmetic
 *  @brief Specialization of the struct Arithmetic for Char.
 *
 * The STK fundamental type Char does not have NA value. If the user
 * ask for a NA value, the method return 0.
 */
template<>
struct Arithmetic<Char> : public std::numeric_limits<Char>
{
  /** Adding a Non Available (NA) special number.
   *  @return the 0 character
   **/
  static Char NA() throw() { return static_cast<Char>(0); }

  /** True if the type has a representation for a "Not Available."
   **/
  static const bool hasNA = false;

  /** Test if x is a Non Available (NA) Char.
   *  @param x the Char to test.
   *  @return always @c false as the Char type does not have NA value.
   **/
  static bool isNA(const Char& x) throw() { return false; }

  /** Test if x is  infinite.
   *  @param x the Char to test.
   *  @return always @c false
   **/
  static bool isInfinite(const Char& x) throw() { return false; }

  /** Test if x is  finite.
   *  @param x the Char to test.
   *  @return always @c true
   **/
  static bool isFinite(const Char& x) throw() { return true; }
};

/** @ingroup RTTI
 *  @brief Specialization of the IdTypeImpl for the Type Char.
 *  This struct return the IdType of a Char.
 **/
template<>
struct IdTypeImpl<Char>
{
  /** Give the IdType of the variable.
   *  @return the IdType
   **/
  static Base::IdType returnType()  { return(Base::character_);}
};


} // namespace STK

#endif /*STK_CHAR_H*/
