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
struct MixtureModelTraits< Categorical_pjk<_Array> >
{
  typedef _Array Array;
  typedef MixtureComponent<_Array, Categorical_pjkParameters> Component;
  typedef Categorical_pjkParameters        Parameters;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Categorical mixture model @c Categorical_pjk is
 *  the most general diagonal Categorical model and have a probability
 *  function of the form
 * \f[
 *    f(\mathbf{x}=(l_1,\ldots,l_d)|\theta)
 *      = \sum_{k=1}^K p_k \prod_{j=1}^d p_{kl_j}^j.
 * \f]
 **/
template<class Array>
class Categorical_pjk : public CategoricalBase<Categorical_pjk<Array> >
{
  public:
    typedef CategoricalBase<Categorical_pjk<Array> > Base;
    typedef typename Clust::MixtureModelTraits< Categorical_pjk<Array> >::Component Component;
    typedef typename Clust::MixtureModelTraits< Categorical_pjk<Array> >::Parameters Parameters;
    typedef typename Array::Col ColVector;

    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

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
    void initializeStep();
    /** Initialize randomly the parameters of the Categorical mixture. */
    void randomInit();
    /** Compute the weighted probabilities. */
    void mStep();
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*this->nbVariable()*(this->modalities_.size()-1);}
    /** get the parameters of the model
     *  @param params the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const;
};

/* Initialize the parameters using mStep. */
template<class Array>
void Categorical_pjk<Array>::initializeStep()
{mStep();}

/* Initialize randomly the parameters of the Categorical mixture.
 */
template<class Array>
void Categorical_pjk<Array>::randomInit()
{
  for (int k = baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    for (int j=baseIdx; j<= p_param(k)->proba_.lastIdx(); ++j)
    {
      p_param(k)->proba_[j].randUnif();
      p_param(k)->proba_[j] /= p_param(k)->proba_[j].sum();
    }
  }
}


/* Compute the modalities probabilities */
template<class Array>
void Categorical_pjk<Array>::mStep()
{
  for (int k = baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    for (int j = p_data()->firstIdxCols(); j <= p_data()->lastIdxCols(); ++j)
    {
      for (int i = p_tik()->firstIdxRows(); i <= p_tik()->lastIdxRows(); ++i)
      { p_param(k)->proba_[j][(*p_data())(i, j)] += (*p_tik())(i, k);}
      // normalize the probabilities
      p_param(k)->proba_[j] /=  p_param(k)->proba_[j].sum();
    }
  }
}

/** get the parameters of the model
 *  @param params the parameters of the model
 **/
template<class Array>
void Categorical_pjk<Array>::getParameters(Array2D<Real>& params) const
{
      int nbClust = this->nbCluster();
      int nbModalities = this->modalities_.size();

      params.resize(nbModalities * nbClust, p_data()->cols());
      for (int k = 0; k < nbClust; ++k)
      {
        for (int j = p_data()->firstIdxCols(); j <= p_data()->lastIdxCols(); ++j)
        {
          for (int l = 0; l < nbModalities; ++l)
          {
            params(k * nbModalities + l + baseIdx, j) = p_param(k)->proba(j, baseIdx + l);
          }
        }
      }

//  for (int k = baseIdx; k <= p_tik()->lastIdxCols(); ++k)
//    params.pushBackRows(p_param(k)->proba_);
}

} // namespace STK

#endif /* STK_CATEGORICAL_PJK_H */
