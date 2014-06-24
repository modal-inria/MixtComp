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

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                   */

/*
 * Project:  stkpp::AAModels
 * Purpose:  implementation class for AA linear models.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DSquare.h"
#include "Arrays/include/STK_Array2DLowerTriangular.h"
#include "Arrays/include/STK_Array2DUpperTriangular.h"
#include "Arrays/include/STK_Array2D_Functors.h"

#include "Arrays/include/STK_Display.h"

#include "Algebra/include/STK_LinAlgebra2D.h"
#include "Algebra/include/STK_GramSchmidt.h"

#include "STatistiK/include/STK_Law_IUnivLaw.h"
#include "STatistiK/include/STK_Law_Normal.h"

#include "Regress/include/STK_MultidimRegression.h"

#include "../include/STK_LinearAAModel.h"

#include <cmath>        // sqrt()


namespace STK
{

/* Constructors.                                                             */
/* Default constructor : compute the Linear AA models of the matrix X
 *  using the local variance as criteria.
**/
LinearAAModel::LinearAAModel( Matrix const& data)
                            : Runner(data)
                            , GaussianAAModel(0)
                            , workData_(data)
{
  p_regressor_ = new MultidimRegression();
  setWorkData(workData_);
}

/** copy constructor.
 *  @param model the model to copy
 **/
LinearAAModel::LinearAAModel( LinearAAModel const& model)
                            : Runner(model)
                            , GaussianAAModel(model)
                            , workData_(model.workData_)
{
  p_regressor_ = new MultidimRegression();
  setWorkData(workData_);
}



/* Virtual destructor */
LinearAAModel::~LinearAAModel()
{
  delete p_regressor_;
}

/* run the probabilistic AAModel */
bool LinearAAModel::run( int const& dim)
{
  setDimension(dim);
  return run();
}


/* run the probabilistic AAModel */
bool LinearAAModel::run()
{
  // compute AAM
  try
  {
    if (!p_reductor_)
    {
      msg_error_ = STKERROR_NO_ARG(LinearAAModel::run(),reductor have not be set);
      return false;
    }
    if (!p_regressor_)
    {
      msg_error_ = STKERROR_NO_ARG(LinearAAModel::run(),regressor have not be set);
      return false;
    }
    // set p_workData to the reductor
    p_reductor_->setData(*p_workData_);
    // compute the projected data set
    reduction();
    // compute the projected covariance
    computeProjectedCovariance();
#ifdef STK_AAMODELS_VERBOSE
    stk_cout << _T("In LinearAAModel::run(), reduction done.\n");
#endif
    // set data
    p_regressor_->setY(p_workData_);
    p_regressor_->setX(p_reduced_);
    // compute the regression function
    regression();
#ifdef STK_AAMODELS_VERBOSE
    stk_cout << _T("In LinearAAModel::run(), regression done.\n");
#endif
    computeModelParameters();
    // check if data have been standardized or centered
    if (isStandardized()) { destandardizeResults();}
    else if (isCentered()){ decenterResults();}
  }
  catch (Exception const& error)
  {
    msg_error_ = _T("Error in LinearAAModel::run():\nWhat: ");
    msg_error_ += error.error();
    return false;
  }
  return true;
}

/* run the probabilistic AAModel */
bool LinearAAModel::run( Vector const& weights, int const& dim)
{
  setDimension(dim);
  return run(weights);
}
bool LinearAAModel::run( Vector const& weights)
{
  try
  {
    if (!p_reductor_)
      throw runtime_error(_T("reductor have not be set."));
    if (!p_regressor_)
      throw runtime_error(_T("regressor have not be set."));
    // set p_workData to the reductor
    p_reductor_->setData(*p_workData_);
    // compute the weighted reduced data set
    reduction(weights);
    // compute the projected covariance
    computeProjectedCovariance();
#ifdef STK_AAMODELS_VERBOSE
    stk_cout << _T("In LinearAAModel::run(weights), reduction done.\n");
#endif
    // set data
    p_regressor_->setY(p_workData_);
    p_regressor_->setX(p_reduced_);
    // compute the weighted regression vectors
    regression(weights);
#ifdef STK_VERBOSE
    stk_cout << _T("In LinearAAModel::run(weights), regression done.\n");
#endif
    computeModelParameters();
    // check if data have been standardized or centered
    if (isStandardized()) { destandardizeResults();}
    else if (isCentered()){ decenterResults();}
  }
  catch (Exception const& error)
  {
    msg_error_ = _T("Error in LinearAAModel::run(weights): ");
    msg_error_ += error.error();
    return false;
  }
  return true;
}

/* Simulate a centered auto-associative linear model of the form
 * \f[
 *    X = X.P.P' + \epsilon
 * \f]
 * with \f$ P'P = I_d \f$ and d < p.
 * @param law, the law to use in order to simulate the data.
 * @param std the standard deviation of the gaussian noise
 * @param data the data to simulate. The dimension of the container
 * give the number of the samples and variables.
 * @param proj the simulated projection matrix. The dimension of the
 * container give the dimension of the AA model.
 **/
void LinearAAModel::simul( const Law::IUnivLaw<Real>& law
                         , Vector const& mu, Real const& std
                         , Matrix& proj, Matrix& data
                         )
{
  if (data.cols() != mu.range())
    STKRUNTIME_ERROR_NO_ARG(LinearAAModel::simul,data.cols()!=mu.range());
  // simul AA model
  law.randArray(data);
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("data simulated\n");
#endif
  // choose arbitrary coefficients for proj
  proj.randGauss();
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("proj simulated\n");
#endif
  // orthonormalize proj
  gramSchmidt(proj);
#ifdef STK_AAMODELS_VERBOSE
  stk_cout << _T("proj orthonormalized\n");
#endif

#ifdef STK_AAMODELS_DEBUG
  stk_cout << _T("proj =") << proj;
#endif
  // compute data
  data = data * (proj * proj.transpose());

  // add noise to the model
  for (int j= data.firstIdxCols(); j<= data.lastIdxCols(); j++)
  {
    Law::Normal noise(mu[j], std);
    for (int i= data.firstIdxRows(); i<= data.lastIdxRows(); i++)
      data(i, j) += noise.rand();
  }
}

} // namespace STK
