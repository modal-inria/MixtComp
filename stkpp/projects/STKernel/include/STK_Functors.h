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
 * Project:  stkpp::Functors
 * created on: 1 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Functors.h
 *  @brief In this file we implement the functors.
 **/


#ifndef STK_FUNCTORS_H
#define STK_FUNCTORS_H

#include <cmath>
#include "STK_Integer.h"

namespace STK
{

namespace hidden
{

/** @ingroup hidden
 * The functor to use.  */
template< typename Functor, int NbParam_>
struct UsedFunctor;

/** @ingroup hidden
 * produce the Nth parameter type of Functor.  */
template< typename Functor, int NbParam_>
class FunctorParamTraits
{
  private:
    /** This class is used as a dummy parameter */
    class Void
    {   class Private {};
      public:
        typedef Private Type;
    };
  public:
    /* If the number of parameters of the Functor is lesser than NbParam_
     *  return a private Void class.
     */
    typedef typename If< Functor::NbParam_ >= NbParam_
                       , UsedFunctor<Functor, NbParam_>
                       , Void>::Result::Type Type;
};

// specialization, until 2 parameters
template< typename Functor> struct UsedFunctor<Functor, 1>
{ typedef typename Functor::param1_type Type;};
template< typename Functor> struct UsedFunctor<Functor, 2>
{ typedef typename Functor::param2_type Type;};

/** @ingroup hidden
 * produce the return type of the Functor.  */
template< typename Functor>
struct FunctorReturnTypeTraits
{ typedef typename Functor::result_type result_type;};


} // namespace hidden

/** @ingroup Functors
 *  @brief Template functor testing if a Char is an end of line.
 **/
struct TestEndOfLineOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Char param1_type ;
  inline TestEndOfLineOp(char* c) : last_(c) {}
  Char* last_;
  inline result_type const operator()(Char c)
  {
    *last_ = c;
    return (c == _T('\n'));
  }
};

/** @ingroup Functors
  * @brief Template functor testing if a number is equal to an other
  */
template<class Type1, class Type2 = Type1>
struct EqualOp
{
  enum { NbParam_ = 2 };
  typedef bool result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type value1, param2_type value2) const
  { return value1 == value2;}
};
/** @ingroup Functors
  * @brief Template functor testing if a number is not equal to an other
  */
template<class Type1, class Type2 = Type1>
struct NotEqualOp
{
  enum { NbParam_ = 2 };
  typedef bool result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type value1, param2_type value2) const
  { return value1 != value2;}
};
/** @ingroup Functors
  * @brief Template functor testing if a number is less than an other
  */
template<class Type1, class Type2 = Type1>
struct LessOp
{
  enum { NbParam_ = 2 };
  typedef bool result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type value1, param2_type value2) const
  { return value1 < value2;}
};
/** @ingroup Functors
  * @brief Template functor testing if a number is less or equal than an other
  */
template<class Type1, class Type2 = Type1>
struct LeqOp
{
  enum { NbParam_ = 2 };
  typedef bool result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type value1, param2_type value2) const
  { return value1 <= value2;}
};
/** @ingroup Functors
  * @brief Template functor testing if a number is greater than an other
  */
template<class Type1, class Type2 = Type1>
struct GreaterOp
{
  enum { NbParam_ = 2 };
  typedef bool result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type value1, param2_type value2) const
  { return value1 > value2;}
};
/** @ingroup Functors
  * @brief Template functor testing if a number is greater or equal than an other
  */
template<class Type1, class Type2 = Type1>
struct GeqOp
{
  enum { NbParam_ = 2 };
  typedef bool result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type value1, param2_type value2) const
  { return value1 >= value2;}
};
/** @ingroup Functors
  * @brief Template functor which compute the maximum of two numbers
  */
template<class Type1, class Type2 = Type1>
struct MaxOp
{
  enum { NbParam_ = 2 };
  typedef typename hidden::Promote<Type1, Type2>::result_type result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type  value1, param2_type value2) const
  { return  (value1 < value2) ? value2 : value1 ;}
};
/** @ingroup Functors
  * @brief Template functor which compute the minimum of two numbers
  */
template<class Type1, class Type2 = Type1>
struct MinOp
{
  enum { NbParam_ = 2 };
  typedef typename hidden::Promote<Type1, Type2>::result_type result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type  value1, param2_type value2) const
  { return  (value1 < value2) ? value1 : value2 ;}
};
/** @ingroup Functors
  * @brief Template functor which compute the sum of two numbers
  */
template<class Type1, class Type2 = Type1>
struct SumOp
{
  enum { NbParam_ = 2 };
  typedef typename hidden::Promote<Type1, Type2>::result_type result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type a, param2_type b) const
  { return a + b; }
};
/** @ingroup Functors
  * @brief Template functor which compute the product of two numbers
  */
template<class Type1, class Type2 = Type1>
struct ProductOp
{
  enum { NbParam_ = 2 };
  typedef typename hidden::Promote<Type1, Type2>::result_type result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type a, param2_type b) const
  { return a * b; }
};
/** @ingroup Functors
  * @brief Template functor which compute the difference of two numbers
  */
template<class Type1, class Type2 = Type1>
struct DifferenceOp
{
  enum { NbParam_ = 2 };
  typedef typename hidden::Promote<Type1, Type2>::result_type result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type a, param2_type b) const
  { return a - b; }
};
/** @ingroup Functors
  * @brief Template functor which compute the division of two numbers
  */
template<class Type1, class Type2 = Type1>
struct DivOp
{
  enum { NbParam_ = 2 };
  typedef typename hidden::Promote<Type1, Type2>::result_type result_type;
  typedef Type1 param1_type ;
  typedef Type2 param2_type ;

  inline result_type const operator()(param1_type a, param2_type b) const
  { return a / b; }
};

// unary functors:
/** @ingroup Functors
  * @brief Template functor to test if a number is less than a fixed number
  */
template<typename Type>
struct LessThanOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline LessThanOp(const LessThanOp& other) : other_(other.other_) { }
  inline LessThanOp(Type other) : other_(other) { }
  inline result_type const operator() (param1_type a) const { return a < other_; }
  const Type other_;
};
/** @ingroup Functors
  * @brief Template functor to test if a number is less or equal than a fixed number
  */
template<typename Type>
struct LeqThanOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline LeqThanOp(LeqThanOp const& other) : other_(other.other_)
  {}
  inline LeqThanOp(Type other) : other_(other)
  {}
  inline result_type const operator() (param1_type a) const
  { return (a <= other_); }
  const Type other_;
};
/** @ingroup Functors
  * @brief Template functor to test if a number is greater than a fixed number
  */
template<typename Type>
struct GreaterThanOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline GreaterThanOp(const GreaterThanOp& other) : other_(other.other_) { }
  inline GreaterThanOp(Type other) : other_(other) { }
  inline result_type const operator() (param1_type a) const { return a > other_; }
  const Type other_;
};
/** @ingroup Functors
  * @brief Template functor to test if a number is greater or equal than a fixed number
  */
template<typename Type>
struct GeqThanOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline GeqThanOp(const GeqThanOp& other) : other_(other.other_) { }
  inline GeqThanOp(Type other) : other_(other) { }
  inline result_type const operator() (param1_type a) const { return a >= other_; }
  const Type other_;
};
/** @ingroup Functors
  * @brief Template functor to test if a number is equal to a fixed number
  */
template<typename Type>
struct EqualThanOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline EqualThanOp(const EqualThanOp& other) : other_(other.other_) { }
  inline EqualThanOp(Type other) : other_(other) { }
  inline result_type const operator() (param1_type a) const { return a == other_; }
  const Type other_;
};
/** @ingroup Functors
  * @brief Template functor to test if a number is different than a fixed number
  */
template<typename Type>
struct NotEqualThanOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline NotEqualThanOp(const NotEqualThanOp& other) : other_(other.other_) { }
  inline NotEqualThanOp(Type other) : other_(other) { }
  inline result_type const operator() (param1_type a) const { return a != other_; }
  const Type other_;
};
/** @ingroup Functors
  * @brief Template functor testing if a number is a NA value
  */
template<class Type>
struct IsNaOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type value1) const
  { return Arithmetic<param1_type>::isNA(value1);}
};
/** @ingroup Functors
  * @brief Template functor testing if a number is a NA value
  */
template<class Type>
struct NotOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type value1) const
  { return !value1;}
};
/** @ingroup Functors
  * @brief Template functor testing if a number is a finite value
  */
template<class Type>
struct IsFiniteOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type value1) const
  { return Arithmetic<param1_type>::isFinite(value1);}
};
/** @ingroup Functors
  * @brief Template functor testing if a number is an infinite value
  */
template<class Type>
struct IsInfiniteOp
{
  enum { NbParam_ = 1 };
  typedef bool result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type value1) const
  { return Arithmetic<param1_type>::isInfinite(value1);}
};
/** @ingroup Functors
  * @brief Template functor which compute the opposite of a number
  */
template<class Type>
struct OppositeOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type a) const
  { return -a; }
};

/** @ingroup Functors
  * @brief Template functor which compute the absolute value of a number
  */
template<class Type>
struct AbsOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type a) const
  { return std::abs(a); }
};
/** @ingroup Functors
  * @brief Template functor which compute the exponential of a number
  */
template<class Type>
struct ExpOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type a) const {return std::exp(a);}
};
/** @ingroup Functors
  * @brief Template functor which compute the logarithm of a number
  */
template<class Type>
struct LogOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type a) const {return std::log(a);}
};
/** @ingroup Functors
  * @brief Template functor to multiply a number by a fixed other one
  */
template<typename Type>
struct MultipleOp
{
  enum { NbParam_ = 1 };
  typedef Type const result_type;
  typedef Type const param1_type ;

  inline MultipleOp(Type const other) : other_(other) {}
  inline MultipleOp( MultipleOp const& other) : other_(other.other_) {}
  inline result_type const operator() (param1_type a) const
  {return a * other_; }
  Type const other_;
};

/** @ingroup Functors
  * @brief Template functor which return a default value if the value is NA
  */
template<class Type>
struct SafeOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline SafeOp(Type const other = Type()) : other_(other) {}
  inline SafeOp( SafeOp const& other) : other_(other.other_) {}
  inline result_type const operator()(param1_type a) const
  { return Arithmetic<Type>::isFinite(a) ? a : other_; }
  Type const other_;
};


template<typename Type, bool isInt>
struct QuotientBaseOp;

template<typename Type>
struct QuotientBaseOp<Type, false>
{
  inline QuotientBaseOp(const QuotientBaseOp& other) : other_(other.other_) { }
  inline QuotientBaseOp(Type other) : other_( static_cast<Type>(1) / other) {}
  inline Type const operator() (Type a) const { return a * other_; }
  Type const other_;
};

template<typename Type>
struct QuotientBaseOp<Type, true>
{
  inline QuotientBaseOp( QuotientBaseOp const& other) : other_(other.other_) {}
  inline QuotientBaseOp( Type other) : other_(other) {}
  inline Type const operator()( Type a) const { return a / other_; }
  Type const other_;
};

/** @ingroup Functors
  * @brief Template functor to divide a number by a fixed other one
  *
  * This functor is used to implement the quotient of a matrix by
  * a number where the number type is not necessarily a floating point type.
  */
template<typename Type>
struct QuotientOp : public QuotientBaseOp<Type, hidden::isInt<Type>::yes >
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline QuotientOp(param1_type other) : QuotientBaseOp<Type, hidden::isInt<Type>::yes >(other) {}
};
/** @ingroup Functors
  * @brief Template functor to add a number to a fixed other one
  */
template<typename Type>
struct AddOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline AddOp(const AddOp& other) : other_(other.other_) { }
  inline AddOp(Type other) : other_(other) { }
  inline result_type const operator() (param1_type a) const { return a + other_; }
  const result_type other_;
};
/** @ingroup Functors
  * @brief Template functor which compute the square root of a number
  */
template<typename Type> struct SqrtOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator()(param1_type a) const { return std::sqrt(a); }
};
/** @ingroup Functors
  * @brief Template functor which compute the cosine of a number
  */
template<typename Type> struct CosOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const { return std::cos(a); }
};
/** @ingroup Functors
  * @brief Template functor which compute the sine of a number
  */
template<typename Type> struct SinOp
{
  enum { NbParam_ = 1 };
  typedef Type const result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const { return std::sin(a); }
};
/** @ingroup Functors
  * @brief Template functor which compute the tan of a number
  */
template<typename Type> struct TanOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const { return std::tan(a); }
};
/** @ingroup Functors
  * @brief Template functor which compute the arc cosine of a number
  */
template<typename Type> struct AcosOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const { return std::acos(a); }
};
/** @ingroup Functors
  * @brief Template functor which compute the arc sine of a number
  */
template<typename Type> struct AsinOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const { return std::asin(a); }
};
/** @ingroup Functors
  * @brief Template functor to raise a number to a power
  */
template<typename Type>
struct PowOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline PowOp(PowOp const& other) : exponent_(other.exponent_) {}
  inline PowOp(Type exponent) : exponent_(exponent) {}
  inline result_type const operator()(param1_type a) const  { return std::pow(a, exponent_); }
  const Type exponent_;
};
/** @ingroup Functors
  * @brief Template functor which compute the inverse of a number
  */
template<typename Type>
struct InverseOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const { return Type(1)/a; }
};
/** @ingroup Functors
  * @brief Template functor which compute the square of a number
  */
template<typename Type>
struct SquareOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const { return a*a; }
};

/** @ingroup Functors
  * @brief Template functor which compute the cube of a number
  */
template<typename Type>
struct CubeOp
{
  enum { NbParam_ = 1 };
  typedef Type result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const { return a*a*a; }
};
/** @ingroup Functors
  * @brief Template functor which cast a type to another type
  */
template<typename Type, typename OtherType>
struct CastOp
{
  enum { NbParam_ = 1 };
  typedef OtherType result_type;
  typedef Type param1_type ;

  inline result_type const operator() (param1_type a) const
  { return static_cast<OtherType>(a); }
};

} // namespace STK


#endif /* STK_FUNCTORS_H */
