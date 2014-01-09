/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2009  Serge Iovleff

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
 * Project:  stkpp::STKernel::Base
 * Purpose:  Define the Range class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Range.h
 *  @brief In this file we define the class Range, a range of Index
 *  denoting a sub-vector region.
 **/

#ifndef STK_RANGE_H
#define STK_RANGE_H

#include "STK_Integer.h"
#include "STK_Constants.h"
#include "STK_Stream.h"

namespace STK
{

template<int Size_> class TRange;

typedef TRange<UnknownSize> Range;

/** @ingroup STKernel
 *  @brief Index sub-vector region: Specialization when the size is unknown.
 *
 *  A Range is an ordered pair [first,last] denoting a sub-vector
 *  region, similar to a Fortran 90 or Matlab colon notation.
 *  For example :
 *  @code
 *  Vector A(10), B(Range(0,20));
 *  Range I(2,4, false);
 *  A(I) = B(Range(0,2, false));
 *  @endcode
 *  overwrite the elements (2, 3, 4) of A by the elements (0, 1, 2) of B.
 *  There is no stride argument, only contiguous regions are allowed.
 **/
template<>
class TRange<UnknownSize>
{
  private:
    int first_;    ///< First index
    int last_;     ///< Last index
    int size_;     ///< Theoretic Dimension size_ = last_- first_ +1

  public:
    /** Default constructor. Null range.*/
    inline TRange() : first_(STKBASEARRAYS), last_(STKBASEARRAYS-1), size_(0) {}
    /** constructor. The first index is defined by the STKBASEARRAYS macro.
     *  @param size the size of the range
     **/
    inline TRange( int size)
                : first_(STKBASEARRAYS)
                , last_(size+STKBASEARRAYS-1)
                , size_(size) {}
    /** Complete constructor. Give the beginning and the size of the sub-region.
     *  @param first is the beginning of the sub-region
     *  @param size is the size of the sub-region.
     **/
    inline TRange( int first, int size)
                : first_(first)
                , last_(first+size-1)
                , size_(size)
    {}
   /** Complete constructor. Give the beginning and the end of the sub-region.
     *  @param first is the beginning of the sub-region
     *  @param last is the end of the sub-region.
     **/
    inline TRange( int first, int last, bool)
                : first_(first)
                , last_(last)
                , size_(last-first+1)
    {}
    /** @brief Copy constructor.
     *  Create a copy of an existing TRange.
     *  @param I The index to copy
     **/
    inline TRange(TRange const& I) : first_(I.first_), last_(I.last_), size_(I.size_)
    {}
    /** destructor. */
    inline ~TRange() {}
    /** get the first index of the TRange.
     *  @return the first index of the range
     **/
    inline int const& firstIdx()  const { return first_;};
    /** get the last index of the TRange.
     *  @return the last index of the range
     **/
    inline int const& lastIdx()    const { return last_;};
    /** get the size of the TRange (the number of elements).
     *  @return the size of the range
     **/
    inline int const& size()   const { return size_;};
    /** check if the range is empty or not.
     *  @return @c true if size <=0, @c false otherwise
     */
    inline bool empty() const { return size_<=0;};
    /** Shift the TRange giving the first element : the size is not modified.
     *  @param first new value of the first element. */
    inline TRange& shift(int first) { return inc(first - first_);}
    /** create the TRange [first_+inc, last_+inc_].
     *  @param inc the increment to apply
     **/
    inline TRange& inc(int inc){ first_ +=inc; last_  +=inc; return *this;}
    /** create the TRange [first_+inc, last_].
     *  @param dec the decrement to apply
     **/
    inline TRange& dec(int dec) { first_ -=dec; last_  -=dec; return *this;}
    /** create the TRange [first_-dec, last_-dec]
     *  @param inc the increment to apply to first_
     **/
    inline TRange& incFirst(int inc){ first_ +=inc; size_ -=inc; return *this;}
    /** create the TRange [first_, last_+inc]
     * @param inc the increment to apply
     **/
    inline TRange& incLast(int inc) { last_ +=inc; size_ +=inc; return *this;}
    /** @brief create the TRange [first_-dec, last_]
     * @param dec the decrement to apply
     **/
    inline TRange& decFirst(int dec) { first_ -=dec; size_  +=dec; return *this;}
    /** create the TRange [first_, last_-dec]
     * @param dec the decrement to apply
     **/
    inline TRange& decLast(int dec){ last_ -=dec; size_    -=dec; return *this;}
    /** @return this %TRange incremented **/
    inline TRange& operator+=(int inc) { first_ += inc; last_ += inc; return *this;}
    /** @return this %TRange decremented **/
    inline TRange& operator-=(int dec) { first_ -= dec; last_ -= dec; return *this;}
    /** @return this %TRange incremented **/
    inline TRange operator+(int inc) const { return(TRange(first_+inc, size_));}
    /** @return %TRange I incremented **/
    inline friend TRange operator+(int inc, TRange const &I) { return I+inc;}
    /** @return this %TRange decremented **/
    inline TRange operator-(int dec) const { return(TRange(first_-dec, size_));}

    /** Take the lowest value of first_ and I.first_ for first_
     *  and the largest value of last_ and I.last_ for last_.
     *  @param I the index to apply
     **/
    template<int OtherSize_>
    inline TRange& sup(TRange<OtherSize_> const& I)
    {
      first_ = std::min(first_, I.firstIdx()); last_  = std::max(last_, I.lastIdx());
      size_  = last_ - first_ +1;
      return *this;
    }
    /** Take the largest value of first_ and I.first_ for first_
     *  and the lowest value of last_ and I.last_ for last_.
     *  @param I the index to apply
     **/
    template<int OtherSize_>
    inline TRange& inf(TRange<OtherSize_> const& I)
    {
      first_ = std::max(first_, I.firstIdx()); last_  = std::min(last_, I.lastIdx());
      size_  = last_ - first_ +1;
      return *this;
    }
    /** check if this TRange in include in an other TRange
     *  @param I the index to compare
     *  @return @c true if this is include in @c I, @c false otherwise
     **/
    template<int OtherSize_>
    inline bool isIn(TRange<OtherSize_> const& I) const
    { return ((first_>= I.firstIdx())&&(lastIdx()<=I.lastIdx()));}
    /** check if the TRange I is include in the this TRange
     *  @param I the range to compare
     *  @return @c true if this contain I, @c false otherwise
     **/
    template<int OtherSize_>
    inline bool isContaining(TRange<OtherSize_> const& I) const
    { return ((first_<= I.firstIdx())&&(lastIdx()>=I.lastIdx()));}
    /** Return true if i is in this TRange
     *  @param i the integer to compare
     *  @return @c true if i is in this, @c false otherwise
     **/
    inline bool isContaining(int i) const
    { return ((first_<=i)&&(lastIdx()>=i));}
    /** compare this range with range @c I
     *  @param I the Index to compare
     *  @return @c true if the range are equals, @c false otherwise
     **/
    template<int OtherSize_>
    inline bool operator==(TRange<OtherSize_> const& I) const
    { return ((first_ == I.firstIdx()) && (lastIdx() == I.lastIdx()));}
    /** compare this range with range @c I
     *  @param I the Index to compare
     *  @return @c true if the range are different, @c false otherwise
     **/
    template<int OtherSize_>
    inline bool operator!=(TRange<OtherSize_> const& I) const
    { return ((first_ != I.firstIdx()) || (lastIdx() != I.lastIdx()));}

    /** @brief compute sup(I,J).
     *  Take the lowest value of I.first_ and J.first_ for first_
     *  and the largest value of I.last_ and J.last_ for last_.
     *  @param I the first Range
     *  @param J the second Range
    */
    static inline TRange sup(Range const& I, Range const& J)
    { return Range(std::min(I.first_, J.first_), std::max(I.last_, J.last_), 0);}

    /** @brief compute inf(I,J).
     *  Take the largest value of I.first_ and J.first_ for first_
     *  and the lowest value of I.last_ and J.last_ for last_.
     *  @param I the first Range
     *  @param J the second Range
     */
    static inline TRange inf(Range const& I, Range const& J)
    { return Range(std::max(I.first_, J.first_), std::min(I.last_, J.last_), 0);}
    /** @brief Read a TRange in the form first:last (MATLAB-like form) from
     * an input stream.
     **/
    friend istream& operator>> (istream& is, Range& I);

};

/** @ingroup STKernel
 *  @brief Index sub-vector region.
 *
 *  An Range is an ordered pair [first,last] denoting a sub-vector
 *  region, similar to a Fortran 90 or Matlab colon notation.
 *  For example :
 *  @code
 *  Vector A(10), B(Range(0,20));
 *  Range I(2,4, false);
 *  A(I) = B(Range(0,2, false));
 *  @endcode
 *  overwrite the elements (2, 3, 4) of A by the elements (0, 1, 2) of B.
 *   There is no stride argument, only contiguous regions are allowed.
 **/
template<int Size_>
class TRange
{
  private:
    int first_;    ///< First index

  public:
    /** Default constructor. Null range.*/
    inline TRange() : first_(STKBASEARRAYS) {}
    /** constructor. The first index is defined by the STKBASEARRAYS macro.
     **/
    inline TRange( int) : first_(STKBASEARRAYS) {}
    /** Complete constructor. Give the beginning and the size of the sub-region.
     *  @param first is the beginning of the sub-region
     **/
    inline TRange( int first, int) : first_(first) {}
    /** Complete constructor. Give the beginning and the end of the sub-region.
     * @param first is the beginning of the sub-region
     **/
    inline TRange( int first, int, bool) : first_(first) {}
    /** Create a copy of an existing TRange.
     *  @param I The index to copy
     **/
    template<int OtherSize_>
    inline TRange(TRange<OtherSize_> const& I) : first_(I.firstIdx()) {}
    /** destructor. */
    inline ~TRange() {}
    /** get the first index of the TRange.
     *  @return the first index of the range
     **/
    inline int const& firstIdx()  const { return first_;};
    /** get the last index of the TRange.
     *  @return the last index of the range
     **/
    inline int lastIdx() const { return first_ + Size_ -1;};
    /** get the size of the TRange (the number of elements).
     *  @return the size of the range
     **/
    inline int size()   const { return Size_;};
    /** check if the range is empty or not.
     *  @return @c true if size <=0, @c false otherwise
     */
    inline bool empty() const { return Size_<=0;};

    /** Shift the TRange giving the first element.
     *  @param first new value of the first element. */
    inline TRange& shift(int first) { return inc(first - first_);}
    /** create the TRange [first_+inc, last_+inc_].
     *  @param inc the increment to apply
     **/
    inline TRange& inc(int inc) { first_ +=inc; return *this;}
    /** create the TRange [first_+inc, last_].
     *  @param inc the increment to apply
     **/
    inline TRange& incFirst(int inc) { first_ +=inc; return *this;}
    /** create the TRange [first_, last_+inc]
     * @param inc the increment to apply
     **/
    inline TRange& incLast(int inc) { first_ +=inc; return *this;}
    /** create the TRange [first_-dec, last_-dec]
     *  @param dec the decrement to apply
     **/
    inline TRange& dec(int dec) { first_ -=dec; return *this;}
    /** @brief create the TRange [first_-dec, last_]
     * @param dec the decrement to apply
     **/
    inline TRange& decFirst(int dec) { first_ -=dec;  return *this;}
    /** create the TRange [first_, last_-dec]
     * @param dec the decrement to apply
     **/
    inline TRange& decLast(int dec){ first_ -=dec; return *this;}
    /** @return this %TRange incremented **/
    inline TRange& operator+=(int inc) { first_ += inc; return *this;}
    /** @return this %TRange decremented **/
    inline TRange& operator-=(int dec) { first_ -= dec; return *this;}
    /** @return this %TRange incremented **/
    inline TRange operator+(int inc) const { return(TRange(first_+inc, Size_));}
    /** @return %TRange I incremented **/
    inline friend TRange operator+(int inc, TRange const &I) { return I+inc;}
    /** @return this %TRange decremented **/
    inline TRange operator-(int dec) const { return(TRange(first_-dec, Size_));}

    /** check if this TRange in include in an other TRange
     *  @param I the index to compare
     *  @return @c true if this is include in @c I, @c false otherwise
     **/
    template<int OtherSize_>
    inline bool isIn(TRange<OtherSize_> const& I) const
    { return ((first_>= I.firstIdx())&&(lastIdx()<=I.lastIdx()));}
    /** check if the TRange I is include in the this TRange
     *  @param I the range to compare
     *  @return @c true if this contain I, @c false otherwise
     **/
    template<int OtherSize_>
    inline bool isContaining(TRange<OtherSize_> const& I) const
    { return ((first_<= I.firstIdx())&&(lastIdx()>=I.lastIdx()));}
    /** Return true if i is in this TRange
     *  @param i the integer to compare
     *  @return @c true if i is in this, @c false otherwise
     **/
    inline bool isContaining(int i) const
    { return ((first_<=i)&&(lastIdx()>=i));}
    /** compare this range with range @c I
     *  @param I the Index to compare
     *  @return @c true if the range are equals, @c false otherwise
     **/
    template<int OtherSize_>
    inline bool operator==(TRange<OtherSize_> const& I) const
    { return ((first_ == I.firstIdx()) && (lastIdx() == I.lastIdx()));}
    /** compare this range with range @c I
     *  @param I the Index to compare
     *  @return @c true if the range are different, @c false otherwise
     **/
    template<int OtherSize_>
    inline bool operator!=(TRange<OtherSize_> const& I) const
    { return ((first_ != I.firstIdx()) || (lastIdx() != I.lastIdx()));}
};


/** @brief Write a TRange in the form first:last (MATLAB-like form) in an
 *  output stream.
 *  @param s output stream
 *  @param I the Range to write
 **/
template<int Size_>
ostream& operator<< (ostream& s, TRange<Size_> const& I)
{
  s << I.firstIdx() << _T(":") << I.lastIdx();
  return s;
}


} // namespace STK

#endif // STK_RANGE_H
