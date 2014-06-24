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
 * Project:  stkpp::Algebra
 * created on: 20 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_lapack_SymEigen.cpp
 *  @brief In this file we implement the SymEigen class.
 **/

#include<cmath>

#include "../include/STK_lapack_SymEigen.h"
#include "STKernel/include/STK_Misc.h"

namespace STK
{

namespace lapack
{

/* Specialization of the constructor for the CArraySquare data*/
SymEigen::SymEigen( CArraySquareXX const& data)
                  : norm_(0.), rank_(0), det_(0.)
                  , range_(data.range()), data_(data)
                  , eigenVectors_(range_.size(), 0.)
                  , eigenValues_(range_.size(), 0.)
                  , SupporteigenVectors_(2*range_.size(), 0)
                  , JOBZ('V'), RANGE('A'), UPLO('U')
                  , VL(0.0), VU(0.0), IL(0), IU(0)
                  , nbEigenvalues_(range_.size())
{}

/* Specialization of the constructor for the CArraySquare data*/
SymEigen::SymEigen( SymEigen const& eigen)
                  : norm_(eigen.norm_), rank_(eigen.rank_), det_(eigen.det_)
                  , range_(eigen.range_), data_(eigen.data_)
                  , eigenVectors_(eigen.eigenVectors_)
                  , eigenValues_(eigen.eigenValues_)
                  , SupporteigenVectors_(eigen.SupporteigenVectors_)
                  , JOBZ(eigen.JOBZ), RANGE(eigen.RANGE), UPLO(eigen.UPLO)
                  , VL(eigen.VL), VU(eigen.VU), IL(eigen.IL), IU(eigen.IU)
                  , nbEigenvalues_(eigen.nbEigenvalues_)
{}

/* Destructor. */
SymEigen ::~SymEigen() {}

/* @brief clone pattern */
SymEigen* SymEigen::clone() const
{ return new SymEigen(*this);}

/* @brief Run eigen decomposition
 *  Launch SYEVR LAPACK routine to perform the eigenvalues decomposition.
 *  @return @c true if no error occur, @c false otherwise
 */
bool SymEigen::run()
{
#ifdef STK_ALGEBRA_VERY_VERBOSE
  stk_cout << _T("Enter in SymEigen::run\n");
#endif
  stk_cout << "\n";
  /* set default behavior */
  Real absTol = 0.0; // let Lapack chose the correct tolerance
  // get optimal size necessary for work
  Real work; // work is just one place, get the optimal size for WORK
  int iwork; // iwork is just on place, get the optimal size for IWORK

  int lwork =-1, liwork =-1; // workspace variable

  int info = 1;
#ifdef STK_ALGEBRA_DEBUG
  stk_cout << _T("Data dimensions: ") << data_.rows() << " " << data_.cols() << "\n";
  stk_cout << _T("eigenValues_ dimensions: ") << eigenValues_.rows() << " " << eigenValues_.cols() << "\n";
  stk_cout << _T("eigenVectors_ dimensions: ") << eigenVectors_.rows() << " " << eigenVectors_.cols() << "\n";
  stk_cout << _T("Options: ") << JOBZ << " " << RANGE << " " << UPLO << "\n";
#endif
  info = syevr( JOBZ, RANGE, UPLO
              , range_.size(), data_.p_data(), range_.size()
              , VL, VU, IL, IU
              , absTol, &nbEigenvalues_,  eigenValues_.p_data()
              , eigenVectors_.p_data(), range_.size(), SupporteigenVectors_.p_data()
              , &work, lwork, &iwork, liwork);
  // check any error
  if (info!=0)
  {
    if (info>0)
    { msg_error_ = STKERROR_NO_ARG(SymEigen::run,internal error);
      return false;
    }
    msg_error_= STKERROR_1ARG(SymEigen::run,-info,error parameter);
    return false;
  }
#ifdef STK_ALGEBRA_DEBUG
  stk_cout << _T("Size needed:") << (int)work << " " << iwork << " " << "\n";
#endif
  // get results and allocate space
  lwork = (int)work;
  liwork = iwork;
  Real* p_work = new Real[lwork];
  int* p_iwork = new int[liwork];

  // Call SYEVR with the optimal block size
  info = syevr( JOBZ, RANGE, UPLO
              , range_.size(), data_.p_data(), range_.size()
              , VL, VU, IL, IU
              , absTol, &nbEigenvalues_, eigenValues_.p_data()
              , eigenVectors_.p_data(), range_.size(), SupporteigenVectors_.p_data()
              , p_work, lwork, p_iwork, liwork);
  // recover memory
  delete[] p_work;
  delete[] p_iwork;

  // shift back
  data_.shift(range_.firstIdx());
  eigenVectors_.shift(range_.firstIdx());
  eigenValues_.shift(range_.firstIdx());
  SupporteigenVectors_.shift(range_.firstIdx());

  // compute rank and determinant
  rank_ = nbEigenvalues_;
  int s = 1;
  for (int i=eigenValues_.firstIdx(); i<= eigenValues_.lastIdx(); ++i )
  {
    norm_ += eigenValues_.elt(i);
    if (std::abs(eigenValues_.elt(i)) <Arithmetic<Real>::epsilon()) { rank_--;}
    s *= sign(eigenValues_.elt(i));
  }
  if (rank_ == nbEigenvalues_)
  {
    Real sum = 0.;
    for (int i=eigenValues_.firstIdx(); i<= eigenValues_.lastIdx(); ++i )
    {
      if (std::abs(eigenValues_.elt(i)) <Arithmetic<Real>::epsilon()) { rank_--;}
      sum += std::log(std::abs(eigenValues_.elt(i)));
    }
    det_ = s* std::exp(sum);
  }
  // return the result of the compulation
  if (!info) return true;
  if (info>0)
  { msg_error_ = STKERROR_NO_ARG(SymEigen ::run,internal error);
    return false;
  }
  msg_error_= STKERROR_1ARG(SymEigen ::run,-info,error parameter);
  return false;
}

/* wrapper of the LAPACK routine to compute the eigenvalues */
int SymEigen::syevr( char jobz, char range, char uplo
                   , int n, Real* a, int lda
                   , Real vl, Real vu, int il, int iu
                   , Real abstol, int *m, Real *w
                   , Real *z, int ldz, int *isuppz
                   , Real *work, int lwork, int *iwork, int liwork
                   )
{
  int info = 1;
#ifdef STKUSELAPACK
#ifdef STKREALAREFLOAT
  ssyevr_(&jobz, &range, &uplo, &n, a, &lda, &vl, &vu, &il,
          &iu, &abstol, m, w, z, &ldz, isuppz, work,
          &lwork, iwork, &liwork, &info);
#else
  dsyevr_(&jobz, &range, &uplo, &n, a, &lda, &vl, &vu, &il,
          &iu, &abstol, m, w, z, &ldz, isuppz, work,
          &lwork, iwork, &liwork, &info);
#endif
#endif
  return info;
}

} // namespace lapack

} // namespace STK


