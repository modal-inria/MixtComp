/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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

 Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 */

/*
 * Project:  stkpp::
 * created on: 13 août 2011
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_GaussianModel.cpp
 *  @brief In this file we implement the GaussianModel class.
 **/

#include "../include/STK_GaussianModel.h"

#include "STatistiK/include/STK_Stat_BivariateRealReal.h"
#include "STatistiK/include/STK_Law_MultiNormal.h"
#include "STatistiK/include/STK_Stat_MultivariateReal.h"

namespace STK
{

/* constructor */
GaussianModel::GaussianModel( ArrayXX const* p_data)
                            : IGaussianModel<ArrayXX>(p_data)
                            , cov_(p_data_->cols())
{
  setNbFreeParameter(nbVariable() + (nbVariable()* (nbVariable()-1))/2);
}

/* constructor */
GaussianModel::GaussianModel( ArrayXX const& data)
                            : IGaussianModel<ArrayXX>(data)
                            , cov_(data.cols())
{
  setNbFreeParameter(nbVariable() + (nbVariable()* (nbVariable()-1))/2);
}

/* destructor */
GaussianModel::~GaussianModel()
{ if (p_law_) delete p_law_;}

/* implementation of the Gaussian statistical model
 * @return @c true if no error occur and @c false otherwise.
 */
bool GaussianModel::run()
{
  // compute the mean
  compMean();
  // compute the covariance matrix
  compCovariance();
  // create p_law_ (will be deleted in base class)
  // update gaussian law (will be deleted in base class)
  if (!p_law_) p_law_ = new Law::MultiNormal<Point>(mean_, cov_);
  else static_cast<Law::MultiNormal<Point>*>(p_law_)->setParameters(mean_, cov_);
  // compute log likelihood of the gaussian law
  setLnLikelihood(static_cast<Law::MultiNormal<Point>* >(p_law_)->lnLikelihood(*p_data_ ));
  // everything ok
  return true;
}

/* implementation of the weighted Gaussian statistical model
 * @param weights the weights of the samples
 * @return @c true if no error occur and @c false otherwise.
 */
bool GaussianModel::run(Vector const& weights)
{
  // compute the mean
  compWeightedMean(weights);
  // compute the covariance matrix
  compWeightedCovariance(weights);
  // create p_law_ (will be deleted in base class)
  // update gaussian law (will be deleted in base class)
  if (!p_law_) p_law_ = new Law::MultiNormal<Point>(mean_, cov_);
  else static_cast<Law::MultiNormal<Point>*>(p_law_)->setParameters(mean_, cov_);
  // compute log likelihood of the gaussian law
  setLnLikelihood(static_cast<Law::MultiNormal<Point>* >(p_law_)->lnLikelihood(*p_data_ ));
  // everything ok
  return true;
}

/** compute the empirical covariance matrix. */
void GaussianModel::compCovariance()
{ Stat::covariance(*p_data_,cov_);}
/** compute the empirical weighted covariance matrix.
 * @param weights the weights of the samples
 **/
void GaussianModel::compWeightedCovariance(ArrayXX::Col const& weights)
{ Stat::covariance(*p_data_, weights, cov_);}

} // namespace STK
