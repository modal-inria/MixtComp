/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::Model
 * created on: 22 juil. 2011
 * Purpose: define the Interface base class ICriterion.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_ICriterion.h
 *  @brief In this file we define the interface base class ICriterion
 **/

#ifndef STK_ICRITERION_H
#define STK_ICRITERION_H

#include "../../Sdk/include/STK_IRunner.h"
#include "STK_IModelBase.h"

namespace STK
{
/** @ingroup Model
 * @brief Interface base class for the selection model criterion. The pure
 * virtual function @c run will be implemented in derived class and compute
 * the value_ member.
  */
class ICriterion : virtual public IRunnerBase
{
  protected:
    /** Constructor.
     *  @param model the current model
     **/
    ICriterion( IModelBase const& model);

  public:
    /** Destructor
     **/
    virtual ~ICriterion();
    /** get the value of the criterion
     * @return The value of the criterion
     **/
    inline Real const& value() const { return value_;}
    /** set the current model.
     *  @param model the current model
     **/
     inline void setModel( IModelBase const& model)
     { p_model_ = &model;}
  protected:
    /** The current statistical model to use*/
     IModelBase const* p_model_;
   /** Computed value of the criterion */
   Real value_;
};

} // namespace STK

#endif
