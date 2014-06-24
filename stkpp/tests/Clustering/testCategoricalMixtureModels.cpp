/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014  Serge Iovleff

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
 * created on: 3 mai 2014
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file testCategoricalMixtureModels.cpp
 *  @brief In this file we test the Categorical mixture models estimation methods.
 **/

#include "StrategiesUtil.h"
#include "SimulUtil.h"

#include "DManager.h"


using namespace STK;


int main(int argc, char *argv[])
{
  int n=500, d=4, K=3, L=5;
  Real pmiss = 0.1;

  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test CategoricalMixture models                    +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Simulate data and fill DataHandler                +\n");

    Array2D<int> dataij;
    Array2DVector<Real> pk;
    Array2DVector<int> zi;
    Array_pkProba proba;
    // simulate pk (uniform Dirichlet if false)
    simulZiLaw(K, pk, true);
    // generate model
    simulCategorical_pkModel(d, K, L, proba);
    // generate data
    simulCategorical_pkMixture(n, d, proba, pk, dataij, zi);
    // add missing values
    simulMissing(pmiss, dataij);

    // add data to datahandler
    std::string idMixture("Categorical_pjk");
    std::string idModel("model_pjk");
    DataHandler handler;
    handler.readDataFromArray2D(dataij, idMixture, idModel);
    handler.writeInfo(std::cout);
    MixtureManager<DataHandler> manager(handler);

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create composers and setDataHandler               +\n");
    IMixtureComposerBase* composer_pjk = new MixtureComposer(handler.nbSample(), handler.nbVariable(),K); // Categorical_pjk
    IMixtureComposerBase* composer_pk  = new MixtureComposer(handler.nbSample(), handler.nbVariable(),K); // Categorical_pk

    IMixtureComposerBase* composerFixed_pjk = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),K);
    IMixtureComposerBase* composerFixed_pk  = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),K);

    MixtureComposer* p_composer_pjk = static_cast<MixtureComposer*>(composer_pjk);
    MixtureComposer* p_composer_pk  = static_cast<MixtureComposer*>(composer_pk);

    MixtureComposerFixedProp* p_composerFixed_pjk = static_cast<MixtureComposerFixedProp*>(composerFixed_pjk);
    MixtureComposerFixedProp* p_composerFixed_pk  = static_cast<MixtureComposerFixedProp*>(composerFixed_pk);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ CreateMixtures                                    +\n");
    p_composer_pjk->createMixture(manager, Clust::Categorical_pjk_, _T("model_pjk"));
    p_composerFixed_pjk->createMixture(manager, Clust::Categorical_pjk_, _T("model_pjk"));
    p_composer_pk->createMixture(manager, Clust::Categorical_pk_, _T("model_pjk"));
    p_composerFixed_pk->createMixture(manager, Clust::Categorical_pk_, _T("model_pjk"));

//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ Composer_pjk parameters                           +\n");
//    p_composer_pjk->writeParameters(stk_cout);
//
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ Composer_pk parameters                            +\n");
//    p_composer_pk->writeParameters(stk_cout);

//
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testSimpleStrategy: composer_pjk                  +\n");
    testSimpleStrategy(composer_pjk);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testSimpleStrategy: composerFixed_pjk             +\n");
    testSimpleStrategy(composerFixed_pjk);

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_pjk                     +\n");
    testXemStrategy(composer_pjk);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_pk                      +\n");
    testXemStrategy(composer_pk);

//
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ True Parameters of the model.                     +\n");
    stk_cout << "Proportions =\n" << pk;

    for (int k= proba.firstIdx(); k <= proba.lastIdx(); ++k)
    {
      stk_cout << _T("---> Component ") << k << _T("\n");
      stk_cout << _T("Probabilities =\n")<< proba[k]*Const::Point<Real>(d);

    }
    delete composer_pjk;
    delete composer_pk;
    delete composerFixed_pjk;
    delete composerFixed_pk;
  }
  catch (Exception const& e)
  {
    stk_cout << _T("An error occur:") << e.error();
  }
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for              +\n");
  stk_cout << _T("+ CategoricalMixture Models.                        +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0;
}

