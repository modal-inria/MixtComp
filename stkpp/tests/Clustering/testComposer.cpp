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
 * Purpose:  test the Composer
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file testComposer.cpp
 *  @brief In this file we test the composer
 **/
#include "StrategiesUtil.h"
#include "DManager.h"

using namespace STK;


int main(int argc, char *argv[])
{
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Test composer                                     +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  DataHandler handler;
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Read first file                                   +\n");
  if (!handler.readDataFromCsvFile( "./tests/Clustering/data/composite1.csv"
                                  , "./tests/Clustering/data/descriptor1.csv"))
  { return -1;}
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Read second file                                   +\n");
  if (!handler.readDataFromCsvFile( "./tests/Clustering/data/composite2.csv"
                                  , "./tests/Clustering/data/descriptor2.csv"))
  { return -1;}
  handler.writeInfo(std::cout);

  MixtureManager<DataHandler> manager(handler);
  //stk_cout << handler.data();
  //  CsvToArray<Array2D<Real>, String > import(handler.data(), 0.2);
  //  import.run();
  //  stk_cout << import.p_data()->safe();
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ composer(), setDataHandler()                      +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Create composers and setDataHandler               +\n");
  MixtureComposer* composer = new MixtureComposer(handler.nbSample(), handler.nbVariable(),3);

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ CreateMixtures() and initializeStep()            +\n");
  stk_cout << _T("+ composer:                                         +\n");
  composer->createMixtures(manager);
//  composer->initializeStep();
//  composer->writeParameters(stk_cout);
//
//  stk_cout << _T("\n\n");
//  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//  stk_cout << _T("+ test randomClassInit(). composer:                 +\n");
//  composer->randomClassInit();
//  composer->writeParameters(stk_cout);
//
//  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//  stk_cout << _T("+ sem iterations. composer:                         +\n");
//  composer->samplingStep();
//  composer->mStep();
//  composer->sStep();
//  composer->eStep();
//  composer->writeParameters(stk_cout);
//  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//  stk_cout << _T("+ em iterations. composer:                           +\n");
//  composer->imputationStep();
//  composer->mStep();
//  composer->eStep();
//  composer->writeParameters(stk_cout);
//  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//  stk_cout << _T("+ cem iterations. composer:                           +\n");
//  composer->imputationStep();
//  composer->cStep();
//  composer->eStep();
//  composer->writeParameters(stk_cout);
  delete composer;
//
  //
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ Testing strategies                                  +\n");
  IMixtureComposerBase* composer1 = new MixtureComposer(handler.nbSample(), handler.nbVariable(),3);
  IMixtureComposerBase* composerFixed1 = new MixtureComposerFixedProp(handler.nbSample(), handler.nbVariable(),3);
  MixtureComposer* p_composer1 = static_cast<MixtureComposer*>(composer1);
  MixtureComposerFixedProp* p_composerFixed1 = static_cast<MixtureComposerFixedProp*>(composerFixed1);

  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ CreateMixtures(), initializeStep()               +\n");
  stk_cout << _T("+ composer:                                         +\n");
  p_composer1->createMixtures(manager);
  p_composer1->initializeStep();
  p_composer1->writeParameters(stk_cout);
  p_composerFixed1->createMixtures(manager);
  p_composerFixed1->initializeStep();
  p_composerFixed1->writeParameters(stk_cout);

  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ testSimpleStrategy: composer1                     +\n");
  testSimpleStrategy((composer1));
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ testSimpleStrategy: composerFixed1                +\n");
  testSimpleStrategy((composerFixed1));

  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ testXemStrategy: composer1                        +\n");
  testXemStrategy((composer1));
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ testXemStrategy: composerFixed1                   +\n");
  testXemStrategy((composerFixed1));

  delete composer1;
  delete composerFixed1;
  stk_cout << _T("\n\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ End of test composer : no error detected          +\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return 0;
}

