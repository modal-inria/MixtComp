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

#include "mixt_MixtureComposer.h"
#include "../Mixture/mixt_IMixture.h"
#include "Arrays/include/STK_Display.h"
#include "Clustering/include/STK_Clust_Util.h"
#include "../Various/mixt_Export.h"
#include "STatistiK/include/STK_Law_Uniform.h"
#include "../Various/mixt_Constants.h"

namespace mixt
{

MixtureComposer::MixtureComposer( int nbSample, int nbVariable, int nbCluster)
                                : STK::IMixtureComposerBase( nbSample, nbVariable, nbCluster)
{}

MixtureComposer::MixtureComposer( MixtureComposer const& composer)
                                : STK::IMixtureComposerBase(composer)
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

STK::Real MixtureComposer::lnComponentProbability(int i, int k)
{
  STK::Real sum=0.0;
  for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  { sum += (*it)->lnComponentProbability(i,k);}
  return sum;
}

STK::Real MixtureComposer::lnObservedLikelihood()
{
  STK::Real lnLikelihood = 0.;
  STK::Array2D<STK::Real> lnComp(nbSample(), nbCluster_, 0.);
  for (int k = 0; k < nbCluster_; ++k)
  {
    for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
    {
      STK::Array2DVector<STK::Real> tempVec(lnComp.col(k), true);
      (*it)->lnObservedLikelihood(&tempVec, k);
      // (*it)->lnObservedLikelihood(&lnComp.col(k), k);
    }
  }

  // Compute the observed likelihood for the complete mixture model
  for (int i = 0; i < nbSample(); ++i)
  {
    STK::Real max = lnComp.row(i).maxElt();
    STK::Real sum = (lnComp.row(i) -= max).exp().dot(prop_);
    lnLikelihood += max + std::log(sum);
  }

  return lnLikelihood;
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
  stk_cout << _T("Composer proportions = ") << *(this->p_pk()) << std::endl;

  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    stk_cout << _T("Parameters of the mixtures:") << (*it)->idName() << _T("\n");
    (*it)->writeParameters(os);
  }
}

void MixtureComposer::initializeStep()
{
  if (v_mixtures_.size() == 0)
    std::cout << "MixtureComposer::initializeStep, no mixture have been registered" << std::endl;
  // compute number of free parameters
  setNbFreeParameter(computeNbFreeParameters());
  // compute proportions
  pStep();
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::initializeStep() called on " << v_mixtures_.size() << " mixtures" << std::endl;
#endif
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
#ifdef MC_DEBUG
    std::cout << "(*it)->idName(): " << (*it)->idName() << std::endl;
    try
    {
#endif
      (*it)->initializeStep();
#ifdef MC_DEBUG
    }
    catch (STK::Clust::exceptions exception)
    {
      if (exception == STK::Clust::initializeStepFail_)
        std::cout << "STK mixture initialization failed" << std::endl;
    }
#endif
  }
  setState(STK::Clust::modelInitialized_);
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

void MixtureComposer::misClasStep(int iteration)
{
  STK::Array2D<STK::Real> probClass(nbSample(), nbCluster(), 0.);

  // computation of the log probability with adequately sampled missing values
  for (int k = 0; k < nbCluster(); ++k)
  {
    zi_ = k; // setting zi_ for the sampling step
    for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
    {
      (*it)->samplingStep();
    }
    for (int i = 0; i < nbSample(); ++i)
    {
      probClass(i, k) = lnComponentProbability(i, k);
    }
  }

  // equivalent of the estep to compute new tik_
  for (int i = tik_.firstIdxRows(); i <= tik_.lastIdxRows(); ++i)
  {
    STK::Array2DPoint<STK::Real> lnComp;
    lnComp = probClass.row(i);
    int kmax;
    STK::Real max = lnComp.maxElt(kmax);
    zi_.elt(i) = kmax;
    // compute sum_k pk exp{lnCom_k - lnComp_kmax}
    STK::Real sum2 =  (lnComp -= max).exp().dot(prop_);
    // compute likelihood of each sample for each component
    tik_.row(i) = (prop_ * lnComp.exp()) / sum2;
  }
}

void MixtureComposer::misClasStepInit(int iteration)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::misClasStepInit" << std::endl;
#endif
  STK::Array2DVector<STK::Real> proba(nbSample());
  STK::Array2DVector<bool> reInit(nbSample(), false);

  // compute the marginal probability to sample a particular individual among others
  for (int i = tik_.firstIdxRows(); i<= tik_.lastIdxRows(); ++i)
  {
    STK::Array2DPoint<STK::Real> lnComp(tik_.cols());
    for (int k = tik_.firstIdxCols(); k <= tik_.lastIdxCols(); ++k)
    {
      lnComp[k] = lnComponentProbability(i,k);
    }
    proba[i] =  lnComp.exp().dot(prop_);
  }
  proba = proba / proba.sum();

  // select samples to be reinitialized, and uniformize t_ik
  for (int i = tik_.firstIdxRows(); i<= tik_.lastIdxRows(); ++i)
  {
    STK::Real probaOutOfSample = pow(1. - proba[i], nbSample());
    STK::Real sampleVal = STK::Law::Uniform::rand(0., 1.);
#ifdef MC_DEBUG
    std::cout << "\ti: " << i << std::endl;
    std::cout << "\tproba[i]: " << proba[i] << std::endl;
    std::cout << "\tprobaOutOfSample: " << probaOutOfSample << std::endl;
    std::cout << "\tsampleVal: " << sampleVal << std::endl;
#endif
    if (sampleVal < probaOutOfSample)
//    if (probaOutOfSample > probaNonMisClass)
    {
      tik_.row(i) = 1. / nbCluster();
      reInit[i] = true;
    }
  }

  // reinitialize missing values
  const STK::Array2DVector<bool>* p_reInit = &reInit;
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->initIndividual(p_reInit);
  }
}

void MixtureComposer::storeShortRun(int iteration)
{
#ifdef MC_LOG
  std::stringstream fileName;
  fileName << "out/log/composer-";
  fileName << iteration;
  fileName << "-z_i.csv";
  writeDataCsv(fileName.str(), p_zi());
#endif
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->storeShortRun(iteration);
  }
}

void MixtureComposer::storeLongRun(int iteration)
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->storeLongRun(iteration);
  }
}

void MixtureComposer::storeData()
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->storeData();
  }
}

void MixtureComposer::exportDataParam() const
{
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->exportDataParam();
  }
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

} /* namespace mixt */
