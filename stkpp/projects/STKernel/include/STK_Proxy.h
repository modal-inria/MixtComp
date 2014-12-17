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
 * Project:  stkpp::STKernel::Base
 * Purpose:  Define the Proxy classes for wrapping any types.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Proxy.h
 *  @brief In this file we define the proxy classes for wrapping
 *  the STKpp fundamental types.
 **/

#ifndef STK_PROXY_H
#define STK_PROXY_H

#include "Sdk/include/STK_MetaTemplate.h"
#include "STK_Global.h"

namespace STK
{
/** @ingroup Base
 *  The Proxy class allow to surdefine operators and methods for
 *  every kind of class without using dynamic_cast.
 *  This class allow:
 *  - To avoid the use of @c dynamic_cast
 *  - To surdefine the predefined operators (like << and >>) for the
 *  C++ fundamental types.
 *  - To handle in a transparent way the NA values.
 **/
 template<class Type>
 class Proxy
 {
   protected:
     Type& x_; ///< A reference on the object wrapped
     Type const& y_; ///< A reference on the object wrapped

   public:
     /** constructor : create a reference of the Real x.
      *  Avoid the case x is of type Type const and (thus ambiguity in the
      *  choice of the constructor) using sfinae.
      *  @param x the object to wrap
      **/
     template< typename U >
     Proxy( typename hidden::If< hidden::isSame<Type const, U>::value, Type, U>::Result& x)
          : x_(x), y_(x) {}
     /** constructor : create a reference of the Real x.
      *  @param x the object to wrap
      */
     inline Proxy(Type const& x) : x_(const_cast<Type&>(x)), y_(x) {}
     /** copy constructor
      *  @param p the object to copy
      **/
     inline Proxy(Proxy const& p) : x_(p.x_), y_(p.y_) {}
     /** copy operator
      *  @param p the proxy to copy
      */
     Proxy& operator=(Proxy const& p) { x_ = p.x_; return *this;}
     /** destructor.*/
     inline ~Proxy() {}
    /** Conversion operator to Type.*/
     inline operator Type &() { return x_;}
     /** Constant Conversion operator to Type. */
     inline operator Type const &() const { return y_;}
     /** @brief Overloading of the operator >> for the type Type using a
      *  Proxy. All input stream should use a Proxy in a STK application.
      *  For the enumerated and String types, we have to overload the method.
      *  Due to the instruction
      *  @code
      *   is >> buff
      *  @endcode
      *  this operator will only work for the fundamental C/C++ types. For the
      *  other types, the operator
      *  @code
      *    operator >> (istream& is, Type& input);
      *  @endcode
      *  have to be implemented.
      *  @param is the input stream
      *  @param p the value to get from the stream
      **/
     friend istream& operator >> (istream& is, Proxy<Type> p)
     {
       // get current file position
       std::ios::pos_type pos = is.tellg();
       // failed to read directly the Type value
       if ( (is >> p.x_).fail() )
       {
         p.x_ = Arithmetic<Type>::NA();
         // clear and reset position
         is.clear(); is.seekg(pos);
         // Try to read a NA value, in all case value is a NA object
         Char* buffer = new Char[stringNaSize+1];

         if (is.get(buffer,stringNaSize+1).fail())
         { is.clear(); is.seekg(pos); is.setstate(std::ios::failbit);}
         else if (!(stringNa.compare(buffer) == 0))
              { is.clear(); is.seekg(pos); is.setstate(std::ios::failbit);}

         delete[] buffer;
       }
       return is;
     }
     /** @brief Overloading of the operator << for the type Type using a
      *  constant Proxy. All output stream should use a Proxy in
      *  a STK application. For the enumerated types, we have also to define
      *  the standard output.
      *  @param os the output stream
      *  @param p the value to send to the stream
      **/
     inline friend ostream& operator << (ostream& os, Proxy<Type> const& p)
     { return Arithmetic<Type>::isNA(p.y_) ? (os <<  stringNa) : (os << p.y_);}
};

 /** @ingroup Base
  *  Specialization of the Proxy class for String.
  **/
  template<>
  class Proxy<String>
  {
    protected:
      String& x_; ///< A reference on the String wrapped
      String const& y_; ///< A constant reference on the String wrapped

    public:
      /** constructor : create a reference of the String x.
       *  @param x the object to wrap
       **/
      inline Proxy(String &x) : x_(x), y_(x) {}
      /** constructor : create a constant reference of the String x.
       *  @param x the object to wrap
       */
      inline Proxy(String const& x) : x_(const_cast<String&>(x)), y_(x) {}
      /** copy constructor
       *  @param p the object to copy
       **/
      inline Proxy(Proxy const& p) : x_(p.x_), y_(p.y_) {}
      /** copy operator
       *  @param p the proxy to copy
       */
      Proxy& operator=(Proxy const& p) { x_ = p.x_; return *this;}
      /** destructor.*/
      inline ~Proxy() {}
     /** Conversion operator to String.*/
      inline operator String &() { return x_;}
      /** Constant Conversion operator to String. */
      inline operator String const &() const { return y_;}
      /** @brief Overloading of the operator >> for the type String using a
       *  Proxy. For String type we just check that we have not read a NA value.
       *  @param is the input stream
       *  @param p the value to get from the stream
       **/
      friend istream& operator >> (istream& is, Proxy<String> p)
      {
        String buff;
        is >> std::skipws >> buff;
        p = (buff  == stringNa) ? Arithmetic<String>::NA() : buff;
        return is;
      }
      /** @brief Overloading of the operator << for the type String using a
       *  constant Proxy.
       *  @param os the output stream
       *  @param p the value to send to the stream
       **/
      inline friend ostream& operator << (ostream& os, Proxy<String> const& p)
      { return Arithmetic<String>::isNA(p.x_) ? (os <<  stringNa) : (os << p.y_);}
 };

} // namespace STK

#endif /*STK_PROXY_H*/
