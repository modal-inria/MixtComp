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
 * created on: Oct 24, 2013
 * Author:   Serge Iovleff
 **/

/** @file STK_Categorical_pk.h
 *  @brief In this file we define the Categorical_pk model
 **/

#ifndef STK_CATEGORICAL_PK_H
#define STK_CATEGORICAL_PK_H

#include "STK_CategoricalBase.h"

namespace STK
{

//forward declaration, to allow for recursive template
template<class Array>class Categorical_pk;

namespace Clust
{
/** @ingroup Clustering
 *  Traits class for the Categorical_pk traits policy. */
template<class _Array>
struct MixtureTraits< Categorical_pk<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Categorical_pkParameters Parameters;
  typedef Array2D<Real>   Param;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Categorical mixture model @c Categorical_pk is
 *  the most general diagonal Categorical model and have a density function of the
 *  form
 * \f[
 *  P(\mathbf{x}=(l_1,\ldots,l_d)|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d p_{kl_j}.
 * \f]
 **/
template<class Array>
class Categorical_pk : public CategoricalBase<Categorical_pk<Array> >
{
  public:
    typedef CategoricalBase<Categorical_pk<Array> > Base;
    typedef typename Clust::MixtureTraits< Categorical_pk<Array> >::Parameters Parameters;

    using Base::p_tik;
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;
    using Base::modalities_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline Categorical_pk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline Categorical_pk( Categorical_pk const& model) : Base(model) {}
    /** destructor */
    inline ~Categorical_pk() {}
    /** Compute the inital weighted mean and the initial common variances. */
    bool initializeStep();
    /** Initialize randomly the parameters of the Categorical mixture.
     *  Probabilities will be choosen uniformly.
     */
    void randomInit();
    /** Compute the weighted proportions of each class. */
    bool mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*(this->modalities_.size()-1);}
    /** set the parameters of the model */
    void setParametersImpl();
};

/* set the parameters of the model */
template<class Array>
void Categorical_pk<Array>::setParametersImpl()
{
  int nbCluster    = this->nbCluster();
  int nbModalities = this->modalities_.size();
  for (int k = 0; k < nbCluster; ++k)
  {
    for (int l = 0; l < nbModalities; ++l)
    { p_param(baseIdx + k)->proba_[modalities_.begin() + l]
                                  = paramBuffer_(baseIdx + k * nbModalities + l, p_data()->beginCols());
    }
  }
}

/* Initialize the parameters using mStep. */
template<class Array>
bool Categorical_pk<Array>::initializeStep() { return mStep();}

/* Initialize randomly the parameters of the Categorical mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Categorical_pk<Array>::randomInit()
{
  for (int k = baseIdx; k < components().end(); ++k)
  {
    p_param(k)->proba_.randUnif();
    p_param(k)->proba_ /= p_param(k)->proba_.sum();
  }
}

/* Compute the weighted mean and the common variance. */
template<class Array>
bool Categorical_pk<Array>::mStep()
{
  for (int k = baseIdx; k < components().end(); ++k)
  {
    p_param(k)->proba_ = 0.;
    for (int j = p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {
      for (int i = p_tik()->beginRows(); i < p_tik()->endRows(); ++i)
      { p_param(k)->proba_[(*p_data())(i, j)] += (*p_tik())(i, k);}
    }
    Real sum = p_param(k)->proba_.sum();
    if (sum<=0.) return false;
    p_param(k)->proba_ /= sum;
  }
  return true;
}

} // namespace STK

#endif /* STK_CATEGORICAL_PK_H */
