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
#include "STK_CategoricalParameters.h"

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
    using Base::p_pk;
    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

    /** default constructor
     *  @param nbCluster number of cluster in the model
     **/
    inline CategoricalBase( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline CategoricalBase( CategoricalBase const& model)
                          : Base(model), modalities_(model.modalities_) {}
    /** destructor */
    inline ~CategoricalBase() {}

  public:
    /** @return an imputation value for the jth variable of the ith sample
     *  @param i,j indexes of the data to impute */
    int impute(int i, int j) const;
    /** @return a simulated value for the jth variable of the ith sample
     * @param i,j indexes of the data to simulate*/
    int sample(int i, int j) const;
    /** Initialize the model. Resize the array of probabilities for each
     *  component.*/
    void initializeModel()
    {
      Base::initializeModel();
      // resize vectors of probabilities
      for(int k=baseIdx; k<= components().lastIdx(); ++k)
      { p_param(k)->initializeParameters(modalities_);}
    }
    /** Write the parameters on the output stream os */
    void writeParameters(ostream& os) const
    {
      Array2D<Real> proba(modalities_, p_data()->cols());
      for (int k= baseIdx; k <= components().lastIdx(); ++k)
      {
        // store proba values in an array for a nice output
        for (int j= baseIdx;  j <= proba.lastIdxCols(); ++j)
        {
          for (int l= modalities_.firstIdx(); l <= modalities_.lastIdx(); ++l)
          { proba(l, j) = p_param(k)->proba(j,l);}
        }
        stk_cout << _T("---> Component ") << k << _T("\n");
        stk_cout << _T("Probabilities =\n")<< proba;
      }
    }
    /** Set the range of the modalities
     *  @param modalities the range of the modalities
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
  int lmax = modalities_.firstIdx();
  Real pmax = -Arithmetic<Real>::max();
  // compute for each modality the pondered probability of occurrence
  for (int l=modalities_.firstIdx(); l<= modalities_.lastIdx(); ++l)
  {
    Real proba = 0.;
    for (int k= p_pk()->firstIdx(); k <= p_pk()->lastIdx(); ++k)
    { proba += p_tik()->elt(i,k) * p_param(k)->proba(j, l);}

    if (pmax < proba) { pmax = proba; lmax = l;}
  }
  //Stat::sumByRow(proba(j) * ptik()->row(i)).maxElt(lmax);
  return lmax;
}

/* Implementation  */
template<class Derived>
int CategoricalBase<Derived>::sample(int i, int j) const
{
  // sample class
  int k = Law::Categorical::rand(p_tik()->row(i));
  // sample from conditional probability
  return Law::Categorical::rand(p_param(k)->proba(j));
}

} // namespace STK

#endif /* STK_CATEGORICALBASE_H */
