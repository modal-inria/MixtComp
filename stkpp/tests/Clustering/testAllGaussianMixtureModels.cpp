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
 * Project:  stkpp::tests
 * created on: 8 août 2011
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file testAllGaussianMixtureModels.cpp
 *  @brief In this file we test the gaussian Mixture models estimation methods.
 **/
#include "DManager.h"
#include "StrategiesUtil.h"
#include "SimulUtil.h"

using namespace STK;

int n=500, d=4, K=3;
Real pmiss = 0.25;

void estimateModel( std::string const& idData,std::string const& idModel
                  , Array2D<Real> const& data
                  , DataHandler& handler
                  , MixtureManager<DataHandler>& manager
                  , IMixtureComposer*& composer
                  , Array2DVector<Real> const& pk
                  , Array2Param const& param
                  )
{
  handler.readDataFromArray2D(data, idData, idModel);
  MixtureComposerFixedProp* p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
  p_composerFixed->createMixture(manager, idData);
  testFullStrategy(composer, Clust::randomParamInit_, 1, Clust::semAlgo_
                   , 1, 2, 1
                   , Clust::semiSemAlgo_
                   , Clust::semiSemAlgo_);
  p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
  stk_cout << _T("+ True Parameters of the model.                     +\n");
  stk_cout << "Proportions =\n" << pk;
  for (int k= param.begin(); k < param.end(); ++k)
  {
    stk_cout << _T("---> Component ") << k << _T("\n");
    stk_cout << _T("Parameters =\n")<< param[k]*Const::Point<Real>(d);
  }
}

int main(int argc, char *argv[])
{
  if (argc >=5)
  {
    n = atoi(argv[1]);
    d = atoi(argv[2]);
    K = atoi(argv[3]);
    pmiss = atof(argv[4]);
  }
  std::string idData, idModel;
  // arrays to simulate
  Array2D<Real> data(n,d);
  Array2DVector<Real> pk(n);
  Array2DVector<int> zi(n);
  // parameters
  Array2Param param(K);
  // simulate pk (uniform Dirichlet if false)
  simulZiLaw(K, pk, true);

  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test GaussianMixture models                          +\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create DataHandler, MixtureManager, FixedPropComposer+\n");
    std::string idData, idModel;
    DataHandler handler;
    MixtureManager<DataHandler> manager(handler);
    IMixtureComposer* composer = new MixtureComposerFixedProp(n, K);
    MixtureComposerFixedProp* p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);

    stk_cout << _T("\n\n");
    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ simulate Gaussian_s data set                         +\n");
    // generate model
    simulGaussian_sModel(K, param);
    // generate data
    simulGaussian_sMixture(n, d, param, pk, data, zi);
    // add missing values
    simulMissing(pmiss, data);

    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testFullStrategy: Gaussian_sjk                       +\n");
    idData = "IdGaussian_sjk"; idModel ="Gaussian_sjk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

//    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testFullStrategy: Gaussian_sk                        +\n");
//    idData = "IdGaussian_sk"; idModel ="Gaussian_sk";
//    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
//    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
//    p_composerFixed->releaseMixture(manager, idData);
//    handler.removeData(idData);
//
//    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testFullStrategy: Gaussian_sj                        +\n");
//    idData = "IdGaussian_sj"; idModel ="Gaussian_sj";
//    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
//    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
//    p_composerFixed->releaseMixture(manager, idData);
//    handler.removeData(idData);
//
//    stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testFullStrategy: Gaussian_s                         +\n");
//    idData = "IdGaussian_s"; idModel ="Gaussian_s";
//    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
//    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
//    p_composerFixed->releaseMixture(manager, idData);
//    handler.removeData(idData);

    delete composer;
  }
  catch (Exception const& e)
  {
    stk_cout << _T("An error occur:") << e.error() << _T("\n");
  }
  stk_cout << _T("\n\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for                 +\n");
  stk_cout << _T("+ GaussianMixture Models.                              +\n");
  stk_cout << _T("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0;
}

