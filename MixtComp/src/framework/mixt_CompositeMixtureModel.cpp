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
                                             ,v_mixtures_(model.v_mixtures_)
{

}

CompositeMixtureModel::~CompositeMixtureModel()
{
  // TODO Auto-generated destructor stub
}

virtual STK::IMixtureModelBase* CompositeMixtureModel::create()
{
  CompositeMixtureModel* newComp =  new CompositeMixtureModel(*this);
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    newComp->v_mixtures_[l] = (v_mixtures_[l]->clone());
    newComp->v_mixtures_[l]->initializeModel();
  }
  return newComp;
}

virtual STK::IMixtureModelBase* CompositeMixtureModel::clone()
{
  CompositeMixtureModel* newComp =  new CompositeMixtureModel(*this);
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    newComp->v_mixtures_[l] = (v_mixtures_[l]->clone());
  }
  return newComp;
}

virtual STK::Real CompositeMixtureModel::lnComponentProbability(int i, int k)
{
  STK::Real sum=0.0;
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    sum += v_mixtures_[l]->lnComponentProbability(i,k);
  }
  return sum;
}

virtual void CompositeMixtureModel::mStep()
{
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    v_mixtures_[l]->paramUpdateStep();
  }
}

virtual void CompositeMixtureModel::writeParameters(std::ostream& os) const
{
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    v_mixtures_[l]->writeParameters(std::ostream& os);
  }
}

virtual void CompositeMixtureModel::initializeModel()
{
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    v_mixtures_[l]->initializeModel();
  }
}

virtual void CompositeMixtureModel::initializeStep()
{
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    v_mixtures_[l]->initializeStep();
  }
}

virtual bool CompositeMixtureModel::randomInit()
{
  return true;
}

virtual void CompositeMixtureModel::computeProportions()
{}

virtual void CompositeMixtureModel::imputationStep()
{
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    v_mixtures_[l]->imputationStep(); // no coupling between ingredients for the imputation step ?
  }
}

virtual void CompositeMixtureModel::finalizeStep()
{
  for (int l = 0; l < v_mixtures_.end(); ++l)
  {
    v_mixtures_[l]->finalizeStep();
  }
}

} /* namespace mixt */
