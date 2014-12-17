/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * created on: 8 août 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file testAllGammaMixtureModels.cpp
 *  @brief In this file we test the Gamma mixture models.
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
  testFullStrategy(composer, Clust::randomClassInit_, 1, Clust::semAlgo_
                   , 1, 1, 1
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
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test GammaMixture models                          +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create DataHandler, MixtureManager, Composer      +\n");
    DataHandler handler;
    MixtureManager<DataHandler> manager(handler);
    IMixtureComposer* composer = new MixtureComposerFixedProp(n, K);
    MixtureComposerFixedProp* p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ simulate gamma_ak_bk data set                     +\n");
    simulGamma_ak_bk_Model(K, param);
    simulGamma_Mixture(n,d,param,pk,data,zi);
    // add missing values
    simulMissing(pmiss, data);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_ajk_bjk mixture                    +\n");
    idData = "IdGamma_ajk_bjk"; idModel ="Gamma_ajk_bjk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_ak_bk mixture                      +\n");
    idData = "IdGamma_ak_bk"; idModel ="Gamma_ak_bk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_ak_bjk mixture                      +\n");
    idData = "IdGamma_ak_bjk"; idModel ="Gamma_ak_bjk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_ajk_bk mixture                     +\n");
    idData = "IdGamma_ajk_bk"; idModel ="Gamma_ajk_bk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ simulate gamma_ak_b data set                      +\n");
    simulGamma_ak_b_Model(K, param);
    simulGamma_Mixture(n,d,param,pk,data,zi);
    simulMissing(pmiss, data);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_ajk_bj mixture                     +\n");
    idData = "IdGamma_ajk_bj"; idModel ="Gamma_ajk_bj";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_ajk_b mixture                      +\n");
    idData = "IdGamma_ajk_b"; idModel ="Gamma_ajk_b";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_ak_bj mixture                      +\n");
    idData = "IdGamma_ak_bj"; idModel ="Gamma_ak_bj";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_ak_b mixture                       +\n");
    idData = "IdGamma_ak_b"; idModel ="Gamma_ak_b";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ simulate gamma_a_bk data set                      +\n");
    simulGamma_a_bk_Model(K, param);
    simulGamma_Mixture(n,d,param,pk,data,zi);
    simulMissing(pmiss, data);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_aj_bjk mixture                     +\n");
    idData = "IdGamma_aj_bjk"; idModel ="Gamma_aj_bjk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_aj_bk mixture                      +\n");
    idData = "IdGamma_aj_bk"; idModel ="Gamma_aj_bk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_a_bjk mixture                      +\n");
    idData = "IdGamma_a_bjk"; idModel ="Gamma_a_bjk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ estimate Gamma_a_bk mixture                       +\n");
    idData = "IdGamma_a_bk"; idModel ="Gamma_a_bk";
    estimateModel( idData, idModel, data, handler, manager, composer, pk, param);
    p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
    p_composerFixed->releaseMixture(manager, idData);
    handler.removeData(idData);

    delete composer;
  }
  catch (Exception const& e)
  {
    stk_cout << _T("An error occur:") << e.error();
  }
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for                +\n");
  stk_cout << _T("+ GammaMixtureModels.                                 +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0;
}

