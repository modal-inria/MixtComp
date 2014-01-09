/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: Dec 4, 2013
 * Author: Serge Iovleff
 **/

/** @file STK_GammaBase.h
 *  @brief In this file we implement the base class for the gamma models
 **/

#ifndef STK_GAMMABASE_H
#define STK_GAMMABASE_H

#include "../STK_IMixtureModel.h"
#include "../../../STatistiK/include/STK_Law_Gamma.h"

namespace STK
{


/** @ingroup Clustering
 *  Base class for the gamma models
 **/
template<class Derived>
class GammaBase : public IMixtureModel<Derived >
{
  public:
    typedef IMixtureModel<Derived > Base;
    using Base::components_;
    using Base::p_tik;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline GammaBase( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline GammaBase( GammaBase const& model) : Base(model) {}
    /** destructor */
    ~GammaBase() {}
    /** @return an imputation value for the jth variable of the ith sample*/
    inline Real impute(int i, int j) const
    {
      Real sum = 0.;
      for (int k= p_tik()->firstIdxCols(); k <= p_tik()->lastIdxCols(); ++k)
      { sum += p_tik()->elt(i,k) * components_[k]->p_param()->shape(j) * components_[k]->p_param()->scale(j);}
      return sum;
    }
    /** @return an simulated value for the jth variable of the ith sample*/
    inline Real sample(int i, int j) const
    {
      Real sum = 0.;
      for (int k= p_tik()->firstIdxCols(); k <= p_tik()->lastIdxCols(); ++k)
      { sum += p_tik()->elt(i,k) * Law::Gamma::rand(components_[k]->p_param()->shape(j),components_[k]->p_param()->scale(j));}
      return sum;
    }
};

} // namespace STK

#endif /* STK_GAMMABASE_H */
