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
 * Purpose:  Implement the Svd Class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Svd.cpp
 *  @brief In this file we implement the Svd Class.
 **/

#include "../include/STK_Svd.h"

#include "Arrays/include/STK_Array2DLowerTriangular.h"
#include "Arrays/include/STK_Array2DUpperTriangular.h"
#include "Arrays/include/STK_Array2D_Functors.h"


#include "../include/STK_Householder.h"
#include "../include/STK_Givens.h"

namespace STK
{

/* Constructors   */
/* Default constructor  */
 Svd::Svd( ArrayXX const& A, bool   ref, bool   withU, bool   withV)
        : U_(A, ref)
        , withU_(withU)
        , withV_(withV)
        , ref_(ref)
{
  init();
  compSvd();
}

/* Copy constructor */
Svd::Svd( const Svd &S)
        : U_(S.U_, S.ref_)
        , V_(S.V_)
        , D_(S.D_)
        , ncolV_(S.ncolV_)
        , ncolD_(S.ncolD_)
        , ncolU_(S.ncolU_)
        , nrowU_(S.nrowU_)
        , withU_(S.withU_)
        , withV_(S.withV_)
        , ref_(S.ref_)
        , norm_(S.norm_)
        , rank_(S.rank_)
        , error_(S.error_)
{}

/* Operator = : overwrite the Svd with S.*/
Svd& Svd::operator=(const Svd &S)
{
  U_ = S.U_;
  V_ = S.V_;
  D_ = S.D_;
  ncolV_ = S.ncolV_;
  ncolD_ = S.ncolD_;
  ncolU_ = S.ncolU_;
  nrowU_ = S.nrowU_;
  withU_ = S.withU_;
  withV_ = S.withV_;
  ref_ = false;        // no reference possible with operator =
  norm_ = S.norm_;
  rank_ = S.rank_;
  error_ = S.error_;

  return *this;
}

/* virtual destructor.  */
Svd::~Svd()
{ ;}


/* Private functions. */
/* initialization of the Svd. */
void Svd::init()
{
  // U_ is just a copy of A, translate begin to 1
  U_.shift(1,1);   // if U_ is a ref on A, this can generate an error

  // If the container is empty, set default
  if (U_.empty())
  { ncolV_ = 0;
    ncolD_ = 0;
    ncolU_ = U_.sizeCols();
    nrowU_ = 0;
    return;
  }

  // The container is not empty, thus we have to compute the dimension
  // and eventually to adjust the container (U_)
  ncolU_ = U_.sizeCols();         // Number of cols of (U_)
  nrowU_ = U_.sizeRows();         // Number of rows of (U_)
  ncolV_ = U_.sizeCols();         // Number of cols of V_
  ncolD_ = std::min(nrowU_, ncolV_); // Maximal number of singular value
  error_ = false;               // no problems...
}


/* Main method for the svd computation.                               */
void Svd::compSvd()
{
  // if the container is empty, there is nothing to do
  if (U_.empty())
  { rank_ = 0;
    norm_ = 0.0;
    return;
  }
  // Bidiagonalize (U_)
  norm_ = bidiag(U_, D_, F_);
  // We need to create V_ before rightEliminate
  if (withV_) {
    V_.resize(Range(1,ncolV_));
    compV();
  }
  // rightEliminate last element of F_ if any
  if (nrowU_ < ncolV_)
  { rightEliminate(D_, F_, nrowU_, V_, withV_, norm_);}
  // If (U_) is not needed, we can destroy the storage
  if (withU_) { compU();}
  // Diagonalize
  error_ = diag(D_, F_, U_, V_, withU_, withV_, norm_);
  // Compute the true inf norm
  norm_ = D_[1];
  // Compute the rank
  rank_ = 0;
  for (int iter=1; iter<=ncolD_; iter++)
    if (norm_+D_[iter]!=norm_) { rank_++;}
    else break;
  // The sub diagonal is now zero
  F_.clear();
}


/* New computation of the Svd.                                        */
void Svd::newSvd( ArrayXX const&   A
                , bool     withU
                , bool     withV
                )
{ // clear old results
  clear();

  // create U_
  U_   = A;           // Copy A in U_
  ref_ = false;       // this is not a ref_

  withU_   = withU;   // copy withU_ value
  withV_   = withV;   // copy withV_ value

  init();             // initialize (U_) and dimensions
  compSvd();          // compute the svd
}


/* clear (U_)                                                         */
void Svd::clearU()
{ U_.clear();
  nrowU_ = 0;
  ncolU_ = 0;
  withU_ = false;
}


/* clear (U_)                                                         */
void Svd::clearV()
{ V_.clear(); withV_ = false;
  ncolV_ = 0;
}


/* clear Svd                                                          */
void Svd::clear()
{ clearU();
  clearV();
  D_.clear();
}


/* Bidiagonalization of the matrix M.                                 */
Real Svd::bidiag(const ArrayXX& M, Point& D, Vector& F)
{
  // norm of the matrix M
  Real norm  = 0.0;
  // compute the number of iteration
  int begin_iter = M.beginCols();
  int last_iter  = M.beginCols() + std::min(M.sizeCols(), M.sizeRows()) -1;
  // Diagonal values
  D.resize(Range(begin_iter, last_iter, 0));
  // Upper diagonal values
  F.resize(Range(begin_iter-1, last_iter, 0));
  F.front() = 0.0;
  // Bidiagonalisation of M
  // loop on the cols and rows
  Range rowRange0(M.rows())
    , rowRange1(Range(M.beginRows()+1, M.lastIdxRows(), 0))
    , colRange1(Range(M.beginCols()+1, M.lastIdxCols(), 0));
  for ( int iter=begin_iter ; iter<=last_iter
      ; iter++
      , rowRange0.incFirst(1)
      , rowRange1.incFirst(1)
      , colRange1.incFirst(1)
      )
  {
    // reference on the current column iter
    Vector X( M, rowRange0, iter);
    // Left Householder
    D[iter] = house(X);
    // apply Householder to next cols
    leftHouseholder(M.col(colRange1), X);
    // check if there is a row
    if ((iter < last_iter)||(M.sizeCols()>M.sizeRows()))
    {
      // ref on the current row iter
      Point P(M, colRange1, iter);
      // Right Householder
      F[iter] = house(P);
      // apply Householder to next rows
      rightHouseholder(M.row(rowRange1), P);
    }
    else
      F[iter] = 0.0;
    // Estimation of the norm of M
    norm = std::max(std::abs(D[iter])+std::abs(F[iter]), norm);
  }
  // return estimated norm
  return norm;
}


/* computation of V_                                                  */
void Svd::compV()
{ // Construction of V_
  // Number of right Householder rotations
  int  niter = (ncolV_>nrowU_) ? (nrowU_) : (ncolV_-1);

  // initialization of the remaining rows and cols of V_ to Identity
  for (int iter=niter+2; iter<=ncolV_; iter++)
  {
    Vector W(V_, V_.cols(), iter);
    W       = 0.0;
    W[iter] = 1.0;
  }

  Range range1(niter+1, ncolV_, 0), range2(niter+2, ncolV_, 0);
  for ( int iter0=niter, iter1=niter+1, iter2=niter+2; iter0>=1
      ; iter0--, iter1--, iter2--
      , range1.decFirst(1), range2.decFirst(1)
      )
  {
    // get beta and test
    Real beta = U_(iter0, iter1);
    if (beta)
    {
      // ref on the row iter1 of V_
      Point  Vrow1(V_, range1, iter1);
      // diagonal element
      Vrow1[iter1] = 1.0+beta;
      // ref on the column iter1
      Vector Vcol1(V_, range2, iter1);
      // get the Householder vector
      Vcol1 = Point(U_, range2, iter0);
      // Apply housholder to next cols
      for (int j=iter2; j<=ncolV_; j++)
      {
        Real aux;
        // ref on the column j
        Vector Vcolj( V_, range2, j);
        // update column j
        Vrow1[j] = (aux = dot(Vcol1, Vcolj) * beta);
        for (int i= iter2; i <= ncolV_; ++i)
        { Vcolj[i]   += Vcol1[i] * aux;}
      }
      // compute the Householder vector
      Vcol1 *= beta;
    }
    else // nothing to do
    {
      V_(range2, iter1) = 0.0;
      V_(iter1, iter1)  = 1.0;
      V_(iter1, range2) = 0.0;
    }
  }
  // First column and rows
  V_(1,1) =1.0;
  V_(Range(2,ncolV_, 0),1) =0.0;
  V_(1,Range(2,ncolV_, 0)) =0.0;
}


/* computation of U_                                                  */
void Svd::compU()
{
  int niter = D_.size();            // Number of iterations
  int ncol  = std::min(nrowU_, ncolU_); // number of non zero cols of U_

  // initialization of the remaining cols of U_ to 0.0
  // put 0 to unused cols
  U_.col(Range(ncol+1, ncolU_, 0)) = 0.0;
  // Computation of U_
  for (int iter=niter, iter1=niter+1; iter>=1; iter--, iter1--)
  {
    // ref of the column iter
    Vector X(U_, Range(iter1,nrowU_, 0), iter);
    // ref of the row iter
    Point P(U_, Range(iter,ncolU_, 0), iter);
    // Get beta and test
    Real beta = P[iter];
    if (beta)
    {
      // update the column iter
      P[iter] = 1.0 + beta;
      // Updating the cols iter+1 to ncolU_
      for (int j=iter1; j<=niter; j++)
      { // product of U_iter by U_j
        Real aux;
        Vector Y(U_, Range(iter1, nrowU_, 0), j); // ref on the column j
        // U_j = aux = beta * X'Y
        P[j] = (aux = dot( X, Y) *beta);
        // U^j += aux * U^iter
        for (int i= iter1; i <= nrowU_; ++i)
        {
          Y[i] += X[i] * aux;
        }
      }
      // compute the vector v
      X *= beta;
    }
    else // U^iter = identity
    {
      P[iter] = 1.0;
      X = 0.0;
    }
    // update the column iter
    U_(Range(1,iter-1, 0), iter) = 0.0;
  }
}


/* eliminate the element of the surdiagonal with right rotations      */
void Svd::rightEliminate( Point& D, Vector& F, int const& nrow
                        , ArraySquareX& V, bool withV, Real const& tol
                        )
{
  // the element to eliminate
  Real z = F[nrow];
  // if the element is not 0.0
  if (std::abs(z)+tol != tol)
  {
    // column of the element to eliminate
    int ncol1 = nrow+1;
    // begin the Givens rotations
    for (int k=nrow, k1=nrow-1; k>=1 ; k--, k1--)
    {
      // compute and apply Givens rotation to the rows (k, k+1)
      Real aux, sinus, cosinus;
      Real y = D[k];
      D[k]   = (aux     = norm(y,z));
      z      = (sinus   = -z/aux) * F[k1];
      F[k1] *= (cosinus =  y/aux);
      // Update V_
      if (withV)
        rightGivens(V, ncol1, k, cosinus, sinus);
      // if 0.0 we can break now
      if (std::abs(z)+tol == tol) break;
    }
  }
  // the element is now 0
  F[nrow] = 0.0;        // is 0.0
}


/* eliminate the element of the surdiagonal with left rotations       */
void Svd::leftEliminate( Point& D, Vector& F
                       , int const& nrow
                       , ArrayXX& U
                       , bool withU
                       , Real const& tol
                       )
{
  //the element to eliminate
  Real z = F[nrow];
  // if the element is not 0.0
  if (std::abs(z)+tol != tol)
  {
    // begin the Givens rotations
    for (int k=nrow+1; k <=D.lastIdx(); k++)
    {
      // compute and apply Givens rotation to the rows (nrow, k)
      Real y = D[k];
      Real aux, cosinus, sinus;
      D[k]  = (aux     = norm(y,z));
      z     = (sinus   = -z/aux) * F[k];
      F[k] *= (cosinus = y/aux);
      // Update U_
      if (withU)
        rightGivens(U, nrow, k, cosinus, sinus);
      if (std::abs(z)+tol == tol) break;
    }
  }
  F[nrow] = 0.0;
}


/*  diagonalization of the bidiag matrix                              */
bool Svd::diag( Point& D
              , Vector& F
              , ArrayXX& U
              , ArraySquareX& V
              , bool withU
              , bool withV
              , Real const& tol
              )
{
  // result of the diag process
  bool error = false;
  // Diagonalization of A : Reduction of la matrice bidiagonale
  for (int end=D.lastIdx(); end>=D.begin(); --end)
  { // 30 iter max
    int iter;
    for (iter=1; iter<=30; iter++)
    { // if  the last element of the surdiagonale is 0.0
      // stop the iterations
      int beg;
      if (std::abs(F[end-1])+tol == tol)  { F[end-1] = 0.0; break;}
      // now F[end-1] !=0
      // if  D[end] == 0, we can annulate F[end-1]
      // with rotations of the columns.
      if (std::abs(D[end])+tol == tol)
      {
        D[end]   = 0.0;
        rightEliminate(D, F, end-1, V, withV, tol);
        break; // Last element of the surdiagonal is 0
      }
      // now D[end] != 0 and F[end-1] != 0
      // look for the greatest matrix such that all the elements
      // of the diagonal and surdiagonal are not zeros
      for (beg = end-1; beg>D.begin(); --beg)
      {
        if ((std::abs(D[beg])+tol == tol)||(std::abs(F[beg])+tol == tol))
        break;
      }
      // now F[beg-1]!=0
      // if D[beg] == 0 and F[beg] != 0,
      // we can eliminate the element F[beg]
      // with rotations of the rows
      if ((std::abs(D[beg])+tol == tol) && (std::abs(F[beg])+tol != tol))
      {
        D[beg] = 0.0;
        leftEliminate(D, F, beg, U, withU, tol);
      }

      // Si F[beg]==0, on augmente beg
      if (std::abs(F[beg])+tol == tol) { F[beg] = 0.0; beg++;}

      // On peut commencer les rotations QR entre les lignes beg et end
      // Shift computation
      // easy shift : commented
      // Real aux = norm(D[end],F[end-1]);
      // Real y   = (D[beg]+aux)*(D[beg]-aux);
      // Real z   = D[beg]*F[beg];
      // Wilkinson shift : look at the doc
      Real dd1 = D[end-1];      // d_1
      Real dd2 = D[end];        // d_2
      Real ff1 = F[end-2];      // f_1
      Real ff2 = F[end-1];      // f_2
      // g
      Real aux = ( (dd1+dd2)*(dd1-dd2)
                 + (ff1-ff2)*(ff1+ff2))/(2*dd1*ff2);
      // A - mu
      Real d1 = D[beg];
      Real f1 = F[beg];
      Real y  = (d1+dd2)*(d1-dd2)
              + ff2*(dd1/(aux+sign(aux,norm(1.0,aux)))- ff2);
      Real z  = d1*f1;
      // chase no null element
      int k, k1;
      for (k=beg, k1 = beg+1; k<end; ++k, ++k1)
      { // Rotation colonnes (k,k+1)
        // Input :  d1 contient D[k],
        //          f1 contient F[k],
        //          y  contient F[k-1]
        // Output : d1 contient F[k],
        //          d2 contient D[k+1],
        //          y  contient D[k]
        Real cosinus=1., sinus=0.;
        Real d2 = D[k1];
        F[k-1] = (aux = norm(y,z));                            // F[k-1]
       // arbitrary rotation if y = z = 0.0
        if (aux)
          y  = (cosinus = y/aux) * d1 - (sinus = -z/aux) * f1; // D[k]
        else
          y  = cosinus * d1 - sinus * f1;                      // D[k]

        z      = -sinus * d2;                                  // z
        d1     =  sinus * d1 + cosinus * f1;                   // F[k]
        d2    *=  cosinus;                                     // D[k+1]
        // Update V
        if (withV)
          rightGivens(V, k1, k, cosinus, sinus);
        // avoid underflow
        // Rotation lignes (k,k+1)
        // Input :  d1 contient F[k],
        //          d2 contient D[k+1],
        //          y  contient D[k]
        // Output : d1 contient D[k+1],
        //          f1 contient F[k+1],
        //          y  contient F[k]
        f1   = F[k1];
        D[k] = (aux = norm(y,z));                              // D[k]
        // arbitrary rotation if y = z = 0.0
        if (aux)
          y  = (cosinus = y/aux) * d1 - (sinus = -z/aux) * d2; // F[k]
        else
          y   = cosinus * d1 - sinus * d2;                     // F[k]

        z   = -sinus * f1;                                    // z
        d1  = sinus *d1 + cosinus * d2;                       // D[k+1]
        f1 *= cosinus;                                        // F[k+1]
        // Update U
        if (withU)
          rightGivens(U, k1, k, cosinus, sinus);
      } // end of the QR updating iteration
      D[end]   = d1;
      F[end-1] = y;
      F[beg-1] = 0.0;  // F[beg-1] is overwritten, we have to set 0.0
    } // iter
    // too many iterations
    if (iter >= 30) { error = true;}
    // positive singular value only
    if (D[end]< 0.0)
    {
      // change sign of the singular value
      D[end]= -D[end];
      // change sign of the column end of V
      if (withV)
      {
        for (int i= V.beginRows(); i <= V.lastIdxRows(); ++i)
        { V(i,end) = -V(i,end);}
      }
    }

    // We have to sort the singular value : we use a basic strategy
    Real z = D[end];        // current value
    for (int i=end+1; i<=D.lastIdx(); i++)
    { if (D[i]> z)                // if the ith singular value is greater
      { D.swap(i-1, i);       // swap the cols
        if (withU) U.swapCols(i-1, i);
        if (withV) V.swapCols(i-1, i);
      }
      else break;
    } // end sort
  } // boucle end
  return error;
}

} // namespace STK
