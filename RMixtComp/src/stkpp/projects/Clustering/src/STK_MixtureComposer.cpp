/*
 * mixt_CompositeMixture.cpp
 *
 *  Created on: 16 oct. 2013
 *      Author: iovleff
 */

#include "../include/STK_IMixture.h"
#include "../../Arrays/include/STK_Display.h"

namespace STK
{

MixtureComposer::MixtureComposer( int nbCluster)
                                : IMixtureModelBase(nbCluster)
                                , p_handler_(0)
{}

MixtureComposer::MixtureComposer( MixtureComposer const& composer)
                                : IMixtureModelBase(composer)
                                , p_handler_(composer.p_handler_)
                                , v_mixtures_(composer.v_mixtures_)
{
  if (!p_handler_)
    STKRUNTIME_ERROR_NO_ARG(MixtureComposer::MixtureComposer,data handler is not set);
  // clone ingredients
  for (size_t l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l] = composer.v_mixtures_[l]->clone();
    v_mixtures_[l]->setMixtureComposer(this);
    v_mixtures_[l]->initializeModel();
  }
  // no need to call IMixtureModelBase::initializeModel as all mixture parameters
  // have been yet copied by the copy constructor in base class
}

MixtureComposer::~MixtureComposer()
{
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  { delete (*it);}
}

/* clone pattern */
MixtureComposer* MixtureComposer::clone() const
{
  if (!p_handler_)
    STKRUNTIME_ERROR_NO_ARG(MixtureComposer::createIngredients,data handler is not set);
  return new MixtureComposer(*this);
}

MixtureComposer* MixtureComposer::create() const
{
  if (!p_handler_)
    STKRUNTIME_ERROR_NO_ARG(MixtureComposer::create,data handler is not set);
  // set dimensions
  MixtureComposer* p_composer = new MixtureComposer(nbCluster_);
  p_composer->createComposer(p_handler_, v_mixtures_);
  return p_composer;
}

// initialize the composer
void MixtureComposer::initializeModel()
{
  if (!p_handler_)
    STKRUNTIME_ERROR_NO_ARG(MixtureComposer::initializeModel,data handler is not set);
  // set dimensions
  this->setNbSample(p_handler_->nbSample());
  this->setNbVariable(p_handler_->nbVariable());
  // initialize all ingredients
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->setMixtureComposer(this);
    (*it)->setData();
    (*it)->initializeModel();
  }
  // create mixture parameters in base class, to call after mixtures initilaization
  // as we need to know the number of free parameters when computing nbFreeParameters
  IMixtureModelBase::initializeModel();
}

/* set the data handler */
void MixtureComposer::setDataHandler(IDataHandler const* p_handler)
{ p_handler_ = p_handler; }

/* @brief Create the composer using existing data handler and ingredients.
 * This method is essentially used by the create() method and can be
 * reused in derived classes. */
void MixtureComposer::createComposer(IDataHandler const* p_handler, std::vector<IMixture*> const& v_mixtures)
{
  setDataHandler(p_handler);
  setNbSample(p_handler_->nbSample());
  setNbVariable(p_handler_->nbVariable());
  v_mixtures_ = v_mixtures;
  for (size_t l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l] = v_mixtures[l]->create();
    v_mixtures_[l]->setMixtureComposer(this);
    v_mixtures_[l]->initializeModel();
  }
  IMixtureModelBase::initializeModel();
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
    stk_cout << _T("Parameters of the ingredient:") << (*it)->idName() << _T("\n");
    (*it)->writeParameters(os);
  }
}

void MixtureComposer::initializeStep()
{
  pStep();
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  { (*it)->initializeStep();}
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

/* create ingredients using info from p_dataHandler */
void MixtureComposer::createIngredients()
{
  if (!p_handler_)
    STKRUNTIME_ERROR_NO_ARG(MixtureComposer::createIngredients,data handler is not set);
  for (InfoMap::const_iterator it=p_handler_->info().begin(); it!=p_handler_->info().end(); ++it)
  {
    std::string name = it->first;
    std::string model= it->second;
    Clust::Ingredient idModel = Clust::stringToIngredient(model);
#ifdef DSTK_MIXTURE_DEBUG
     stk_cout << _T("model = ") << model << _T("\n");
     stk_cout << _T("name = ") << name << _T("\n");
     stk_cout << _T("Ingredient = ") << idModel << _T("\n");
#endif
    if (idModel != Clust::unknown_mixture_)
    { registerIngredient(Clust::createIngredient(idModel, name, nbCluster_ ));}
  }
}

/* create ingredients using specific idModel and */
void MixtureComposer::createIngredient(Clust::Ingredient idModel, String const& idName)
{
  if (idModel == Clust::unknown_mixture_) return;
  registerIngredient(Clust::createIngredient(idModel, idName, nbCluster_ ));
}

void MixtureComposer::registerIngredient(IMixture* mixture)
{
  v_mixtures_.push_back(mixture);
  v_mixtures_.back()->setMixtureComposer(this);
}

// set data to all mixtures
void MixtureComposer::setData()
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {  (*it)->setData();}
}


/* Create a composer, but reinitialize the ingredients parameters. */
MixtureComposerFixedProp* MixtureComposerFixedProp::create() const
{
  MixtureComposerFixedProp* p_composer = new MixtureComposerFixedProp(nbCluster_);
  p_composer->createComposer(p_handler(), v_mixtures());
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
