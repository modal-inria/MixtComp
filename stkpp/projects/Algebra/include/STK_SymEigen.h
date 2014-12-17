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
 * Purpose:  Define The SymEigen Class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_SymEigen.h
 *  @brief In this file we define the SymEigen class (for a symmetric matrix).
 **/
 
#ifndef STK_SYMEIGEN_H
#define STK_SYMEIGEN_H

#include "STK_ISymEigen.h"
#include "Arrays/include/STK_Array2DVector.h"

namespace STK
{
/** @ingroup Algebra
 *  @brief The class SymEigen compute the eigenvalue Decomposition
 *  of a symmetric ArrayXX.
 * 
 *  The decomposition of a symmetric matrix require
 *  - Input:  A symmetric matrix A of size (n,n)
 *  - Output:
 *     -# P Array of size (n,n).
 *     -# D Vector of dimension n
 *     -# \f$ A = PDP' \f$
 *  The matrix A can be copied or overwritten by the class.
 *
 *  The 2-norm (operator norm) of the matrix is given. if the 2-norm is less
 *  than the arithmetic precision of the type @c Real, the rank is not
 *  full.
 *  Thus the user can be faced with a deficient rank matrix and with a norm and
 *  a determinant very small (but not exactly 0.0).
 **/
class SymEigen : public ISymEigen<SymEigen>
{
  public:
    typedef ISymEigen<SymEigen> Base;
    /** @brief Constructor
     *  @param data reference on a symmetric square matrix
     *  @param ref @c true if we overwrite the data set, @c false otherwise
     */
    inline SymEigen( CSquareXd const& data, bool ref =false)
                   : Base(data, ref)
                   , begin_(data.begin())
                   , last_(data.lastIdx())
    {}
    /** constructor.
     *  @param data A reference on the symmetric matrix to decompose.
     **/
    template<class Derived>
    SymEigen( ExprBase<Derived> const& data)
            : Base(data)
            , begin_(data.begin()), last_(data.lastIdx())
    {}
    /** Copy constructor.
     *  @param S the EigenValue to copy
     **/
    SymEigen(SymEigen const& S)
            : Base(S)
            , begin_(S.begin_)
            , last_(S.last_)
    {}
    /** virtual destructor */
    inline virtual ~SymEigen() {}
    /** clone pattern */
    inline virtual SymEigen* clone() const { return new SymEigen(*this);}

    /** @brief Diagonalization of eigenVectors_
     *  @return @c true if no error occur, @c false otherwise
     * */
    bool runImpl();
    /** Operator = : overwrite the SymEigen with S.
     *  @param S SymEigen to copy
     *  @return a reference on this
     **/
    inline SymEigen& operator=(const SymEigen &S)
    {
      Base::operator=(S);
      begin_ = S.begin_;    // first value
      last_  = S.last_;     // last value
      return *this;
    }
    /** get rotation matrix
     *  @return the rotation matrix
     **/
    inline CSquareXd const& rotation() const{ return eigenVectors_;}

  protected:
    /// first row/col of eigenVectors_
    int begin_;
    /// last row/col of eigenVectors_
    int last_;

  private:
    /** Temporary vector. Values of the sub-diagonal. */
    Vector F_;
    /** @brief compute the tri-diagonalization of eigenVectors_ */
    void tridiagonalize();
    /** @brief compute the Householder matrix and P */
    void compHouse();
    /** computing the diagonalization of eigenValues_ and F_ */
    void diagonalize();
};

} // namespace STK

#endif //STK_SYMEIGEN_H
