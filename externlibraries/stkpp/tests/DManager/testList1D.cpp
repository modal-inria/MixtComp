/*--------------------------------------------------------------------*/
/*   Copyright (C) 2004-2007  Serge Iovleff

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
 * Project: stkpp::Arrays
 * Purpose:  test program for testing Arrays classes.
 * Author:   Serge Iovleff, serge.iovleff@stkpp.org
 */

/** @file testArray1D.h
 *  @brief In this file we test the TContainer 1D classes.
 **/

#include "../../include/DManager.h"

using namespace STK;


template< class Type, template<class> class Container1D >
void write_List1D(const Container1D<Type>& C, String const& message, String const& name)
{
  stk_cout << message;
  stk_cout << name << _T(".range() = ") << C.range() << _T("\n");
  stk_cout << name << _T(".p_firstIdx() = ") << C.p_firstIdx() << _T("\n");
  stk_cout << name << _T(".p_lastIdx() = ") << C.p_lastIdx() << _T("\n");
  if (C.p_firstIdx() != 0)
    stk_cout << name << _T(".p_firstIdx()->getLeft() = ") << C.p_firstIdx()->getLeft() << _T("\n");
  if (C.p_lastIdx() != 0)
    stk_cout << name << _T(".p_lastIdx()->getRight() = ") << C.p_lastIdx()->getRight() << _T("\n");

  stk_cout << name << _T(".p_firstIdx() = ") << C.p_firstIdx() << _T("\n");
  stk_cout << name << _T(" = ") << C << _T("\n\n");
}

/* templated main test function for Array1D.                          */
template< class Type, template<class> class Container1D >
void test_List1D( int M, Range J, bool output)
{
  Container1D<Type> C;
  if (output)
  { write_List1D(C, _T("Test null constructor: C()\n"), _T("C"));}

  Container1D<Type> B(M,4.0);
  if (output)
  { write_List1D(B, _T("Test constructor:  B(M,4.0)\n"), _T("B"));}
 
  Container1D<Type> D(Range(0,M, 0)); D= 3.0;
  if (output)
  { write_List1D(D, _T("Test constructor: D(Range(0,M, 0)); D = 3.0\n"), _T("D"));}

  D.sub(J) = Type(5);
  if (output)
  { write_List1D(D, _T("Test D[J] = 5\n"), _T("D"));}

  D.merge(B);
  if (output)
  { write_List1D(D, _T("Test D.merge(B);\n"), _T("D"));}

  D.shift(1);
  if (output)
  { write_List1D(D, _T("Test D.shift(1)\n"), _T("D"));}

  D.insert(Range(2,2),2); D[2] = 1.0; D[3] = 4.0;
  if (output)
  { write_List1D(D, _T("Test D.insert(Range(2,2),2); D[2]=1.0; D[3]=4.0\n"), _T("D"));}

  D.pushBack(2); D.sub(Range(M,D.lastIdx(), 0)) = 1.0;
  if (output)
  { stk_cout << _T("Range(M,D.lastIdx(), 0) =") << Range(M,D.lastIdx(), 0) << "\n";
    write_List1D(D, _T("Test D.pushBack(2); D.sub(Range(M,D.lastIdx(), 0)) = 1.0;\n"), _T("D"));}

  D.swap(D.firstIdx(),D.lastIdx());
  if (output)
  { write_List1D(D, _T("Test D.swap(D.firstIdx(),D.lastIdx())\n"), _T("D"));}

  D.popBack();
  if (output)
  { write_List1D(D, _T("Test D.popBack()\n"), _T("D"));}

  D.insertElt(D.lastIdx()); D[D.lastIdx()-1] = 4.0;
  if (output)
  { write_List1D(D, _T("Test D.insertElt(D.lastIdx()); D[D.lastIdx()-1] = 4.0\n"), _T("D"));}

  C = D;
  if (output)
  { write_List1D(D, _T("Test C = D\n"), _T("C"));}

  D.erase(2, D.size()-1);
  if (output)
  { write_List1D(D, _T("Test D.erase(2,D.size()-1)\n"), _T("D"));}

  D.insertElt(D.lastIdx());  D[D.lastIdx()-1] = 4.0;
  if (output)
  { write_List1D(D, _T("Test D.insertElt(D.lastIdx()); D[D.lastIdx()-1] = 4.0\n"), _T("D"));}

  D.popBack();
  if (output)
  { write_List1D(D, _T("Test D.popBack()\n"), _T("D"));}

  D.erase(D.lastIdx());
  if (output)
  { write_List1D(D, _T("Test D.erase(D.lastIdx())\n"), _T("D"));}
 
  D.pushBack(M); D = 5.0;
  if (output)
  { write_List1D(D, _T("Test D.pushBack(M); D=5.0\n"), _T("D"));}
 
  C.front() = -2.0; C.back()  = -2.0;
  if (output)
  { write_List1D(C, _T("Test C.front() = -2.0; C.back()  = -2.0;\n"), _T("C"));}

  C.at(J.firstIdx()) = C.front();  C.at(J.lastIdx()) = C.back();
  if (output)
  { write_List1D(C, _T("Test C.at(J.firstIdx()) = C.front();  C.at(J.lastIdx()) = C.back();;\n"), _T("C"));}

  C.pushBack(); C.back() = Type(2);
  if (output)
  { write_List1D(C, _T("Test C.push_back(2.0); C.back() = Type(2);\n"), _T("C"));}
 
  C.insert(Range(2,4, 0), -3.0);
  C.insertElt(1); C.at(1) = 1.0;
  if (output)
  { write_List1D(C, _T("Test C.insert(Range(2,4), -3.0); C.insertElt(1); C.at(1) = 1.0)\n"), _T("C"));}

  C.erase(J.firstIdx(), J.size());
  if (output)
  { write_List1D(C, _T("Test C.erase(J.firstIdx(), J.size())\n"), _T("C"));}
}


/* main. */
int main(int argc, char *argv[])
{
  int M;
  int Jbeg, Jend;
  bool output = true;
  
  if (argc < 5)
  {
    M=5;
    Jbeg = 2;
    Jend = 4;
  }
  else
  {
    M = atoi(argv[1]);
    Jbeg = atoi(argv[3]);
    Jend = atoi(argv[4]);
    if (argc == 6) output = atoi(argv[5]);
  }

  try
  {
    Range J(Jbeg, Jend, 0);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test STK::List1D                                   +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("Using M  = ") << M  << _T(" and J = ") << J << _T("\n");
    stk_cout << _T("\n\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("Test List1D<int> : \n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    test_List1D<int, List1D>(M, J, output);
    stk_cout << _T("\n\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("Test List1D<Real> : \n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    test_List1D<Real, List1D>(M, J, output);
    stk_cout << _T("\n\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for STK::List1D    +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception & error)
  {
    std::cerr << "An error occured : " << error.error() << "\n";
    return -1;
  }
  return 0;
}
