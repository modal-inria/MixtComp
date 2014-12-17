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

/** @file STK_GammaBase.h
 *  @brief In this file we implement the base class for the gamma models
 **/

#ifndef STK_GAMMABASE_H
#define STK_GAMMABASE_H

#include "../STK_IMixtureModel.h"
#include "STK_GammaParameters.h"

#include "../../../STatistiK/include/STK_Law_Categorical.h"
#include "../../../STatistiK/include/STK_Stat_Functors.h"

#include "../../../Analysis/include/STK_Algo_FindZero.h"
#include "../../../Analysis/include/STK_Funct_raw.h"
#include "../../../Analysis/include/STK_Funct_gamma.h"

namespace STK
{

namespace hidden
{
/** @ingroup hidden
 *  Functor computing the derivative of the lnLikelihood of a gamma_ajk_bjk model */
class invPsiMLog : public IFunction<invPsiMLog >
{
  public:
    inline invPsiMLog( Real y): y_(y)  {}
    /** @return the value of the function at a
     * @param a a positive real value
     **/
    inline Real fImpl(Real a) const
    { return (y_ + std::log(a) - Funct::psi_raw(a));}
    /** @return the minimal value of the function at x */
    inline Real xminImpl() const { return 0;}

  private:
    Real y_;
};

/** @ingroup hidden
 *  Functor computing the difference between the psi function  and a fixed value
 **/
class invPsi : public IFunction<invPsi >
{
  public:
    /** initialize y_ */
    inline invPsi( Real y): y_(y) {}
    /** @return the value of the function at a
     *  @param x a positive real value
     **/
    inline Real fImpl(Real x) const { return (y_ - Funct::psi_raw(x));}
    /** @return the minimal value of the function at x */
    inline Real xminImpl() const { return 0;}
  private:
    Real y_;
};

} // namespace hidden


/** @ingroup Clustering
 *  Base class for the gamma models
 **/
template<class Derived>
class GammaBase : public IMixtureModel<Derived >
{
  public:
    typedef IMixtureModel<Derived > Base;
    typedef ArrayXX::Col ColVector;

    using Base::p_tik;
    using Base::components;
    using Base::p_data;
    using Base::p_param;
    using Base::paramBuffer_;

  protected:
    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline GammaBase( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline GammaBase( GammaBase const& model) : Base(model) {}
    /** destructor */
    inline ~GammaBase() {}

  public:
    /** @return a value to impute for the jth variable of the ith sample*/
    Real impute(int i, int j) const
    {
      Real sum = 0.;
      for (int k= p_tik()->beginCols(); k < components().end(); ++k)
      { sum += p_tik()->elt(i,k) * p_param(k)->shape(j) * p_param(k)->scale(j);}
      return sum;
    }
    /** @return a simulated value for the jth variable of the ith sample
     *  @param i,j indexes of the value to sample
     **/
    Real sample(int i, int j) const
    {
      int k = Law::Categorical::rand(p_tik()->row(i));
      return Law::Gamma::rand(p_param(k)->shape(j), p_param(k)->scale(j));
    }
    /** get the parameters of the model
     *  @param params the array to fill with the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const;
    /** @return the parameters of the model in an array of size (K * 2d). */
    ArrayXX getParametersImpl() const;
    /** Write the parameters on the output stream os */
    void writeParameters(ostream& os) const;

  protected:
    /** compute the Q(theta) value. */
    Real qValue() const;
    /** compute the weighted moments of a gamma mixture. */
    bool moments();
    /** get the weighted mean of the jth variable of the kth cluster. */
    inline Real meanjk( int j, int k) { return p_param(k)->mean_[j];}
    /** get the weighted variance of the jth variable of the kth cluster. */
    inline Real variancejk( int j, int k) { return p_param(k)->variance_[j];}
    /** get the weighted mean of the jth variable of the kth cluster. */
    inline Real meank( int k) { return p_param(k)->mean_.mean();}
    /** get the weighted variance of the jth variable of the kth cluster. */
    inline Real variancek( int k) { return p_param(k)->variance_.mean();}
};

/*get the parameters of the model*/
template<class Derived>
void GammaBase<Derived>::getParameters(Array2D<Real>& params) const
{
  int nbClust = this->nbCluster();
  params.resize(2*nbClust, p_data()->cols());
  for (int k= 0; k < nbClust; ++k)
  {
    for (int j= p_data()->beginCols();  j < p_data()->endCols(); ++j)
    {
      params(2*k+  baseIdx, j) = p_param(baseIdx+k)->shape(j);
      params(baseIdx+2*k+1, j) = p_param(baseIdx+k)->scale(j);
    }
  }
}
/* get the parameters of the model in an array of size (K * 2d). */
template<class Derived>
ArrayXX GammaBase<Derived>::getParametersImpl() const
{
  ArrayXX params;
  int nbClust = this->nbCluster();
  params.resize(2*nbClust, p_data()->cols());
  for (int k= 0; k < nbClust; ++k)
  {
    for (int j= p_data()->beginCols();  j < p_data()->endCols(); ++j)
    {
      params(2*k+  baseIdx, j) = p_param(baseIdx+k)->shape(j);
      params(baseIdx+2*k+1, j) = p_param(baseIdx+k)->scale(j);
    }
  }
  return params;
}

/* Write the parameters on the output stream os */
template<class Derived>
void GammaBase<Derived>::writeParameters(ostream& os) const
{
    Array2DPoint<Real> shape(p_data()->cols()), scale(p_data()->cols());
    for (int k= baseIdx; k < components().end(); ++k)
    {
      // store shape and scale values in an array for a nice output
      for (int j= p_data()->beginCols();  j < p_data()->endCols(); ++j)
      {
        shape[j] = p_param(k)->shape(j);
        scale[j] = p_param(k)->scale(j);
      }
      os << _T("---> Component ") << k << _T("\n");
      os << _T("shape = ") << shape;
      os << _T("scale = ") << scale;
    }
}

/* compute safely the weighted moments of a gamma law. */
template<class Derived>
bool GammaBase<Derived>::moments()
{
  for (int k= p_tik()->beginCols(); k < components().end(); ++k)
  {
    ColVector tik(p_tik()->col(k), true); // create a reference
    p_param(k)->tk_ = tik.sum();
    for (int j=p_data()->beginCols(); j<p_data()->endCols(); ++j)
    {
      // mean
      Real mean =  p_data()->col(j).wmean(tik);
      if ( (mean<=0) || Arithmetic<Real>::isNA(mean) ) { return false;}
      p_param(k)->mean_[j] = mean;
      // mean log
      Real meanLog =  p_data()->col(j).log().wmean(tik);
      if (Arithmetic<Real>::isNA(meanLog)) { return false;}
      p_param(k)->meanLog_[j] = meanLog;
      // variance
      Real variance =  p_data()->col(j).wvariance(mean, tik);
      if ((variance<=0)||Arithmetic<Real>::isNA(variance)){ return false;}
      p_param(k)->variance_[j] = variance;
    }
  }
  return true;
}

/*get the parameters of the model*/
template<class Derived>
Real GammaBase<Derived>::qValue() const
{
  Real value = 0.;
  for (int k= p_tik()->beginCols(); k < components().end(); ++k)
  {
    Real sumjk=0.0;
    for (int j=p_data()->beginCols(); j<p_data()->endCols(); ++j)
    {
      Real shape = p_param(k)->shape(j), scale = p_param(k)->scale(j);
      sumjk += shape * (p_param(k)->meanLog_[j] -std::log(scale))
              - p_param(k)->mean_[j]/scale - STK::Funct::gammaLn(shape);
    }
    value += p_param(k)->tk_ * sumjk;
  }
  return value;
}

} // namespace STK

#endif /* STK_GAMMABASE_H */
