/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * Project:  stkpp::aam
 * created on: 27 sept. 2010
 * Purpose:  Define the STK_LocalVariancePage.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_LocalVariancePage.h
 *  @brief In this file we define the LocalVariancePage class.
 **/

#ifndef STK_LOCALVARIANCEPAGE_H
#define STK_LOCALVARIANCEPAGE_H

#include "DManager/include/STK_IPage.h"
#include "STK_LocalVariance.h"

namespace STK
{

/** @brief In this Page, we manage the options for the computation of the
 * LocalVariance.
 * The LocalVariance page have the form
 * @code
 *   # LocalVariancePage options
 *  [LocalVariance]
 *    # proximity graph type using prim or distance
 *    type graph = distance
 *    # number of neighbors to use
 *    neighborhood = 2
 * @endcode
 * */
class LocalVariancePage: public IPage
{
  public:
    /** default constructor */
    LocalVariancePage();
    /** constructor with a given level.
     * @param level the level of the page
     **/
    LocalVariancePage( int const& level);
    /** copy constructor.
     * @param page the page to copy
     **/
    LocalVariancePage( LocalVariancePage const& page);
    /** destructor. */
    virtual ~LocalVariancePage();
    /** clone pattern
     *  @return a pointer on the clone of this
     **/
    inline virtual LocalVariancePage* clone() const
    { return new LocalVariancePage(*this);}

    /** get the type of graph
     *  @return the TypeGraph used for computing the local variance
     **/
    inline LocalVariance::TypeGraph typeGraph() const { return type_;}
    /** @return the number of neighbors to used in order to construct the
     *  proximity graph
     **/
    inline int nbNeighbor() const { return nbNeighbor_;}
    /** validate the options. Check if the values are coherent.
     *  @return @c true if the options are correct, @c false otherwise.
     **/
    virtual bool validate();

  private:
    /** type of the graph to compute */
    LocalVariance::TypeGraph type_;
    /** number of neighbors of each individual in the graph. */
    int nbNeighbor_;
};

} // namespace STK


#endif /* STK_LOCALVARIANCEPAGE_H */
