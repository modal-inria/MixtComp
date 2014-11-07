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
#include "../Various/mixt_IO.h"

namespace mixt
{

MixtureComposer::MixtureComposer( int nbSample, int nbVariable, int nbCluster)
                                : mixt::IMixtureComposerBase( nbSample, nbVariable, nbCluster)
{}

MixtureComposer::~MixtureComposer()
{
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    delete (*it);
  }
}

STK::Real MixtureComposer::lnComponentProbability(int i, int k)
{
  STK::Real sum=0.0;
  for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    sum += (*it)->lnComponentProbability(i, k);
  }
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
  {
    (*it)->paramUpdateStep();
  }
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
    (*it)->initializeStep();
  }
  setState(modelInitialized_);
}

// implement computeNbFreeParameters
int MixtureComposer::computeNbFreeParameters() const
{
  int sum = nbCluster_-1; // proportions
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    sum+= (*it)->nbFreeParameter();
  }
  return sum;
}

/* @brief Simulation of all the latent variables and/or missing data
 *  excluding class labels.
 */
void MixtureComposer::samplingStep(int ind)
{
  std::pair<int, int> range(forRange(ind));
  for (int i = range.first; i < range.second; ++i)
  {
    for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
    {
      (*it)->samplingStep(i);
    }
  }
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
      for (int i = 0; i < nbSample(); ++i)
      {
        (*it)->samplingStep(i);
      }
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

void MixtureComposer::storeShortRun(int iteration)
{
#ifdef MC_LOG
  std::stringstream fileName;
  fileName << "out/log/composer-";
  fileName << iteration;
  fileName << "-z_i.csv";
  writeDataCsv(fileName.str(), *p_zi());
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

void MixtureComposer::storeData(int sample,
                                int iteration,
                                int iterationMax)
{
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->storeData(sample,
                     iteration,
                     iterationMax);
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

void MixtureComposer::gibbsSampling(int nbGibbsIter)
{
  for (int i = 0; i < nbSample(); ++i)
  {
    for (int iterGibbs = 0; iterGibbs < nbGibbsIter; ++iterGibbs)
    {
      sStep(i);
      samplingStep(i);
      eStep(i);
      storeData(i,
                iterGibbs,
                nbGibbsIter);
    }
  }
}

} /* namespace mixt */
