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
 * Project:  stkpp::AAModels
 * created on: 17 avr. 2010
 * Purpose:  Abstract class for the computation of the Index in the
 * AAM models.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_ILinearReduct.cpp In this file we implement the interface base
 * class ILinearReduct.
 **/

#include "../include/STK_ILinearReduct.h"

namespace STK
{
/*
 * Constructor.
 * @param data the input data set
 */
ILinearReduct::ILinearReduct() : IReduct()
{}

/*
 * Constructor.
 * @param data the input data set
 */
ILinearReduct::ILinearReduct( Matrix const* p_data) : IReduct(p_data)
{}
/*
 * Constructor.
 * @param data the input data set
 */
ILinearReduct::ILinearReduct( Matrix const& data) : IReduct(data)
{}
/* copy Constructor.
 *  @param reductor the reductor to copy.
 **/
ILinearReduct::ILinearReduct( ILinearReduct const& reductor)
                            : idx_values_(reductor.idx_values_)
                            , axis_(reductor.axis_)
{}
/* Destructor */
ILinearReduct::~ILinearReduct()
{}

/* Compute the Index.
 *  @param nbAxis number of Axis to compute
 */
bool ILinearReduct::run()
{
  try
  {
    // maximize the Index and compute the axis
    maximizeCriteria();
    // project data
    projection();

  } catch (Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
  return true;
}

/*
 * Compute the weighted index.
 * @param weights the weights to used
 * @param nbAxis number of Axis to compute
 */
bool ILinearReduct::run( Vector const& weights)
{
  try
  {
    // maximize the Index and compute the axis
    maximizeCriteria(weights);
    // project data
    projection();

  } catch (Exception const& e)
  {
    msg_error_ = e.error();
    return false;
  }
  return true;
}

/* Compute the reduction of the data set on the Axis. */
void ILinearReduct::projection()
{
  // check if p_reduced exists
  if (!p_reduced_) p_reduced_ = new Matrix;
  // compute matrix multiplication
  *p_reduced_ =   (*p_data_) * axis_;
}

} // namespace STK

