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

/** @file STK_lapack_SymEigen.h
 *  @brief In this file .
 **/


#ifndef STK_LAPACK_SYMEIGEN_H
#define STK_LAPACK_SYMEIGEN_H

#include "STKernel/include/STK_Real.h"
#include "Sdk/include/STK_IRunner.h"

#include "Arrays/include/STK_Array2DSquare.h"
#include "Arrays/include/STK_CArraySquare.h"
#include "Arrays/include/STK_CArrayVector.h"

#ifdef STKUSELAPACK

extern "C"
{
#ifdef STKREALAREFLOAT
/** LAPACK routine in float to compute the eigenvalues */
extern void ssyevr_( char *, char *, char *, int *, float *, int *, float *,
                     float *, int *, int *, float *, int *, float *, float *, int *,
                    int *, float *, int *, int *, int *, int *);
#else
/** LAPACK routine in double to compute the eigenvalues */
extern void dsyevr_( char *, char *, char *, int *, double *, int *, double *,
                     double *, int *, int *, double *, int *, double *, double *, int *,
                    int *, double *, int *, int *, int *, int *);
#endif
}

#endif // STKUSELAPACK

namespace STK
{

namespace lapack
{
/** @ingroup Algebra
 *  {
 *    @class SymEigen
 *    @brief  SymEigen computes the eigenvalues and optionally the
 *    eigenvectors of a symmetric real matrix using the syevr Lapack routine.
 */
class SymEigen : public IRunnerBase
{
  public:
    /** @brief Constructor.
     *  @param data reference on a C-like symmetric square array
     */
    SymEigen( CArraySquareXX const& data);
    /** @brief Constructor
     *  @param data reference on a symmetric square expression
     */
    template<class Derived>
    SymEigen( ArrayBase<Derived> const& data)
            : norm_(0.), rank_(0), det_(0.)
            , range_(data.range()), data_()
            , eigenVectors_(range_.size(), 0.)
            , eigenValues_(range_.size(), 0.)
            , SupporteigenVectors_(2*range_.size(), 0)
            , JOBZ('V'), RANGE('A'), UPLO('U')
            , VL(0.0), VU(0.0), IL(0), IU(0)
            , nbEigenvalues_(range_.size())
    {
      data_ = data.asDerived();
      data_.shift(0);
      eigenVectors_.shift(0);
      eigenValues_.shift(0);
      SupporteigenVectors_.shift(0);
    }
    /** @brief copy constructor
     *  @param eigen the SymEigen to copy
     */
    SymEigen( SymEigen const& eigen);
    /** Destructor. */
    virtual ~SymEigen();
    /** @brief clone pattern */
    virtual SymEigen* clone() const;
    /** @brief Run eigenvalues decomposition
     *  Launch SYEVR LAPACK routine to perform the eigenvalues decomposition.
     *  @return @c true if no error occur, @c false otherwise
     */
    bool run();
    /** @return the trace norm of the matrix */
    inline Real norm()  const { return norm_;}
    /** @return the rank of the matrix */
    inline int rank()  const { return rank_;}
    /** @return the determinant of the Matrix */
    inline Real det()  const { return det_;}
    /**  @return the rotation matrix */
    inline CArraySquareXX const& eigenVectors() const{ return eigenVectors_;}
    /** @return the eigenvalues */
    inline CVectorX const& eigenValues() const { return eigenValues_;}

  protected:
    /** trace norm */
    Real norm_;
    /** rank */
    Real rank_;
    /** determinant */
    Real det_;
    /** range of the original data set. The data_ array is shifted in order
     *  to get a zero based array, so we need to conserve the origianl range
     **/
   Range range_;
   /** array with the original data. Will be overwritten. */
   CArraySquareXX data_;
   /** Square matrix or the eigenvectors. */
   CArraySquareXX eigenVectors_;
   /** Array of the eigenvalues */
   CVectorX eigenValues_;
   /** Array for the support of the eigenvectors */
   CArrayVector<int> SupporteigenVectors_;

    /** wrapper of the LAPACK SYEVR routine. Compute the eigenvalues of a symmetric
     *  square matrix.
     *
     *  @param[in] jobz
     * \verbatim
     *  CHARACTER*1
     *  = 'N':  Compute eigenvalues only;
     *  = 'V':  Compute eigenvalues and eigenvectors.
     * \endverbatim
     *
     *  @param[in] range
     *  \verbatim
     *  CHARACTER*1
     *  = 'A': all eigenvalues will be found.
     *  = 'V': all eigenvalues in the half-open interval  (VL,VU]  will be found.
     *  = 'I': the IL-th through IU-th eigenvalues will be found.
     * \endverbatim
     *
     *  @param[in] uplo
     * \verbatim
     * CHARACTER*1
     * = 'U':  Upper triangle of A is stored;
     * = 'L':  Lower triangle of A is stored.
     * \endverbatim
     *
     * @param[in] n The order of the matrix A.  N >= 0.
     *
     * @param[in,out] a Real array, dimension (LDA, N)
     * \verbatim
     * On entry, the symmetric matrix A.  If UPLO = 'U', the leading
     * N-by-N upper triangular part of A contains the upper  triangular part
     * of the  matrix  A.   If  UPLO  = 'L', the leading N-by-N lower triangular
     * part of A contains the lower triangular part of the matrix A.  On
     * exit, the lower triangle (if UPLO='L') or the upper triangle
     * (if UPLO='U') of A, including the diagonal, is destroyed.
     * \endverbatim
     *
     * @param[in] lda The leading dimension of the array A.  LDA >= max(1,N).
     *
     * @param[in] vl,vu
     * \verbatim
     *  Real If RANGE='V', the lower and  upper  bounds
     *  of  the  interval to be searched for eigenvalues. VL < VU.  Not
     *  referenced if RANGE = 'A' or 'I'.
     * \endverbatim
     *
     * @param[in] il, iu
     * \verbatim
     *  INTEGER If RANGE='I', the indices (in ascending order)
     *  of the smallest and largest eigenvalues to be returned.
     *  1 <= IL <= IU <= NL, if NL > 0; IL = 1 and IU = 0 if NL = 0. Not
     *  referenced if RANGE = 'A' or 'V'.
     * \endverbatim
     *
     * @param[in] abstol
     * \verbatim
     *  The  absolute error tolerance for the eigenvalues.  An approximate
     *  eigenvalue is accepted as converged when it is  determined
     *  to lie in an interval [a,b] of width less than or equal to
     *  ABSTOL + EPS *   max( |a|,|b| ) ,
     *  where  EPS is the machine precision.  If ABSTOL is less than or
     *  equal to zero, then  EPS*|T|  will be used in its place,  where
     *  |T|  is the 1-norm of the tridiagonal matrix obtained by reducing A
     *  to tridiagonal form.
     *  If high relative accuracy is important, set ABSTOL  to  SLAMCH(
     *  'Safe minimum' ).  Doing so will guarantee that eigenvalues are
     *  computed to high relative  accuracy  when  possible  in  future
     *  releases.   The current code does not make any guarantees about
     *  high relative accuracy, but future releases will. See J. Barlow
     *  and J. Demmel, "Computing Accurate Eigensystems of Scaled Diagonally
     *  Dominant Matrices", LAPACK Working Note #7, for  a  discussion of
     *  which matrices define their eigenvalues to high relative accuracy.
     * \endverbatim
     *  @see "Computing Small Singular  Values  of  Bidiagonal  Matrices
     *  with  Guaranteed  High Relative Accuracy," by Demmel and Kahan,
     *  LAPACK Working Note #3.
     *
     * @param[out] m
     * \verbatim
     *   The  total number of eigenvalues found.  0 <= M <= NL.  If RANGE
     *   = 'A', M = NL, and if RANGE = 'I', M = IU-IL+1.
     * \endverbatim
     *
     * @param[out] w
     * \verbatim
     *  array, dimension (NL)
     *  The first  M  elements  contain  the  selected  eigenvalues  in
     *  ascending order.
     * \endverbatim
     *
     * @param[out] z
     * \verbatim
     *  array, dimension (LDZ, max(1,M))
     *  If  JOBZ = 'V', then if INFO = 0, the first M columns of Z contain
     *  the orthonormal eigenvectors of the matrix A corresponding
     *  to  the selected eigenvalues, with the i-th column of Z holding
     *  the eigenvector associated with W(i).  If JOBZ = 'N', then Z is
     *  not  referenced.   Note:  the  user  must  ensure that at least
     *  max(1,M) columns are supplied in the array Z; if RANGE  =  'V',
     *  the exact value of M is not known in advance and an upper bound
     *  must be used.  Supplying N columns is always safe.
     * \endverbatim
     *
     * @param[in] ldz
     * \verbatim
     *  The leading dimension of the array Z.  LDZ >= 1, and if JOBZ  =
     *  'V', LDZ >= max(1,N).
     * \endverbatim
     *
     * @param[out] isuppz array, dimension ( 2*max(1,M) )
     * \verbatim
     *  The  support  of the eigenvectors in Z, i.e., the indices indicating
     *  the nonzero elements  in  Z.  The  i-th  eigenvector  is
     *  nonzero only in elements ISUPPZ( 2*i-1 ) through ISUPPZ( 2*i ).
     * \endverbatim
     *
     * @param[in,out] work Real array, dimension (MAX(1,LWORK))
     * \verbatim
     *   On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
     * \endverbatim
     *
     * @param[in] lwork The  dimension  of  the array WORK
     * \verbatim
     *  LWORK >= max(1,26*N). For optimal efficiency, LWORK >= (NB+6)*N, where
     *  NB is the max of the  blocksize for SSYTRD and SORMTR returned by ILAENV.
     *  If LWORK = -1, then a workspace query is assumed; the routine only
     *  calculates  the  optimal  sizes  of  the WORK and IWORK arrays,
     *  returns these values as the first entries of the WORK and IWORK
     *  arrays,  and  no  error  message  related to LWORK or LIWORK is
     *  issued by XERBLA.
     * \endverbatim
     *
     * @param[in,out] iwork array, dimension (MAX(1,LIWORK))
     * \verbatim
     *  On exit, if INFO = 0, IWORK(1) returns the optimal LWORK.
     * \endverbatim
     *
     * @param[in] liwork The dimension of the array IWORK.
     * \verbatim
     *  LIWORK >=  max(1,10*N). If LIWORK  =  -1, then a workspace query is
     *  assumed; the routine only calculates the optimal sizes of the WORK and
     *  IWORK arrays, returns these values as the first entries of the WORK and
     *  IWORK arrays, and no error message related  to  LWORK  or  LIWORK  is
     *  issued by XERBLA.
     * \endverbatim
     *
     * @return info
     * \verbatim
     *  = 0:  successful exit
     *  < 0:  if INFO = -i, the i-th argument had an illegal value
     *  > 0:  Internal error
     * \endverbatim
     */
    int syevr( char jobz, char range, char uplo
             , int n, double* a, int lda
             , double vl, double vu, int il, int iu
             , double abstol, int *m, double *w
             , double *z, int ldz, int *isuppz
             , double *work, int lwork, int *iwork, int liwork
             );
  private:
    /** Lapack pptions */
    char JOBZ, RANGE, UPLO;
    Real VL, VU;
    int IL, IU;
    /** The  total number of eigenvalues found */
    int nbEigenvalues_;
};

/** @} */

} // namespace lapack

} // namespace STK

#endif /* STK_LAPACK_SYMEIGEN_H */
