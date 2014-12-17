/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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
 * Project:  stkpp::Arrays
 * created on: 27 sept. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ExprBaseFunctors.h
 *  @brief In this file we define the way to apply functors to Expressions.
 **/

#ifndef STK_EXPRBASEFUNCTORS_H
#define STK_EXPRBASEFUNCTORS_H

#include "Arrays/include/STK_ExprBase.h"

namespace STK
{

// forward declaration
template<typename Derived, typename Functor, bool isVector> struct ApplyFunctorByCol;
template<typename Derived, typename Functor, bool isVector> struct ApplyFunctorByRow;
template<typename Derived, typename Functor> struct ApplyFunctor;

namespace hidden
{
/** @ingroup hidden
 *  helper class for choosing the ApplyFunctor.
 **/
template<class Derived, template<class> class Functor, bool isVector>
struct ApplyFunctorChoser;

/** @ingroup hidden
 *  specialization for general arrays: apply functor to the the row or the
 *  column.
 **/
template<class Derived, template<class> class Functor>
struct ApplyFunctorChoser<Derived, Functor, false>
{
  typedef typename Derived::Col VectorType;
  typedef Functor<VectorType> ByColFunctor;
  typedef ApplyFunctorByCol<Derived, ByColFunctor, false> ColOp;

  typedef typename Derived::Row PointType;
  typedef Functor<PointType> ByRowFunctor;
  typedef ApplyFunctorByRow<Derived, ByRowFunctor, false> RowOp;
};

/** @ingroup hidden
 *  specialization for vectors and points.
 **/
template<class Derived, template<class> class Functor>
struct ApplyFunctorChoser<Derived, Functor, true>
{
  typedef Functor<Derived> ByColFunctor;
  typedef ApplyFunctor<Derived, ByColFunctor> ColOp;

  typedef Functor<Derived> ByRowFunctor;
  typedef ApplyFunctor<Derived, ByRowFunctor> RowOp;
};

/** @ingroup hidden
 *  Utility class that will select the type of operator to apply.
 *  The result can be either a number if the data are in a vector or a point,
 *  or a vector if the data are in a matrix
 **/
template<typename Derived, template<class> class Functor>
struct ApplyFunctorSelector
{
  enum
  {
    isVector_   =  (  Derived::structure_ == int(Arrays::vector_)
                   || Derived::structure_ == int(Arrays::point_)
                   || Derived::structure_ == int(Arrays::number_)
                   )
  };

  typedef typename ApplyFunctorChoser<Derived, Functor, (bool)isVector_>::ColOp ColOp;
  typedef typename ApplyFunctorChoser<Derived, Functor, (bool)isVector_>::RowOp RowOp;

  typedef typename ColOp::resultByColType resultByColType;
  typedef typename RowOp::resultByRowType resultByRowType;
};

} // namespace hidden

/** @ingroup Arrays
 *  @brief compute the Functor Funct by column.
 **/
template<typename Derived, typename Funct>
struct ApplyFunctorByCol<Derived, Funct, false>
{
  typedef typename Derived::Type Type;
  typedef Array2DPoint<Type> resultByColType;

  /** constructor */
  inline ApplyFunctorByCol( ExprBase<Derived> const& lhs) : lhs_(lhs.asDerived()) {}
  /** @return the applied functor by column */
  resultByColType operator()()
  {
    resultByColType res(lhs_.cols());
    for (int j= lhs_.beginCols(); j <= lhs_.lastIdxCols(); ++j)
    { res[j] = Funct(lhs_.col(j))();}
    return res;
  }
  template<typename Weights>
  resultByColType operator()(ExprBase<Weights> const& w)
  {
    resultByColType res_(lhs_.cols());
    for (int j= lhs_.beginCols(); j <= lhs_.lastIdxCols(); ++j)
    { res_[j] = Funct(lhs_.col(j))(w);}
    return res_;
  }
  /** @return the applied functor by column */
  resultByColType operator()(bool option)
  {
    resultByColType res(lhs_.cols());
    for (int j= lhs_.beginCols(); j <= lhs_.lastIdxCols(); ++j)
    { res[j] = Funct(lhs_.col(j))( option);}
    return res;
  }
  template<typename Weights>
  resultByColType operator()(ExprBase<Weights> const& w, bool option)
  {
    resultByColType res_(lhs_.cols());
    for (int j= lhs_.beginCols(); j <= lhs_.lastIdxCols(); ++j)
    { res_[j] = Funct(lhs_.col(j))(w, option);}
    return res_;
  }
  template<typename Weights>
  resultByColType operator()(ExprBase<Weights> const& w, Type value)
  {
    resultByColType res_(lhs_.cols());
    for (int j= lhs_.beginCols(); j <= lhs_.lastIdxCols(); ++j)
    { res_[j] = Funct(lhs_.col(j))(w, value);}
    return res_;
  }
  /** @return the applied functor by column */
  resultByColType operator()(resultByColType const& value, bool option)
  {
#ifdef STK_BOUNDS_CHECK
  if (lhs_.cols() != value.cols()) STKRUNTIME_ERROR_NO_ARG(ApplyFunctorByCol::operator(value,option),lhs_.cols()!=value.cols());
#endif
    resultByColType res(lhs_.cols());
    for (int j= lhs_.beginCols(); j <= lhs_.lastIdxCols(); ++j)
    { res[j] = Funct(lhs_.col(j))(value[j], option);}
    return res;
  }
  template<typename Weights>
  resultByColType operator()(ExprBase<Weights> const& w, resultByColType const& value, bool option)
  {
#ifdef STK_BOUNDS_CHECK
  if (lhs_.cols() != value.cols()) STKRUNTIME_ERROR_NO_ARG(ApplyFunctorByCol::operator(w,value,option),lhs_.cols()!=value.cols());
#endif
    resultByColType res_(lhs_.cols());
    for (int j= lhs_.beginCols(); j <= lhs_.lastIdxCols(); ++j)
    { res_[j] = Funct(lhs_.col(j))(w, value[j], option);}
    return res_;
  }
  protected:
    Derived const& lhs_;
};

/** @ingroup Arrays
 *  @brief compute the Functor Funct by column.
 **/
template<typename Derived, typename Funct>
struct ApplyFunctorByRow<Derived, Funct, false>
{
  typedef typename Derived::Type Type;
  typedef Array2DVector<Type> resultByRowType;

  /** constructor */
  inline ApplyFunctorByRow( ExprBase<Derived> const& lhs) : lhs_(lhs.asDerived()) {}
  resultByRowType operator()()
  {
    resultByRowType res(lhs_.rows());
    for (int i= lhs_.beginRows(); i <= lhs_.lastIdxRows(); ++i)
    { res[i] = Funct(lhs_.row(i))();}
    return res;
  }
  template<typename Weights>
  inline resultByRowType operator()(ExprBase<Weights> const& w)
  {
    resultByRowType res_(lhs_.rows());
    for (int i= lhs_.beginRows(); i <= lhs_.lastIdxRows(); ++i)
    { res_[i] = Funct(lhs_.row(i))(w);}
    return res_;
  }
  resultByRowType operator()(bool option)
  {
    resultByRowType res(lhs_.rows());
    for (int j= lhs_.beginRows(); j <= lhs_.lastIdxRows(); ++j)
    { res[j] = Funct(lhs_.row(j))( option);}
    return res;
  }
  template<typename Weights>
  resultByRowType operator()(ExprBase<Weights> const& w, bool option)
  {
    resultByRowType res_(lhs_.rows());
    for (int j= lhs_.beginRows(); j <= lhs_.lastIdxRows(); ++j)
    { res_[j] = Funct(lhs_.row(j))(w, option);}
    return res_;
  }
  resultByRowType operator()(resultByRowType const& value, bool option)
  {
#ifdef STK_BOUNDS_CHECK
  if (lhs_.rows() != value.rows()) STKRUNTIME_ERROR_NO_ARG(ApplyFunctorByRow::operator(value,option),lhs_.rows()!=value.rows());
#endif
    resultByRowType res(lhs_.rows());
    for (int j= lhs_.beginRows(); j <= lhs_.lastIdxRows(); ++j)
    { res[j] = Funct(lhs_.row(j))(value[j], option);}
    return res;
  }
  template<typename Weights>
  resultByRowType operator()(ExprBase<Weights> const& w, resultByRowType const& value, bool option)
  {
#ifdef STK_BOUNDS_CHECK
  if (lhs_.rows() != value.rows()) STKRUNTIME_ERROR_NO_ARG(ApplyFunctorByRow::operator(w,value,option),lhs_.rows()!=value.rows());
#endif
    resultByRowType res_(lhs_.rows());
    for (int j= lhs_.beginRows(); j <= lhs_.lastIdxRows(); ++j)
    { res_[j] = Funct(lhs_.row(j))(w, value[j], option);}
    return res_;
  }
  protected:
    Derived const& lhs_;
};

/** @ingroup Arrays
 *  @brief compute the minimal value by column.
 **/
template<typename Derived, typename Funct>
struct ApplyFunctor
{
    typedef typename Derived::Type Type;
    typedef Type resultByColType;
    typedef Type resultByRowType;
    /// constructor
    inline ApplyFunctor( ExprBase<Derived> const& lhs) : lhs_(lhs.asDerived())
    { STK_STATICASSERT_VECTOR_ONLY(Derived);}
    /** apply without argument*/
    inline resultByColType operator()() { return Funct(lhs_)();}
    /** apply with weights*/
    template<typename Weights>
    inline resultByColType operator()(ExprBase<Weights> const& w) { return Funct(lhs_)(w);}
    /** apply with an option argument*/
    inline resultByColType operator()(bool option) { return Funct(lhs_)(option);}
    /** apply with weight and an option argument*/
    template<typename Weights>
    inline resultByColType operator()(ExprBase<Weights> const& w, bool option)
    { return Funct(lhs_)(w, option);}
    /** apply with a value and an option argument*/
    inline resultByColType operator()(Type value, bool option)
    { return Funct(lhs_)(value, option);}
    /** apply with weight, a value and an option argument*/
    template<typename Weights>
    inline resultByColType operator()(ExprBase<Weights> const& w, Type value, bool option)
    { return Funct(lhs_)(w, value, option);}

  protected:
    Derived const& lhs_;
};


} // namespace STK

#endif /* STK_EXPRBASEVISITOR_H */
