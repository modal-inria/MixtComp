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
 * Purpose:  Define The Svd Class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Svd.h
 *  @brief In this file we define the Svd Class.
 **/
 
#ifndef STK_SVD_H
#define STK_SVD_H

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DPoint.h"
#include "Arrays/include/STK_Array2DVector.h"
#include "Arrays/include/STK_Array2DSquare.h"

namespace STK
{

/** @ingroup Algebra
 *  @brief The class Svd compute the Singular Value Decomposition
 *  of a Array with the Golub-Reinsch Algorithm.
 * 
 *  The method take as:
 *  - input: A matrix A(nrow,ncol)
 *  - output:
 *    -# U Array (nrow,ncolU).
 *    -# D Vector (ncol)
 *    -# V Array (ncol,ncol).
 *  and perform the decomposition: 
 *  - A = UDV' (transpose V).
 *  U can have more cols than A,
 *  and it is possible to ompute some (all) vectors of Ker(A).
 **/
class Svd
{
  protected:
    /* containers */
    ArrayXX       U_;    ///< U_ matrix
    ArraySquareX V_;    ///< V_ square matrix
    Point        D_;    ///< Array of the singular values

    /* dimensions */
    int ncolV_;   ///< Number of cols (and rows) of V
    int ncolD_;   ///< Number of cols of D_
    int ncolU_;   ///< Number of cols of U
    int nrowU_;   ///< Number of rows of U_

    /* flags */
    bool withU_;        ///< Compute U_ ?
    bool withV_;        ///< Compute V_ ?
    bool ref_;          ///< Is this structure just a pointer on U_ ?

    /* results */
    Real    norm_;       ///< norm of the matrix (largest singular value)
    int rank_;       ///< rank of the matrix
    bool    error_;      ///< Everything OK during computation ?

  public :   
    /** Default constructor
     *  @param A the matrix to decompose.
     *  @param ref if true, U_ is a reference of A.
     *  @param withU if true, we save the left housolder transforms
     *  in U_.
     *  @param withV if true, we save the right housolder transforms
     *  in V_.
     **/
    Svd( ArrayXX const&    A       = ArrayXX()
       , bool      ref     = false
       , bool      withU   = true
       , bool      withV   = true
       );

    /** Copy Constructor
     *  @param S the Svd to copy
     **/
    Svd( const Svd &S);

    /** destructor.
     **/
    virtual ~Svd();
 
    /** Operator = : overwrite the Svd with S.
     *  @param S the Svd to copy
     **/
    Svd& operator=(const Svd &S);

    /** clear U_.                                                     */
    void clearU();

    /** clear V_.                                                     */
    void clearV();

    /** clear U_, V_ and D_.                                          */
    void clear();

    /** Compute the svd of the Array A and copy the data
     *  see the corresponding constructor Take care that if U_ was previously
     *  a reference, it cannot be modified.
     *  @param A is the matrix to decompose.
     *  @param withU if true, we save the left housolder transforms
     *  in U_.
     *  @param withV if true, we save the right housolder transforms
     *  in V_.
     **/    
    void newSvd( ArrayXX const&    A       = ArrayXX()
               , bool      withU   = true
               , bool      withV   = true
               );

    /** Computing the bidiagonalisation of M.
     *  The diagonal and the subdiagonal are stored in D and F
     *  @param M the matrix to bidiagonalize, the matrix is overwritten
     *  with the left and right Householder vectors.
     *  The method return the estimate of the inf norm of M.
     *  @param D the element of the diagonal
     *  @param F the element of the surdiagnal
     **/
    static Real bidiag(const ArrayXX& M, Point& D, Vector& F);

    /** right eliminate the element on the subdiagonal of the row nrow
     *  @param D the diagonal of the matrix
     *  @param F the surdiagonal of the matrix
     *  @param nrow the number of the row were we want to rightEliminate
     *  @param V a right orthogonal Square ArrayXX
     *  @param withV true if we want to update V
     *  @param tol the tolerance to use
     **/
    static void rightEliminate( Point& D
                              , Vector& F
                              , int const& nrow
                              , ArraySquareX& V
                              , bool withV = true
                              , Real const& tol = Arithmetic<Real>::epsilon()
                              );

    /** left eliminate the element on the subdiagonal of the row nrow
     *  @param D the diagonal of the matrix
     *  @param F the surdiagonal of the matrix
     *  @param nrow the number of the row were we want to rightEliminate
     *  @param U a left orthogonal ArrayXX
     *  @param withU true if we want to update U
     *  @param tol the tolerance to use
     **/
    static void leftEliminate( Point& D
                             , Vector& F
                             , int const& nrow
                             , ArrayXX& U
                             , bool withU = true
                             , Real const& tol = Arithmetic<Real>::epsilon()
                             );

    /** Computing the diagonalisation of a bidiagnal matrix
     *  @param D the diagoanl of the matrix
     *  @param F the subdiagonal of the matrix
     *  @param U a left orthogonal ArrayXX
     *  @param withU true if we want to update U
     *  @param V a right orthogonal Square ArrayXX
     *  @param withV true if we want to update V
     *  @param tol the tolerance to use
     **/
    static bool diag( Point& D
                    , Vector& F
                    , ArrayXX& U
                    , ArraySquareX& V
                    , bool withU = true
                    , bool withV = true
                    , Real const& tol = Arithmetic<Real>::epsilon()
                    );

    /// Number of rows of U_
    inline int nrowU() const { return U_.sizeRows();}
    /// Number of cols of U_
    inline int ncolU() const { return U_.sizeCols();}
    /// Number of rows of D_
    inline int ncolD() const { return ncolD_;}
    /// Number of rows of V_
    inline int nrowV() const { return V_.sizeRows();}
    /// Number of cols of V_
    inline int ncolV() const { return V_.sizeCols();}
    /// Norm of the matrix
    inline Real normSup()  const { return norm_;}
    /// rank of the matrix
    inline int rank()  const { return rank_;}
    /// if an error occur during svd()
    inline bool error()    const { return error_;}
    /// get U (const)
    inline ArrayXX const&       getU() const { return U_;}
    /// get V (const)
    inline ArraySquareX const& getV() const { return V_;}
    /// get D (const)
    inline const Point&        getD() const { return D_;}
    
    /// get U
    inline ArrayXX&       getU() { return U_;}
    /// get V
    inline ArraySquareX& getV() { return V_;}
    /// get D
    inline Point&        getD() { return D_;}
    
  private:
    /// Values of the Surdiagonal
    Vector F_;
    /// Initialize the containers
    void init();
    /// Svd main steps
    void compSvd();
    /// Compute U (if withU_ is true)
    void compU();
    /// Compute V (if withV_ is true)
    void compV();
};

} // namespace STK

#endif
// STK_SVD_H
