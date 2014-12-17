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

/* @brief Run eigen decomposition
 *  Launch SYEVR LAPACK routine to perform the eigenvalues decomposition.
 *  @return @c true if no error occur, @c false otherwise
 */
bool SymEigen::runImpl()
{
#ifdef STK_ALGEBRA_VERY_VERBOSE
  stk_cout << _T("Enter in SymEigen::run\n");
#endif
  // shift data sets
  eigenVectors_.shift(0);
  eigenValues_.shift(0);
  SupportEigenVectors_.shift(0);
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
  stk_cout << _T("Options: ") << JOBZ_ << " " << RANGE_ << " " << UPLO_ << "\n";
#endif
  info = syevr( JOBZ_, RANGE_, UPLO_
              , range_.size(), data_.p_data(), range_.size()
              , VL_, VU_, IL_, IU_
              , absTol, &rank_,  eigenValues_.p_data()
              , eigenVectors_.p_data(), range_.size(), SupportEigenVectors_.p_data()
              , &work, lwork, &iwork, liwork);
  // check any error
  if (info!=0)
  {
    if (info>0)
    { this->msg_error_ = STKERROR_NO_ARG(SymEigen::run,internal error);
      return false;
    }
    this->msg_error_= STKERROR_1ARG(SymEigen::run,-info,error parameter);
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
  info = syevr( JOBZ_, RANGE_, UPLO_
              , range_.size(), data_.p_data(), range_.size()
              , VL_, VU_, IL_, IU_
              , absTol, &rank_, eigenValues_.p_data()
              , eigenVectors_.p_data(), range_.size(), SupportEigenVectors_.p_data()
              , p_work, lwork, p_iwork, liwork);
  // recover memory
  delete[] p_work;
  delete[] p_iwork;

  // finalize
  data_.shift(range_.begin());
  eigenVectors_.shift(range_.begin());
  eigenValues_.shift(range_.begin());
  SupportEigenVectors_.shift(range_.begin());
  this->finalizeStep();
  // return the result of the computation
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
  int info = 0;
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


