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
 * Purpose:  Implementation of the EigenValue class
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_SymEigen.cpp
 *  @brief In this file we implement the SymEigen class.
 **/


#include "../include/STK_SymEigen.h"
#include "../include/STK_Householder.h"
#include "../include/STK_Givens.h"


#ifdef STK_ALGEBRA_VERBOSE
#include "Arrays/include/STK_Display.h"
#endif

#define MAXITER 100

namespace STK
{


/* Main methods.  */
/* Compute diagonalization of the symmetric matrix */
bool SymEigen::runImpl()
{
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("Entering in SymEigen::run()\n");
#endif
  try
  {
    // copy data
    eigenValues_.resize(eigenVectors_.range());
    begin_ = eigenVectors_.begin();
    last_ = eigenVectors_.lastIdx();
    norm_ = 0.;
    rank_ = 0;
    det_ = 0.;
    // tridiagonalize eigenVectors_
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("calling SymEigen::tridiagonalize()\n");
#endif
    tridiagonalize();
    // compute eigenVectors_
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("calling SymEigen::compHouse()\n");
#endif
    compHouse();
    // Diagonalize
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("calling SymEigen::diagonalize()\n");
#endif
    diagonalize();
    // compute rank, norm and determinant
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("calling SymEigen::finalize()\n");
#endif
    finalizeStep();
  }
  catch (Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
  return true;
}


/* tridiagonalisation of the symetric matrix eigenVectors_. Only the lower
 * part of eigenVectors_ used. eigenVectors_ is overwritten with the Householder vectors.
 * eigenValues_ contains the diagonal.
 **/
void SymEigen::tridiagonalize()
{
  // Upper diagonal values
  F_.resize(Range(begin_-1, last_, 0));
  F_.front() = 0.0; F_.back() =0.0;
  // initial range of the Householder vectors
  Range range1(Range(begin_+1, last_, 0));
  Range range2(Range(begin_+2, last_, 0));
  // Bidiagonalisation of eigenVectors_
  // loop on the cols and rows
  for ( int i=begin_, i1=begin_+1, i2=begin_+2; i<last_
      ; i++, i1++, i2++, range1.incFirst(1), range2.incFirst(1)
      )
  {
    // ref on the current column iter in the range iter1:last_
    CSquareXd::Col v(eigenVectors_.col(range1, i));
    // Compute Householder vector and get sub-diagonal element
    F_[i] = house(v);
    // Save diagonal element
    eigenValues_[i] = eigenVectors_(i,i);
    // get beta
    Real beta = v.front();
    if (beta)
    {
      // ref on the current column iter1 in the range iter1:last_
      CSquareXd::Col M1(eigenVectors_.col(range1, i1));
      // aux1 will contain <v,p>
      Real aux1 = 0.0;
      // apply left and right Householder to eigenVectors_
      // compute D(iter1:last_) = p and aux1 = <p,v>
      // Computation of p_iter1 = beta * eigenVectors_ v using the lower part of eigenVectors_
      // save p_iter1 in the unused part of eigenValues_ and compute p'v
      aux1 += (eigenValues_[i1] = beta*(M1[i1] + M1.sub(range2).dot(v.sub(range2)))) /* *1.0 */;
      // other cols
      for (int k=i2; k<=last_; k++)
      {
        // Computation of p_i = beta * eigenVectors_ v using the lower part of eigenVectors_
        // save p_i in the unusued part of eigenValues_ and compute p'v
        Real aux = M1[k] /* *1.0 */;
        for (int j=i2;  j<=k;     j++) { aux += eigenVectors_(k,j)*v[j];}
        for (int j=k+1; j<=last_; j++) { aux += eigenVectors_(j,k)*v[j];}
        // save p_i in the unusued part of eigenValues_ and compute p'v
        aux1 += (eigenValues_[k] = beta*aux) * v[k];
      }
      // update diagonal element M_ii+= 2 v_i * q_i = 2* q_i (i=iter1)
      // aux = q_iter1 and aux1 = beta*<p,v>/2 (we don't save aux in eigenValues_)
      Real aux = (eigenValues_[i1] + (aux1 *= (beta/2.0)));
      M1[i1] += 2.0*aux;
      // update lower part: all rows
      // compute q_i and update the lower part of eigenVectors_
      for (int k=i2; k<=last_; k++)
      {
        // get q_i and save it in eigenValues_i=q_i = p_i + <p,v> * beta * v_i/2
        eigenValues_[k] += aux1 * v[k];
        // Compute eigenVectors_ + u q' + q u',
        // update the row i, begin_ element
        M1[k] += eigenValues_[k] /* *1.0 */+ v[k] * aux;
        // update the row i: all cols under the diagonal
        for (int j=i2; j<=k; j++)
          eigenVectors_(k,j) += v[j] * eigenValues_[k] + v[k] * eigenValues_[j];
      }
    }
  }
  // Last col
  eigenValues_[last_] = eigenVectors_(last_,last_);
}

// Compute eigenVectors_ from the Householder rotations
void SymEigen::compHouse()
{
  // compute eigenVectors_
  // iter0 is the column of the Householder vector
  // iter is the current column to compute
  for ( int iter0=last_-1, iter=last_, iter1=last_+1
      ; iter0>=begin_
      ; iter0--, iter--, iter1--)
  {
    // reference on the Householder vector
    CSquareXd::Col v(eigenVectors_.col(Range(iter, last_, 0), iter0));
    // Get Beta
    Real beta = v[iter];
    if (beta)
    {
      // Compute Col iter -> eigenVectors_ e_{iter}= e_{iter}+ beta v
      eigenVectors_(iter, iter) = 1.0 + beta /* *1.0 */;
      for (int i=iter1; i<=last_; i++)
      { eigenVectors_(i, iter) = beta * v[i];}

      // Update the other Cols
      for (int i=iter1; i<=last_; i++)
      {
        // compute beta*<v, eigenVectors_^i>
        Real aux = 0.0;
        for (int j=iter1; j<=last_; j++)
        { aux += eigenVectors_(j, i) * v[j]; }
        aux *= beta;
        // begin_ row (iter)
        eigenVectors_(iter, i) = aux;
        // other rows
        for (int j=iter1; j<=last_; j++)
        { eigenVectors_(j, i) += aux * v[j];}
      }
    }
    else // beta = 0, nothing to do
    { eigenVectors_(iter,iter)=1.0;
      for (int j=iter1; j<=last_; j++)
      { eigenVectors_(iter, j) =0.0; eigenVectors_(j, iter) = 0.0;}
    }
  }
  // begin_ row and begin_ col
  eigenVectors_(begin_, begin_) = 1.0;
  for (int j=begin_+1; j<=last_; j++)
  { eigenVectors_(begin_, j) = 0.0; eigenVectors_(j, begin_) = 0.0;}
}

// diagonalize eigenValues_ and F_
void SymEigen::diagonalize()
{
  // Diagonalisation of eigenVectors_
  for (int iend=last_; iend>=begin_+1; iend--)
  {
    int iter;
    for (iter=0; iter<MAXITER; iter++) // fix the number of iterations max
    {
      // check cv for the last element
      Real sum = std::abs(eigenValues_[iend]) + std::abs(eigenValues_[iend-1]);
      // if the first element of the small subdiagonal
      // is 0. we stop the QR iterations and increment iend
      if (std::abs(F_[iend-1])+sum == sum)
      { F_[iend-1] = 0.0 ; break;}
      // look for a single small subdiagonal element to split the matrix
      int ibeg = iend-1;
      while (ibeg>begin_)
      {
        ibeg--;
        // if a subdiagonal is zero, we get a sub matrix unreduced
        //sum = std::abs(eigenValues_[ibeg])+std::abs(eigenValues_[ibeg+1]);
        if (std::abs(F_[ibeg])+std::abs(eigenValues_[ibeg]) == std::abs(eigenValues_[ibeg]))
        { F_[ibeg] = 0.; ibeg++; break;}
      };
      // QR rotations between the rows/cols ibeg et iend
      // Computation of the Wilkinson's shift
      Real aux = (eigenValues_[iend-1] - eigenValues_[iend])/(2.0 * F_[iend-1]);
      // initialisation of the matrix of rotation
      // y is the current F_[k-1],
      Real y = eigenValues_[ibeg]-eigenValues_[iend] + F_[iend-1]/sign(aux, STK::norm(aux,1.0));
      // z is the element to annulate
      Real z       = F_[ibeg];
      // Fk is the temporary F_[k]
      Real Fk      = z;
      // temporary DeltaD(k)
      Real DeltaDk = 0.0;
      // Index of the columns
      int k,k1;
      // Givens rotation to restore tridiaonal form
      for (k=ibeg, k1=ibeg+1; k<iend; k++, k1++)
      {
        // underflow ? we have a tridiagonal form exit now
        if (z == 0.0) { F_[k]=Fk;  break;}
        // Rotation columns (k,k+1)
        F_[k-1] = (aux = STK::norm(y,z));    // compute final F_[k-1]
        // compute cosinus and sinus
        Real cosinus = y/aux, sinus = -z/aux;
        Real Dk   = eigenValues_[k] + DeltaDk;      // compute current D[k]
        Real aux1 = 2.0 * cosinus * Fk + sinus * (Dk - eigenValues_[k1]);
        // compute DeltaD(k+1)
        eigenValues_[k]     = Dk - (DeltaDk =  sinus * aux1);  // compute eigenValues_[k]
        y         = cosinus*aux1 - Fk;    // temporary F_[k]
        Fk        = cosinus * F_[k1];     // temporary F_[k+1]
        z         = -sinus * F_[k1];      // temporary z
        // update eigenVectors_
        rightGivens(eigenVectors_, k1, k, cosinus, sinus);
      }
      // k=iend if z!=0 and k<iend if z==0
      eigenValues_[k] += DeltaDk ; F_[k-1] = y;
      // restore F[ibeg-1]
      F_[ibeg-1] = 0.;
    } // iter
    if (iter == MAXITER)
    { msg_error_ = _T("Warning, max iteration reached in SymEigen::diagonalize()\n");}
    // We have to sort the eigenvalues : we use a basic strategy
    Real z = eigenValues_[iend];        // current value
    for (int i=iend+1; i<eigenValues_.end(); i++)
    { if (eigenValues_[i] > z)           // if the ith eigenvalue is greater
      { eigenValues_.swap(i-1, i);       // swap the cols
        eigenVectors_.swapCols(i-1, i);
      }
      else break;
    } // end sort
  } // iend
  // sort first value
  Real z = eigenValues_[begin_];        // current value
  for (int i=begin_+1; i<eigenValues_.end(); i++)
  { if (eigenValues_[i] > z)           // if the ith eigenvalue is greater
    {
      eigenValues_.swap(i-1, i);       // swap the cols
      eigenVectors_.swapCols(i-1, i);
    }
    else break;
  } // end sort
}

} // namespace STK

#undef MAXITER
