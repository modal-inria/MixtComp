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

/** @file testGaussianMixtureModels.cpp
 *  @brief In this file we test the gaussian Mixture models estimation methods.
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
    stk_cout << _T("+ Test GaussianMixture models                       +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    Array2D<Real> dataij;
    Array2DVector<Real> pk;
    Array2DVector<int> zi;
    Array_sParam param;

    // simulate pk (uniform Dirichlet if false)
    simulZiLaw(K, pk, true);
    // generate model
    simulGaussian_sModel(d, K, param);
    // generate data
    simulGaussian_sMixture(n, d, param, pk, dataij, zi);
    // add missing values
    simulMissing(pmiss, dataij);

    DataHandler handler;
    std::string idMixture("Gaussian_s");
    std::string idModel("Model_s");

    handler.readDataFromArray2D(dataij, idMixture, idModel);
    handler.writeInfo(std::cout);
    MixtureManager<DataHandler> manager(handler);

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create composers and setDataHandler               +\n");
    IMixtureComposerBase* composer_sjk = new MixtureComposer(handler.nbSample(), handler.nbVariable(),K); // Gaussian_sjk
    IMixtureComposerBase* composer_s   = new MixtureComposer(handler.nbSample(), handler.nbVariable(),K); // Gaussian_s
    IMixtureComposerBase* composer_sj  = new MixtureComposer(handler.nbSample(), handler.nbVariable(),K); // Gaussian_sj
    IMixtureComposerBase* composer_sk  = new MixtureComposer(handler.nbSample(), handler.nbVariable(),K); // Gaussian_sk

    IMixtureComposerBase* composerFixed_sjk = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),K);
    IMixtureComposerBase* composerFixed_s   = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),K);
    IMixtureComposerBase* composerFixed_sj  = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),K);
    IMixtureComposerBase* composerFixed_sk  = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),K);

    MixtureComposer* p_composer_sjk = static_cast<MixtureComposer*>(composer_sjk);
    MixtureComposer* p_composer_s   = static_cast<MixtureComposer*>(composer_s);
    MixtureComposer* p_composer_sj  = static_cast<MixtureComposer*>(composer_sj);
    MixtureComposer* p_composer_sk  = static_cast<MixtureComposer*>(composer_sk);

    MixtureComposerFixedProp* p_composerFixed_sjk = static_cast<MixtureComposerFixedProp*>(composerFixed_sjk);
    MixtureComposerFixedProp* p_composerFixed_s   = static_cast<MixtureComposerFixedProp*>(composerFixed_s);
    MixtureComposerFixedProp* p_composerFixed_sj  = static_cast<MixtureComposerFixedProp*>(composerFixed_sj);
    MixtureComposerFixedProp* p_composerFixed_sk  = static_cast<MixtureComposerFixedProp*>(composerFixed_sk);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ CreateMixtures                                    +\n");
    p_composer_sjk->createMixture(manager, Clust::Gaussian_sjk_, _T("Model_s"));
    p_composer_s->createMixture(manager, Clust::Gaussian_s_, _T("Model_s"));
    p_composer_sj->createMixture(manager, Clust::Gaussian_sj_, _T("Model_s"));
    p_composer_sk->createMixture(manager, Clust::Gaussian_sk_, _T("Model_s"));

    p_composerFixed_sjk->createMixture(manager, Clust::Gaussian_sjk_, _T("Model_s"));
    p_composerFixed_s->createMixture(manager, Clust::Gaussian_s_, _T("Model_s"));
    p_composerFixed_sj->createMixture(manager, Clust::Gaussian_sj_, _T("Model_s"));
    p_composerFixed_sk->createMixture(manager, Clust::Gaussian_sk_, _T("Model_s"));

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_sjk                     +\n");
    testXemStrategy(composer_sjk);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_s                       +\n");
    testXemStrategy(composer_s);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_sk                      +\n");
    testXemStrategy(composer_sk);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_sj                      +\n");
    testXemStrategy(composer_sj);

//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testXemStrategy: p_composerFixed_sk               +\n");
//    testXemStrategy((p_composerFixed_sk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testXemStrategy: p_composerFixed_s                +\n");
//    testSemStrategy((p_composerFixed_s));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testXemStrategy: p_composerFixed_sk               +\n");
//    testXemStrategy((p_composerFixed_sk));

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ True Parameters of the model.                     +\n");
    stk_cout << "Proportions =\n" << pk;

    for (int k= param.firstIdx(); k <= param.lastIdx(); ++k)
    {
      stk_cout << _T("---> Component ") << k << _T("\n");
      stk_cout << _T("Parameters =\n")<< param[k]*Const::Point<Real>(d);

    }
    delete composer_sjk;
    delete composer_s;
    delete composer_sj;
    delete composer_sk;

    delete composerFixed_sjk;
    delete composerFixed_s;
    delete composerFixed_sj;
    delete composerFixed_sk;
  }
  catch (Exception const& e)
  {
    stk_cout << _T("An error occur:") << e.error();
  }
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for              +\n");
  stk_cout << _T("+ GaussianMixture Models.                           +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0;
}

