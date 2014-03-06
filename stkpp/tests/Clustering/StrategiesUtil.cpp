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
#include "../../include/StatModels.h"
#include "../../include/STatistiK.h"
#include "../../include/DManager.h"

#include "../../projects/Clustering/include/STK_MixtureInit.h"

using namespace STK;

void testSimpleStrategy( IMixtureComposerBase*& p_composer)
{
  StrategyFacade facade(p_composer);
  facade.createSimpleStrategy( Clust::randomClassInit_, 3, Clust::semAlgo_, 20, 1e-04
                             , 5, Clust::emAlgo_, 500, 1e-08);
  facade.run();
  stk_cout << _T("\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ SimpleStrategy terminated. Results:               +\n");
  p_composer->writeParameters(stk_cout);
}

void testXemStrategy( IMixtureComposerBase*& p_composer)
{
  StrategyFacade facade(p_composer);
  facade.createXemStrategy( Clust::randomClassInit_, 3, Clust::semAlgo_, 20, 1e-04
                          , 3
                          , 3, Clust::cemAlgo_, 100, 1e-05, Clust::emAlgo_, 500, 1e-08);
  facade.run();
  stk_cout << _T("\n");
  stk_cout << _T("+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  stk_cout << _T("+ XemStrategy terminated. Results:                  +\n");
  p_composer->writeParameters(stk_cout);
}

