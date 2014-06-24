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
 * Project:  stkpp::Algebra
 * Purpose:  Define the Qr Class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Qr.h
 *  @brief This file define the Qr class (QR decomposition).
 **/
 
#ifndef STK_QR_H
#define STK_QR_H

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DUpperTriangular.h"

namespace STK
{

/** @ingroup Algebra
 *  @brief The class Qr perform the QR decomposition of a Matrix.
 * 
 *  The user have to take care to the behavior of the class if withq_
 *  is false: in this case the QR decompostion cannot be updated if
 *  he want to add a column.
 *
 *  Also the Addcol or insertCols methods will fail if ncolmax
 *  is not large enough.
 *
 *  - Input:  A matrix (nrow,ncolq)
 *  - Output:
 *    -# Q  matrix (nrow,ncol) orthonormal.
 *    -# R  matrix square (ncol,ncolq) upper triangular
 *  - A=QR.
 */
class Qr
{
  protected :
    /** Q Matrix of the QR decomposition */
    Matrix Q_;
    /** R Matrix of th QR decomposition */
    MatrixUpperTriangular R_;
    /// Number of cols of R actually computed
    int  ncolr_;
    /// Number of cols used by Q and Number of rows of R_
    int  ncolq_;
    /// Number of rows used by Q
    int  nrowq_;
    /// is Q computed ?
    bool compq_;

  public :
    /** Default constructor.
     *  @param A the matrix to decompose
     *  @param ref true if we overwrite A
     **/
    Qr( Matrix const&  A = Matrix(), bool ref = false);

    /** virtual destructor */
    virtual ~Qr();

    /** Operator = : overwrite the Qr with S. */
    Qr& operator=(const Qr &S);
    /** Is Q computed ?
     *  @return @c true if Q_ is computed, @c false otherwise
     */
    inline bool isCompQ() const { return compq_;}
    /** give the matrix Q of the QR decomposition.
     * @return the matrix Q of the QR decomposition
     **/
    inline Matrix const& Q() const  { return Q_;}
    /** give the matrix R of the QR decomposition.
     * @return the matrix R of the QR decomposition
     **/
    inline MatrixUpperTriangular const& R() const { return R_;}
    /** clear Q_ and R_. */
    void clear();
    /** Compute the QR decomposition. **/
    void run();
    /** Compute Q (to use after run). After the run process, Q_ store
     *  the householder vector in its column. Call compQ, if you want to
     *  obtain Q in its true form.
     *  Without effect if (compq_ == true)
     **/
    void compQ();
    /** Delete the n last columns and update the QR decomposition.
     *  @param n number of column to delete
     **/    
    void popBackCols(int const& n =1);
    /** Delete the column pos and update the QR decomposition.
     *  @param pos the position of the column to delete
     **/    
    void eraseCol(int const& pos);

    /** Add a column with value T and update th QR decomposition.
     *  @param T the column to add
     **/
    void pushBackCol(Vector const& T);

    /** Add a column with value T at the position pos and update the QR
     *  decomposition.
     *  @param T the column to insert
     *  @param pos the position of the column to insert
     **/
    void insertCol(Vector const& T, int const& pos);

    /* TODO : Delete the ith row and update the QR decomposition :
     *  default is the last row.
     **/    
    //Qr& popBackRows();
    //Qr& eraseRows(int i);

    /* TODO : Add a row with value T and update th QR decomposition :
     *  default is the last column position.
     **/
    //Qr& pushBackRows(const Array2DPoint<double> &T);
    //Qr& insertRows(const Array2DPoint<double> &T, int i);

  private:
  /** Compute the qr decoposition of the matrix Q_ */
    void qr();
};

} // namespace STK

#endif
// STK_QR_H

