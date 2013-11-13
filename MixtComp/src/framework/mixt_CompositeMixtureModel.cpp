/*
 * mixt_CompositeMixture.cpp
 *
 *  Created on: 16 oct. 2013
 *      Author: iovleff
 */

#include "mixt_CompositeMixtureModel.h"
#include "../mixtures/StkppMixtures/mixt_GammaMixture.h"

namespace mixt
{

CompositeMixtureModel::CompositeMixtureModel(DataHandler* p_dataHandler, int nbCluster)
                           : STK::IMixtureModelBase(nbCluster)
                           , p_dataHandler_(p_dataHandler)
                           , nbClusters_(nbCluster)
{
  createMixtureParameters();
}

CompositeMixtureModel::CompositeMixtureModel(DataHandler* p_dataHandler, int nbCluster, int nbSamples)
                           : STK::IMixtureModelBase(nbCluster,nbSamples)
                           , p_dataHandler_(p_dataHandler)
                           , nbClusters_(nbCluster)
{
  createMixtureParameters();
}

CompositeMixtureModel::CompositeMixtureModel(CompositeMixtureModel const& model)
                           : STK::IMixtureModelBase(model)
                           , p_dataHandler_(model.p_dataHandler_)
                           , v_mixtures_(model.v_mixtures_)
                           , nbClusters_(model.nbClusters_)
{

}

CompositeMixtureModel::~CompositeMixtureModel()
{
  for (int i = 0 ; i < v_mixtures_.size() ; i++)
    delete v_mixtures_[i];
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
    sum += v_mixtures_[l]->lnComponentProbability(i,k);
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
  std::cout << "Composer lnLikelihood = " << lnLikelihood() << std::endl;
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

void CompositeMixtureModel::registerMixture(IMixture* mixture)
{
  v_mixtures_.push_back(mixture);
}

void CompositeMixtureModel::createMixtures(std::string filename, char sep)
{
  std::vector<MixtureParam> v_Mixture_Param = p_dataHandler_->readMixtureParam(filename, sep);
  v_mixtures_.resize(v_Mixture_Param.size());

  for (int l = 0 ; l < v_Mixture_Param.size() ; ++l)
  {
    if (v_Mixture_Param[l].mixtureType_ == "Gamma_ajk_bjk")
      v_mixtures_[l] = new Gamma_ajk_bjk ( v_Mixture_Param[l].firstIndex_
                                         , v_Mixture_Param[l].lastIndex_
                                         , nbClusters_
                                         , this);
  }
}

void CompositeMixtureModel::setData()
{
  for (int l = 0; l < v_mixtures_.size(); ++l)
    v_mixtures_[l]->setData();
}

} /* namespace mixt */
