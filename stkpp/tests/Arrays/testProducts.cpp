/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  stkpp::Arrays
 * created on: 1 janv. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file testProducts.cpp
 *  @brief In this file we test the behavior of the matricial product.
 **/

#include "STKernel.h"
#include "Arrays.h"

using namespace STK;

/** @ingroup Arrays
  * @brief allow to number to the element of an Arrays
  */
template <typename Type>
struct numberingVisitor
{
  Type value_;
  numberingVisitor() : value_(1) {};
  inline void operator() ( Type& value)
  { value = value_; ++value_;}
};

/** Utility function for numbering an array */
template<typename Derived>
void numbering(Derived& matrix)
{
  typedef typename hidden::Traits<Derived>::Type Type;
  numberingVisitor<Type> visitor;
  ArrayBaseApplier<Derived, numberingVisitor<Type> > arrayVisitor(matrix);
  arrayVisitor.apply(visitor);
}

CArrayXX tab1(200, 300), tab2(300, 300), res(200, 300), eps(200, 300);

/*--------------------------------------------------------------------*/
/* main. */
int main(int argc, char *argv[])
{
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test general products                               +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // time variables
    int iMin, jMin, iMax, jMax;
    Array2D<Real> lhs(22, 19), rhs(19, 100), res1, res2(22,100, 0.0), diff;
    numbering(lhs); numbering(rhs);

    Chrono::start();
    res1.move(mult(lhs, rhs));
    stk_cout << _T("Product Using MultOp done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");

    Chrono::start();
    hidden::bp<Array2D<Real>, Array2D<Real>, Array2D<Real> >::run(lhs, rhs, res2);
    stk_cout << _T("Product Using bp done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");

    diff = res1 - res2;
    stk_cout << _T("diff.max=") << diff.maxElt(iMax, jMax) << _T("\n");
    stk_cout << _T("diff.min=") << diff.minElt(iMin, jMin) << _T("\n");

    Chrono::start();
    res1 = lhs * rhs;
    stk_cout << _T("Product Using res1 = lhs * rhs done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    diff = res1 - res2;
    stk_cout << _T("diff.max=") << diff.maxElt(iMax, jMax) << _T("\n");
    stk_cout << _T("diff.min=") << diff.minElt(iMin, jMin) << _T("\n");

    Array2D<Real> blhs(202, 22), brhs(22, 102), bres1;
    numbering(blhs); numbering(brhs);
    Chrono::start();
    bres1.move(mult(blhs, brhs));
    stk_cout << _T("Product Using MultOp done. Elapsed time = ") << Chrono::elapsed() << _T("\n");

    CArrayXX clhs(202, 22), crhs(22, 102), cres2(202,102, 0.0);
    numbering(clhs); numbering(crhs);

    Chrono::start();
    hidden::pb<CArrayXX, CArrayXX, CArrayXX >::run(clhs, crhs, cres2);
    stk_cout << _T("Product Using MultOp done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");

    diff = bres1 - cres2;

    stk_cout << _T("diff.max=") << diff.maxElt(iMax, jMax) << _T("\n");
    stk_cout << _T("diff.min=") << diff.minElt(iMin, jMin) << _T("\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for general products +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test operator* : Dot Operation                      +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    Array2DVector<Real> v1(1000), v2(1000), v3, vdiff;
    numbering(v1); numbering(v2);
    Chrono::start();
    v3 = v1 *v2;
    stk_cout << _T("Product Using v3=v1*v2 done. Elapsed time = ") << Chrono::elapsed() << _T("\n");
    vdiff = v1 - v3.sqrt();
    stk_cout << _T("vdiff.max=") << vdiff.maxElt(iMax) << _T("\n");
    stk_cout << _T("vdiff.min=") << vdiff.minElt(iMin) << _T("\n");
    Array2DPoint<Real> p1(1000), p2(1000), p3, pdiff;
    numbering(p1); numbering(p2);
    Chrono::start();
    p3 = p1 *p2;
    stk_cout << _T("Product Using p3=p1*p2 done. Elapsed time = ") << Chrono::elapsed() << _T("\n");
    pdiff = p1 - p3.sqrt();
    stk_cout << _T("pdiff.max=") << pdiff.maxElt(iMax) << _T("\n");
    stk_cout << _T("pdiff.min=") << pdiff.minElt(iMin) << _T("\n");

    stk_cout << _T("Product Using p3=p1*p2 done. Elapsed time = ") << Chrono::elapsed() << _T("\n");

    Chrono::start();
    CArrayNumber<Real, Arrays::by_col_> res;
    res = p1*v1;
    Real eps = res - p3.sum();
    stk_cout << _T("Product Using res=p1*v1 done. Elapsed time = ") << Chrono::elapsed() << _T("\n");
    stk_cout << _T("res.elt() = ") << res.elt() << _T(" and eps = res - p3.sum() =") << eps << _T("\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for dot products   +\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test operator* : Diagonal Matrices                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    Array2DDiagonal<Real> d10(22, 10.0), eps10;
    CArrayVector<Real, 202> r10; r10 = 10.0;
    Chrono::start();
    res2 = blhs * d10;
    stk_cout << _T("Product Using res2=blhs*d10 done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    diff = 10.0 * blhs - res2;
    stk_cout << _T("diff.max=") << diff.maxElt(iMax, jMax) << _T("\n");
    stk_cout << _T("diff.min=") << diff.minElt(iMin, jMin) << _T("\n");

    Chrono::start();
    d10.resize(202) = 10.0;
    res2 = d10 * clhs;
    stk_cout << _T("Product Using res2=d10*blhs done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    diff = res2 - 10.0 * clhs;
    stk_cout << _T("diff.max=") << diff.maxElt(iMax, jMax) << _T("\n");
    stk_cout << _T("diff.min=") << diff.minElt(iMin, jMin) << _T("\n");
    stk_cout << _T("Product Using res2=r10*d10 done. Elapsed time = ") << Chrono::elapsed() << _T("\n");

    eps10 = d10 - r10;
    stk_cout << _T("eps10 = d10 - r10. eps10.sqrt().max=") << eps10.sqrt().maxElt(iMax) << _T("\n");
    stk_cout << _T("eps10 = d10 - r10. eps10.sqrt().min=") << eps10.sqrt().minElt(iMin) << _T("\n");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test operator* : vector Operation                   +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    CArrayVector<Real, 22> cv1(22, 0.01);
    CVectorX cres;
    Chrono::start();
    v3.move(mult(clhs, cv1));
    stk_cout << _T("Product Using v3.move(mult(clhs,cv1)) done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    Chrono::start();
    cres = clhs * cv1;
    stk_cout << _T("Product Using cres=clhs * cv1 done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    vdiff = v3 - cres;
    stk_cout << _T("vdiff.max=") << vdiff.maxElt(iMax) << _T("\n");
    stk_cout << _T("vdiff.min=") << vdiff.minElt(iMin) << _T("\n");

    CArrayPoint<Real, 202> pv1(22, 0.01);
    CArrayPoint<Real, 22> pres;
    Chrono::start();
    p3.move(mult(pv1, clhs));
    stk_cout << _T("Product Using p3.move(mult(pv1,clhs)) done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    Chrono::start();
    pres = pv1*clhs;
    stk_cout << _T("Product Using cres=clhs * cv1 done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    vdiff = pres - p3;
    stk_cout << _T("vdiff.max=") << vdiff.maxElt(iMax) << _T("\n");
    stk_cout << _T("vdiff.min=") << vdiff.minElt(iMin) << _T("\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for vector products+\n");
    stk_cout << _T("+ No errors detected.                                 +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test operator* : big dimension                      +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    Array2D<Real> mBig(501, 3000), resBig, res2Big;
    Array2DSquare<Real> mBigSquare(501);
    Array2DVector<Real> vBig(501), vResBig, vRes2Big;
    Array2DPoint<Real> pBig(501), pResBig, pRes2Big;
    mBig.randGauss();
    mBigSquare.randGauss();
    vBig.randUnif();
    pBig.randUnif();
    // matrix by matrix product
    Chrono::start();
    resBig.move(mult(mBigSquare, mBig));
    stk_cout << _T("Product Using resBig.move(mult(mBigSquare, mBig)) done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    Chrono::start();
    res2Big = mBigSquare * mBig;
    stk_cout << _T("Product Using res2Big = mBigSquare * mBig done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    diff = resBig - res2Big;
    stk_cout << _T("diff.max=") << diff.maxElt(iMax, jMax) << _T("\n");
    stk_cout << _T("diff.min=") << diff.minElt(iMin, jMin) << _T("\n");

    // matrix by vector product
    Chrono::start();
    vResBig.move(mult(mBigSquare, vBig));
    stk_cout << _T("Product Using vResBig.move(mult(mBigSquare, vBig)) done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    Chrono::start();
    vRes2Big = mBigSquare * vBig;
    stk_cout << _T("Product Using vRes2Big = mBigSquare * mBig done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    vdiff = vResBig - vRes2Big;
    stk_cout << _T("vdiff.max=") << vdiff.maxElt(iMax) << _T("\n");
    stk_cout << _T("vdiff.min=") << vdiff.minElt(iMin) << _T("\n");

    // matrix by vector product
    Chrono::start();
    pResBig.move(mult(pBig, mBigSquare));
    stk_cout << _T("Product Using pResBig.move(mult(pBiog, mBigSquare)) done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    Chrono::start();
    pRes2Big = pBig * mBigSquare;
    stk_cout << _T("Product Using pRes2Big = pBig * mBigSquare done. Elapsed time = ")
             << Chrono::elapsed() << _T("\n");
    pdiff = pResBig - pRes2Big;
    stk_cout << _T("pdiff.max=") << pdiff.maxElt(iMax) << _T("\n");
    stk_cout << _T("pdiff.min=") << pdiff.minElt(iMin) << _T("\n");

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Successful completion of testing for big dimension  +\n");
    stk_cout << _T("+  products. No errors detected.                      +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("\n\n");
  }
  catch (Exception const& error)
  { stk_cerr << error.error() << _T("\n");}
  return 0;
}



