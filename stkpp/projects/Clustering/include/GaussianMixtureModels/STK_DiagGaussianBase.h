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

/** @file STK_DiagGaussianBase.h
 *  @brief In this file we implement the base class for the Gaussian diagonal models
 **/

#ifndef STK_DIAGGAUSSIANBASE_H
#define STK_DIAGGAUSSIANBASE_H

#include "../STK_IMixtureModel.h"
#include "STK_DiagGaussianParameters.h"

#include "../../../STatistiK/include/STK_Law_Categorical.h"
#include "../../../STatistiK/include/STK_Law_Uniform.h"

namespace STK
{


/** @ingroup Clustering
 *  Base class for the diagonal Gaussian models
 **/
template<class Derived>
class DiagGaussianBase : public IMixtureModel<Derived >
{
  public:
    typedef IMixtureModel<Derived > Base;
    typedef Array2D<Real>::Col ColVector;

    using Base::p_tik;
    using Base::p_data;
    using Base::p_param;
    using Base::components;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    inline DiagGaussianBase( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    inline DiagGaussianBase( DiagGaussianBase const& model) : Base(model) {}
    /** destructor */
    inline ~DiagGaussianBase() {}
    /** @return an imputation value for the jth variable of the ith sample
     *  @param i,j indexes of the data to impute */
    Real impute(int i, int j) const
    {
      Real sum = 0.;
      for (int k= baseIdx; k <= p_tik()->lastIdxCols(); ++k)
      { sum += p_tik()->elt(i,k) * p_param(k)->mean(j);}
      return sum;
    }
    /** @return a simulated value for the jth variable of the ith sample
     * @param i,j indexes of the data to simulate*/
    Real sample(int i, int j) const
    {
      int k = Law::Categorical::rand(p_tik()->row(i));
      return Law::Normal::rand(p_param(k)->mean(j), p_param(k)->sigma(j));
    }
    /** get the parameters of the model
     *  @param params the parameters of the model
     **/
    void getParameters(Array2D<Real>& params) const
    {
      int nbClust = this->nbCluster();
      params.resize(2*nbClust, p_data()->cols());

      for (int k= 0; k < nbClust; ++k)
      {
        for (int j=  p_data()->firstIdxCols();  j <= p_data()->lastIdxCols(); ++j)
        {
          params(2*k+  baseIdx, j) = p_param(k+baseIdx)->mean(j);
          params(2*k+1+baseIdx, j) = p_param(k+baseIdx)->sigma(j);
        }
      }
    }
    /** Write the parameters on the output stream os */
    void writeParameters(ostream& os) const
    {
      Array2DPoint<Real> sigma(p_data()->cols());
      for (int k= baseIdx; k <= components().lastIdx(); ++k)
      {
        // store sigma values in an array for a nice output
        for (int j= sigma.firstIdx();  j <= sigma.lastIdx(); ++j)
        { sigma[j] = p_param(k)->sigma(j);}
        stk_cout << _T("---> Component ") << k << _T("\n");
        stk_cout << _T("mean = ") << p_param(k)->mean_;
        stk_cout << _T("sigma = ")<< sigma;
      }
    }

  protected:
    /** sample randomly the mean of each component by sampling randomly a row
     *  of the data set.
     **/
    void randomMean();
    /** compute the initial weighted mean of a Gaussian mixture. */
    void initialMean();
    /** compute the weighted mean of a Gaussian mixture. */
    void updateMean();
};

template<class Derived>
void DiagGaussianBase<Derived>::randomMean()
{
  for (int k= baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    // random number in [0, size[
    int i = p_data()->firstIdxRows() + std::floor(Law::Uniform::rand(0.,1.)*this->nbSample());
    p_param(k)->mean_.copy(p_data()->row(i));
  }
}

template<class Derived>
void DiagGaussianBase<Derived>::initialMean()
{
  for (int k= baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    ColVector tik(p_tik()->col(k), true); // create a reference
    p_param(k)->mean_ = Stat::mean(*p_data(), p_tik()->col(k));
#ifdef MC_DEBUG
    std::cout << "DiagGaussianBase<Derived>::initialMean()" << std::endl;
    std::cout << "\tk: " << k << std::endl;
    std::cout << "\tp_param(k)->mean_.nbAvailableValues(): " << p_param(k)->mean_.nbAvailableValues() << std::endl;
    std::cout << "\tp_param(k)->mean_.size(): " << p_param(k)->mean_.size() << std::endl;
#endif
    if (p_param(k)->mean_.nbAvailableValues() != p_param(k)->mean_.size()) throw Clust::initializeStepFail_;
  }
}

template<class Derived>
void DiagGaussianBase<Derived>::updateMean()
{
  for (int k= baseIdx; k <= p_tik()->lastIdxCols(); ++k)
  {
    ColVector tik(p_tik()->col(k), true); // create a reference
    p_param(k)->mean_ = Stat::mean(*p_data(), tik);
    if (p_param(k)->mean_.nbAvailableValues() != p_param(k)->mean_.size()) throw Clust::mStepFail_;
  }
}
} // namespace STK

#endif /* STK_DIAGGAUSSIANBASE_H */
