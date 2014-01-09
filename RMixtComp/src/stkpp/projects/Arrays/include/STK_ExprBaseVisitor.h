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

#include "../../Sdk/include/STK_MetaTemplate.h"
#include "../../STatistiK/include/STK_RandBase.h"
#include "STK_Arrays_Util.h"
#include "./visitors/STK_Visitors.h"

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
  typedef hidden::VisitorSelector<Visitor, Derived, structure_, sizeRows_, sizeCols_> Impl;
  Impl::run(this->asDerived(), visitor);
}


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
/* sum the values of all the array */
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::variance() const
{ return (this->sizeArray() >0) ?
   ((*this-mean()).square().sum()/Type(this->sizeArray())) : Arithmetic<Type>::NA();
}

/* sum the values of all the array */
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::sumSafe() const
{
  hidden::SumSafeVisitor<Type> visitor;
  visit(visitor);
  return visitor.res_;
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::normSafe() const
{ return Type(std::sqrt(norm2Safe()));}
/* @return the square norm of this*/
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::norm2Safe() const
{ return square().sumSafe();}
/* sum the values of all the array */
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::meanSafe() const
{
  int size = nbValues();
  return (size>0) ? sumSafe()/Type(size) : Arithmetic<Type>::NA();
}
template<typename Derived>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::varianceSafe() const
{
  int size = nbValues();
  return (size >0) ?
    ((*this - (sumSafe()/Type(size))).square().sumSafe()/Type(size)) : Arithmetic<Type>::NA();
}


/* @return the weighted sum of all the elements of this using a Visitor*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wsum(ExprBase<Rhs> const& weights) const
{ return dot(weights);}
/* @return the norm of this*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wnorm(ExprBase<Rhs> const& weights) const
{ return Type(std::sqrt(wnorm2(weights)));}
/* @return the square norm of this*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wnorm2(ExprBase<Rhs> const& weights) const
{ return (square().dot(weights));}
/* @return the mean of all the elements of this using a Visitor*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wmean(ExprBase<Rhs> const& weights) const
{ return (this->sizeArray() >0) ? wsum(weights)/weights.sum() : Arithmetic<Type>::NA();}
/* @return the variance of all the elements of this using a Visitor*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wvariance(ExprBase<Rhs> const& weights) const
{
  Type size = weights.sum();
  return (size >0) ?
    ((*this-(wsum(weights)/size)).square().wsum(weights)/size) : Arithmetic<Type>::NA();;
}

/* @return the weighted sum of all the elements of this using a Visitor*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wsumSafe(ExprBase<Rhs> const& weights) const
{ return dotSafe(weights);}
/* @return the norm of this*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wnormSafe(ExprBase<Rhs> const& weights) const
{ return Type(std::sqrt(wnorm2Safe(weights)));}
/* @return the square norm of this*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wnorm2Safe(ExprBase<Rhs> const& weights) const
{ return (square().dotSafe(weights));}
/* @return the mean of all the elements of this using a Visitor*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wmeanSafe(ExprBase<Rhs> const& weights) const
{
  Type size = weights.sum();
  return (size > 0) ? wsum(weights)/size : Arithmetic<Type>::NA();
}
/* @return the variance of all the elements of this using a Visitor*/
template<typename Derived>
template<typename Rhs>
typename hidden::Traits<Derived>::Type const ExprBase<Derived>::wvarianceSafe(ExprBase<Rhs> const& weights) const
{
  Type size = weights.sum();
  return (size > 0) ?
    ((*this-(wsumSafe(weights)/size)).square().wsumSafe(weights)/size) : Arithmetic<Type>::NA();
}


/* count the number values in the array */
template<typename Derived>
int const ExprBase<Derived>::nbValues() const
{ return isFinite().template cast<int>().sum();}



} // namespace STK

#endif /* STK_EXPRBASEVISITOR_H */
