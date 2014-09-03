/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: July 2, 2014
 *  Authors:    Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#include "mixt_IMixture.h"

namespace mixt
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
STK::CArrayPoint<STK::Real> const* IMixture::p_pk() const
{ return p_composer_->p_pk(); }

/* This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
STK::Array2D<STK::Real> const* IMixture::p_tik() const
{ return p_composer_->p_tik(); }

/* This function can be used in derived classes to get proportions from the framework.
 * @return Pointer to proportions.
 */
STK::CArrayVector<int> const* IMixture::p_zi() const
{ return p_composer_->p_zi(); }

} // namespace mixt
