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
 * Project:  stkpp::Reduct
 * created on: 17 avr. 2010
 * Purpose:  Abstract class for the computation of the Index in the SLAAM.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IReduct.h In this file we define the interface base
 *  class IReduct.
 **/

#ifndef STK_IREDUCT_H
#define STK_IREDUCT_H

#include "Sdk/include/STK_IRunner.h"
#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DVector.h"

namespace STK
{

/** @ingroup Reduct
 *  @brief A IReduct is an interface base class for dimension
 *  reduction techniques.
 *
 * The class receive a matrix in input of size (n,p).
 * - n is the number of samples,
 * - p is the number of variables.
 * The observations can be weighted.
 *
 * The derived class will compute a @em reduced data set of dimension (n,d).
 */
class IReduct : public IRunnerUnsupervised<ArrayXX, Vector>
{
  typedef IRunnerUnsupervised<ArrayXX, Vector> Runner;
  protected:
    /** Default constructor. */
    IReduct();
    /** Constructor with a pointer on the constant data set.
     *  @param p_data a pointer on the data set to reduce.
     * */
    IReduct( ArrayXX const* p_data);
    /** Constructor with a constant reference on the data set.
     *  @param data the data set to reduce.
     * */
    IReduct( ArrayXX const& data);
    /** Copy constructor. Use the operator = of the clas ArrayXX
     *  @param reductor The reductor to copy.
     * */
    IReduct( IReduct const& reductor);

  public:
    /** virtual destructor. Delete reduced data set if allocated. */
    virtual ~IReduct();
    /** get the number of dimension.
     *  @return The number of dimension computed
     **/
    inline int dim() const { return dim_;}
    /** get a pointer on the reduced data set
     * @return a constant pointer on the data set
     **/
    inline ArrayXX* p_reduced() const { return p_reduced_; }
    /** set the number of dimension.
     *  @param dim the number of dimension to set
     **/
    inline void setDimension( int const& dim) { dim_ = dim;}

  protected:
    /** dimension of the reduced data set */
    int dim_;
    /** The reduced data set. */
    ArrayXX* p_reduced_;
};

} // namespace STK

#endif /* STK_IREDUCT_H */
