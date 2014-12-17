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
 * Project:  Algebra
 * Purpose:  Implement the Qr Class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Qr.cpp
 *  @brief In this file we implement the Qr Class (QR decomposition).
 **/
 
#include "../include/STK_Qr.h"
#include "../include/STK_Givens.h"
#include "Arrays/include/STK_Array2DPoint.h"

#ifdef STK_ALGEBRA_DEBUG
#include "../../Arrays/include/STK_Display.h"

template< class Container2D >
void print(Container2D const& A, STK::String const& name)
{
  stk_cout << "print: " << name << _T("\n";);
  stk_cout << name << _T(".isRef() =")        << A.isRef()  << _T("\n");
  stk_cout << name << _T(".capacityHo() =")   << A.capacityHo()  << _T("\n");
  stk_cout << name << _T(".cols() =")      << A.cols()  << _T("\n");
  stk_cout << name << _T(".rows() =")      << A.rows()  << _T("\n\n");
  stk_cout << name << _T(".rangeCols().isRef() =")  << A.rangeCols().isRef() << _T("\n");
  stk_cout << name << _T(".rangeCols() =\n")  << A.rangeCols() << _T("\n");
  stk_cout << name << _T(".capacityCols().isRef() =") << A.capacityCols().isRef()  << _T("\n");
  stk_cout << name << _T(".capacityCols() =\n") << A.capacityCols()  << _T("\n");
}
#endif

namespace STK
{

/* Computation of the QR decomposition */
void Qr::qr()
{
  R_.resize(Q_.rows(), Q_.cols());
  // start left householder reflections
  Range r(Q_.rows()), c(Q_.cols());
  for(int j = R_.beginRows(); (j < R_.endRows()) && (j < R_.endCols()) ; ++j)
  {
    Vector u(Q_, r, j);    // get a reference on the j-th column in the range r
    R_(j, j) = house(u);   // compute the housolder vector
    leftHouseholder(Q_(r, c.incFirst(1)), u); // apply-it to the remaining part of Q_
    R_.row(j, c).copy(Q_.row(j, c)); // copy current row of Q_ in the range c in R_
    r.incFirst(1);  // decrease the range
  }
}


} // namespace STK

