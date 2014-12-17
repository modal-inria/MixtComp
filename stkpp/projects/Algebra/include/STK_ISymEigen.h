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
 * Purpose:  Define The Interface SymEigen Class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ISymEigen.h
 *  @brief In this file we define the ISymEigen class (for a symmetric matrix).
 **/
 
#ifndef STK_ISYMEIGEN_H
#define STK_ISYMEIGEN_H

#include "STKernel/include/STK_Real.h"
#include "Sdk/include/STK_IRunner.h"

#include "Arrays/include/STK_CArraySquare.h"
#include "Arrays/include/STK_CArrayVector.h"
#include "Arrays/include/STK_CArrayPoint.h"

namespace STK
{
/** @ingroup Algebra
 *  @brief The class ISymEigen is an interface class for the method
 *  computing the eigenvalue Decomposition of a symmetric ArrayXX.
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
 *  than the arithmetic precision of the type @c Real, the rank is not full.
 *  Thus the user can be faced with a deficient rank matrix and with a norm
 *  very small (i.e. not exactly 0.0).
 **/
template<class Derived>
class ISymEigen : public IRunnerBase, public IRecursiveTemplate<Derived>
{
  public:
    typedef IRunnerBase Base;
    /** @brief Constructor
     *  The original data set can be overwritten by the eigenvectors if it is
     *  stored in a CSquareXd. Observe that in this case the base index have
     *  to be 0 otherwise.
     *  @param data reference on a symmetric square matrix
     *  @param ref @c true if we overwrite the data set, @c false otherwise
     */
    ISymEigen( CSquareXd const& data, bool ref =false)
             : Base()
             , norm_(0.), rank_(0), det_(0.)
             , eigenVectors_(data, ref)
             , eigenValues_(data.size(), 0.)
             , SupportEigenVectors_(2*data.size(), 0)
    {}
    /** @brief Templated constructor
     *  @param data reference on a symmetric square expression
     */
    template<class OtherDerived>
    ISymEigen( ExprBase<OtherDerived> const& data)
             : Base()
             , norm_(0.), rank_(0), det_(0.)
             , eigenVectors_(data.asDerived())
             , eigenValues_(data.size(), 0.)
             , SupportEigenVectors_(2*data.size(), 0)
    {
      STK_STATICASSERT(OtherDerived::structure_==(int)Arrays::square_,YOU_HAVE_TO_USE_A_SQUARE_MATRIX_IN_THIS_METHOD)
    }
    /** Copy constructor.
     *  @param eigen the EigenValue to copy
     **/
    ISymEigen( ISymEigen const& eigen)
             : Base(eigen)
             , norm_(eigen.norm_), rank_(eigen.rank_), det_(eigen.det_)
             , eigenVectors_(eigen.eigenVectors_)
             , eigenValues_(eigen.eigenValues_)
             , SupportEigenVectors_(eigen.SupportEigenVectors_)
    {}

    /** virtual destructor */
    inline virtual ~ISymEigen() {}
    /** Operator = : overwrite the ISymEigen with eigen.
     *  @param eigen ISymEigen to copy
     *  @return a reference on this
     **/
    ISymEigen& operator=( ISymEigen const& eigen)
    {
      norm_   = eigen.norm_;     // norm of the matrix
      rank_   = eigen.rank_;     // rank of the matrix
      det_    = eigen.det_;      // determinant of the matrix
      eigenVectors_ = eigen.eigenVectors_;
      eigenValues_  = eigen.eigenValues_;
      SupportEigenVectors_ = eigen.SupportEigenVectors_;
      return *this;
    }
    /** @return the trace norm of the matrix */
    inline Real norm()  const { return norm_;}
    /** @return the rank of the matrix */
    inline int rank()  const { return rank_;}
    /** @return the determinant of the Array */
    inline Real det()  const { return det_;}
    /**  @return the rotation matrix */
    inline CSquareXd const& rotation() const{ return eigenVectors_;}
    /**  @return the rotation matrix */
    inline CSquareXd const& eigenVectors() const{ return eigenVectors_;}
    /** @return the eigenvalues */
    inline CVectorX const& eigenValues() const { return eigenValues_;}
    /** Compute the generalized inverse of the symmetric matrix and put
     *  the result in res.
     *  @param res the generalized inverse of the ArrayXX.
     */
    template<class ArraySquare>
    void ginv(ArraySquare& res)
    {
      STK_STATICASSERT(ArraySquare::structure_==(int)Arrays::square_,YOU_HAVE_TO_USE_A_SQUARE_MATRIX_IN_THIS_METHOD)
      // create pseudo inverse matrix
      res.resize(eigenVectors_.range());
      res = 0;
      // compute tolerance
      Real tol = Arithmetic<Real>::epsilon() * norm_;
      // compute PDP'
      for (int k = eigenVectors_.begin(); k< eigenVectors_.end(); k++)
      {
        Real value = eigenValues_[k];
        if (std::abs(value) > tol)
        {
          res += (eigenVectors_.col(k) * eigenVectors_.col(k).transpose())/value;
        }
      }
    }
    /** overloading of setData.
     * @param data the data set to set.
     **/
    template<class OtherDerived>
    void setData( ExprBase<OtherDerived> const& data)
    {
      norm_ = 0.; rank_ = 0; det_ = 0.;
      eigenVectors_ = data;
      eigenValues_.resize(data.range());
      SupportEigenVectors_.resize(2*data.size());
    }
    virtual bool run()
    {
      if (eigenVectors_.empty()) { return true;}
      return this->asDerived().runImpl();
    }

  protected:
    /** trace norm */
    Real norm_;
    /** rank */
    int rank_;
    /** determinant */
    Real det_;
   /** Square matrix or the eigenvectors. */
   CSquareXd eigenVectors_;
   /** Array of the eigenvalues */
   CVectorX eigenValues_;
   /** Array for the support of the eigenvectors */
   CArrayVector<int> SupportEigenVectors_;
   /** finalize the computation by computing the rank, the trace norm and the
    * determinant of the matrix.
    **/
   void finalizeStep();
};


/* finalize the computation by computing the rank, the trace norm and the
 * determinant of the matrix.
 **/
template<class Derived>
void ISymEigen<Derived>::finalizeStep()
{
  norm_ = 0;
  rank_ = eigenValues_.size();
  det_  = 0;
  // compute trace norm sign of the determinant
  int s = 1;
  for (int i=eigenValues_.begin(); i< eigenValues_.end(); ++i )
  {
    Real value = eigenValues_.elt(i);
    norm_ += value;
    s     *= sign(value);
    if (std::abs(value) < Arithmetic<Real>::epsilon()) { rank_--;}
  }
  // compute the determinant for full rank matrices
  if (rank_ == eigenValues_.size())
  {
    Real sum = 0.;
    for (int i=eigenValues_.begin(); i< eigenValues_.end(); ++i )
    { sum += std::log(std::abs(eigenValues_.elt(i)));}
    det_ = s* std::exp(sum);
  }
}

} // namespace STK

#endif //STK_ISYMEIGEN_H
