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
    int impute(int i, int j) const;
    /** @return a simulated value for the jth variable of the ith sample
     * @param i,j indexes of the data to simulate*/
    int sample(int i, int j) const;
    /** @return a safe value for the jth variable
     *  @param j index of the column with the safe value needed
     **/
    inline int safeValue(int j) const;
    /** */
    void initializeModel()
    {
      Base::initializeModel();
      // resize vectors of probabilities
      for(int k=components().firstIdx(); k<= components().lastIdx(); ++k)
      { components(k)->p_param()->initializeParameters(modalities_);}
    }
    /** get the parameters of the model
     *  @param params the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const
    {
      int firstId = params.firstIdxRows();
      int nbClust = this->nbCluster();
      int nbModalities = modalities_.size();

      params.resize(nbModalities * nbClust, p_data()->cols());
      for (int k = 0; k <= nbClust; ++k)
      {
        for (int j = p_data()->firstIdxCols(); j <= p_data()->lastIdxCols(); ++j)
        {
          for (int l = 0; l < nbModalities; ++l)
          {
            params(k * nbModalities + l + firstId, j) = p_param(k)->proba(j, l);
          }
        }
      }
    }
    /** Write the parameters on the output stream os */
    void writeParameters(ostream& os) const
    {
      Array2D<Real> proba(modalities_, p_data()->cols());
      for (int k= components().firstIdx(); k <= components().lastIdx(); ++k)
      {
        // store proba values in an array for a nice output
        for (int j= proba.firstIdxRows();  j <= proba.lastIdxRows(); ++j)
        {
          for (int l= modalities_.firstIdx(); l <= modalities_.lastIdx(); ++l)
          { proba(l, j) = p_param(k)->proba(j,l);}}
        stk_cout << _T("---> Component ") << k << _T("\n");
        stk_cout << _T("Probabilities = ")<< proba;
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

/* Implementation  */
template<class Derived>
int CategoricalBase<Derived>::impute(int i, int j) const
{
      // compute argmax ??
      return 0;
}

/* Implementation  */
template<class Derived>
int CategoricalBase<Derived>::sample(int i, int j) const
{
   return 0;
}
/* Implementation  */
template<class Derived>
int CategoricalBase<Derived>::safeValue(int j) const
{
   return 0;
}

} // namespace STK

#endif /* STK_DIAGGAUSSIANBASE_H */
