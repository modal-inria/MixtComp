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
struct MixtureModelTraits< Categorical_pk<_Array> >
{
  typedef _Array Array;
  typedef Categorical_pkParameters Parameters;
  typedef MixtureComponent<_Array, Parameters> Component;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Categorical mixture model @c Categorical_pk is
 *  the most general diagonal Categorical model and have a density function of the
 *  form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *   .
 * \f]
 **/
template<class Array>
class Categorical_pk : public CategoricalBase<Categorical_pk<Array> >
{
  public:
    typedef CategoricalBase<Categorical_pk<Array> > Base;
    typedef typename Clust::MixtureModelTraits< Categorical_pk<Array> >::Component Component;
    typedef typename Clust::MixtureModelTraits< Categorical_pk<Array> >::Parameters Parameters;
    typedef typename Array::Col ColVector;

    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

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
    void initializeStep();
    /** Initialize randomly the parameters of the Categorical mixture.
     *  Probabilities will be choosen uniformly.
     */
    void randomInit();
    /** Compute the weighted proportions of each class. */
    void mStep();
    /** get the parameters of the model
     *  @param params An Array2D with the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const;
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*(this->modalities_.size()-1);}
};

/** Initialize the parameters using mStep. */
template<class Array>
void Categorical_pk<Array>::initializeStep()
{}

/* Initialize randomly the parameters of the Categorical mixture. The centers
 *  will be selected randomly among the data set and the standard-deviation
 *  will be set to 1.
 */
template<class Array>
void Categorical_pk<Array>::randomInit()
{
  for (int k = baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    p_param(k)->proba_.randUnif();
    p_param(k)->proba_ /= p_param(k)->proba_.sum();
  }
}

/* Compute the weighted mean and the common variance. */
template<class Array>
void Categorical_pk<Array>::mStep()
{
  for (int k = baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    for (int j = p_data()->firstIdxCols(); j <= p_data()->lastIdxCols(); ++j)
    {
      for (int i = p_tik()->firstIdxRows(); i <= p_tik()->lastIdxRows(); ++i)
      {
        p_param(k)->proba_[(*p_data())(i, j)] += (*p_tik())(i, k);
      }
    }
    p_param(k)->proba_ /=  p_param(k)->proba_.sum();
  }
}

/** get the parameters of the model
 *  @param params the parameters of the model
 **/
template<class Array>
void Categorical_pk<Array>::getParameters(Array2D<Real>& params) const
{
  for (int k = baseIdx; k <= p_tik()->lastIdxCols(); ++k)
    params.pushBackRows(p_param(k)->proba_*Const::Point<Real>(this->nbCluster()));
//  int firstId = params.firstIdxRows();
//  int nbClust = this->nbCluster();
//  int nbModalities = modalities_.size();
//
//  params.resize(nbModalities * nbClust, p_data()->cols());
//  for (int k = 0; k <= nbClust; ++k)
//  {
//    for (int j = p_data()->firstIdxCols(); j <= p_data()->lastIdxCols(); ++j)
//    {
//      for (int l = 0; l < nbModalities; ++l)
//      {
//        params(k * nbModalities + l + firstId, j) = p_param(k)->proba(j, l);
//      }
//    }
//  }
}

} // namespace STK

#endif /* STK_CATEGORICAL_PK_H */
