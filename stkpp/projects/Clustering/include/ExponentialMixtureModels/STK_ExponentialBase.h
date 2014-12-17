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

/** @file STK_ExponentialBase.h
 *  @brief In this file we implement the base class for the exponential models
 **/

#ifndef STK_EXPONENTIALBASE_H
#define STK_EXPONENTIALBASE_H

#include "../STK_IMixtureModel.h"
#include "STK_ExponentialParameters.h"

#include "../../../STatistiK/include/STK_Law_Categorical.h"

namespace STK
{

/** @ingroup Clustering
 *  Base class for the Exponential models
 **/
template<class Derived>
class ExponentialBase : public IMixtureModel<Derived >
{
  public:
    typedef IMixtureModel<Derived > Base;

    using Base::p_tik;
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;

  protected:
    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline ExponentialBase( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline ExponentialBase( ExponentialBase const& model) : Base(model) {}
    /** destructor */
    inline ~ExponentialBase() {}

  public:
    /** @return a value to impute for the jth variable of the ith sample*/
    Real impute(int i, int j) const
    {
      Real sum = 0.;
      for (int k= p_tik()->beginCols(); k < components().end(); ++k)
      { sum += p_tik()->elt(i,k) * p_param(k)->scale(j);}
      return sum;
    }
    /** @return a simulated value for the jth variable of the ith sample
     *  @param i,j indexes of the value to sample
     **/
    Real sample(int i, int j) const
    {
      int k = Law::Categorical::rand(p_tik()->row(i));
      return Law::Exponential::rand(p_param(k)->scale(j));
    }
    /** get the parameters of the model
     *  @param params the array to fill with the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const;
    /** @return the parameters of the model in an array of size (K * d). */
    void getParametersImpl() const;
    /** Write the parameters on the output stream os */
    void writeParameters(ostream& os) const;
};

/* Write the parameters on the output stream os */
template<class Derived>
void ExponentialBase<Derived>::writeParameters(ostream& os) const
{
  Array2DPoint<Real> shape(p_data()->cols()), scale(p_data()->cols());
  for (int k= baseIdx; k < components().end(); ++k)
  {
    // store shape and scale values in an array for a nice output
    for (int j= p_data()->beginCols();  j < p_data()->endCols(); ++j)
    { scale[j] = p_param(k)->scale(j);}
    os << _T("---> Component ") << k << _T("\n");
    os << _T("scale = ") << scale;
  }
}

/*get the parameters of the model*/
template<class Derived>
void ExponentialBase<Derived>::getParameters(Array2D<Real>& params) const
{
  params.resize(this->nbCluster(), p_data()->cols());
  for (int k= params.beginRows(); k < params.endRows(); ++k)
  {
    for (int j= p_data()->beginCols();  j < p_data()->endCols(); ++j)
    { params(k, j) = p_param(k)->scale(j);}
  }
}
/* get the parameters of the model in an array of size (K * d). */
template<class Derived>
void ExponentialBase<Derived>::getParametersImpl() const
{
  for (int k= paramBuffer_.beginRows(); k < paramBuffer_.endRows(); ++k)
  {
    for (int j= p_data()->beginCols();  j < p_data()->endCols(); ++j)
    { paramBuffer_(k, j) = p_param(k)->scale(j);}
  }
}


} // namespace STK

#endif /* STK_EXPONENTIALBASE_H */
