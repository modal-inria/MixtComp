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

/** @file STK_ExprBaseVisitor.h
 *  @brief In this file we define the Visitors for ExprBase.
 **/

#ifndef STK_EXPRBASEVISITOR_H
#define STK_EXPRBASEVISITOR_H

#include "visitors/STK_Visitors.h"

namespace STK
{

/** Run the visitor @a visitor to the whole coefficients of the array.
  * The template parameter @a Visitor is the type of the visitor and provides
  * the following interface:
  * @code
  * struct MyVisitor {
  *   // called for all  coefficients
  *   inline void operator() (Type const& value, Index i, Index j);
  * };
  * @endcode
  *
  * @note visitors offer automatic unrolling for small fixed size matrix.
  *
  * @sa minElt, maxElt
  */
template<typename Derived>
template<typename Visitor>
void ExprBase<Derived>::visit(Visitor& visitor) const
{
  typedef typename hidden::VisitorSelector<Visitor, Derived>::Impl Impl;
  Impl::run(this->asDerived(), visitor);
}

/* count the number of not-zero values in the expression */
template<typename Derived>
int const ExprBase<Derived>::count() const
{
  hidden::CountVisitor<Type> visitor;
  visit(visitor);
  return visitor.res_;
}

/* count the number values in the expression */
template<typename Derived>
int const ExprBase<Derived>::nbAvailableValues() const
{ return isFinite().template cast<int>().sum();}


template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::minElt( int& row, int& col) const
{
  hidden::MinEltVisitor<Type> visitor;
  visit(visitor);
  row = visitor.row_;
  col = visitor.col_;
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::maxElt( int& row, int& col) const
{
  typedef hidden::MaxEltVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  row = visitor.row_;
  col = visitor.col_;
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::minElt( int& idx) const
{
  typedef hidden::MinEltVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  idx = hidden::GetIdx<Visitor, hidden::Traits<Derived>::structure_ >::idx(visitor);
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::maxElt( int& idx) const
{
  typedef hidden::MaxEltVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  idx = hidden::GetIdx<Visitor, hidden::Traits<Derived>::structure_ >::idx(visitor);
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::minElt() const
{
  typedef hidden::MinEltVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::maxElt() const
{
  typedef hidden::MaxEltVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  return visitor.res_;
}


template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::minEltSafe( int& row, int& col) const
{
  typedef hidden::MinEltSafeVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  row = visitor.row_;
  col = visitor.col_;
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::maxEltSafe( int& row, int& col) const
{
  typedef hidden::MaxEltSafeVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  row = visitor.row_;
  col = visitor.col_;
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::minEltSafe( int& idx) const
{
  typedef hidden::MinEltSafeVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  idx = hidden::GetIdx<Visitor, hidden::Traits<Derived>::structure_ >::idx(visitor);
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::maxEltSafe( int& idx) const
{
  typedef hidden::MaxEltSafeVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  idx = hidden::GetIdx<Visitor, hidden::Traits<Derived>::structure_ >::idx(visitor);
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::minEltSafe() const
{
  typedef hidden::MinEltSafeVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::maxEltSafe() const
{
  typedef hidden::MaxEltSafeVisitor<Type> Visitor;
  Visitor visitor;
  visit(visitor);
  return visitor.res_;
}


/* sum the values of all the array */
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::sum() const
{
  hidden::SumVisitor<Type> visitor;
  visit(visitor);
  return visitor.res_;
}
/* @return the norm of this*/
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::norm() const
{ return Type(std::sqrt(norm2()));}
/* @return the square norm of this*/
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::norm2() const
{ return square().sum();}
/* @return the norm of this*/
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::normInf() const
{ return abs().maxElt();}

/* sum the values of all the array */
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::mean() const
{ return (this->sizeArray() >0) ? sum()/Type(this->sizeArray()) : Arithmetic<Type>::NA();}
/* compute the variance  of all the array */
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::variance() const
{ if (this->sizeArray()<=0) return Arithmetic<Type>::NA();
  Type mu = mean();
  return (*this-mu).square().sum()/Type(this->sizeArray());
}
/* compute the variance with given mean of all the elements of this*/
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::variance(Type const mean) const
{ if (this->sizeArray()<=0) return Arithmetic<Type>::NA();
  return (*this-mean).square().sum()/Type(this->sizeArray());
}

/* @return the weighted sum of all the elements of this using a Visitor*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wsum(ExprBase<Rhs> const& weights) const
{
  STK_STATICASSERT_VECTOR_ONLY(Derived);
  STK_STATICASSERT_ONE_DIMENSION_ONLY(Rhs);
  return dot(weights);
}
/* @return the norm of this*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wnorm(ExprBase<Rhs> const& weights) const
{
  STK_STATICASSERT_VECTOR_ONLY(Derived);
  STK_STATICASSERT_ONE_DIMENSION_ONLY(Rhs);
  return Type(std::sqrt(wnorm2(weights)));
}
/* @return the square norm of this*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wnorm2(ExprBase<Rhs> const& weights) const
{
  STK_STATICASSERT_VECTOR_ONLY(Derived);
  STK_STATICASSERT_ONE_DIMENSION_ONLY(Rhs);
 return square().dot(weights);
}
/* @return the weighted mean */
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wmean(ExprBase<Rhs> const& weights) const
{
  STK_STATICASSERT_VECTOR_ONLY(Derived);
  STK_STATICASSERT_ONE_DIMENSION_ONLY(Rhs);
  Type size = weights.sum();
  if (size <= 0) return Arithmetic<Type>::NA();
  return wsum(weights)/size;
}
/* @return the variance of all the elements of this using a Visitor*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wvariance(ExprBase<Rhs> const& weights) const
{
  STK_STATICASSERT_VECTOR_ONLY(Derived);
  STK_STATICASSERT_ONE_DIMENSION_ONLY(Rhs);
  Type size = weights.sum();
  if (size <= 0) return Arithmetic<Type>::NA();
  Type mean = wsum(weights)/size;
  return (*this-mean).square().wsum(weights)/size;
}

/** @return the variance with given mean of all the elements of this*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wvariance(Type const wmean, ExprBase<Rhs> const& weights) const
{
  STK_STATICASSERT_VECTOR_ONLY(Derived);
  STK_STATICASSERT_ONE_DIMENSION_ONLY(Rhs);
  Type size = weights.sum();
  if (size <= 0) return Arithmetic<Type>::NA();
  return (*this-wmean).square().wsum(weights)/size;
}

} // namespace STK

#endif /* STK_EXPRBASEVISITOR_H */
