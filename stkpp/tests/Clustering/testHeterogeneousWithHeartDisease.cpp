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
  if (argc >1) { K= atoi(argv[1]);}
  Array2D<Real> parameters;

  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test Heterogeneous models with HeartDisease data  +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    DataHandler handler;
    handler.setWithNames(true);
    std::string idData_pjk("IdCategorical_pjk"), idData_sjk("IdGaussian_sjk");
    std::string idModel_pjk("Categorical_pjk"), idModel_sjk("Gaussian_sjk");

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Read first file                                   +\n");
    if (!handler.readDataFromCsvFile( "./tests/data/HeartDisease.cat.csv",idData_pjk, idModel_pjk))
    { return -1;}
    if (!handler.readDataFromCsvFile( "./tests/data/HeartDisease.cont.csv",idData_sjk, idModel_sjk))
    { return -1;}
    handler.writeInfo(std::cout);
    stk_cout << handler.data();

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create MixtureManger.                             +\n");
    MixtureManager<DataHandler> manager(handler);

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create composers.                                 +\n");
    IMixtureComposer* composer = new MixtureComposer(handler.nbSample(),K);
    MixtureComposer* p_composer = static_cast<MixtureComposer*>(composer);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ CreateMixtures                                    +\n");
    p_composer->createMixture(manager, idData_pjk);
    p_composer->createMixture(manager, idData_sjk);

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testSimpleStrategy.                               +\n");
    StrategyFacade facade(composer);
    facade.createSimpleStrategy( Clust::randomInit_, 2, Clust::emAlgo_, 2, 1e-04
                               , 2
                               , Clust::emAlgo_, 500, 1e-08);
    facade.run();
    p_composer = static_cast<MixtureComposer*>(composer);
    p_composer->writeParameters(stk_cout);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testFullStrategy.                                 +\n");
    facade.createFullStrategy( Clust::randomInit_, 2, Clust::emAlgo_, 2, 1e-04
                             , 2, 2, 2
                             , Clust::semiSemAlgo_, 100, 1e-04, Clust::semiSemAlgo_, 500, 1e-07);
    facade.run();
    p_composer = static_cast<MixtureComposer*>(composer);
    p_composer->writeParameters(stk_cout);
    delete composer;
  }
  catch (Exception const& e)
  {
    stk_cout << _T("An error occur:") << e.error();
  }
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for              +\n");
  stk_cout << _T("+ Heterogeneous models with HeartDisease data.      +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0;
}

