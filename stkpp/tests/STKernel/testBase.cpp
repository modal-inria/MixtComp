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
 * Purpose:  test program for testing Base classes.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/** @file testBase.cpp
 *  @brief In this file we test the classes of the STKernel::Base subproject.
 **/

#include "STKernel.h"

using namespace STK;

template<typename Type>
int testTypedStream(String const& s1, String const& s2, String const& str)
{
  istringstream is(s1+STRING_BLANK+stringNa+STRING_BLANK+s2);
  stk_cout << _T("Testing type: ") << str << _T("\n");
  stk_cout << _T("With string: ") <<is.str() << _T("\n");
  Type x, y, z;
  int flag1 = !(is >> Proxy<Type>(x) >> Proxy<Type>(y) >> Proxy<Type>(z)).fail();
  String sx, sy, sz;
  sx = typeToString(x);
  sy = typeToString(y);
  sz = typeToString(z);
  int flag2 = ((stringToType<Type>(sx)==stringToType<Type>(s1))
             &&(sy==stringNa)
             &&(stringToType<Type>(sz)==stringToType<Type>(s2)));
  stk_cout << _T("Get: sx =") <<sx << _T(", sy =") << sy << _T(", sz =") << sz << _T("\n");
  stk_cout << _T("flag1 =")<< flag1 << _T(", flag2 =") << flag2 << _T("\n\n");
  return (flag1&&flag2);
}
template<typename Type>
void testNaValues(String const& str)
{
  stk_cout << _T("-----------------\nTesting type = ") << str << _T("\n");
  Type x;
  stk_cout << _T("stringToType(x, stringNa) = ") << stringToType(x, stringNa) << _T("\n");
  stk_cout << _T("x =")             << Proxy<Type>(x) << _T("\n");
  stk_cout << _T("isNA(x) =")       << STK::Arithmetic<Type>::isNA(x) << _T("\n");
  stk_cout << _T("isFinite(c) =") << STK::Arithmetic<Type>::isFinite(x) << _T("\n");
  stk_cout << _T("isInfinite(c) =") << STK::Arithmetic<Type>::isInfinite(x) << _T("\n");
}
template<typename Type>
void testStringToType(String const& str)
{
  stk_cout << _T("Test with string =====") << str << _T("=====\n");
  Type res = stringToType<Type>(str);
  stk_cout << _T("result =") << Proxy<Type>(res) << _T("\n");
}

/* main. */
int main(int argc, char *argv[])
{
  int count = 0;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::Base                                      +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test Real Type                                      +\n");
    String s1(_T("3.14")), s2(_T("0.01"));
    count -= testTypedStream<Real>(s1, s2, _T("Real"));
    s1 = _T("-.14"); s2= _T("0.01");
    count -= testTypedStream<Real>(s1, s2, _T("Real"));
    testStringToType<Real>(stringNa);
    testStringToType<Real>(" "+ stringNa);
    testStringToType<Real>(" "+ stringNa + " ");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test Integer Type                                   +\n");
    s1 = _T("-2"); s2 = _T("101");
    count -= testTypedStream<Integer>(s1, s2, _T("Integer"));
    s1 = _T("-.2"); s2 = _T("101");
    count -= testTypedStream<Integer>(s1, s2, _T("Integer"));
    testStringToType<Real>(stringNa);
    testStringToType<Real>(" "+ stringNa);
    testStringToType<Real>(" "+ stringNa + " ");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test Binary Type                                    +\n");
    s1 = _T("1"); s2 = _T("0");
    count -= testTypedStream<Binary>(s1, s2, _T("Binary"));
    s1 = _T("-1"); s2 = _T("0");
    count -= testTypedStream<Binary>(s1, s2, _T("Binary"));
    testStringToType<Real>(stringNa);
    testStringToType<Real>(" "+ stringNa);
    testStringToType<Real>(" "+ stringNa + " ");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test Sign Type                                      +\n");
    s1 = _T("1"); s2 = _T("-1");
    count -= testTypedStream<Sign>(s1, s2, _T("Sign"));
    s1 = _T("1"); s2 = _T("0");
    count -= testTypedStream<Sign>(s1, s2, _T("Sign"));

    stk_cout << _T("\nNumber of failure=") << count;



    // second test
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Testing NA values for all types:\n");
    testNaValues<Char>(_T("Char"));
    testNaValues<String>(_T("String"));
    testNaValues<Real>(_T("Real"));
    testNaValues<Integer>(_T("Integer"));
    testNaValues<Binary>(_T("Binary"));
    testNaValues<Sign>(_T("Sign"));
    testNaValues<Range>(_T("Range"));

    // fourth test
    stk_cout << _T("\n\n");
    stk_cout << _T("Testing Range:\n");
    Range I;
    String ranges = "2:5";
    stk_cout << _T("Test with string =") << ranges << _T("\n");
    stringToType(I, ranges);
    stk_cout << _T("Range = ")  << typeToString(I)<< _T("\n");
    ranges = " 1 : 8 ";
    stk_cout << _T("Test with string =") << ranges << _T("\n");
    stringToType(I, ranges);
    stk_cout << _T("Range = ")  << typeToString(I)<< _T("\n");
    ranges = " 12 ";
    stk_cout << _T("Test with string =") << ranges << _T("\n");
    stringToType(I, ranges);
    stk_cout << _T("Range = ")  << typeToString(I)<< _T("\n");
    ranges = " >2 ";
    stk_cout << _T("Test with string =") << ranges << _T("\n");
    stringToType(I, ranges);
    stk_cout << _T("Range = ")  << typeToString(I)<< _T("\n");
    ranges = " 3:     >2 ";
    stk_cout << _T("Test with string =") << ranges << _T("\n");
    stringToType(I, ranges);
    stk_cout << _T("Range = ")  << typeToString(I)<< _T("\n");
    ranges = " 5:     2>";
    stk_cout << _T("Test with string =") << ranges << _T("\n");
    stringToType(I, ranges);
    stk_cout << _T("Range = ")  << typeToString(I)<< _T("\n");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK::Base      +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << _T("\n";);
  }
  return 0;
}
