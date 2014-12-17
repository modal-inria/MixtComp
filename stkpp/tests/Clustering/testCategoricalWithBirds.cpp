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

int main(int argc, char *argv[])
{
  int K=3;
  Array2D<Real> parameters;

  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test Categorical models with Birds data           +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    DataHandler handler;
    handler.setWithNames(true);
    std::string idData_pjk("IdCategorical_pjk"), idData_pk("IdCategorical_pk");
    std::string idModel_pjk("Categorical_pjk"), idModel_pk("Categorical_pk");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Read first file                                   +\n");
    if (!handler.readDataFromCsvFile( "./tests/data/birds.csv",idData_pjk,  idModel_pjk))
    { return -1;}
    if (!handler.readDataFromCsvFile( "./tests/data/birds.csv",idData_pk,  idModel_pk))
    { return -1;}
    handler.writeInfo(std::cout);
    stk_cout << handler.data();

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create MixtureManger.                             +\n");
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

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testSimpleStrategy: composer_pjk                  +\n");
    StrategyFacade facade(composer_pjk);
    facade.createSimpleStrategy( Clust::randomInit_, 3, Clust::semAlgo_, 20, 1e-04
                               , 5, Clust::emAlgo_, 500, 1e-08);
    facade.run();
    p_composer_pjk = static_cast<MixtureComposer*>(composer_pjk);
    stk_cout << _T("+ Get parameters.                                    +\n");
    p_composer_pjk->getParameters(manager, idData_pjk, parameters);
    stk_cout << _T("parameters=\n") << parameters;
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

