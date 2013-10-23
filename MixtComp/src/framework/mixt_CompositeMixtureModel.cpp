/*
 * mixt_CompositeMixture.cpp
 *
 *  Created on: 16 oct. 2013
 *      Author: iovleff
 */

#include "mixt_CompositeMixtureModel.h"

namespace mixt
{

CompositeMixtureModel::CompositeMixtureModel(int nbCluster)
                           : STK::IMixtureModelBase(nbCluster)
{
  createMixtureParameters();

}

CompositeMixtureModel::CompositeMixtureModel(CompositeMixtureModel const& model)
                           : STK::IMixtureModelBase(model)
                           , v_mixtures_(model.v_mixtures_)
{

}

CompositeMixtureModel::~CompositeMixtureModel()
{
  // TODO Auto-generated destructor stub
}

CompositeMixtureModel* CompositeMixtureModel::create() const
{
  CompositeMixtureModel* newComp =  new CompositeMixtureModel(*this);
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    newComp->v_mixtures_[l] = (v_mixtures_[l]->clone());
    newComp->v_mixtures_[l]->initializeModel();
  }
  return newComp;
}

CompositeMixtureModel* CompositeMixtureModel::clone() const
{
  CompositeMixtureModel* newComp =  new CompositeMixtureModel(*this);
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    newComp->v_mixtures_[l] = (v_mixtures_[l]->clone());
  }
  return newComp;
}

STK::Real CompositeMixtureModel::lnComponentProbability(int i, int k)
{
  STK::Real sum=0.0;
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    sum += v_mixtures_[l]->logComponentProbability(i,k);
  }
  return sum;
}

void CompositeMixtureModel::mStep()
{
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l]->paramUpdateStep();
  }
}

void CompositeMixtureModel::writeParameters(std::ostream& os) const
{
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l]->writeParameters(os);
  }
}

void CompositeMixtureModel::initializeModel()
{
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l]->initializeModel();
  }
}

void CompositeMixtureModel::initializeStep()
{
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l]->initializeStep();
  }
}

void CompositeMixtureModel::randomInit()
{}

void CompositeMixtureModel::computeProportions()
{}

// TODO implement computeNbFreeParameters
int CompositeMixtureModel::computeNbFreeParameters() const
{
  return 0;
}

void CompositeMixtureModel::imputationStep()
{
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l]->imputationStep();
  }
}

void CompositeMixtureModel::finalizeStep()
{
  for (int l = 0; l < v_mixtures_.size(); ++l)
  {
    v_mixtures_[l]->finalizeStep();
  }
}

void CompositeMixtureModel::registerMixture(IMixture& mixture)
{
  v_mixtures_.push_back(&mixture);
}


} /* namespace mixt */
