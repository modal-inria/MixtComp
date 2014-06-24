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

/** @file testGammaMixtureModels.cpp
 *  @brief In this file we test the Gamma mixture models estimation methods.
 **/
#include "StrategiesUtil.h"
#include "SimulUtil.h"

#include "DManager.h"


using namespace STK;

int main(int argc, char *argv[])
{
  int n=500, d=4, K=3;
  Real pmiss = 0.1;

  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test GammaMixture models                          +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    Array2D<Real> dataij;
    Array2DVector<Real> pk;
    Array2DVector<int> zi;
    Array_bjParam param;

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Simulate data and fill DataHandler                +\n");

    // simulate pk (uniform Dirichlet if false)
    simulZiLaw(K, pk, true);
    // generate model
    simulGamma_bjModel(d, K, param);
    // generate data
    simulGamma_bjMixture(n, d, param, pk, dataij, zi);
    // add missing values
    simulMissing(pmiss, dataij);

    DataHandler handler;
    std::string idMixture("Gamma_ajk_bjk");
    std::string idModel("Model_ajk_bjk");

    handler.readDataFromArray2D(dataij, idMixture, idModel);
    handler.writeInfo(std::cout);
    MixtureManager<DataHandler> manager(handler);

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create composers and setDataHandler               +\n");
    IMixtureComposerBase* composer_bjk = new MixtureComposer(handler.nbSample(), handler.nbVariable(),K);
    IMixtureComposerBase* composer_bj = new MixtureComposer(handler.nbSample(), handler.nbVariable(),K);
    IMixtureComposerBase* composerFixed_bjk = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),K);
    IMixtureComposerBase* composerFixed_bj = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),K);

    MixtureComposer* p_composer_bjk = static_cast<MixtureComposer*>(composer_bjk);
    MixtureComposer* p_composer_bj = static_cast<MixtureComposer*>(composer_bj);
    MixtureComposerFixedProp* p_composerFixed_bjk = static_cast<MixtureComposerFixedProp*>(composerFixed_bjk);
    MixtureComposerFixedProp* p_composerFixed_bj = static_cast<MixtureComposerFixedProp*>(composerFixed_bj);


    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ CreateMixtures                                    +\n");
    p_composer_bjk->createMixture(manager,Clust::Gamma_ajk_bjk_, _T("Model_ajk_bjk"));
    p_composer_bj->createMixture(manager, Clust::Gamma_ajk_bj_, _T("Model_ajk_bjk"));
    p_composerFixed_bjk->createMixture(manager, Clust::Gamma_ajk_bjk_, _T("Model_ajk_bjk"));
    p_composerFixed_bj->createMixture(manager, Clust::Gamma_ajk_bj_, _T("Model_ajk_bjk"));

//
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSimpleStrategy: composer_bjk                     +\n");
//    testSimpleStrategy((composer_bjk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSimpleStrategy: composer_bj                     +\n");
//    testSimpleStrategy((composer_bj));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSimpleStrategy: composerFixed_bjk                +\n");
//    testSimpleStrategy((composerFixed_bjk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSimpleStrategy: composerFixed_bj                +\n");
//    testSimpleStrategy((composerFixed_bj));

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_bjk                        +\n");
    testXemStrategy((composer_bjk));
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_bj                        +\n");
    testXemStrategy((composer_bj));
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composerFixed_bjk                   +\n");
    testXemStrategy((composerFixed_bjk));
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composerFixed_bj                   +\n");
    testXemStrategy((composerFixed_bj));

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ True Parameters of the model.                     +\n");
    stk_cout << "Proportions =\n" << pk;

    for (int k= param.firstIdx(); k <= param.lastIdx(); ++k)
    {
      stk_cout << _T("---> Component ") << k << _T("\n");
      stk_cout << _T("Parameters =\n")<< param[k]*Const::Point<Real>(d);

    }
    delete composer_bjk;
    delete composer_bj;
    delete composerFixed_bjk;
    delete composerFixed_bj;
  }
  catch (Exception const& e)
  {
    stk_cout << _T("An error occur:") << e.error();
  }
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for              +\n");
  stk_cout << _T("+ GammaMixture Models.                           +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0;
}

