/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

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

/** @file STK_Categorical_pjk.h
 *  @brief In this file we implement the Categorical_pjk class
 **/

#ifndef STK_CATEGORICAL_PJK_H
#define STK_CATEGORICAL_PJK_H

#include "STK_CategoricalBase.h"

namespace STK
{

//forward declaration, to allow for recursive template
template<class Array>class Categorical_pjk;

namespace Clust
{
/** @ingroup Clustering
 *  Traits class for the Categorical_pjk traits policy. */
template<class _Array>
struct MixtureTraits< Categorical_pjk<_Array> >
{
  typedef _Array Array;
  typedef typename Array::Type Type;
  typedef Categorical_pjkParameters        Parameters;
  typedef Array2D<Real>        Param;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Categorical mixture model @c Categorical_pjk is
 *  the most general diagonal Categorical model and have a probability
 *  function of the form
 * \f[
 *    P(\mathbf{x}=(l_1,\ldots,l_d)|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d p_{kl_j}^j.
 * \f]
 **/
template<class Array>
class Categorical_pjk : public CategoricalBase<Categorical_pjk<Array> >
{
  public:
    typedef CategoricalBase<Categorical_pjk<Array> > Base;
    typedef typename Clust::MixtureTraits< Categorical_pjk<Array> >::Parameters Parameters;

    using Base::p_tik;
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;
    using Base::modalities_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    Categorical_pjk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    Categorical_pjk( Categorical_pjk const& model) : Base(model) {}
    /** destructor */
    ~Categorical_pjk() {}
    /** Compute the initial weighted probabilities of the mixture */
    bool initializeStep();
    /** Initialize randomly the parameters of the Categorical mixture. */
    void randomInit();
    /** Compute the weighted probabilities. */
    bool mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*((this->nbModalities_-1).sum());}
    /** set the parameters of the model */
    void setParametersImpl();
};

/* set the parameters of the model */
template<class Array>
void Categorical_pjk<Array>::setParametersImpl()
{
  int nbCluster    = this->nbCluster();
  int nbModalities = this->modalities_.size();
  for (int k = 0; k < nbCluster; ++k)
  {
    for (int j = p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {
      for (int l = 0; l < nbModalities; ++l)
      { p_param(baseIdx + k)->proba_[j][modalities_.begin() + l]
                          = paramBuffer_(baseIdx + k * nbModalities + l, j);
      }
    }
  }
}

/* Initialize the parameters using mStep. */
template<class Array>
bool Categorical_pjk<Array>::initializeStep()
{ return mStep();}

/* Initialize randomly the parameters of the Categorical mixture.
 */
template<class Array>
void Categorical_pjk<Array>::randomInit()
{
  for (int k = baseIdx; k < components().end(); ++k)
  {
    for (int j=baseIdx; j< p_param(k)->proba_.end(); ++j)
    {
      p_param(k)->proba_[j].randUnif();
      p_param(k)->proba_[j] /= p_param(k)->proba_[j].sum();
    }
  }
}


/* Compute the modalities probabilities */
template<class Array>
bool Categorical_pjk<Array>::mStep()
{
  for (int k = baseIdx; k < components().end(); ++k)
  {
    for (int j = p_data()->beginCols(); j < p_data()->endCols(); ++j)
    {
      // count the number of modalities weigthed by the tik
      p_param(k)->proba_[j] = 0.;
      for (int i = p_tik()->beginRows(); i < p_tik()->endRows(); ++i)
      { p_param(k)->proba_[j][(*p_data())(i, j)] += (*p_tik())(i, k);}
      // normalize the probabilities
      Real sum = p_param(k)->proba_[j].sum();
      if (sum<=0.) return false;
      p_param(k)->proba_[j] /= sum;
    }
  }
  return true;
}

} // namespace STK

#endif /* STK_CATEGORICAL_PJK_H */
