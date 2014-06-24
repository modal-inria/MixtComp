/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * Project:  stkpp::AAModels
 * created on: 23 déc. 2010
 * Purpose:  implement the Interface base class .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IAAModel.cpp
 *  @brief In this file we implement the interface class IAAModel.
 **/

#include "../include/STK_IAAModel.h"

#include "Arrays/include/STK_Array2DSquare.h"

#include "Algebra/include/STK_LinAlgebra2D.h"

#include "STatistiK/include/STK_Stat_Transform.h"
#include "STatistiK/include/STK_Stat_MultivariateReal.h"

#include "Reduct/include/STK_IReduct.h"
#include "Regress/include/STK_IRegression.h"

#ifdef STK_AAMODELS_VERBOSE
#include "Arrays/include/STK_Display.h"
#endif

namespace STK
{

/* Constructor.
 *  @param p_workData A pointer on the the working data set
 **/
IAAModel::IAAModel( Matrix* p_workData)
                  : p_regressor_(0)
                  , p_reductor_(0)
                  , p_workData_(p_workData)
                  , p_reduced_(0)
                  , p_predicted_(0)
                  , p_residuals_(0)
                  , dim_(0)
                  , mean_()
                  , std_()
                  , isCentered_(false)
                  , isStandardized_(false)
{}

IAAModel::IAAModel( Matrix& workData)
                  : p_regressor_(0)
                  , p_reductor_(0)
                  , p_workData_(&workData)
                  , p_reduced_(0)
                  , p_predicted_(0)
                  , p_residuals_(0)
                  , dim_(0)
                  , mean_()
                  , std_()
                  , isCentered_(false)
                  , isStandardized_(false)
{ }

/* destructor */
IAAModel::~IAAModel()
{ }

/* set the dimension of the model
 */
void IAAModel::setDimension( int const& dim) { dim_ = dim;}

/* set the working set with the Data to treat.
 * @param p_reductor a pointer on the reduction dimension method to use
 */
void IAAModel::setWorkData( Matrix& workData)
{
  p_workData_ = &workData;
  isCentered_     = false;
  isStandardized_ = false;
}
/* set the reduction dimension method.
 * @param p_reductor a pointer on the reduction dimension method to use
 */
void IAAModel::setReductor( IReduct* p_reductor)
{ p_reductor_ = p_reductor;}
/* set the regression method.
 * @param p_regressor a pointer on the regresssion method to use
 */
void IAAModel::setRegressor( IRegression<Matrix, Matrix, Vector>* p_regressor)
{ p_regressor_ = p_regressor;}

/** delete the reductor allocated set to this model. */
void IAAModel::freeReductor()
{
  if (p_reductor_) delete p_reductor_;
  p_reductor_ = 0;
}
/** delete the regressor allocated to this model.*/
void IAAModel::freeRegressor()
{
  if (p_regressor_) delete p_regressor_;
  p_regressor_ = 0;
}

/* standardize the local data set */
void IAAModel::center()
{
#ifdef STK_AAMODELS_DEBUG
  if (!p_workData_)
    throw runtime_error(_T("Error in IAAModel::center(): "
                           "work data is not initialized."));
#endif
  // we have to decenter in case they have been centered with weights
  if (isCentered_)
  {
    Stat::decenter(*p_workData_, mean_);
    isCentered_ = false;
  }
  Stat::center(*p_workData_, mean_);
  isCentered_ = true;
}

/* center the local data set */
void IAAModel::center(Vector const& weights)
{
  // we have to decenter in case of this is not the same weights
  if (isCentered_)
  {
    Stat::decenter(*p_workData_, mean_);
    isCentered_ = false;
  }
  // center
  Stat::center(*p_workData_, weights, mean_);
  isCentered_ = true;
}

/* standardize the local data set */
void IAAModel::standardize()
{
  // we have to destandardize in case they have been standardized with weights
  if (isStandardized_)
  {
    Stat::destandardize(*p_workData_, mean_, std_);
    isStandardized_ = false;
  }
  Stat::standardize(*p_workData_, mean_, std_);
  isStandardized_ = true;
}

/* standardize the local data set */
void IAAModel::standardize(Vector const& weights)
{
#ifdef STK_AAMODELS_DEBUG
  if (!p_workData_)
    throw runtime_error(_T("Error in IAAModel::standardize(weights): "
                            "work data is not initialized."));
#endif

  // we have to destandardize in case of this is not the same weights
  if (isStandardized_)
  {
    Stat::destandardize(*p_workData_, mean_, std_);
    isStandardized_ = false;
  }
  // standardize
  Stat::standardize(*p_workData_, weights, mean_, std_);
  isStandardized_ = true;
}


/* compute the dimension reduction **/
void IAAModel::reduction()
{
  if (!p_reductor_)
    throw runtime_error(_T("Error in IAAModel::reduction(): "
                           "reductor have not be set."));
  // compute axis
  p_reductor_->setDimension(dim_);
  p_reductor_->run();
  // compute matrix multiplication
  p_reduced_= p_reductor_->p_reduced();
}

/* compute the weighted dimension reduction **/
void IAAModel::reduction( Vector const& weights)
{
  if (!p_reductor_)
    throw runtime_error(_T("Error in IAAModel::reduction(weigths): "
                           "reductor have not be set."));
  // compute axis
  p_reductor_->setDimension(dim_);
  p_reductor_->run(weights);
  // get the reduced data set
  p_reduced_= p_reductor_->p_reduced();
}

/* compute the regression **/
void IAAModel::regression()
{
  if (!p_regressor_)
    throw runtime_error(_T("Error in IAAModel::regression(): "
                           "regressor have not be set."));
  // compute regression
  p_regressor_->run();
  // get results
  p_predicted_ = p_regressor_->p_predicted();
  p_residuals_ = p_regressor_->p_residuals();
}
/* compute the weighted regression **/
void IAAModel::regression( Vector const& weights)
{
  if (!p_regressor_)
    throw runtime_error(_T("Error in IAAModel::regression(weights): "
                           "regressor have not be set."));
  p_regressor_->run(weights);
  // get results
  p_predicted_ = p_regressor_->p_predicted();
  p_residuals_ = p_regressor_->p_residuals();
}

/* destandardize the predicted result and residuals */
void IAAModel::decenterResults()
{
  if (!p_predicted_)
    throw runtime_error(_T("Error in IAAModel::decenterResults(): "
                           "predictions have not been computed."));
  Stat::decenter(*p_predicted_, mean_);
}
/* destandardize the predicted result and residuals */
void IAAModel::destandardizeResults()
{
  if (!p_predicted_)
    throw runtime_error(_T("Error in IAAModel::destandardizeResults(): "
                           "predictions have not been computed."));
  if (!p_residuals_)
    throw runtime_error(_T("Error in IAAModel::destandardizeResults(): "
                           "residuals have not been computed."));
  Stat::destandardize(*p_predicted_, mean_, std_);
  Stat::destandardize(*p_residuals_, std_);
}


} // namespace STK
