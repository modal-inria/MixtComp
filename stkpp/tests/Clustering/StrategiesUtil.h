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

/** @file StrategiesUtil.h
 *  @brief Common functions used for testing mixture models
 **/

#ifndef STRATEGIESUTIL_H
#define STRATEGIESUTIL_H

#include "Clustering.h"

namespace STK
{
void testSimpleStrategy( IMixtureComposer*& p_composer);

void testFullStrategy( IMixtureComposer*& p_composer
                     , Clust::initType init = Clust::randomClassInit_
                     , int nbTryInInit =5, Clust::algoType initAlgo = Clust::semAlgo_
                     , int nbTry =2, int nbInitRun =10, int nbShortRun =5
                     , Clust::algoType shortAlgo= Clust::semiSemAlgo_
                     , Clust::algoType longAlgo= Clust::semiSemAlgo_);
} // namespace STK

#endif
