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

/** @file STK_VisitorSelector.h
 *  @brief In this file we implement the selector of the visitor and apply pattern.
 **/

#ifndef STK_VISITORSELECTOR_H
#define STK_VISITORSELECTOR_H

namespace STK
{

namespace hidden
{
// forward declaration
template<typename Visitor, typename Derived, int Structure_, int SizeRows_, int SizeCols_>
struct VisitorSelectorHelper;

/** @ingroup hidden
 *  @brief visitor selector. If @c Derived is a full two-dimensionnal array and
 *  the visitation can be unrolled,then we use directly the VisitorArrayImpl
 *  class in order to computate the result of the visitation.
 *
 *  Otherwise we delegate the search of the correct implementation to the
 *  VisitorSelectorHelper class.
 **/
template<typename Visitor, typename Derived>
struct VisitorSelector
{
  enum
  {
    structure_ = hidden::Traits<Derived>::structure_,
    orient_    = hidden::Traits<Derived>::orient_,
    sizeRows_  = hidden::Traits<Derived>::sizeRows_,
    sizeCols_  = hidden::Traits<Derived>::sizeCols_,
    storage_   = hidden::Traits<Derived>::storage_,
    unrollRows_ = (sizeRows_ < MaxUnrollSquareRoot),
    unrollCols_ = (sizeCols_ < MaxUnrollSquareRoot),
    is2D_       = (structure_ == (int)Arrays::array2D_ || structure_ == (int)Arrays::square_)
  };

  typedef typename VisitorSelectorHelper<Visitor, Derived, structure_, sizeRows_, sizeCols_>::Impl HelperImpl;
  typedef VisitorArrayImpl<Visitor, Derived, orient_, sizeRows_, sizeCols_> ArrayImpl;
  // the other cases will be take into account by the helper class
  typedef typename If<is2D_ && unrollRows_ && unrollCols_, ArrayImpl, HelperImpl >::Result Impl;
};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays */
template<typename Visitor, typename Derived, int SizeRows_, int sizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::diagonal_, SizeRows_, sizeCols_>
{ typedef VisitorDiagonalImpl<Visitor, Derived, sizeCols_> Impl;};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays. Desambiguate specialization */
template<typename Visitor, typename Derived>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::diagonal_, UnknownSize, UnknownSize>
{ typedef VisitorDiagonalImpl<Visitor, Derived, UnknownSize> Impl;};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays. Debugging specialization */
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::diagonal_, SizeRows_, UnknownSize>
{ /* get an error at compile time */};

/** @ingroup hidden
 *  @brief specialization for the diagonal arrays. Debugging specialization */
template<typename Visitor, typename Derived, int sizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::diagonal_, UnknownSize, sizeCols_>
{ /* get an error at compile time */};


/** @ingroup hidden
 *  @brief specialization for the vectors */
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::vector_, SizeRows_, 1>
{ typedef VisitorVectorImpl<Visitor, Derived, SizeRows_> Impl;};

/** @ingroup hidden
 *  @brief specialization for the vectors. Disambiguate */
template<typename Visitor, typename Derived>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::vector_, UnknownSize, 1>
{ typedef VisitorVectorImpl<Visitor, Derived, UnknownSize> Impl;};

/** @ingroup hidden
 *  @brief specialization for the vectors. Debugging specialization. */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::vector_, SizeRows_, SizeCols_>
{ /* get an error at compile time */};

/** @ingroup hidden
 *  @brief specialization for the row vectors */
template<typename Visitor, typename Derived, int SizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::point_, 1, SizeCols_>
{ typedef VisitorPointImpl<Visitor, Derived, SizeCols_> Impl;};

/** @ingroup hidden
 *  @brief specialization for the row vectors. Disambiguate */
template<typename Visitor, typename Derived>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::point_, 1, UnknownSize>
{ typedef VisitorPointImpl<Visitor, Derived, UnknownSize> Impl;};

/** @ingroup hidden
 *  @brief specialization for the row vectors. Debugging specialization. */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::point_, SizeRows_, SizeCols_>
{ /* get an error at compile time */};

/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::upper_triangular_, SizeRows_, SizeCols_>
{
  enum
  { orient_    = hidden::Traits<Derived>::orient_};
  typedef VisitorUpperImpl<Visitor, Derived, orient_> Impl;
};

/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays. Disambiguate */
template<typename Visitor, typename Derived, int SizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::upper_triangular_, UnknownSize, SizeCols_>
{
  enum
  { orient_    = hidden::Traits<Derived>::orient_};
  typedef VisitorUpperImpl<Visitor, Derived, orient_> Impl;
};
/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays. Disambiguate */
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::upper_triangular_, SizeRows_, UnknownSize>
{
  enum
  { orient_    = hidden::Traits<Derived>::orient_};
  typedef VisitorUpperImpl<Visitor, Derived, orient_> Impl;
};
/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays. Disambiguate */
template<typename Visitor, typename Derived>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::upper_triangular_, UnknownSize, UnknownSize>
{
  enum
  { orient_    = hidden::Traits<Derived>::orient_};
  typedef VisitorUpperImpl<Visitor, Derived, orient_> Impl;
};

/** @ingroup hidden
 *  @brief specialization for the lower triangular arrays */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::lower_triangular_, SizeRows_, SizeCols_>
{
  enum
  { orient_    = hidden::Traits<Derived>::orient_};
  typedef VisitorLowerImpl<Visitor, Derived, orient_> Impl;
};

/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays. Disambiguate */
template<typename Visitor, typename Derived, int SizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::lower_triangular_, UnknownSize, SizeCols_>
{
  enum
  { orient_    = hidden::Traits<Derived>::orient_};
  typedef VisitorLowerImpl<Visitor, Derived, orient_> Impl;
};
/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays. Disambiguate */
template<typename Visitor, typename Derived, int SizeRows_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::lower_triangular_, SizeRows_, UnknownSize>
{
  enum
  { orient_    = hidden::Traits<Derived>::orient_};
  typedef VisitorLowerImpl<Visitor, Derived, orient_> Impl;
};
/** @ingroup hidden
 *  @brief specialization for the upper triangular arrays. Disambiguate */
template<typename Visitor, typename Derived>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::lower_triangular_, UnknownSize, UnknownSize>
{
  enum
  { orient_    = hidden::Traits<Derived>::orient_};
  typedef VisitorLowerImpl<Visitor, Derived, orient_> Impl;
};

/** @ingroup hidden
 *  @brief specialization for the vectors */
template<typename Visitor, typename Derived>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::number_, 1, 1>
{ typedef VisitorNumberImpl<Visitor, Derived> Impl;};

/** @ingroup hidden
 *  @brief specialization for the vectors */
template<typename Visitor, typename Derived, int SizeRows_, int SizeCols_>
struct VisitorSelectorHelper<Visitor, Derived, Arrays::number_, SizeRows_, SizeCols_>
{ /* get an error at compile time */};


/** @ingroup hidden
 *  @brief Helper for the Visitor selector: allow to select the correct
 *  implementation to instantiate. General case for general 2D arrays with
 *  known dimensions of the rows and columns but at least one of the dimension
 *  is too great for beeing unrolled.
 **/
template<typename Visitor, typename Derived, int Structure_, int SizeRows_, int SizeCols_>
struct VisitorSelectorHelper
{
  enum
  {
    orient_    = hidden::Traits<Derived>::orient_,
    unrollRows_ = (SizeRows_ < MaxUnrollSquareRoot),
    unrollCols_ = (SizeCols_ < MaxUnrollSquareRoot)
  };
  typedef VisitorArrayImpl<Visitor, Derived, orient_, SizeRows_, UnknownSize> RowUnrollImpl;
  typedef VisitorArrayImpl<Visitor, Derived, orient_, UnknownSize, SizeCols_> ColUnrollImpl;
  typedef VisitorArrayImpl<Visitor, Derived, orient_, UnknownSize, UnknownSize> NoUnrollImpl;
  typedef typename If<unrollCols_, ColUnrollImpl
                                 , typename If<unrollRows_, RowUnrollImpl, NoUnrollImpl>::Result >::Result Impl;
};

/**  @ingroup hidden
 * @brief Specialization for general 2D arrays with unknown dimensions */
template<typename Visitor, typename Derived, int Structure_>
struct VisitorSelectorHelper< Visitor,  Derived,  Structure_,  UnknownSize,  UnknownSize>
{
  enum
  {
    orient_    = hidden::Traits<Derived>::orient_
  };
  typedef VisitorArrayImpl<Visitor, Derived, orient_,  UnknownSize,  UnknownSize> Impl;
};

/** @ingroup hidden
 *  @brief Specialization for general 2D arrays with unknown dimensions for the columns */
template<typename Visitor, typename Derived, int Structure_, int SizeRows_>
struct VisitorSelectorHelper< Visitor,  Derived,  Structure_,  SizeRows_,  UnknownSize>
{
    enum
    {
      orient_    = hidden::Traits<Derived>::orient_,
      unrollRows_ = (SizeRows_ < MaxUnrollSquareRoot)
    };
   typedef VisitorArrayImpl<Visitor, Derived, orient_, SizeRows_, UnknownSize> RowUnrollImpl;
   typedef VisitorArrayImpl<Visitor, Derived, orient_, UnknownSize, UnknownSize> NoUnrollImpl;
   typedef typename If<unrollRows_, RowUnrollImpl, NoUnrollImpl>::Result Impl;
};

/** @ingroup hidden
 *  @brief Specialization for general 2D arrays with unknown dimensions for the rows */
template<typename Visitor, typename Derived, int Structure_, int SizeCols_>
struct VisitorSelectorHelper< Visitor,  Derived,  Structure_, UnknownSize, SizeCols_>
{
    enum
    {
      orient_    = hidden::Traits<Derived>::orient_,
      unrollCols_ = (SizeCols_ < MaxUnrollSquareRoot)
    };
   typedef VisitorArrayImpl<Visitor, Derived, orient_, UnknownSize, SizeCols_> ColUnrollImpl;
   typedef VisitorArrayImpl<Visitor, Derived, orient_, UnknownSize, UnknownSize> NoUnrollImpl;
   typedef typename If<unrollCols_, ColUnrollImpl, NoUnrollImpl>::Result Impl;
};


} // namespace hidden

} // namespace STK

#endif /* STK_VISITORSIMPL_H */
