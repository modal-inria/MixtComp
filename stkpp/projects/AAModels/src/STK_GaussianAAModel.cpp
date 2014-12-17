  /*--------------------------------------------------------------------*/
/*     Copyright (C) 2004  Serge Iovleff

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
 * Purpose:  Interface base class for AA models.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_GaussianAAModel.cpp
 *  @brief In this file we implement the class GaussianAAModel for the
 *  auto-associative models.
 **/

#include "../include/STK_GaussianAAModel.h"

#include "Reduct/include/STK_IReduct.h"
#include "Regress/include/STK_IRegression.h"

#include "STatistiK/include/STK_Stat_Transform.h"
#include "STatistiK/include/STK_Stat_MultivariateReal.h"

#include "STatistiK/include/STK_Law_MultiNormal.h"

#ifdef STK_AAMODELS_VERBOSE
#include "Arrays/include/STK_Display.h"
#endif

namespace STK
{

/* Constructor.
 *  @param p_workData a pointer on the data set to process
 **/
GaussianAAModel::GaussianAAModel( ArrayXX* p_workData)
                                : IAAModel(p_workData)
                                , IStatModel<ArrayXX>(p_workData)
                                , projectedCovariance_()
                                , residualCovariance_()
                                , residualVariance_(0.)
                                , projectedLnLikelihood_(0.)
                                , residualLnLikelihood_(0.)
{ }

// constructor
GaussianAAModel::GaussianAAModel( ArrayXX& workData)
                                : IAAModel(workData)
                                , IStatModel<ArrayXX>(workData)
                                , projectedCovariance_()
                                , residualCovariance_()
                                , residualVariance_(0.)
                                , projectedLnLikelihood_(0.)
                                , residualLnLikelihood_(0.)
{ }

// destructor
GaussianAAModel::~GaussianAAModel()
{}

/* update the container when the data set is modified. **/
void GaussianAAModel::setWorkData(ArrayXX& workData)
{
  // update data set and flags for the IAAModel part
  IAAModel::setWorkData(workData);
  // set dimensions to new size for the IStatModel part
  IStatModel<ArrayXX>::setData(workData);
}

/* compute the ln-likelihood of the model */
void GaussianAAModel::computeModelParameters()
{
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeModelParameters().\n");
#endif
  // compute the number of free parameters
  computeNbFreeParameters();
  // compute the covariance matrix of the residual and the rsidual variance
  computeResidualCovariance();
  // compute projected lnLikelihood
  computeProjectedLnLikelihood();
  // compute projected lnLikelihood
  computeResidualLnLikelihood();
  // compute complete nLikelihood
  setLnLikelihood(projectedLnLikelihood_ + residualLnLikelihood_);

#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeModelParameters() done.\n");
#endif
}


/* @brief compute the number of free parameter of the model. **/
void GaussianAAModel::computeNbFreeParameters()
{
  // get number of free parameters
  setNbFreeParameter(p_regressor_->nbFreeParameter() + dim() * (dim()+1)/2 + 1);
}

/* compute the ln-likelihood of the model */
void GaussianAAModel::computeProjectedLnLikelihood()
{
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeProjectedLnLikelihood().\n");
#endif

  // range of the column to use
  Range cols = Range(p_reduced_->beginCols(), dim());

  // create a reference with the first columns of the reduced data
  ArrayXX reducedData(*p_reduced_, p_reduced_->rows(), cols);
  // create a reference with the first columns of the reduced data
  ArraySquareX reducedCovariance(projectedCovariance(), cols);

  // compute first part of the ln-likelihood
  Point mean(cols, 0.);

  Law::MultiNormal<Point> normalP(mean, reducedCovariance);
  projectedLnLikelihood_ = normalP.lnLikelihood(reducedData);

#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeProjectedLnLikelihood() done.\n");
  stk_cout << _T("projectedLnLikelihood_ = ") << projectedLnLikelihood_ << _T("\n");
#endif
}

/* compute the ln-likelihood of the model */
void GaussianAAModel::computeResidualLnLikelihood()
{
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeResidualsLnLikelihood().\n");
#endif
  // compute constant part and determinant part of the log-likelihood
  residualLnLikelihood_ = ( Const::_LNSQRT2PI_ + 0.5*std::log(residualVariance_ ))
                          * (dim() - nbVariable()) * nbSample();
  // compute second part of the log-likelihood
  const int firstSample = p_residuals_->beginRows(), endSample= p_residuals_->endRows();
  for (int i=firstSample; i<endSample; i++)
  { residualLnLikelihood_ -= p_residuals_->row(i).norm2()/(2.*residualVariance_);}

#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeResidualsLnLikelihood() done.\n");
  stk_cout << _T("residualLnLikelihood_ = ") << residualLnLikelihood_ << _T("\n");
#endif
}

/* compute the covariance matrix of the projected data set. */
void GaussianAAModel::computeProjectedCovariance()
{
  if (!p_reduced_)
    throw runtime_error(_T("Error in GaussianAAModel::computeProjectedCovariance(): "
                           "projected data have not been computed."));
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeProjectedCovariance().\n");
#endif
  Stat::covariance(*p_reduced_, projectedCovariance_);
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeProjectedCovariance() done.\n");
#endif
}
/* compute the covariance matrix of the residuals. */
void GaussianAAModel::computeResidualCovariance()
{
  if (!p_residuals_)
    throw runtime_error(_T("Error in IAAModel::computeResidualCovariance(): "
                           "residuals have not been computed."));
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("in GaussianAAModel::computeResidualCovariance().\n");
#endif
  Stat::covariance(*p_residuals_, residualCovariance_);
  residualVariance_ = (residualCovariance_.trace())/Real(nbVariable()-dim());
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("GaussianAAModel::computeResidualCovariance() done.\n");
  stk_cout << _T("residualVariance_ = ") << residualVariance_ << _T("\n");
#endif
}


} // namespace STK

