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
 * Purpose: define the class IUnivStatModel.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_GammaComponent.h
 *  @brief In this file we define the component for the gamma mixture models.
 **/

#ifndef STK_GAMMACOMPONENT_H
#define STK_GAMMACOMPONENT_H

#include <cmath>


#include "STK_GammaParameters.h"
#include "../STK_IMixtureComponent.h"

#include "../../../STatistiK/include/STK_Law_Gamma.h"

namespace STK
{

/** @ingroup Clustering
 *  Base class for the gamma components.
 *  Provide the virtual computeLnLikelihood() method required by base class.
 *
 *  @note The Parameters class have to derive from gammaParametersBase.
 **/
template <class _Array, class _Parameters>
class GammaComponent : public IMixtureComponent< _Array, _Parameters >
{
  public:
    typedef _Array Array;
    typedef _Parameters Parameters;
    typedef typename _Array::Row RowVector;
    /** Base class */
    typedef IMixtureComponent< _Array, _Parameters > Base;
    using Base::p_param;

    /** default constructor. */
    inline GammaComponent() : Base() {}
    /** Copy constructor. */
    inline GammaComponent( GammaComponent const& model): Base(model) {}
    /** destructor */
    inline virtual ~GammaComponent() {}
    /** clone pattern */
    inline GammaComponent*  clone() const { return new GammaComponent(*this);};
    /** compute the log Likelihood of an observation.
     *  @param rowData the observation
     **/
    virtual Real computeLnLikelihood( RowVector const& rowData) const
    {
      Real sum =0.;
      for (Integer j= rowData.firstIdx(); j <= rowData.lastIdx(); ++j)
      { sum += Law::Gamma::lpdf(rowData[j], p_param()->shape(j), p_param()->scale(j));}
      return sum;
    }
};

} // namespace STK

#endif /* STK_GAMMACOMPONENT_H */
