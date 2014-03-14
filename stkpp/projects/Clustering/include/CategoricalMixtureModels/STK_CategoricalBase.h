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

/** @file STK_CategoricalBase.h
 *  @brief In this file we implement the base class for the Categorical
 *  diagonal models
 **/

#ifndef STK_CATEGORICALBASE_H
#define STK_CATEGORICALBASE_H

#include "../STK_IMixtureModel.h"
#include "STK_CategoricalComponent.h"

#include "../../../STatistiK/include/STK_Law_Categorical.h"

namespace STK
{

/** @ingroup Clustering
 *  Base class for the Categorical models
 **/
template<class Derived>
class CategoricalBase : public IMixtureModel<Derived >
{
  protected:
    typedef IMixtureModel<Derived > Base;
    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline CategoricalBase( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline CategoricalBase( CategoricalBase const& model) : Base(model) {}
    /** destructor */
    inline ~CategoricalBase() {}

  public:
    /** @return an imputation value for the jth variable of the ith sample
     *  @param i,j indexes of the data to impute */
    int impute(int i, int j) const
    {
      // compute argmax ??
      return 0.;
    }
    /** @return a simulated value for the jth variable of the ith sample
     * @param i,j indexes of the data to simulate*/
    int sample(int i, int j) const
    {
      // simulate
      return 0.;
    }
    /** @return a safe value for the jth variable
     *  @param j index of the column with the safe value needed
     **/
    inline int safeValue(int j) const
    {
      // compute arg max ??
      return 0;
    }
    /** get the parameters of the model
     *  @param params the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const
    {
      params.resize(this->nbCluster(), p_data()->cols());
      for (int k= params.firstIdxRows(); k <= params.lastIdxRows(); ++k)
      {
        for (int j=  p_data()->firstIdxCols();  j <= p_data()->lastIdxCols(); ++j)
        {
          params(k, j) = p_param(k)->proba(j);
        }
      }

    }
    /** Write the parameters on the output stream os */
    void writeParameters(ostream& os) const
    {
      Array2DPoint<Real> proba(p_data()->cols());
      for (int k= components().firstIdx(); k <= components().lastIdx(); ++k)
      {
        // store proba values in an array for a nice output
        for (int j= proba.firstIdx();  j <= proba.lastIdx(); ++j)
        { proba[j] = p_param(k)->proba(j);}
        stk_cout << _T("---> Component ") << k << _T("\n");
        stk_cout << _T("proba = ")<< proba;
      }
    }
    /** Set the range of the modalities
     * @param modalities the range of the modalities
     **/
    inline void setModalities( Range const& modalities) { modalities_ = modalities;}

  protected:
    /** range of the modalities */
    Range modalities_;
};

} // namespace STK

#endif /* STK_DIAGGAUSSIANBASE_H */
