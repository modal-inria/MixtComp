/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

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
 * Project:  stkpp::Clustering
 * created on: 16 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_IMixtureModelBase.cpp
 *  @brief In this file we implement the interface base class IMixtureModelBase
 **/

#include "../include/STK_IMixtureModelBase.h"

namespace STK
{


/* default constructor */
IMixtureModelBase::IMixtureModelBase( int nbCluster)
                                : p_pk_(0), p_tik_(0), p_zi_(0)
                                , nbCluster_(nbCluster)
                                , nbSample_(0)
                                , nbVariable_(0)
{}
/* copy constructor. If the pointer on the mixture parameters are not zero
 *  then they are cloned.
 *  @note if the model have not created the parameters, then the pointer are
 *  initialized to 0.
 *  @param model the model to clone
 **/
IMixtureModelBase::IMixtureModelBase( IMixtureModelBase const& model)
                                : p_pk_(0)
                                , p_tik_(0)
                                , p_zi_(0)
                                , nbCluster_(model.nbCluster_)
                                , nbSample_(model.nbSample_)
                                , nbVariable_(model.nbVariable_)
{}

/* destructor */
IMixtureModelBase::~IMixtureModelBase() {}
/* set the parameters of the  mixture model using external
 * proportions, tik and zi.
 *  @param p_pk pointer on the proportions of the mixture model
 *  @param p_tik pointer on the posterior probabilities
 *  @param p_zi pointer on the class labels
 * */
void IMixtureModelBase::setMixtureParameters( CArrayPoint<Real> const* p_pk
                                          , Array2D<Real> const* p_tik
                                          , CArrayVector<int> const* p_zi
                                          )
{
  p_pk_ = p_pk;
  p_tik_ = p_tik;
  p_zi_ = p_zi;
}


} // namespace STK

