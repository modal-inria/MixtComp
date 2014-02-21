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
 * Purpose:  test the Normal and MultiNormal classes.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file testJointModels.cpp
 *  @brief In this file we test the joint Statistical models.
 **/
#include "StrategiesUtil.h"

#include "../../include/Clustering.h"
#include "../../include/StatModels.h"
#include "../../include/STatistiK.h"
#include "../../include/DManager.h"


using namespace STK;

int main(int argc, char *argv[])
{
  try
  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test GammaMixture models                          +\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    DataHandler handler;
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Read files and fill handler                       +\n");
    if (!handler.readDataFromCsvFile( "./tests/Clustering/data/composite1.csv"
                                    , "./tests/Clustering/data/descriptor1.csv"))
    { return -1;}
    if (!handler.readDataFromCsvFile( "./tests/Clustering/data/composite2.csv"
                                    , "./tests/Clustering/data/descriptor2.csv"))
    { return -1;}
    handler.writeInfo(std::cout);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create composers and setDataHandler               +\n");
    IMixtureComposerBase* composer_bjk = new MixtureComposer(3);
    IMixtureComposerBase* composer_bj = new MixtureComposer(3);
    IMixtureComposerBase* composerFixed_bjk = new MixtureComposerFixedProp(3);
    IMixtureComposerBase* composerFixed_bj = new MixtureComposerFixedProp(3);

    MixtureComposer* p_composer_bjk = static_cast<MixtureComposer*>(composer_bjk);
    MixtureComposer* p_composer_bj = static_cast<MixtureComposer*>(composer_bj);
    MixtureComposerFixedProp* p_composerFixed_bjk = static_cast<MixtureComposerFixedProp*>(composerFixed_bjk);
    MixtureComposerFixedProp* p_composerFixed_bj = static_cast<MixtureComposerFixedProp*>(composerFixed_bj);


    p_composer_bjk->setDataHandler(&handler);
    p_composer_bj->setDataHandler(&handler);
    p_composerFixed_bjk->setDataHandler(&handler);
    p_composerFixed_bj->setDataHandler(&handler);
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ CreateMixtures                                    +\n");
    p_composer_bjk->createMixture(Clust::Gamma_ajk_bjk_, _T("model1"));
    p_composer_bj->createMixture(Clust::Gamma_ajk_bj_, _T("model3"));
    p_composerFixed_bjk->createMixture(Clust::Gamma_ajk_bjk_, _T("model1"));
    p_composerFixed_bj->createMixture(Clust::Gamma_ajk_bj_, _T("model3"));

//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSemStrategy: composer_bjk                        +\n");
//    testSemStrategy((composer_bjk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSemStrategy: composer_bj                        +\n");
//    testSemStrategy((composer_bj));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSemStrategy: composerFixed_bjk                   +\n");
//    testSemStrategy((composerFixed_bjk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSemStrategy: composerFixed_bj                   +\n");
//    testSemStrategy((composerFixed_bj));
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

