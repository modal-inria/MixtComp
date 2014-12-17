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
 * Purpose:  Abstract class for the computation of the Index in the SLAAM.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_ILinearReduct.h In this file we define the interface base class
 *  ILinearReduct.
 **/

#ifndef STK_ILINEAREDUCT_H
#define STK_ILINEAREDUCT_H

#include "../../../include/Arrays.h"

#include "STK_IReduct.h"

namespace STK
{
/** @ingroup Reduct
 *  @brief A ILinearReduct is an interface base class for reduction method
 *  using a linear reduction. In order to find the linear reduction,
 *  the derived classes have to  furnish an index to maximize.
 *
 * The class receive a matrix in input of size (n,p).
 * - n is the number of samples,
 * - p is the number of variables.
 * The n samples can be weighted.
 *
 * The class compute the optimal axis (stored in the @c axis_ )
 * attribute and the @c projected data set (stored in the @c p_reduct_ atribute
 * of the base class IReduct) when the user use the virtual method @c run()
 * (for not weighted observations) or @c run(weights) (for weighted observations).
 *
 * The Array axis_ is computed by maximizing some criteria defined in
 * derived classes. It is constructed using the pure virtual functions:
 * @code
 *  virtual void maximizeCriteria() =0;
 *  virtual void maximizeCriteria(weights) =0;
 * @endcode
 */
class ILinearReduct : public IReduct
{
  protected:
    /** Default Constructor. */
    ILinearReduct();
    /** Constructor.
     *  @param p_data a pointer on the constant data set to reduce.
     **/
    ILinearReduct( ArrayXX const* p_data);
    /** Constructor.
     *  @param data a constant reference on the data set to reduce.
     **/
    ILinearReduct( ArrayXX const& data);
    /** copy Constructor.
     *  @param reductor the reductor to copy.
     **/
    ILinearReduct( ILinearReduct const& reductor);

  public:
    /** virtual destructor  */
    virtual ~ILinearReduct();
    /** Compute the projection matrix @c axis_ by maximizing the criteria and
     *  project the data set in order to obtain @c p_projected_.
     **/
    virtual bool run();
    /** Compute the projection matrix set by maximizing the weighted criteria
     *  and project the data set in order to obtain @c p_projected_.
     *  @param weights the weights to used in the index.
     */
    virtual bool run(Vector const& weights);
    /** get the values of the index of each axis
     * @return a constant reference Vector of the value of the Index
     **/
    inline Vector const& criteriaValues() const { return idx_values_; }
    /** Get the axis
     *  @return a constant reference Array of the axis
     **/
    inline ArrayXX const& axis() const { return axis_; }

  protected:
    /** The values of the index for each axis. */
    Vector idx_values_;
    /** The computed axis. */
    ArrayXX axis_;

  private:
    /** Find the axis by maximizing the Index. */
    virtual void maximizeCriteria() =0;
    /** Find the axis by maximizing the weighted Index.
     *  @param weights the weights to used
     * */
    virtual void maximizeCriteria( Vector const& weights) =0;
    /** Compute the projection of the data set on the Axis. */
    void projection();
};

} // namespace STK

#endif /* STK_ILINEARREDUCT_H */
