/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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
 * Project:  stkpp::
 * created on: Sep 23, 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_MetaTemplate.h
 *  @brief This file contains generic meta-programming classes which are not
 *  specifically related to STK++.
 **/


#ifndef STK_METATEMPLATE_H
#define STK_METATEMPLATE_H

namespace STK
{
namespace hidden
{

struct TrueType {  enum { value = 1 }; };
struct FalseType { enum { value = 0 }; };

template<bool Condition, typename Then, typename Else>
struct If;

template<typename Then, typename Else>
struct If <true, Then, Else> { typedef Then Result; };

template<typename Then, typename Else>
struct If <false, Then, Else> { typedef Else Result; };

/* C/C++ fundamental Types */
template<typename T>
struct isArithmetic { enum { yes = false, no = true}; };

/* specializations */
template<> struct isArithmetic<float>         { enum { yes = true, no = false }; };
template<> struct isArithmetic<double>        { enum { yes = true, no = false }; };
template<> struct isArithmetic<long double>   { enum { yes = true, no = false }; };

template<> struct isArithmetic<bool>          { enum { yes = true, no = false }; };
template<> struct isArithmetic<char>          { enum { yes = true, no = false }; };
template<> struct isArithmetic<signed char>   { enum { yes = true, no = false }; };
template<> struct isArithmetic<unsigned char> { enum { yes = true, no = false }; };
template<> struct isArithmetic<signed short>  { enum { yes = true, no = false }; };
template<> struct isArithmetic<unsigned short>{ enum { yes = true, no = false }; };
template<> struct isArithmetic<signed int>    { enum { yes = true, no = false }; };
template<> struct isArithmetic<unsigned int>  { enum { yes = true, no = false }; };
template<> struct isArithmetic<signed long>   { enum { yes = true, no = false }; };
template<> struct isArithmetic<unsigned long> { enum { yes = true, no = false }; };
//template<> struct isArithmetic<signed long long>   { enum { yes = true, no = false }; };
//template<> struct isArithmetic<unsigned long long> { enum { yes = true, no = false }; };

/* C/C++ fundamental Types */
template<typename T>
struct isInt { enum { yes = false, no = true}; };

template<> struct isInt<bool>          { enum { yes = true, no = false }; };
template<> struct isInt<char>          { enum { yes = true, no = false }; };
template<> struct isInt<signed char>   { enum { yes = true, no = false }; };
template<> struct isInt<unsigned char> { enum { yes = true, no = false }; };
template<> struct isInt<signed short>  { enum { yes = true, no = false }; };
template<> struct isInt<unsigned short>{ enum { yes = true, no = false }; };
template<> struct isInt<signed int>    { enum { yes = true, no = false }; };
template<> struct isInt<unsigned int>  { enum { yes = true, no = false }; };
template<> struct isInt<signed long>   { enum { yes = true, no = false }; };
template<> struct isInt<unsigned long> { enum { yes = true, no = false }; };
//template<> struct isInt<signed long long>   { enum { yes = true, no = false }; };
//template<> struct isInt<unsigned long long> { enum { yes = true, no = false }; };

/** check if T and U are of the same type. */
template<typename T, typename U> struct isSame { enum { value = 0 }; };
template<typename T> struct isSame<T,T> { enum { value = 1 }; };


/** @ingroup hidden
  * @brief Convenient struct to get the result Type of a unary or binary functor
  * (taken from Eigen).
  *
  * It supports both the current STL mechanism (using the result_type member)
  * as well as upcoming next STL generation (using a templated result member).
  * If none of these members is provided, then the Type of the first argument
  * is returned.
  */
template<typename T> struct result_of {};

struct has_none {int a[1];};
struct has_std_result_type {int a[2];};
struct has_tr1_result {int a[3];};

template<typename Func, typename ArgType, int SizeOf=sizeof(has_none)>
struct unary_result_of_select {typedef ArgType Type;};

template<typename Func, typename ArgType>
struct unary_result_of_select<Func, ArgType, sizeof(has_std_result_type)>
{typedef typename Func::result_type Type;};

template<typename Func, typename ArgType>
struct unary_result_of_select<Func, ArgType, sizeof(has_tr1_result)>
{typedef typename Func::template result<Func(ArgType)>::Type Type;};


template<typename Func, typename ArgType>
struct result_of<Func(ArgType)>
{
  template<typename T>
  static has_std_result_type testFunctor(T const *, typename T::result_type const * = 0);
    template<typename T>
    static has_tr1_result      testFunctor(T const *, typename T::template result<T(ArgType)>::Type const * = 0);
    static has_none            testFunctor(...);

    // note that the following indirection is needed for gcc-3.3
    enum {FunctorType = sizeof(testFunctor(static_cast<Func*>(0)))};
    typedef typename unary_result_of_select<Func, ArgType, FunctorType>::Type Type;
};

template<typename Func, typename Type1, typename Type2, int SizeOf=sizeof(has_none)>
struct binary_result_of_select {typedef Type1 Type;};

template<typename Func, typename Type1, typename Type2>
struct binary_result_of_select<Func, Type1, Type2, sizeof(has_std_result_type)>
{typedef typename Func::result_type Type;};

template<typename Func, typename Type1, typename Type2>
struct binary_result_of_select<Func, Type1, Type2, sizeof(has_tr1_result)>
{typedef typename Func::template result<Func(Type1,Type2)>::Type Type;};

template<typename Func, typename Type1, typename Type2>
struct result_of<Func(Type1,Type2)>
{
    template<typename T>
    static has_std_result_type testFunctor(T const *, typename T::result_type const * = 0);
    template<typename T>
    static has_tr1_result      testFunctor(T const *, typename T::template result<T(Type1,Type2)>::Type const * = 0);
    static has_none            testFunctor(...);

    // note that the following indirection is needed for gcc-3.3
    enum {FunctorType = sizeof(testFunctor(static_cast<Func*>(0)))};
    typedef typename binary_result_of_select<Func, Type1, Type2, FunctorType>::Type Type;
};

/** @ingroup hidden
  * Convenient struct to Promote the result Type of some binary functors.
  */
template<typename Type1, typename Type2>
struct Promote
{ typedef typename If<(sizeof(Type1) > sizeof(Type2)), Type1, Type2>::Result result_type;};
/** @ingroup STKernel
  * Specialization when we have the same type.
  */
template<typename Type>
struct Promote<Type, Type>
{ typedef Type result_type;};


}// namespace hidden

} // namespace STK

#endif /* STK_METATEMPLATE_H_ */
