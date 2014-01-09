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

#include "../../Arrays/include/STK_Array2DSquare.h"
#include "../../Arrays/include/STK_Array2DLowerTriangular.h"
#include "../../Arrays/include/STK_Array2D_Functors.h"

#include "../include/STK_Householder.h"
#include "../include/STK_Givens.h"

namespace STK
{

#ifdef STK_DEBUG
template< class Container2D >
void print(Container2D const& A, String const& name)
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
  stk_cout << name << _T(".p_data() =") << A.allocator().p_data()  << _T("\n");
}
#endif

/* Constructor */
Qr::Qr( Matrix const& A, bool ref)
      : Q_(A, ref)      // Creating Q
      , R_()            // Creating R
{ run();}

/* Computing the QR decomposition of the matrix Q_.                   */
void Qr::run()
{
  if (Q_.empty())     // if the container is empty
  {
    ncolr_ =0;
    ncolq_ =0;
    nrowq_ =0;
    compq_  = true;  // Q_ is computed
    return;
  }

  Q_.shift(1,1);      // translate the beg to 1
  ncolr_ = Q_.sizeCols(); // Number of cols of R
  ncolq_ = Q_.sizeCols(); // Number of column of Q
  nrowq_ = Q_.sizeRows(); // Number of rows of Q
  compq_  = false;    // Q_ is not computed
  // compute QR decomposition
  qr();
}


/* Computation of the QR decomposition */
void Qr::qr()
{
  int niter = std::min(nrowq_,ncolr_);   // number of iterations
  R_.resize(Range(1,nrowq_), Range(1,ncolr_));
  R_ = 0.0;                   // initialize to 0.0

  /* Main loop. */
  for (int iter=1, iter1=2; iter<=niter; iter++, iter1++)
  { 
    // A ref on the row iter of the matrix R_
    Point  Rrow0(R_, Range(iter, ncolr_, 0), iter);
    // A ref on the row iter of the matrix Q_
    Point  Qrow0(Q_, Range(iter, ncolq_, 0), iter);
    // A ref on the column iter of the matrix Q_. Householder vector
    Vector u(Q_, Range(iter, nrowq_, 0), iter);
    // compute the Householder vector of the current column
    Rrow0[iter] = house<Vector>(u);
    // get beta
    Real beta = u.front();
    if (beta)
    {
      // ref on the essential part of the Householder vector
      Vector v(u, Range(iter1, nrowq_, 0));
      // Apply Householder to next cols
      for (int j=iter1; j<=ncolr_; j++)
      {
        // Auxiliary data
        Real aux;
        // a ref on the jth column of Q_
        Vector z(Q_, Range(iter1, nrowq_, 0), j);
        // save the  current row of R_
        Rrow0[j] = Qrow0[j] + (aux = ( Qrow0[j] + dot<Vector, Vector>(v, z)) * beta);
        // update the next cols of Q_ 
        z += v * aux;
      }
    }
    else
    {
      // just copy the row iter in R_
      for (int j=iter1; j<=ncolr_; j++) { Rrow0[j] = Qrow0[j];}
    }
  }
}


/* Computation of Q. */
void Qr::compQ()
{
  // if Q_ is computed yet
  if (compq_) return;
  // number of non zero cols of Q_  
  int ncol  = std::min(nrowq_, ncolq_);
  // Q_ is square
  if (nrowq_ < ncolq_)
  { Q_.popBackCols(ncolq_-nrowq_);}
  else
  { 
    Q_.pushBackCols(nrowq_-ncolq_);
    Q_.col(Range(ncol+1,nrowq_, 0) ) = 0.0;
  }
  // the number of col_ is equal to the number of row
  ncolq_ = nrowq_;
  // compute added column
  for (int iter=ncolq_; iter> ncol; --iter) { Q_(iter, iter) = 1.0;}
  // compute other columns
  for (int iter=ncol, iter1=ncol+1; iter>=1; iter--, iter1--)
  {
    // Get beta and test
    Real beta = Q_(iter,iter);
    if (beta)
    {
      // ref of the row iter
      Point P(Q_, Range(iter,ncolq_, 0), iter);
      // ref of the column iter
      Vector X(Q_, Range(iter1,nrowq_, 0), iter);
      // Update the cols from iter+1 to ncol
      for (int j=iter1; j<=ncolq_; j++)
      { Real aux;
        Vector Y(Q_, Range(iter1,nrowq_, 0), j); // ref on the column j
        // Q_(iter, j) = beta * X'Y
        P[j] = (aux = dot<Vector, Vector>( X, Y) * beta);
        // Q^j += aux * Q^iter
        Y += X * aux;
      }
      P[iter] = 1.0 + beta;
      // Q^iter *= beta
      X *= beta;
    }
    else // Q^iter = identity
    {
      Q_(iter, iter) = 1.0;
      Q_(Range(iter1,nrowq_, 0), iter) = 0.0;
    }
    // update the column iter
    Q_(Range(1,iter-1, 0), iter) = 0.0;
  }
  // Q_ is now computed
  compq_ = true;
}


/* Destructeur de la classe Qr */
Qr::~Qr() { ;}

/* clear Q_ and R_. */
void Qr::clear()
{
  Q_.clear();
  R_.clear();
}


/* Operator = : overwrite the Qr with S. */
Qr& Qr::operator=(const Qr& S)
{
  ncolr_ = S.ncolr_;       //< Number of cols of R actually computed
  ncolq_ = S.ncolq_;       //< Number of cols used by Q
  nrowq_ = S.nrowq_;       //< Number of rows used by Q
    
  compq_ = S.compq_;       //< Is Q computed ?

  Q_ = S.Q_;               //< Matrix V
  R_ = S.R_;               //< Singular values

  return *this;
}

/* Delete the jth column and update the QR decomposition : default
 * is the last col
 **/    
void Qr::popBackCols(int const& n)
{
  // delete n cols
  R_.popBackCols(n);
  ncolr_ -= n;
}

void Qr::eraseCol(int const& pos)
{
  if (pos < R_.firstIdxCols())
  { STKOUT_OF_RANGE_1ARG(Qr::eraseCol,pos,pos<R_.firstIdxCols());}
  if (R_.lastIdxCols() < pos)
  { STKOUT_OF_RANGE_1ARG(Qr::eraseCol,pos,pos<R_.lastIdxCols()<pos);}
  // if Q_ is not computed yet
  if (!compq_) compQ();
  // compute the number of iteration for updating to zeroed
  int niter = R_.firstIdxCols()-1+std::min(R_.sizeRows(), R_.sizeCols());
  // Zeroed the unwanted elements (z)
  for (int iter = pos+1; iter<=niter; iter++)
  {
    Real sinus, cosinus;
    // compute the Givens rotation
    R_(iter-1, iter) = compGivens( R_(iter-1, iter), R_(iter, iter), cosinus, sinus);
    R_(iter, iter)   = 0.0;
    // if necessary update R_ and Q_
    if (sinus)
    {
      // create a reference on the sub-Matrix
      MatrixUpperTriangular Rsub(R_.col(Range(iter+1, R_.lastIdxCols(), 0)), true);
      // Update the next rows (iter1:ncolr_) of R_
      leftGivens (Rsub, iter-1, iter, cosinus, sinus);
      // Update the cols of Q_
      rightGivens(Q_, iter-1, iter, cosinus, sinus);
    }
  }
  // erase the column pos
  R_.eraseCols(pos);
  ncolr_--;
  
  // update the range of the remaining cols of the container
  R_.update(Range(pos, std::min(R_.lastIdxRows(), R_.lastIdxCols()), 0));
}


/* Adding the last column and update the QR decomposition.               */    
void Qr::pushBackCol(Vector const& T)
{
  // check conditions
  if (T.range() != Q_.rows())
  { STKRUNTIME_ERROR_NO_ARG(Qr::pushBackCol,T.range() != Q_.rows());}
  // if Q_ is not computed yet
  if (!compq_) compQ();
  // Adding a column to R
  R_.pushBackCols();
  ncolr_++;
  // Create an auxiliary container
  Vector Rncolr(Q_.cols());
  // Multipliate T by Q'and put the result in Rncolr
  for (int j=Q_.firstIdxCols(); j<=Q_.lastIdxCols(); j++)
    Rncolr[j] = dot(Q_.col(j), T);
  // update Q_
  for (int iter = ncolq_-1, iter1 = ncolq_; iter>=ncolr_; iter--, iter1--)
  { 
    Real sinus, cosinus, y = Rncolr[iter], z = Rncolr[iter1] ;
    // compute the Givens rotition
    Rncolr[iter]  = compGivens(y, z, cosinus, sinus);
    // apply Givens rotation if necessary
    if (sinus)
    {
      // Update the cols of Q_
      rightGivens(Q_, iter, iter1, cosinus, sinus);
    }
  }
  // update R_
  R_.col(ncolr_) = Rncolr.sub(R_.rangeRowsInCol(ncolr_));
}


/* Adding the jth column and update the QR decomposition.               */
void Qr::insertCol(Vector const& T, int const& pos)
{
  if (pos < R_.firstIdxCols())
  { STKOUT_OF_RANGE_1ARG(Qr::insertCol,pos,pos<R_.firstIdxCols());}
  if (R_.lastIdxCols() < pos)
  { STKOUT_OF_RANGE_1ARG(Qr::insertCol,pos,pos<R_.lastIdxCols()<pos);}
  if (T.range() != Q_.rows())
  { STKRUNTIME_ERROR_1ARG(Qr::insertCol,pos,T.range() != Q_.rows());}
  // if Q_ is not computed yet
  if (!compq_) compQ();
  // Adding a column to R
  R_.insertCols(pos);
  ncolr_++;
  // update the range of the remaining cols of R_
  R_.update(Range(pos+1, std::min(R_.lastIdxRows(), R_.lastIdxCols()), 0));
  for (int i=pos+1; i<= std::min(R_.lastIdxRows(), R_.lastIdxCols()); ++i) R_(i,i) = 0.0;
  // A ref on the last column of R_
  Vector Rpos(Q_.cols());
  // Multipliate T by Q'. we cannot use mult as we are using ncolq_ columns.
  for (int j=Q_.firstIdxCols(); j<=Q_.lastIdxCols(); j++)
    Rpos[j] = dot(Q_.col(j), T);
  // Zeroed the unwanted elements
  for (int iter= ncolq_-1, iter1= ncolq_; iter>=pos; iter--, iter1--)
  { 
    Real sinus, cosinus, y = Rpos[iter], z = Rpos[iter1] ;
    // compute the Givens rotation
    Rpos[iter]  = compGivens(y, z, cosinus, sinus);
    // apply Givens rotation if necessary
    if (sinus)
    {
      // create a reference on the sub-Matrix
      MatrixUpperTriangular Rsub(R_.col(Range(iter1, R_.lastIdxCols(), 0)), true);
      // Update the next rows (iter1:ncolr_) of R_
      leftGivens( Rsub, iter, iter1, cosinus, sinus);
      // Update the cols of Q_
      rightGivens(Q_, iter, iter1, cosinus, sinus);
    }
  }
  // update R_
  R_.col(pos) = Rpos.sub(R_.rangeRowsInCol(pos));
  R_.update(pos);
}

} // namespace STK

