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
 * Purpose: define the class MixtureComponent.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_MixtureComponent.h
 *  @brief In this file we define the class MixtureComponent.
 **/

#ifndef STK_MIXTURECOMPONENT_H
#define STK_MIXTURECOMPONENT_H

#include <cmath>

#include "StatModels/include/STK_IMultiParameters.h"

namespace STK
{

/** @ingroup Clust
 *  @brief Interface base class for the components of a Mixture Model.
 *
 *  From a computational point of view a Component is defined with
 *  the help of two elements
 *  - A data set where the number of samples is the number of rows and the number
 *  of variable is the number of columns.
 *  - A set of parameters stored in a class of type @c Parameters. The type
 *  Parameters have to derive from IMultiParameters. The parameters are created
 *  at the instantiation of the component and deleted at its destruction.
 *
 *  @sa GammaParametersBase, GaussianParametersBase, CategoricalParametersBase
 *  @note This class is a "kitchen" providing tools but does not propose any
 *  estimation method.
 **/
template <class Array, class Parameters>
class MixtureComponent
{
  public:
    /** Type of the row vector of the container */
    typedef typename  Array::Row RowVector;

    /** default constructor. */
    inline MixtureComponent() : p_param_(new Parameters) {}
    /** Copy constructor.
     *  @param component the component to copy
     **/
    inline MixtureComponent( MixtureComponent const& component)
                           : p_param_(component.p_param_->clone())
    {}
    /** destructor */
    inline ~MixtureComponent()
    { if (p_param_) { delete p_param_->asPtrDerived();} }
    /** clone pattern. */
    inline MixtureComponent*  clone() const
    { return new MixtureComponent(*this);};
    /** @return the pointer on the parameters */
    inline Parameters* const p_param() const
    { return (p_param_) ? p_param_->asPtrDerived() : 0;}

    /** compute the log Likelihood of an observation.
     *  The computation is delegated to the parameter class.
     *  @param rowData the data
     **/
    inline Real computeLnLikelihood( RowVector const& rowData) const
    { return p_param()->computeLnLikelihood(rowData);}

    /** Write the parameters of the component.
     *  @param os the output stream for the parameters
     **/
    inline void writeParameters(ostream &os) { p_param_->print(os);}

  private:
    /** Pointer on the parameters of the model. */
    IMultiParameters<Parameters>* p_param_;
};

} // namespace STK

#endif /* STK_MIXTURECOMPONENT_H */
