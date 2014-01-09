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

/** @file STK_EigenvaluesSymmetric.cpp
 *  @brief In this file we implement the EigenvaluesSymmetric class.
 **/

#include "../../Arrays/include/STK_Array2DVector.h"

#include "../include/STK_Householder.h"
#include "../include/STK_Givens.h"

#include "../include/STK_EigenvaluesSymmetric.h"


#ifdef STK_ALGEBRA_VERBOSE
#include "../../Arrays/include/STK_Display.h"
#endif

#define MAXITER 100

namespace STK
{
/* Constructors.
  *  A is the matrix to decompose.
  **/
EigenvaluesSymmetric::EigenvaluesSymmetric( MatrixSquare const* p_data)
                                          : Runner(p_data)
                                          , P_()
                                          , D_()
                                          , first_()
                                          , last_()
                                          , norm_(0)
                                          , rank_(0)
                                          , det_(0)
{ }
EigenvaluesSymmetric::EigenvaluesSymmetric( MatrixSquare const& data)
                                          : Runner(data)
                                          , P_()
                                          , D_()
                                          , first_()
                                          , last_()
                                          , norm_(0)
                                          , rank_(0)
                                          , det_(0)
{ }

/* Copy constructor */
EigenvaluesSymmetric::EigenvaluesSymmetric( EigenvaluesSymmetric const& S)
                                          : Runner(S)
                                          , P_(S.P_)
                                          , D_(S.D_)
                                          , first_(S.first_)
                                          , last_(S.last_)
                                          , norm_(S.norm_)
                                          , rank_(S.rank_)
                                          , det_(S.det_)
{ ;}

/* Operator = : overwrite the EigenvaluesSymmetric with S.
  **/
EigenvaluesSymmetric& EigenvaluesSymmetric::operator=( EigenvaluesSymmetric const& S)
{
  P_     = S.P_;        // Matrix P
  D_     = S.D_;        // Singular values
  first_ = S.first_;    // first value
  last_  = S.last_;     // last value
  norm_  = S.norm_;     // norm of the matrix
  rank_  = S.rank_;     // rank of the matrix
  det_   = S.det_;      // determinant of the matrix

  return *this;
}

/* destructor  */
EigenvaluesSymmetric::~EigenvaluesSymmetric() {}

/*
 * Compute the generalized inverse of the diagonalized matrix.
 * The result is allocated dynamically and is not liberated by this
 * class.
 * @return a pointer on the generalized inverse.
 */
MatrixSquare* EigenvaluesSymmetric::ginv()
{
  // create pseudo inverse matrix
  MatrixSquare* res = new MatrixSquare;
  ginv(*res);
  return res;
}

/*
 * Compute the generalized inverse of the diagonalized matrix P_ and put
 * the result in res.
 * @param res the generalized inverse of the matrix P_.
 */
void EigenvaluesSymmetric::ginv(MatrixSquare& res)
{
  // create pseudo inverse matrix
  res.resize(P_.range());
  // compute tolerance
  Real tol = Arithmetic<Real>::epsilon() * norm_;
  // compute PDP'
  for (int i = first_; i<=last_; i++)
  {
    for (int j = first_; j<=last_; j++)
    {
      res(j,i) = 0.0;
      for (int k = first_; k<=last_; k++)
      {
        if (std::abs(D_[k]) > tol)
          res(j,i) += (P_(i, k) * P_(j, k)) / D_[k];
      }
    }
  }
}


/* Main methods.  */
/* Compute diagonalization of the symmetric matrix */
bool EigenvaluesSymmetric::run()
{
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("Entering in EigenvaluesSymmetric::run()\n");
#endif
  try
  {
    // copy data
    if (!p_data_)
    { STKRUNTIME_ERROR_NO_ARG(Error in EigenvaluesSymmetric::run(),data set is not set);}
    // copy data
    P_ = *p_data_;
    D_.resize(P_.range());
    first_ = P_.firstIdx();
    last_ = P_.lastIdx();
    norm_ = 0.;
    rank_ = 0;
    det_ = 0.;
    // tridiagonalize P_
#ifdef STK_VERBSTK_ALGEBRA_VERBOSEOSE
    stk_cout << _T("calling EigenvaluesSymmetric::tridiagonalize()\n");
#endif
    tridiagonalize();
    // compute P_
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("calling EigenvaluesSymmetric::compHouse()\n");
#endif
    compHouse();
    // Diagonalize
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("calling EigenvaluesSymmetric::diagonalize()\n");
#endif
    diagonalize();
    // compute rank, norm and determinant
#ifdef STK_ALGEBRA_VERBOSE
    stk_cout << _T("calling EigenvaluesSymmetric::compEstimates()\n");
#endif
    compEstimates();
  }
  catch (Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
  return true;
}

/* Main methods.  */
/* Compute diagonalization of the symmetric matrix                     */
bool EigenvaluesSymmetric::run(Vector const& weights)
{

  try
  {
    // copy data
    if (!p_data_)
    { throw runtime_error(_T("Error in EigenvaluesSymmetric::run(weights) "
                             "data set is not set.\n"));
    }
    // copy data
    if ( !weights.range().isContaining(p_data_->rows()))
    { throw runtime_error(_T("Error in EigenvaluesSymmetric::run(weights) "
                             "!weights.range().isContaining(p_data_->rows()).\n"));
    }
    // copy data
    // copy data
    P_ = p_data_->asDerived();
    D_.resize(P_.range());
    first_ = P_.firstIdx();
    last_ = P_.lastIdx();
    norm_ = 0.;
    rank_ = 0;
    det_ = 0.;
    // weight rows and columns of the container
    for (int i= first_; i <= last_; ++i)
    {
      Real aux = Real(sqrt((double)weights[i]));
      for (int j= P_.firstIdx(); j <= P_.lastIdx(); ++i)
      {
        P_(i,j) *= aux;
        P_(j,i) *= aux;
      }
    }

    // tridiagonalize P_
    tridiagonalize();
    // compute P_
    compHouse();
    // Diagonalize
    diagonalize();
    // compute rank, norm and determinant
    compEstimates();

    // Unweighed rows and columns of the rotation
    for (int i= first_; i <= last_; ++i)
    {
      Real aux = Real(sqrt(weights[i]));
      if (aux)
      {
        for (int j= P_.firstIdx(); j <= P_.lastIdx(); ++i)
        {
          P_(i,j) /= aux;
          P_(j,i) /= aux;
        }
      }
    }
  }
  catch (Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
  return true;
}

/* tridiagonalisation of the symetric matrix P_. Only the lower
 * part of P_ used. P_ is overwritten with the Householder vectors.
 * D_ contains the diagonal. F_ contains the subdiagonal.
 **/
void EigenvaluesSymmetric::tridiagonalize()
{
  // Upper diagonal values
  F_.resize(Range(first_-1, last_, 0));
  F_.front() = 0.0; F_.back() =0.0;
  // initial range of the Householder vectors
  Range range1(Range(first_+1, last_, 0));
  Range range2(Range(first_+2, last_, 0));
  // Bidiagonalisation of P_
  // loop on the cols and rows
  for ( int iter=first_, iter1=first_+1, iter2=first_+2
      ; iter<last_
      ; iter++, iter1++, iter2++, range1.incFirst(1), range2.incFirst(1)
      )
  {
    // ref on the current column iter in the range iter1:last_
    Vector v(P_, range1, iter);
    // Compute Householder Vector and get subdiagonal element
    F_[iter] = house<Vector>(v);
    // Save diagonal element
    D_[iter] = P_(iter,iter);
    // get beta
    Real beta = v.front();
    if (beta)
    {
      // ref on the current column iter1 in the range iter1:last_
      Vector M1(P_, range1, iter1);
      // aux1 will contain <v,p>
      Real aux1 = 0.0;
      // apply left and right Householder to P_
      // compute D(iter1:last_) = p and aux1 = <p,v>
      // Computation of p_iter1 = beta * P_ v using the lower part of P_
      // save p_iter1 in the unused part of D_ and compute p'v
      aux1 += (D_[iter1] = beta*(M1[iter1] + M1.sub(range2).dot(v.sub(range2)))) /* *1.0 */;
      // other cols
      for (int i=iter2; i<=last_; i++)
      {
        // Computation of p_i = beta * P_ v using the lower part of P_
        // save p_i in the unusued part of D_ and compute p'v
        aux1 += ( D_[i] = beta* (M1[i] + P_.row(i,Range(iter2,i,0)).dot(v.sub(Range(iter2,i,0)))
                                       + P_.col(Range(i+1,last_,0),i).dot(v.sub(Range(i+1,last_,0)))
                                )
                ) * v[i];
      }
      // update diagonal element M_ii+= 2 v_i * q_i = 2* q_i (i=iter1)
      // aux = q_iter1 and aux1 = beta*<p,v>/2 (we don't save aux in D_)
      Real aux = (D_[iter1] + (aux1 *= (beta/2.0)));
      M1[iter1] += 2.0*aux;
      // update lower part: all rows
      // compute q_i and update the lower part of P_
      for (int i=iter2; i<=last_; i++)
      {
        // get q_i and save it in D_i=q_i = p_i + <p,v> * beta * v_i/2
        D_[i] += aux1 * v[i];
        // Compute P_ + u q' + q u',
        // update the row i, first_ element
        M1[i] += D_[i] /* *1.0 */+ v[i] * aux;
        // update the row i: all cols under the diagonal
//        P_.row(i, Range(iter2,i,0)) += (v.sub(Range(iter2,i,0)) * D_[i]
//                                     + v[i] * D_.sub(Range(iter2,i,0)));
        for (int j=iter2; j<=i; j++)
          P_(i,j) += v[j] * D_[i] + v[i] * D_[j];
      }
    }
  }
  // Last col: F[last_] = 0.0;
  D_[last_] = P_(last_,last_);
}

// Compute P_ from the Householder rotations
void EigenvaluesSymmetric::compHouse()
{
  // compute P_
  // iter0 is the column of the Householder vector
  // iter is the current column to compute
  for ( int iter0=last_-1, iter=last_, iter1=last_+1
      ; iter0>=first_
      ; iter0--, iter--, iter1--)
  {
    // reference on the Householder vector
    Vector v(P_, Range(iter, last_, 0), iter0);
    // Get Beta
    Real beta = v[iter];
    if (beta)
    {
      // Compute Col iter -> P_ e_{iter}= e_{iter}+ beta v
      P_(iter, iter) = 1.0 + beta /* *1.0 */;
      for (int i=iter1; i<=last_; i++)
      { P_(i, iter) = beta * v[i];}

      // Update the other Cols
      for (int i=iter1; i<=last_; i++)
      {
        // compute beta*<v, P_^i>
        Real aux = 0.0;
        for (int j=iter1; j<=last_; j++)
        { aux += P_(j, i) * v[j]; }
        aux *= beta;
        // first_ row (iter)
        P_(iter, i) = aux;
        // other rows
        for (int j=iter1; j<=last_; j++)
        { P_(j, i) += aux * v[j];}
      }
    }
    else // beta = 0, nothing to do
    { P_(iter,iter)=1.0;
      for (int j=iter1; j<=last_; j++)
      { P_(iter, j) =0.0; P_(j, iter) = 0.0;}
    }
  }
  // first_ row and first_ col
  P_(first_, first_) = 1.0;
  for (int j=first_+1; j<=last_; j++)
  { P_(first_, j) = 0.0; P_(j, first_) = 0.0;}
}

// diagonalize D_ and F_
void EigenvaluesSymmetric::diagonalize()
{
  // Diagonalisation of P_
  for (int iend=last_; iend>=first_+1; iend--)
  {
    int iter;
    for (iter=0; iter<MAXITER; iter++) // fix the number of iterations max
    {
      // check cv for the last element
      Real sum = std::abs(D_[iend]) + std::abs(D_[iend-1]);
      // if the first element of the small subdiagonal
      // is 0. we stop the QR iterations and increment iend
      if (std::abs(F_[iend-1])+sum == sum)
      { F_[iend-1] = 0.0 ; break;}
      // look for a single small subdiagonal element to split the matrix
      int ibeg = iend-1;
      while (ibeg>first_)
      {
        ibeg--;
        // if a subdiagonal is zero, we get a sub matrix unreduced
        //sum = std::abs(D_[ibeg])+std::abs(D_[ibeg+1]);
        if (std::abs(F_[ibeg])+std::abs(D_[ibeg]) == std::abs(D_[ibeg]))
        { F_[ibeg] = 0.; ibeg++; break;}
      };
      // QR rotations between the rows/cols ibeg et iend
      // Computation of the Wilkinson's shift
      Real aux = (D_[iend-1] - D_[iend])/(2.0 * F_[iend-1]);
      // initialisation of the matrix of rotation
      // y is the current F_[k-1],
      Real y = D_[ibeg]-D_[iend] + F_[iend-1]/sign(aux, STK::norm(aux,1.0));
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
        Real Dk   = D_[k] + DeltaDk;      // compute current D[k]
        Real aux1 = 2.0 * cosinus * Fk + sinus * (Dk - D_[k1]);
        // compute DeltaD(k+1)
        D_[k]     = Dk - (DeltaDk =  sinus * aux1);  // compute D_[k]
        y         = cosinus*aux1 - Fk;    // temporary F_[k]
        Fk        = cosinus * F_[k1];     // temporary F_[k+1]
        z         = -sinus * F_[k1];      // temporary z
        // update P_
        rightGivens(P_, k1, k, cosinus, sinus);
      }
      // k=iend if z!=0 and k<iend if z==0
      D_[k] += DeltaDk ; F_[k-1] = y;
      // restore F[ibeg-1]
      F_[ibeg-1] = 0.;
    } // iter
    if (iter == MAXITER) { msg_error_ = _T("Warning, max iteration reached in EigenvaluesSymmetric::diagonalize()\n");}
    // We have to sort the eigenvalues : we use a basic strategy
    Real z = D_[iend];        // current value
    for (int i=iend+1; i<=D_.lastIdx(); i++)
    { if (D_[i] > z)           // if the ith eigenvalue is greater
      { D_.swap(i-1, i);       // swap the cols
        P_.swapCols(i-1, i);
      }
      else break;
    } // end sort
  } // iend
  // sort first value
  Real z = D_[first_];        // current value
  for (int i=first_+1; i<=D_.lastIdx(); i++)
  { if (D_[i] > z)                // if the ith eigenvalue is greater
    {
      D_.swap(i-1, i);       // swap the cols
      P_.swapCols(i-1, i);
    }
    else break;
  } // end sort
}

/* compute rank, norm and determinant. */
void EigenvaluesSymmetric::compEstimates()
{
  // compute 2-norm_
  norm_ = std::abs(D_[1]);
  det_ = 1;
  // trivial case
  if (norm_ < Arithmetic<Real>::epsilon())
  {
    rank_ = 0;
    // compute determinant
    for (int i = first_; i<= last_; i++)
    { det_ *= D_[i];}
    return;
  }
  // compute tolerance
  Real tol = Arithmetic<Real>::epsilon();
  // compute rank_and determinant
  for (int i = first_; i<= last_; i++)
  {
    det_ *= D_[i];
    if (std::abs(D_[i])> tol ) rank_++;
  }
}

} // namespace STK

#undef MAXITER
