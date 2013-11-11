/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

#ifndef MIXT_GAMMAMIXTURE_H
#define MIXT_GAMMAMIXTURE_H

#include "../../mixtureInterface/mixt_Traits.h"
#include "mixt_MixtureBridge.h"

namespace mixt
{

/** Given the Id of a mixture model, the struct Traits allow to get
 *  type of the input data and of the parameters.
 **/
template<>
struct Traits<'G'>
{
   /** */
   typedef STK::Array2D<STK::Real> Data;
   typedef STK::Array2D<STK::Real> Param;
};

class Gamma_ajk_bjk : MixtureBridge<STK::Gamma_pk_ajk_bjk<typename Traits<'G'>::Data> >
{
  public:
    // get the Type of the data
    typedef MixtureBridge<STK::Gamma_pk_ajk_bjk<typename Traits<'G'>::Data> > Base;
    /** constructor. @param id id of the mixture */
    Gamma_ajk_bjk( char id, int nbCluster, mixt::CompositeMixtureModel const* p_model )
                   : Base(id, nbCluster, p_model)
    {}
    /** copy constructor */
    Gamma_ajk_bjk( MixtureBridge const& original)
                   : Base(original)
    {}
    virtual Gamma_ajk_bjk* clone()
    {
      return new Gamma_ajk_bjk(*this);
    }

    virtual void setData()
    {
      data_.move(p_compositeModel_->getData<Data>());
    }
};

} /* namespace mixt */

#endif /* MIXT_GAMMAMIXTURE_H_ */
