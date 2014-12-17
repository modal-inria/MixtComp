/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014 Serge IOVLEFF

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
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;

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
    /** get the parameters of the model in an array of size (K * L) * d.
     *  @param params the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const;
    /** @return the parameters of the model in an array of size (K * L) * d. */
    ArrayXX getParametersImpl() const;
    /** Initialize the model. Resize the array of probabilities for each
     *  component.*/
    void initializeModelImpl()
    {
      nbModalities_.resize(p_data()->cols());
      int amin = Arithmetic<int>::max(), amax = Arithmetic<Real>::min();
      for (int j= p_data()->beginCols(); j < p_data()->endCols(); ++j)
      {
        int min = p_data()->col(j).minElt(), max = p_data()->col(j).maxElt();
        amin = std::min(amin, min); amax = std::max(amax, max);
        nbModalities_[j] = max-min+1;
      }
      // set range of the modalities
      modalities_ = _R(amin, amax);
      // resize vectors of probabilities
      for(int k=baseIdx; k < components().end(); ++k)
      { p_param(k)->initializeParameters(modalities_);}
      // resize paramBuffer_
      paramBuffer_.resize(modalities_.size() * this->nbCluster(), p_data()->cols());
      paramBuffer_ = 0.;
    }
    /** Write the parameters on the output stream os */
    void writeParameters(ostream& os) const
    {
      Array2D<Real> proba(modalities_, p_data()->cols());
      for (int k= baseIdx; k < components().end(); ++k)
      {
        // store proba values in an array for a nice output
        for (int j= baseIdx;  j < proba.endCols(); ++j)
        {
          for (int l= modalities_.begin(); l < modalities_.end(); ++l)
          { proba(l, j) = p_param(k)->proba(j,l);}
        }
        os << _T("---> Component ") << k << _T("\n");
        os << _T("probabilities =\n") << proba  << _T("\n");
      }
    }

  protected:
    /** Array with the number of modalities of each columns of the data set */
    Array2DPoint<int> nbModalities_;
    /** range of the modalities */
    Range modalities_;
};

/* Implementation  */
template<class Derived>
int CategoricalBase<Derived>::impute(int i, int j) const
{
  int lmax = modalities_.begin();
  Real pmax = -Arithmetic<Real>::max();
  // compute for each modality the pondered probability of occurrence
  for (int l=modalities_.begin(); l< modalities_.end(); ++l)
  {
    Real proba = 0.;
    for (int k= p_pk()->begin(); k < p_pk()->end(); ++k)
    { proba += p_tik()->elt(i,k) * p_param(k)->proba(j, l);}

    if (pmax < proba) { pmax = proba; lmax = l;}
  }
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

/* get the parameters of the model
 *  @param params the parameters of the model
 **/
template<class Derived>
void CategoricalBase<Derived>::getParameters(Array2D<Real>& params) const
{
  int nbCluster    = this->nbCluster();
  int nbModalities = modalities_.size();

  params.resize(nbModalities * nbCluster, p_data()->cols());
  for (int k = 0; k < nbCluster; ++k)
  {
    for (int j = p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {
      for (int l = 0; l < nbModalities; ++l)
      { params(baseIdx + k * nbModalities + l, j) = p_param(baseIdx + k)->proba(j, modalities_.begin() + l);}
    }
  }
}

/* get the parameters of the model in an array of size (K * L) * d.
 **/
template<class Derived>
ArrayXX CategoricalBase<Derived>::getParametersImpl() const
{
  ArrayXX params;
  int nbCluster    = this->nbCluster();
  int nbModalities = modalities_.size();

  params.resize(nbModalities * nbCluster, p_data()->cols());
  for (int j = p_data()->beginCols(); j < p_data()->endCols(); ++j)
  {
    for (int k = 0; k < nbCluster; ++k)
    {
      for (int l = 0; l < nbModalities; ++l)
      { params(baseIdx + k * nbModalities + l, j) = p_param(baseIdx + k)->proba(j, modalities_.begin() + l);}
    }
  }
  return params;
}

} // namespace STK

#endif /* STK_CATEGORICALBASE_H */
