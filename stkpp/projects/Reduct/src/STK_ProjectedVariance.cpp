/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Project:  stkpp::Reduct
 * Purpose:  Implementation of the ILinearReduct interface using the
 * projected variance.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

#include "../include/STK_ProjectedVariance.h"
#include "STatistiK/include/STK_Stat_MultivariateReal.h"
#include "Algebra/include/STK_SymEigen.h"

namespace STK
{
/* default constructor */
ProjectedVariance::ProjectedVariance() : ILinearReduct()
{ ;}
/* Constructor.
 *  @param p_data a pointer on the constant data set to reduce.
 **/
ProjectedVariance::ProjectedVariance(ArrayXX const* p_data) : ILinearReduct(p_data)
{;}

/* Constructor.
 *  @param data a constatn reference on the data set to reduce.
 **/
ProjectedVariance::ProjectedVariance(ArrayXX const& data) : ILinearReduct(data)
{}
/* Copy constructor.
 * @param reductor the reductor to copy
 **/
ProjectedVariance::ProjectedVariance(ProjectedVariance const& reductor) : ILinearReduct(reductor)
{}

 /* Destructor */
ProjectedVariance::~ProjectedVariance()
{}

/* Find the axis by maximizing the Index. */
void ProjectedVariance::maximizeCriteria()
{
  if (!p_data_)
  { throw runtime_error(_T("Error in ProjectedVariance::maximizeCriteria()\nWhat: no data set."));}
  Stat::covariance(*p_data_, covariance_, true);
  computeAxis();
}

/* Find the axis by maximizing the weighed Index.
 * @param weights the weights of the samples.
 **/
void ProjectedVariance::maximizeCriteria(Vector const& weights)
{
  if (!p_data_)
  { throw runtime_error(_T("Error in ProjectedVariance::maximizeCriteria(weights)\nWhat: no data set."));}
  Stat::covariance(*p_data_, weights, covariance_, true);
  computeAxis();
}

/* compute axis and index. */
void ProjectedVariance::computeAxis()
{
  SymEigen eigen(covariance_);
  eigen.run();

  // compute the number of axis
  Range range(p_data_->beginCols(), std::min(p_data_->beginCols()+dim_-1, p_data_->lastIdxCols()), 0);
  const int begin_axis = range.begin();
  const int last_axis = range.lastIdx();
  // copy axis and index values
  axis_.resize(p_data_->cols(), range);
  idx_values_.resize(range);
  for (int j=begin_axis; j<=last_axis; j++)
  {
    axis_.col(j) = eigen.rotation().col(j);
    idx_values_[j] = eigen.eigenValues()[j];
  }
}

/* update the class if a new data set is set */
void ProjectedVariance::update()
{
  covariance_.clear();
}

} // namespace STK
