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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::Model
 * created on: 22 juil. 2011
 * Purpose: define the class IMixtureComponent.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IMixtureComponent.h
 *  @brief In this file we define the class IMixtureComponent.
 **/

#ifndef STK_IMIXTURECOMPONENT_H
#define STK_IMIXTURECOMPONENT_H

#include <cmath>

#include "../../Sdk/include/STK_IRecursiveTemplate.h"
#include "../../StatModels/include/STK_IMultiParameters.h"
#include "../../Sdk/include/STK_Macros.h"

namespace STK
{

/** @ingroup Clust
 *  @brief Interface base class for the components of a Mixture Model.
 *
 *  From a computational point of view a Component is defined with
 *  the help of two elements
 *  - A data set where the number of samples is the number of rows and the number
 *  of variable is the number of columns. This data set is accessed via
 *  a pointer of type @c Array const*.
 *  - A set of parameters stored in a class of type @c Parameters. The type
 *  Parameters have to derive from IMultiParameters. The parameters are created
 *  at the instantiation of the component and deleted at its destruction.
 *
 *  DerivedComponent implementations of this interface have to implement the
 *  virtual methods:
 *  @code
 *    IMixtureComponent* clone() const;
 *    Real computeLnLikelihood( RowVector const& rowData) const;
 *  @endcode
 *
 *  @note This class is a "kitchen" providing tools but does not propose any
 *  estimation method.
 **/
template <class Array, class Parameters>
class IMixtureComponent
{
  public:
    /** Type of the row vector of the container */
    typedef typename  Array::Row RowVector;

  protected:
    /** default constructor. */
    IMixtureComponent() : p_data_(0), p_param_(new Parameters) {}
    /** Copy constructor.
     *  @param component the component to copy
     **/
    IMixtureComponent( IMixtureComponent const& component)
                     : p_data_(component.p_data_)
                     , p_param_(component.p_param_->clone())
    {}
    /** destructor */
    virtual ~IMixtureComponent()
    { if (p_param_) { delete p_param_->asPtrDerived();} }

  public:
    /** clone pattern */
    virtual IMixtureComponent*  clone() const =0 ;
    /** @return the pointer on the data set */
    inline Array const*  p_data() const { return(p_data_);}
    /** @return the pointer on the parameters */
    inline Parameters* const p_param() const
    { return (p_param_) ? p_param_->asPtrDerived() : 0;}

    /** compute the log Likelihood of the component. */
    Real computeLnLikelihood() const
    {
      Real sum = 0.0;
      for (int i= p_data_->firstIdxRows(); i<= p_data_->lastIdxRows(); i++)
      { sum += computeLnLikelihood(p_data_->row(i));}
      return(sum);
    }
    /** compute the log Likelihood of an observation.
     *  @param rowData the data
     **/
    virtual Real computeLnLikelihood( RowVector const& rowData) const =0;

    /** Set the data of the component.
     *  @param p_data the pointer on the data set
     **/
    inline void setData(Array const* p_data)
    {
      p_data_ = p_data;
      if ( p_data_) { p_param_->resize(p_data_->cols());}
    }
    /** Write the parameters of the component.
     *  @param os the output stream for the parameters
     **/
     inline void writeParameters(ostream &os) { p_param_->print(os);}

  protected:
    /** Pointer on the data set. */
    Array const* p_data_;
    /** Pointer on the parameters of the model. */
    IMultiParameters<Parameters>* p_param_;
};

} // namespace STK

#endif /* STK_IMIXTURECOMPONENT_H */
