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
 * created on: 14 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_MixtureComposer.cpp
 *  @brief In this file we implement the class MixtureComposer.
 **/

#include "../include/STK_IMixture.h"
#include "../../Arrays/include/STK_Display.h"

namespace STK
{


MixtureComposer::MixtureComposer( int nbSample, int nbVariable, int nbCluster)
                                : IMixtureComposerBase( nbSample, nbVariable, nbCluster)
{}

MixtureComposer::MixtureComposer( MixtureComposer const& composer)
                                : IMixtureComposerBase(composer)
                                , v_mixtures_(composer.v_mixtures_)
{
  // clone mixtures
  for (size_t l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l] = composer.v_mixtures_[l]->clone();
    v_mixtures_[l]->setMixtureComposer(this);
    v_mixtures_[l]->initializeStep();
  }
}

MixtureComposer::~MixtureComposer()
{
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  { delete (*it);}
}

/* clone pattern */
MixtureComposer* MixtureComposer::clone() const
{ return new MixtureComposer(*this);}

MixtureComposer* MixtureComposer::create() const
{
  // set dimensions
  MixtureComposer* p_composer = new MixtureComposer(nbSample(), nbVariable(), nbCluster());
  p_composer->createComposer( v_mixtures_);
  return p_composer;
}

Real MixtureComposer::lnComponentProbability(int i, int k)
{
  Real sum=0.0;
  for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  { sum += (*it)->lnComponentProbability(i,k);}
  return sum;
}

void MixtureComposer::mStep()
{
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  { (*it)->paramUpdateStep();}
}

void MixtureComposer::writeParameters(std::ostream& os) const
{
  stk_cout << _T("Composer lnLikelihood = ") << lnLikelihood() << std::endl;
  stk_cout << _T("Composer nbFreeParameter = ") << this->nbFreeParameter() << std::endl;
  stk_cout << _T("Composer proportions = ") << *(this->p_prop()) << std::endl;

  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    stk_cout << _T("Parameters of the mixtures:") << (*it)->idName() << _T("\n");
    (*it)->writeParameters(os);
  }
}

void MixtureComposer::initializeStep()
{
  if (v_mixtures_.size() == 0)
    STKRUNTIME_ERROR_NO_ARG(MixtureComposer::initializeStep,no mixture have been registered);
  // compute number of free parameters
  setNbFreeParameter(computeNbFreeParameters());
  // compute proportions
  pStep();
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->initializeStep();}
  setState(Clust::modelInitialized_);
}

void MixtureComposer::randomInit()
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->randomInit();}
}


// implement computeNbFreeParameters
int MixtureComposer::computeNbFreeParameters() const
{
  int sum = nbCluster_-1; // proportions
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { sum+= (*it)->nbFreeParameter();}
  return sum;
}

void MixtureComposer::imputationStep()
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->imputationStep();}
}

/* @brief Simulation of all the latent variables and/or missing data
 *  excluding class labels.
 */
void MixtureComposer::samplingStep()
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->samplingStep();}
}


void MixtureComposer::finalizeStep()
{
  for (size_t l = 0; l < v_mixtures_.size(); ++l)
  { v_mixtures_[l]->finalizeStep();}
}

/* register the mixture in the composer*/
void MixtureComposer::registerMixture(IMixture* p_mixture)
{
  p_mixture->setMixtureComposer(this);
  v_mixtures_.push_back(p_mixture);
}

/* @brief Create the composer using existing data handler and ingredients.
 * This method is essentially used by the create() method and can be
 * reused in derived classes. */
void MixtureComposer::createComposer( std::vector<IMixture*> const& v_mixtures)
{
  intializeMixtureParameters();
  v_mixtures_.resize( v_mixtures.size());
  for (size_t l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l] = v_mixtures[l]->create();
    v_mixtures_[l]->setMixtureComposer(this);
    v_mixtures_[l]->initializeStep();
  }
}

/* Create a composer, but reinitialize the ingredients parameters. */
MixtureComposerFixedProp* MixtureComposerFixedProp::create() const
{
  MixtureComposerFixedProp* p_composer = new MixtureComposerFixedProp(nbSample(), nbVariable(), nbCluster());
  p_composer->createComposer(v_mixtures());
  return p_composer;
}
/* Create a clone of the current model, with ingredients parameters preserved. */
MixtureComposerFixedProp* MixtureComposerFixedProp::clone() const
{ return new MixtureComposerFixedProp(*this);}

// implement computeNbFreeParameters
int MixtureComposerFixedProp::computeNbFreeParameters() const
{ return MixtureComposer::computeNbFreeParameters()-nbCluster()+1;}


/* overloading of the computePropotions() method.
 * Let them initialized to 1/K. */
void MixtureComposerFixedProp::pStep() {}

} /* namespace mixt */
