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
 * Project:  stkpp::hidden::TContainer
 * Purpose:  Define the Interface 1D Container class.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IContainer1D.h
 *  @brief Interface Base class for 1D Containers.
 **/

#ifndef STK_ICONTAINER1D_H
#define STK_ICONTAINER1D_H

#include "../../STKernel/include/STK_Range.h"

namespace STK
{
/** @ingroup Arrays
 *  @brief Interface base class for homogeneous 1D containers.
 *
 * The IContainer1D class is an interface base class for all
 * one dimensional containers.
 **/
class IContainer1D
{
  private:
    /** Range of the indexes. */
    Range range_;

  protected:
    /** Default constructor */
    inline IContainer1D() : range_() {}
    /** constructor with specified range
     *  @param I the Range of the container
     **/
    inline IContainer1D(Range const& I) : range_(I) {}
    /** copy constructor
     *  @param V the container to copy
     **/
    inline IContainer1D(IContainer1D const& V) : range_(V.range_) {}
    /** destructor. */
    inline ~IContainer1D() {}
  public:
    /**  get the index of the first element
     *   @return the index of the first element
     */
    inline int const& firstIdx() const { return range_.firstIdx();}
    /**  get the index of the last element
     *   @return the index of the last element
     */
    inline int const& lastIdx() const  { return range_.lastIdx();}
    /**  get the number of elements
     *   @return the size of the container
     */
    inline int const& size() const  { return range_.size();};
    /**  get the range of the container
     *   @return the range of the container
     */
    inline Range const& range() const  { return range_;}
    /** Is there some data ?
     *  @return @c true if the container is empty, @c false otherwise
     **/
    inline bool empty() const { return range().empty();}
    /** exchange this container with T
     * @param T the container to exchange with T
     **/
     inline void exchange(IContainer1D& T)
     { std::swap(T.range_, range_ );}

  protected:
    /** Set the beginning of the range
     *  @param beg the first index of the range
     **/
    inline void shift( int const& beg)
    { range_.shift(beg);}
    /** Set range of the rows of the container.
     *  @param I the range to set (default empty)
     **/
    inline void setRange(Range const& I = Range()) { range_ = I;}
    /** increment the range of the container (can be negative).
     *  @param inc increment to apply to the range
     **/
    inline void incRange(int const& inc) { range_.inc(inc);}
    /** increment the beginning of the container (can be negative).
     *  @param inc the increment to apply to the beginning of the range
     **/
    inline void incFirst(int const& inc) { range_.incFirst(inc);}
    /** decrement the beginning of the container.
     *  @param inc the decrement to apply to the beginning of the range
     **/
    inline void decFirst(int const& inc) { range_.decFirst(inc);}
    /** increment the end of the container (can be negative).
     *  @param inc the increment to apply to the end of the range
     **/
    inline void incLast(int const& inc =1) { range_.incLast(inc);}
    /** decrement the end of the container.
     *  @param inc the decrement to apply to the end of the range
     **/
    inline void decLast(int const& inc =1) { range_.decLast(inc);}
};

} // namespace STK

#endif
// STK_ICONTAINER1D_H
