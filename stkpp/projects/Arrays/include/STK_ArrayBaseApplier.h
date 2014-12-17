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

/** @file STK_ArrayBaseApplier.h
 *  @brief In this file we define the ArrayBaseApplier classes.
 **/

#ifndef STK_ARRAYBASEAPPLYER_H
#define STK_ARRAYBASEAPPLYER_H

namespace STK
{

/** Apply the Visitor @a visitor to the whole coefficients of the array.
  * The template parameter @a Visitor is the type of the visitor and provides
  * the following interface:
  * @code
  * struct MyVisitor {
  *   // called for all  coefficients
  *   void operator() (Type& value);
  * };
  * @endcode
  *
  * @note visitors offer automatic unrolling for small fixed size matrix.
  *
  * @sa setValue
  */
template<typename Derived>
template<typename Visitor>
void ArrayBase<Derived>::apply(Visitor& visitor)
{
  typedef typename hidden::VisitorSelector<Visitor, Derived>::Impl Impl;
  Impl::apply(this->asDerived(), visitor);
}

/*
 * start public function of the ArrayBase class using visitor
 */
/* @brief Set the value to all the Allocator */
template<typename Derived>
Derived& ArrayBase<Derived>::setValue(Type const& value)
{
  hidden::ValueVisitor<Type> visitor(value);
  apply(visitor);
  return this->asDerived();
}

/* @brief Set the value one to all the Array */
template<typename Derived>
void ArrayBase<Derived>::ones()
{
  hidden::ValueVisitor<Type> visitor(Type(1));
  apply(visitor);
}

/* @brief Set the value one to all the Array */
template<typename Derived>
void ArrayBase<Derived>::zeros()
{
  hidden::ValueVisitor<Type> visitor(Type(0));
  apply(visitor);
}

template<typename Derived>
void ArrayBase<Derived>::randUnif()
{
  hidden::RandUnifVisitor<Type> visitor;
  apply(visitor);
}

template<typename Derived>
void ArrayBase<Derived>::randGauss()
{
    hidden::RandGaussVisitor<Type> visitor;
    apply(visitor);
}




/** @ingroup Arrays
 *  @brief Applies the visitor @a visitor to the whole elements of the matrix or
 *  vector.
  *
  * The template parameter @a Visitor is the type of the visitor and provides
  * the following interface:
  * @code
  * struct MyVisitor {
  *   // called for all elements
  *   void operator() (Type& value, int i, int j);
  * };
  * };
  * @endcode
  * The value is modified by the Visitor.
  *
  * @note compared to one or two @c for loops, visitors offer automatic
  * unrolling for small fixed size matrix.
  */
template< typename Derived, typename Visitor>
class ArrayBaseApplier
{
  private:
    Derived& array_;
  public:
    typedef typename hidden::VisitorSelector<Visitor, Derived>::Impl Impl;

    ArrayBaseApplier( Derived& T) : array_(T) { }
    ~ArrayBaseApplier() { }
    inline void apply(Visitor& funct) { Impl::apply(array_, funct);}
};


} // namespace STK

#endif /* STK_ARRAYBASEAPPLYER_H */
