/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014 Serge IOVLEFF, Inria

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
 * Authors: Serge Iovleff, Vincent KUBICKI
 **/

/** @file STK_GammaBase.h
 *  @brief In this file we implement the base class for the gamma models
 **/

#ifndef STK_GAMMABASE_H
#define STK_GAMMABASE_H

#include "../STK_IMixtureModel.h"
#include "STK_GammaComponent.h"

#include "../../../STatistiK/include/STK_Law_Categorical.h"

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

    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

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
    Real impute(int i, int j) const
    {
      Real sum = 0.;
      for (int k= p_tik()->firstIdxCols(); k <= p_tik()->lastIdxCols(); ++k)
      { sum += p_tik()->elt(i,k) * p_param(k)->shape(j) * p_param(k)->scale(j);}
      return sum;
    }
    /** @return a simulated value for the jth variable of the ith sample
     *  @param i,j indexes of the value to sample
     **/
    Real sample(int i, int j) const
    {
      int k = Law::Categorical::rand(p_tik()->row(i));
      return Law::Gamma::rand(p_param(k)->shape(j),p_param(k)->scale(j));
    }
    /** @brief compute a safe value for the column j, missing values are replaced by 1.
     *  @return a safe value for the jth variable
     *  @param j the index of the column with a missing value
     * */
    inline Real safeValue(int j) const
    { return this->p_data()->col(j).safe(1.).mean();}
    /** get the parameters of the model
     *  @param params the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const
    {
      int nbClust = this->nbCluster();
      params.resize(2*nbClust, p_data()->cols());
      int firstId = params.firstIdxRows();

      for (int k= 0; k < nbClust; ++k)
      {
        for (int j=  p_data()->firstIdxCols();  j <= p_data()->lastIdxCols(); ++j)
        {
          params(2*k+  firstId, j) = p_param(k+firstId)->shape(j);
          params(2*k+1+firstId, j) = p_param(k+firstId)->scale(j);
        }
      }
    }
    /** Write the parameters on the output stream os */
    void writeParameters(ostream& os) const
    {
      Array2DPoint<Real> shape(p_data()->cols()), scale(p_data()->cols());
      for (int k= components().firstIdx(); k <= components().lastIdx(); ++k)
      {
        // store shape and scale values in an array for a nice output
        for (int j= p_data()->firstIdxCols();  j <= p_data()->lastIdxCols(); ++j)
        {
          shape[j] = p_param(k)->shape(j);
          scale[j] = p_param(k)->scale(j);
        }
        stk_cout << _T("---> Component ") << k << _T("\n");
        stk_cout << _T("shape = ") << shape;
        stk_cout << _T("scale = ") << scale;
      }
    }
};

} // namespace STK

#endif /* STK_GAMMABASE_H */
