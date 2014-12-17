/*--------------------------------------------------------------------*/
/*     Copyright (C) 2013-2013  Serge Iovleff, Quentin Grimonprez

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::tests
 * created on: 3 juil. 2013
 * Author:   Quentin Grimonprez
 **/

/** @file testCG.cpp
 *  @brief In this file we test the Conjugate Gradient algorithm.
 **/

#include "Algebra.h"
#include "Arrays.h"
using namespace STK;

/** @ingroup Arrays */
struct TestMultiplicator
{
    CVectorX operator()(CVectorX const& x) const
    {
      CVectorX a;
      a = A_ * x;
      return   a ;
    }
    // with fixed size
    TestMultiplicator(int n = 3)
    {
      A_.resize(2*n, n);
      A_.randGauss();
      A_ = A_.transpose() * A_;
    }
    // fixed matrix
    TestMultiplicator(CArrayXX const& A) : A_(A) {}
    CArrayXX A_;
};

/** @ingroup Arrays */
struct IdMultiplicator
{
  CVectorX operator()(CVectorX const& x) const
  { return   x ;}
};

/** @ingroup Arrays */
struct DiagonalMultiplicator
{
  CVectorX operator()(CVectorX const& x) const
  {
    CVectorX a;
    a = M_ * x;
    return   a ;
  }
  DiagonalMultiplicator(CArrayXX const& A) : M_(A.rows())
  { for (int i = M_.begin(); i< M_.end() ;i++) M_[i]=1/A(i,i);}
  Array2DDiagonal<Real> M_;
};


int main()
{
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test GC                                          +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");

  stk_cout<<"test0:"<<_T("\n");
  TestMultiplicator functor0(3);
  stk_cout << functor0.A_ << _T("\n");

  stk_cout<<"test1 :"<<_T("\n");
  TestMultiplicator functor1(50);
  CVectorX b1(50);
  b1.randGauss();

  CG<TestMultiplicator,CVectorX> testcg1(functor1,b1);
  if (!testcg1.run())
  { stk_cout << "An errror occur:" << testcg1.error();}
  stk_cout<<"CG Residuals = " << testcg1.r().norm2() <<_T("\n");

  CVectorX bEstim = functor1(testcg1.x());
  Real res = (bEstim-b1).norm2();
  stk_cout<<"Computed Residuals = " << res <<_T("\n");

  /*-----------------------------*/
  stk_cout<<_T("\n")<<"test2 :"<<_T("\n");
  TestMultiplicator functor2(1000);
  CVectorX b2(1000);
  b2.randGauss();

  CG<TestMultiplicator,CVectorX> testcg2(functor2,b2);
  if (!testcg2.run())
  { stk_cout << "An errror occur:" << testcg2.error();}
  stk_cout<<"CG Residuals = " << testcg2.r().norm2() <<_T("\n");

  bEstim = functor2(testcg2.x());
  res = (bEstim-b2).norm2();
  stk_cout<<"Computed Residuals = " << res <<_T("\n");

  /*-----------------------------*/
  stk_cout<<_T("\ntest3\n");
  int n(3), p(3);
  CArrayXX data(n,p);
  CVectorX b3(n,2);
  b3[1]=5;
  data << 1, 2, 3
        , 2, 5, 2
        , 3, 2, 5;
  TestMultiplicator functor3(data);
  CG<TestMultiplicator,CVectorX> testcg3(functor3,b3);

  if (!testcg3.run())
  { stk_cout << "An errror occur:" << testcg3.error();}
  stk_cout<<"CG Residuals = " << testcg3.r().norm2() <<_T("\n");

  bEstim = functor3(testcg3.x());
  stk_cout<<"bEstim = " << bEstim <<_T("\n");
  res = (bEstim-b3).norm2();
  stk_cout<<"Computed Residuals = " << res <<_T("\n");


  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test PGC                                          +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");
  stk_cout<<"Preconditioned CG with Id :"<<_T("\n");
  IdMultiplicator idFunctor1;
  PCG<TestMultiplicator,IdMultiplicator,CVectorX> testpcg1(functor1, idFunctor1, b1);
  if (!testpcg1.run())
  { stk_cout << "An errror occur:" << testpcg1.error();}
  stk_cout<<"PCG Residuals = " << testpcg1.r().norm2() <<_T("\n");

  CVectorX bEstimPCG = functor1(testpcg1.x());
  Real resPCG = (bEstimPCG-b1).norm2();
  stk_cout<<"Computed Residuals = " << resPCG <<_T("\n");


  stk_cout<<"Preconditioned CG with Diag(A) :"<<_T("\n");
  DiagonalMultiplicator diagFunctor1(functor1.A_);
  PCG<TestMultiplicator,DiagonalMultiplicator,CVectorX> testpcg1b(functor1, diagFunctor1, b1);
  if (!testpcg1b.run())
  { stk_cout << "An errror occur:" << testpcg1b.error();}
  stk_cout<<"PCG Residuals = " << testpcg1b.r().norm2() <<_T("\n");

  CVectorX bEstimPCGb = functor1(testpcg1b.x());
  Real resPCGb = (bEstimPCGb-b1).norm2();
  stk_cout<<"Computed Residuals = " << resPCGb <<_T("\n");

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for STK::CG          +\n");
  stk_cout << _T("+ No errors detected.                                   +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("\n\n");
  return 0;
}
