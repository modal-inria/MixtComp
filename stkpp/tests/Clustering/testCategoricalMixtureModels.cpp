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
                  )
{
  handler.readDataFromArray2D(data, idData, idModel);
  MixtureComposerFixedProp* p_composerFixed = static_cast<MixtureComposerFixedProp*>(composer);
  p_composerFixed->createMixture(manager, idData);
  testFullStrategy(composer, Clust::randomClassInit_, 1, Clust::semAlgo_
                   , 1, 1, 1
                   , Clust::semiSemAlgo_
                   , Clust::semiSemAlgo_);
}

int main(int argc, char *argv[])
{
  int L=5;
  Real pmiss = 0.1;
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test CategoricalMixture models                    +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    Array2D<int> c1;
    Array2DVector<Real> pk;
    Array2DVector<int> zi;
    Array_pkProba proba;

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Simulate data and fill DataHandler                +\n");
    // simulate pk (uniform Dirichlet if false)
    simulZiLaw(K, pk, true);
    stk_cout << _T("pk=\n") << pk;
    // generate model
    simulCategorical_pkModel(d, K, L, proba);
    stk_cout << _T("proba=\n") << proba;
    // generate data
    simulCategorical_pkMixture(n, d, proba, pk, c1, zi);
    writeSimulatedData(c1, 10);
    // add missing values
    std::vector< std::pair< std::pair<int,int>, int > > tmiss;
    simulMissing(pmiss, c1, tmiss);
    writeMissingValues(tmiss, 10);

    // duplicate original data set
    Array2D<int> c2(c1);

    // add data to datahandler
    DataHandler handler;
    std::string idData_pjk("IdCategorical_pjk"), idData_pk("IdCategorical_pk");
    std::string idModel_pjk("Categorical_pjk"), idModel_pk("Categorical_pk");

    handler.readDataFromArray2D(c1, idData_pjk, idModel_pjk);
    handler.readDataFromArray2D(c2, idData_pk, idModel_pk);
    handler.writeInfo(std::cout);
    MixtureManager<DataHandler> manager(handler);

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create composers.                                 +\n");
    IMixtureComposer* composer_pjk = new MixtureComposer(handler.nbSample(),K); // Categorical_pjk
    IMixtureComposer* composer_pk  = new MixtureComposer(handler.nbSample(),K); // Categorical_pk

    IMixtureComposer* composerFixed_pjk = new MixtureComposerFixedProp(handler.nbSample(),K);
    IMixtureComposer* composerFixed_pk  = new MixtureComposerFixedProp(handler.nbSample(),K);

    MixtureComposer* p_composer_pjk = static_cast<MixtureComposer*>(composer_pjk);
    MixtureComposer* p_composer_pk  = static_cast<MixtureComposer*>(composer_pk);

    MixtureComposerFixedProp* p_composerFixed_pjk = static_cast<MixtureComposerFixedProp*>(composerFixed_pjk);
    MixtureComposerFixedProp* p_composerFixed_pk  = static_cast<MixtureComposerFixedProp*>(composerFixed_pk);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ CreateMixtures                                    +\n");
    p_composer_pjk->createMixture(manager, idData_pjk);
    p_composerFixed_pjk->createMixture(manager, idData_pjk);
    p_composer_pk->createMixture(manager, idData_pk);
    p_composerFixed_pk->createMixture(manager, idData_pk);

    //
    Array2D<Real> parameters;
    std::vector< std::pair< std::pair<int,int>, int > > miss;
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testSimpleStrategy: composer_pjk                  +\n");
    testSimpleStrategy(composer_pjk);
    p_composer_pjk = static_cast<MixtureComposer*>(composer_pjk);
    stk_cout << _T("+ Get parameters.                                    +\n");
    p_composer_pjk->getParameters(manager, idData_pjk, parameters);
    stk_cout << _T("parameters=\n") << parameters;
    stk_cout << _T("+ Get Missing values.                                +\n");
    manager.getMissingValues(idData_pjk, miss);
    writeMissingValues(miss, 10);
    stk_cout << _T("+ True Parameters of the model.                     +\n");
    stk_cout << "Proportions =\n" << pk;
    for (int k= proba.begin(); k <= proba.lastIdx(); ++k)
    {
      stk_cout << _T("---> Component ") << k << _T("\n");
      stk_cout << _T("Probabilities =\n")<< proba[k]*Const::Point<Real>(d);
    }

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testSimpleStrategy: composerFixed_pjk             +\n");
    testSimpleStrategy(composerFixed_pjk);
    p_composer_pjk = static_cast<MixtureComposer*>(composer_pjk);
    stk_cout << _T("+ Get parameters.                                    +\n");
    p_composer_pjk->getParameters(manager, idData_pjk, parameters);
    stk_cout << _T("parameters=\n") << parameters;
    stk_cout << _T("+ Get Missing values.                                +\n");
    manager.getMissingValues(idData_pjk, miss);
    writeMissingValues(miss, 10);
    stk_cout << _T("+ True Parameters of the model.                     +\n");
    stk_cout << "Proportions =\n" << pk;
    for (int k= proba.begin(); k <= proba.lastIdx(); ++k)
    {
      stk_cout << _T("---> Component ") << k << _T("\n");
      stk_cout << _T("Probabilities =\n")<< proba[k]*Const::Point<Real>(d);
    }

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testFullStrategy: composer_pjk                    +\n");
    testFullStrategy(composer_pjk, Clust::randomClassInit_
                     , 1, Clust::semAlgo_
                     , 1, 1, 1
                     , Clust::semiSemAlgo_
                     , Clust::semiSemAlgo_);
    p_composer_pjk = static_cast<MixtureComposer*>(composer_pjk);
    stk_cout << _T("+ Get parameters.                                    +\n");
    p_composer_pjk->getParameters(manager, idData_pjk, parameters);
    stk_cout << _T("parameters=\n") << parameters;
    stk_cout << _T("+ Get Missing values.                                +\n");
    manager.getMissingValues(idData_pjk, miss);
    writeMissingValues(miss, 10);
    stk_cout << _T("+ True Parameters of the model.                     +\n");
    stk_cout << "Proportions =\n" << pk;
    for (int k= proba.begin(); k <= proba.lastIdx(); ++k)
    {
      stk_cout << _T("---> Component ") << k << _T("\n");
      stk_cout << _T("Probabilities =\n")<< proba[k]*Const::Point<Real>(d);
    }

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testFullStrategy: composer_pk                     +\n");
    testFullStrategy(composer_pk, Clust::randomClassInit_
                     , 1, Clust::semAlgo_
                     , 1, 1, 1
                     , Clust::semiSemAlgo_
                     , Clust::semiSemAlgo_);
    p_composer_pk = static_cast<MixtureComposer*>(composer_pk);
    stk_cout << _T("+ Get parameters.                                    +\n");
    p_composer_pk->getParameters(manager, idData_pk, parameters);
    stk_cout << _T("parameters=\n") << parameters;
    stk_cout << _T("+ Get Missing values.                                +\n");
    manager.getMissingValues(idData_pk, miss);
    writeMissingValues(miss, 10);

    stk_cout << _T("+ True Parameters of the model.                     +\n");
    stk_cout << "Proportions =\n" << pk;
    for (int k= proba.begin(); k <= proba.lastIdx(); ++k)
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

