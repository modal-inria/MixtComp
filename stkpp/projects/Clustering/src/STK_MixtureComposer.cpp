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
#include "../include/STK_MixtureComposer.h"
#include "Arrays/include/STK_Display.h"

namespace STK
{

/* Constructor.
 *  @param nbCluster,nbSample, number of clusters and samples.
 */
MixtureComposer::MixtureComposer( int nbSample, int nbCluster)
                                : IMixtureComposer( nbSample, nbCluster)
                                , meanlnLikelihood_(0.)
{ setNbFreeParameter(nbCluster-1);}

/* copy constructor.
 *  @param composer the composer to copy
 */
MixtureComposer::MixtureComposer( MixtureComposer const& composer)
                                : IMixtureComposer(composer)
                                , v_mixtures_(composer.v_mixtures_)
                                , meanlnLikelihood_(composer.meanlnLikelihood_)
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
  MixtureComposer* p_composer = new MixtureComposer(nbSample(), nbCluster());
  p_composer->createComposer( v_mixtures_);
  return p_composer;
}

Real MixtureComposer::lnComponentProbability(int i, int k) const
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
  os << _T("Composer nbSample = ") << nbSample() << std::endl;
  os << _T("Composer nbVariable = ") << nbVariable() << std::endl;
  os << _T("Composer nbCluster = ") << nbCluster() << std::endl;
  os << _T("Composer nbFreeParameter = ") << nbFreeParameter() << std::endl;
  os << _T("Composer lnLikelihood = ") << lnLikelihood() << std::endl;
  os << _T("Composer proportions = ") << *(p_pk());

  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    os << _T("\nParameters of the mixture: ") << (*it)->idName() << _T("\n");
    (*it)->writeParameters(os);
  }
}

void MixtureComposer::initializeStep()
{
  if (v_mixtures_.size() == 0)
    STKRUNTIME_ERROR_NO_ARG(MixtureComposer::initializeStep,no mixture have been registered);
  // call base class initializeStep()
  IMixtureComposer::initializeStep();
  // initialize registered mixtures
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->initializeStep();}
}

void MixtureComposer::randomInit()
{
#ifdef STK_MIXTURE_VERBOSE
  stk_cout << _T("Entering MixtureComposer::RandomInit()\n");
#endif
  if (state() < 2) { initializeStep();}
  if (randomFuzzyTik()<2) throw(Clust::randomParamInitFail_);
  mapStep();
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->randomInit();}
  eStep();
  setState(Clust::modelParamInitialized_);
#ifdef STK_MIXTURE_VERBOSE
  stk_cout << _T("MixtureComposer::RandomInit() done\n");
#endif
}


// implement computeNbFreeParameters
int MixtureComposer::computeNbFreeParameters() const
{
  int sum = nbCluster_-1; // proportions
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { sum+= (*it)->nbFreeParameter();}
  return sum;
}

/* @brief compute the number of variables of the model.
 *  lookup on the mixtures and sum the nbFreeParameter.
 **/
int MixtureComposer::computeNbVariables() const
{
  int sum = 0;
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { sum+= (*it)->nbVariable();}
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

/* store the  intermediate results */
void MixtureComposer::storeIntermediateResults(int iteration)
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->storeIntermediateResults(iteration);}
  meanlnLikelihood_ += (lnLikelihood() - meanlnLikelihood_)/iteration;
}

void MixtureComposer::releaseIntermediateResults()
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->releaseIntermediateResults();}
  meanlnLikelihood_ = 0.;
}

/* Utility method allowing to signal to a mixture to set its parameters */
void MixtureComposer::setParameters()
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->setParameters();}
  setLnLikelihood(meanlnLikelihood_);
  meanlnLikelihood_ = 0.;
}

/* finalize */
void MixtureComposer::finalizeStep()
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->finalizeStep();}
}

/* register the mixture in the composer*/
void MixtureComposer::registerMixture(IMixture* p_mixture)
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("In MixtureComposer::registerMixture, registering mixture: ")
           << p_mixture->idName() << _T("\n");
#endif
  p_mixture->setMixtureComposer(this);
  v_mixtures_.push_back(p_mixture);
  // update nbVariables and NbFreeParameters
  setNbVariable(nbVariable()+p_mixture->nbVariable());
  setNbFreeParameter(nbFreeParameter()+p_mixture->nbFreeParameter());
}

/* @brief Create the composer using existing data handler and ingredients.
 * This method is essentially used by the create() method and can be
 * reused in derived classes. */
void MixtureComposer::createComposer( std::vector<IMixture*> const& v_mixtures)
{
  initializeMixtureParameters();
  initialize(nbSample(), nbVariable());
  v_mixtures_.resize( v_mixtures.size());
  for (size_t l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l] = v_mixtures[l]->create();
    v_mixtures_[l]->setMixtureComposer(this);
    v_mixtures_[l]->initializeStep();
  }
  // compute number of free parameters and of variables
  setNbVariable(computeNbVariables());
  setNbFreeParameter(computeNbFreeParameters());
}

/* Utility lookup function allowing to find a Mixture from its idData
 *  @param idData the id name of the mixture we want to get
 *  @return a pointer on the mixture
 **/
IMixture* MixtureComposer::getMixture( String const& idData) const
{
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { if ((*it)->idName() == idData) return (*it);}
  return 0;
}

/* Utility lookup function allowing to find a Mixture from its idData
 *  @param idData the id name of the mixture we want to get
 *  @return a pointer on the mixture
 **/
void MixtureComposer::releaseMixture( String const& idData)
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    if ((*it)->idName() == idData)
    {
      // update nbVariable and nbFreeParameters
       setNbVariable(nbVariable()-(*it)->nbVariable());
       setNbFreeParameter(nbFreeParameter()-(*it)->nbFreeParameter());
       // remove mixture
       delete (*it);
       v_mixtures_.erase(it);
       // update log-likelihood
       if (v_mixtures_.size() == 0)
       { setLnLikelihood(-Arithmetic<Real>::infinity());}
       else
       { setLnLikelihood(computeLnLikelihood());}
       // and break
       break;
    }
  }
}
/** Utility method allowing to create all the mixtures using the DataHandler
 *  info of the manager.
 **/
void MixtureComposer::createMixtures(IMixtureManager& manager)
{ manager.createMixtures(*this, nbCluster());}
/* Utility method allowing to create a mixture with a given data set
 *  and register it. The Mixture Manager will find the associated model
 *  to use with this data set.
 *  @param manager the manager with the responsibility of the creation.
 *  @param idData the id name of the data to modelize.
 **/
//    template<class MixtureManager>
void MixtureComposer::createMixture(IMixtureManager& manager, String const& idData)
{
  IMixture* p_mixture = manager.createMixture( idData, nbCluster());
#ifdef STK_MIXTURE_DEBUG
  if (!p_mixture)
  { stk_cout << _T("In MixtureComposer::createMixture(manager,")<< idData << _T(") failed.\n");}
#endif
  if (p_mixture) registerMixture(p_mixture);
}

/* Utility method allowing to release completely a mixture with its data set.
 *  The MixtureManager will find and release the associated data set.
 *  @param manager the manager with the responsibility of the release.
 *  @param idData the id name of the data to modelize.
 **/
void MixtureComposer::releaseMixture(IMixtureManager& manager, String const& idData)
{
  IMixture* p_mixture = getMixture(idData);
#ifdef STK_MIXTURE_DEBUG
  if (!p_mixture)
  { stk_cout << _T("In MixtureComposer::releaseMixture(manager,")<< idData << _T(") failed.\n");}
#endif
  if (p_mixture)
  {
    releaseMixture(idData);
    manager.releaseMixtureData( idData);
  }
}

/* Constructor.
 * @param nbCluster,nbSample number of clusters and samples
 */
MixtureComposerFixedProp::MixtureComposerFixedProp( int nbSample, int nbCluster)
                                                  : MixtureComposer( nbSample, nbCluster)
{ setNbFreeParameter(0); /* remove the count of the pk parameters */}

/* copy constructor.
 *  @param model the model to copy
 */
MixtureComposerFixedProp::MixtureComposerFixedProp( MixtureComposer const& model)
                                                  : MixtureComposer(model) {}

/* Create a composer, but reinitialize the ingredients parameters. */
MixtureComposerFixedProp* MixtureComposerFixedProp::create() const
{
  MixtureComposerFixedProp* p_composer = new MixtureComposerFixedProp(nbSample(), nbCluster());
  p_composer->createComposer(v_mixtures());
  /* remove the count of the pk parameters */
  p_composer->setNbFreeParameter(p_composer->nbFreeParameter()-(nbCluster()-1));
  return p_composer;
}
/* Create a clone of the current model, with ingredients parameters preserved. */
MixtureComposerFixedProp* MixtureComposerFixedProp::clone() const
{ return new MixtureComposerFixedProp(*this);}

/* overloading of the computeProportions() method.
 * Let them initialized to 1/K. */
void MixtureComposerFixedProp::pStep() {}

} /* namespace mixt */
