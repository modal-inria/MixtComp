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
 * created on: 26 nov. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ExprBaseProduct.h
 *  @brief In this file we define the matrix-matrix product and its particular cases.
 **/


#ifndef STK_EXPRBASEPRODUCT_H
#define STK_EXPRBASEPRODUCT_H

#include "Sdk/include/STK_StaticAssert.h"

namespace STK
{

// this is the different class that will handle the product
// there will be specialized for the different product cases
// in the product folder
template<typename Lhs, typename Rhs> class DotProduct;

template<typename Lhs, typename Rhs> class ArrayByArrayProduct;
template<typename Lhs, typename Rhs> class ArrayByVectorProduct;
template<typename Lhs, typename Rhs> class ArrayByDiagonalProduct;

template<typename Lhs, typename Rhs> class PointByArrayProduct;
template<typename Lhs, typename Rhs> class PointByDiagonalProduct;

template<typename Lhs, typename Rhs> class VectorByPointProduct;

template<typename Lhs, typename Rhs> class DiagonalByArrayProduct;
template<typename Lhs, typename Rhs> class DiagonalByVectorProduct;


namespace hidden
{
/** @ingroup hidden
 *  @brief Helper class to select the correct ProductreturnType.
 *
 *  If there is n structure, there is potentially n(n-1)/2 kind
 *  of product. Default is ArrayByArrayProduct.
 **/
template<typename Lhs, typename Rhs, int LStructure_, int RStructure_>
struct ProductSelector
{ typedef ArrayByArrayProduct<Lhs, Rhs> ReturnType;};

template<typename Lhs, typename Rhs, int LStructure_>
struct ProductSelector<Lhs, Rhs, LStructure_, Arrays::vector_>
{ typedef ArrayByVectorProduct<Lhs, Rhs> ReturnType;};

template<typename Lhs, typename Rhs, int RStructure_>
struct ProductSelector<Lhs, Rhs, Arrays::point_, RStructure_>
{ typedef PointByArrayProduct<Lhs, Rhs> ReturnType;};

template<typename Lhs, typename Rhs>
struct ProductSelector<Lhs, Rhs, Arrays::point_, Arrays::diagonal_>
{  typedef PointByDiagonalProduct<Lhs, Rhs> ReturnType;};


template<typename Lhs, typename Rhs>
struct ProductSelector<Lhs, Rhs, Arrays::vector_, Arrays::point_>
{  typedef VectorByPointProduct<Lhs, Rhs> ReturnType;};


template<typename Lhs, typename Rhs, int LStructure_>
struct ProductSelector<Lhs, Rhs, LStructure_, Arrays::diagonal_>
{ typedef ArrayByDiagonalProduct<Lhs, Rhs> ReturnType;};

template<typename Lhs, typename Rhs, int RStructure_>
struct ProductSelector<Lhs, Rhs, Arrays::diagonal_, RStructure_>
{ typedef DiagonalByArrayProduct<Lhs, Rhs> ReturnType;};



template<typename Lhs, typename Rhs>
struct ProductSelector<Lhs, Rhs, Arrays::diagonal_, Arrays::vector_>
{ typedef typename Lhs::Type LType;
  typedef typename Rhs::Type RType;
  typedef DiagonalByVectorProduct< Lhs, Rhs> ReturnType;
};

template<typename Lhs, typename Rhs>
struct ProductSelector<Lhs, Rhs, Arrays::vector_, Arrays::vector_>
{ typedef typename Lhs::Type LType;
  typedef typename Rhs::Type RType;
  typedef BinaryOperator<ProductOp<LType, RType>, Lhs, Rhs> ReturnType;
};

template<typename Lhs, typename Rhs>
struct ProductSelector<Lhs, Rhs, Arrays::diagonal_, Arrays::diagonal_>
{ typedef typename Lhs::Type LType;
  typedef typename Rhs::Type RType;
  typedef BinaryOperator<ProductOp<LType, RType>, Lhs, Rhs> ReturnType;
};

template<typename Lhs, typename Rhs>
struct ProductSelector<Lhs, Rhs, Arrays::point_, Arrays::point_>
{
  typedef typename Lhs::Type LType;
  typedef typename Rhs::Type RType;
  typedef BinaryOperator<ProductOp<LType, RType>, Lhs, Rhs> ReturnType;
};

template<typename Lhs, typename Rhs>
struct ProductSelector<Lhs, Rhs, Arrays::point_, Arrays::vector_>
{ typedef typename Traits<Lhs>::Type LType;
  typedef typename Traits<Rhs>::Type RType;
  typedef DotProduct<Lhs, Rhs> ReturnType;
};

// FIXME: will not work as UnaryOperator constructor take only Lhs
template<typename Lhs, typename Rhs, int RStructure_>
struct ProductSelector<Lhs, Rhs, Arrays::number_, RStructure_>
{ typedef typename Rhs::Type Type;
  typedef UnaryOperator<MultipleOp<Type>, Rhs> ReturnType;
};

// FIXME: will not work as UnaryOperator constructor take only Lhs
template<typename Lhs, typename Rhs, int LStructure_>
struct ProductSelector<Lhs, Rhs, LStructure_, Arrays::number_>
{ typedef typename Lhs::Type Type;
  typedef UnaryOperator<MultipleOp<Type>, Lhs> ReturnType;
};

} // namespace hidden

/**@ingroup Arrays
  *
  * @brief Helper class to get the correct returned type of operator*
  *
  * @tparam Lhs the type of the left-hand side
  * @tparam Rhs the type of the right-hand side
  *
  * This class defines the typename ReturnType representing the product
  * expression between two matrix expressions. In practice, using
  * ProductReturnType<Lhs,Rhs>::ReturnType is the recommended way to define the
  * result type of a function returning an expression
  * which involve a matrix product.
  **/
template<typename Lhs, typename Rhs>
struct ProductReturnType
{
  enum
  {
    lhs_structure_ = Lhs::structure_,
    rhs_structure_ = Rhs::structure_
  };
  typedef typename hidden::ProductSelector<Lhs, Rhs, lhs_structure_, rhs_structure_>::ReturnType ReturnType;
};


template<typename Derived>
template<typename Rhs>
inline typename ProductReturnType<Derived, Rhs>::ReturnType const
ExprBase<Derived>::operator*( ExprBase<Rhs> const& other) const
{
  enum
  {
    productSizeIsValid_ =(  Derived::sizeCols_ == UnknownSize
                         || Rhs::sizeRows_     == UnknownSize
                         || int(Derived::sizeCols_) == int(Rhs::sizeRows_)
                         ),
    areVectors_   = (  Derived::structure_ == int(Arrays::vector_)
                    && Rhs::structure_     == int(Arrays::vector_)),
    arePoints_    = (  Derived::structure_ == int(Arrays::point_)
                    && Rhs::structure_     == int(Arrays::point_)),
    haveSameSizeRows_ = (  Derived::sizeRows_ == UnknownSize
                        || Rhs::sizeRows_     == UnknownSize
                        || int(Derived::sizeRows_) == int(Rhs::sizeRows_)
                        ),
    haveSameSizeCols_ = (  Derived::sizeCols_ == UnknownSize
                        || Rhs::sizeCols_     == UnknownSize
                        || int(Derived::sizeCols_) == int(Rhs::sizeCols_)
                        )
  };
  STK_STATICASSERT(productSizeIsValid_ || !(areVectors_ && haveSameSizeRows_),INVALID_VECTOR_VECTOR_PRODUCT);
  STK_STATICASSERT(productSizeIsValid_ || !(arePoints_  && haveSameSizeCols_),INVALID_POINT_POINT_PRODUCT);

  return typename ProductReturnType<Derived, Rhs>::ReturnType(this->asDerived(), other.asDerived());
}

} // namespace STK

#endif /* STK_EXPRBASEPRODUCT_H */
