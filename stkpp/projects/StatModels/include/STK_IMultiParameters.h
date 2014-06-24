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
 * Project:  stkpp::StatModels
 * created on: 22 juil. 2011
 * Purpose: define the class IMultiStatModel.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_IMultiParameters.h
 *  @brief In this file we define the interface IMultiParameters.
 **/

#ifndef STK_IMULTIPARAMETERS_H
#define STK_IMULTIPARAMETERS_H

#include "Sdk/include/STK_IRecursiveTemplate.h"
#include "STKernel/include/STK_Range.h"

namespace STK
{

/** @ingroup StatModels
 *  @brief Interface base class for the parameters of a multivariate model.
  */
template<class Parameters>
class IMultiParameters : public IRecursiveTemplate<Parameters>
{
  protected:
    /** default constructor.*/
    inline IMultiParameters(): range_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline IMultiParameters( Range const& range): range_(range) {}

    /** copy constructor.*/
    inline IMultiParameters(IMultiParameters const& param): range_(param.range_) {}
    /** Destructor */
    inline ~IMultiParameters() {}

  public:
    /** @return the range of the data set */
    inline Range const& range() const { return range_;}
    /** resize the parameters only if the range is modified, otherwise, stay
     *  with the current values.
     *  @param range the range of the parameters (= range of the variables of the model)
     **/
    inline void resize( Range const& range)
    { if (range != range_)
      { range_ = range;
        this->asDerived().resizeImpl(range);
      }
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void print(ostream &os) { this->asDerived().printImpl(os);}

  private:
    Range range_;
};

} // namespace STK

#endif /* STK_IMULTIPARAMETERS_H */
