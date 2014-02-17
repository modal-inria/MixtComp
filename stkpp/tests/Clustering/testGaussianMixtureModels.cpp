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
//  try
//  {
    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Test GaussianMixture models                          +\n");
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
    // gaussian_sk
    if (!handler.readDataFromCsvFile( "./tests/Clustering/data/composite3.csv"
                                    , "./tests/Clustering/data/descriptor3.csv"))
    { return -1;}
    // gaussian_sj
    if (!handler.readDataFromCsvFile( "./tests/Clustering/data/composite3.csv"
                                    , "./tests/Clustering/data/descriptor4.csv"))
    { return -1;}
    handler.writeInfo(std::cout);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ Create composers and setDataHandler               +\n");
    IMixtureComposerBase* composer_sjk = new MixtureComposer(3); // Gaussian_sjk
    IMixtureComposerBase* composer_s   = new MixtureComposer(3); // Gaussian_s
    IMixtureComposerBase* composer_sj  = new MixtureComposer(3); // Gaussian_sj
    IMixtureComposerBase* composer_sk  = new MixtureComposer(3); // Gaussian_sk

    IMixtureComposerBase* composerFixed_sjk = new MixtureComposerFixedProp(3);
    IMixtureComposerBase* composerFixed_s   = new MixtureComposerFixedProp(3);
    IMixtureComposerBase* composerFixed_sj  = new MixtureComposerFixedProp(3);
    IMixtureComposerBase* composerFixed_sk  = new MixtureComposerFixedProp(3);

    MixtureComposer* p_composer_sjk = static_cast<MixtureComposer*>(composer_sjk);
    MixtureComposer* p_composer_s   = static_cast<MixtureComposer*>(composer_s);
    MixtureComposer* p_composer_sj  = static_cast<MixtureComposer*>(composer_sj);
    MixtureComposer* p_composer_sk  = static_cast<MixtureComposer*>(composer_sk);

    MixtureComposerFixedProp* p_composerFixed_sjk = static_cast<MixtureComposerFixedProp*>(composerFixed_sjk);
    MixtureComposerFixedProp* p_composerFixed_s   = static_cast<MixtureComposerFixedProp*>(composerFixed_s);
    MixtureComposerFixedProp* p_composerFixed_sj  = static_cast<MixtureComposerFixedProp*>(composerFixed_sj);
    MixtureComposerFixedProp* p_composerFixed_sk  = static_cast<MixtureComposerFixedProp*>(composerFixed_sk);

    p_composer_sjk->setDataHandler(&handler);
    p_composer_s->setDataHandler(&handler);
    p_composer_sj->setDataHandler(&handler);
    p_composer_sk->setDataHandler(&handler);

    p_composerFixed_sjk->setDataHandler(&handler);
    p_composerFixed_s->setDataHandler(&handler);
    p_composerFixed_sj->setDataHandler(&handler);
    p_composerFixed_sk->setDataHandler(&handler);

    stk_cout << _T("\n\n");
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ CreateMixtures and setData                     +\n");
    p_composer_sjk->createMixture(Clust::Gaussian_sjk_, _T("model4"));
    p_composer_sjk->setData();

    p_composer_s->createMixture(Clust::Gaussian_s_, _T("model2"));
    p_composer_s->setData();

    p_composer_sj->createMixture(Clust::Gaussian_sj_, _T("model_sj"));
    p_composer_sj->setData();

    p_composer_sk->createMixture(Clust::Gaussian_sk_, _T("model_sk"));
    p_composer_sk->setData();

    p_composerFixed_sjk->createMixture(Clust::Gaussian_sjk_, _T("model4"));
    p_composerFixed_sjk->setData();

    p_composerFixed_s->createMixture(Clust::Gaussian_s_, _T("model2"));
    p_composerFixed_s->setData();

    p_composerFixed_sj->createMixture(Clust::Gaussian_sj_, _T("model_sj"));
    p_composerFixed_sj->setData();

    p_composerFixed_sk->createMixture(Clust::Gaussian_sk_, _T("model_sk"));
    p_composerFixed_sk->setData();

//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSemStrategy: composer_sjk                        +\n");
//    testSemStrategy((composer_sjk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSemStrategy: composer_s                        +\n");
//    testSemStrategy((composer_s));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSemStrategy: p_composerFixed_sk                   +\n");
//    testSemStrategy((p_composerFixed_sk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSemStrategy: p_composerFixed_s                   +\n");
//    testSemStrategy((p_composerFixed_s));
//
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSimpleStrategy: composer_sjk                     +\n");
//    testSimpleStrategy((composer_sjk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSimpleStrategy: composer_s                     +\n");
//    testSimpleStrategy((composer_s));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSimpleStrategy: p_composerFixed_sk                +\n");
//    testSimpleStrategy((p_composerFixed_sk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testSimpleStrategy: p_composerFixed_s                +\n");
//    testSimpleStrategy((p_composerFixed_s));

    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_sjk                        +\n");
    testXemStrategy(composer_sjk);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_s                        +\n");
    testXemStrategy(composer_s);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_sk                      +\n");
    testXemStrategy(composer_sk);
    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    stk_cout << _T("+ testXemStrategy: composer_sj                      +\n");
    testXemStrategy(composer_sj);

//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testXemStrategy: p_composerFixed_sk                   +\n");
//    testXemStrategy((p_composerFixed_sk));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testXemStrategy: p_composerFixed_s                   +\n");
//    testSemStrategy((p_composerFixed_s));
//    stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//    stk_cout << _T("+ testXemStrategy: p_composerFixed_sk                   +\n");
//    testXemStrategy((p_composerFixed_sk));

    delete composer_sjk;
    delete composer_s;
    delete composer_sj;
    delete composer_sk;

    delete composerFixed_sjk;
    delete composerFixed_s;
    delete composerFixed_sj;
    delete composerFixed_sk;
//  }
//  catch (Exception const& e)
//  {
//    stk_cout << _T("An error occur:") << e.error();
//  }
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Successful completion of testing for              +\n");
  stk_cout << _T("+ GaussianMixture Models.                           +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0;
}

