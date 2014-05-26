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

/** @file STK_IMixture.cpp
 *  @brief In this file we implement the abstract base class IMixture.
 **/

#include "../include/STK_IMixture.h"

namespace STK
{

/* constructor */
IMixture::IMixture( std::string const& idName, int nbCluster)
                  : p_composer_(0), idName_(idName), nbCluster_(nbCluster)
{}

/* copy constructor */
IMixture::IMixture( IMixture const& mixture)
                  : p_composer_(0)
                  , idName_(mixture.idName_)
                  , nbCluster_(mixture.nbCluster_)
{}
/* Virtual destructor. */
IMixture::~IMixture(){}

/* set the mixture composer to the mixture */
void IMixture::setMixtureComposer( MixtureComposer const* p_composer)
{ p_composer_ = p_composer;}

/* @return the class labels of the composer */
int const* IMixture::classLabels() const
{ return p_composer_->p_zi()->p_data();}

/* @return the proportions of the composer */
double const* IMixture::proportions() const
{ return p_composer_->p_pk()->p_data();}

/* @return the number of sample */
int IMixture::nbSample() const
{ return p_composer_->nbSample();}

/* This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
CArrayPoint<STK::Real> const* IMixture::p_pk() const
{ return p_composer_->p_pk(); }

/* This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
Array2D<STK::Real> const* IMixture::p_tik() const
{ return p_composer_->p_tik(); }

/* This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
CArrayVector<int> const* IMixture::p_zi() const
{ return p_composer_->p_zi(); }

} // namespace STK
