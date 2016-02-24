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
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#include "mixt_MixtureComposer.h"
#include "../Mixture/mixt_IMixture.h"
#include "../IO/mixt_IO.h"
#include "../Various/mixt_Timer.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Various/mixt_Various.h"

namespace mixt
{

MixtureComposer::MixtureComposer(int nbInd,
                                 int nbClass,
                                 Real confidenceLevel) :
    idName_("z_class"),
    nbClass_(nbClass),
    nbInd_(nbInd),
    nbVar_(0),
    prop_(nbClass),
    tik_(nbInd,
         nbClass),
    sampler_(*this,
             zClassInd_,
             tik_,
             nbClass),
    paramStat_(prop_,
               confidenceLevel),
    dataStat_(zClassInd_),
    confidenceLevel_(confidenceLevel)
{
  zClassInd_.setIndClass(nbInd, nbClass);
  initializeProp(); // default values that will be overwritten either by pStep (learning), or eStepObserved (prediction)
  initializeTik();
}

MixtureComposer::~MixtureComposer()
{
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    delete (*it);
  }
}

void MixtureComposer::initializeProp()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::intializeMixtureParameters" << std::endl;
#endif
  prop_     = 1./(Real)nbClass_;
}

void MixtureComposer::initializeTik()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::intializeMixtureParameters" << std::endl;
#endif
  tik_      = 1./(Real)nbClass_;
}

Real MixtureComposer::lnObservedProbability(int i, int k)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnCompletedLikelihood(int i, int k), i: " << i << ", k: " << k << std::endl;
#endif
  Real sum = std::log(prop_[k]);

  for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    Real logProba = (*it)->lnObservedProbability(i, k);
    sum += logProba;
#ifdef MC_DEBUG
    std::cout << (*it)->idName() << ", sum: " << sum << std::endl;
#endif
  }

  return sum;
}

Real MixtureComposer::lnObservedLikelihood()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnObservedLikelihood() " << std::endl;
#endif
  Real lnLikelihood = 0.;
  Matrix<Real> lnComp(nbInd_,
                      nbClass_);

  for (int k = 0; k < nbClass_; ++k)
  {
    for (int i = 0; i < nbInd_; ++i)
    {
      lnComp(i, k) = lnObservedProbability(i, k);
    }
  }

  for (int i = 0; i < nbInd_; ++i) // sum is inside a log, hence the numerous steps for the computation
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
    std::cout << "lnComp.row(i): " << lnComp.row(i) << std::endl;
#endif
    RowVector<Real> dummy;
    lnLikelihood += dummy.logToMulti(lnComp.row(i));
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
    std::cout << "lnComp.row(i): " << lnComp.row(i) << std::endl;
    std::cout << "max: " << max << std::endl;
    std::cout << "std::log(sum): " << std::log(sum) << std::endl;
    std::cout << "lnLikelihood: " << lnLikelihood << std::endl;
#endif
  }

  return lnLikelihood;
}

Real MixtureComposer::lnCompletedLikelihood()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnSemiCompletedLikelihood() " << std::endl;
#endif
  Real lnLikelihood = 0.;

  // Compute the completed likelihood for the complete mixture model, using the completed data
  for (int i = 0; i < nbInd_; ++i) {
    lnLikelihood += lnObservedProbability(i, zClassInd_.zi().data_(i));
  }

  return lnLikelihood;
}

Real MixtureComposer::lnCompletedProbability(int i, int k)
{
  Real sum = std::log(prop_[k]);

  for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    Real logProba = (*it)->lnCompletedProbability(i, k);
    sum += logProba;
  }

  return sum;
}

void MixtureComposer::mStep(EstimatorType bias) {
  pStep(); // computation of z_ik frequencies, which correspond to ML estimator of proportions
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it) {
    (*it)->mStep(bias); // call mStep on each variable
  }
}

void MixtureComposer::sStepCheck() {
  for (int i = 0; i < nbInd_; ++i) {
    sStepCheck(i);
  }
}

void MixtureComposer::sStepCheck(int i) {
  sampler_.sStepCheck(i);
}

void MixtureComposer::sStepNoCheck() {
  for (int i = 0; i < nbInd_; ++i) {
    sStepNoCheck(i);
  }
}

void MixtureComposer::sStepNoCheck(int i) {
  sampler_.sStepNoCheck(i);
}

void MixtureComposer::eStep() {
  for (int i = 0; i < nbInd_; ++i) {
    eStep(i);
  }
}

void MixtureComposer::eStep(int i) {
  RowVector<Real> lnComp(nbClass_);
  for (int k = 0; k < nbClass_; k++) {
    lnComp[k] = lnCompletedProbability(i, k);
  }

  tik_.row(i).logToMulti(lnComp);
}

void MixtureComposer::pStep() {
  for (int i = 0; i < zClassInd_.zi().data_.rows(); ++i) {
    prop_(zClassInd_.zi().data_(i)) += 1.;
  }
  prop_ = prop_ / prop_.sum();
}

void MixtureComposer::writeParameters() const
{
#ifdef MC_VERBOSE
  std::cout << "Composer nbFreeParameter = " << nbFreeParameters() << std::endl;
  std::cout << "Composer proportions = " << prop_ << std::endl;
#endif

  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
#ifdef MC_VERBOSE
    std::cout << "Parameters of the mixtures: " << (*it)->idName() << "\n";
#endif
    (*it)->writeParameters();
  }
}

int MixtureComposer::nbFreeParameters() const
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::computeNbFreeParameters()" << std::endl;
#endif
  int sum = nbClass_ - 1; // proportions
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    sum+= (*it)->nbFreeParameter();
  }
  return sum;
}

void MixtureComposer::samplingStepCheck()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::samplingStepCheck" << std::endl;
#endif

  for (int i = 0; i < nbInd_; ++i)
  {
    samplingStepCheck(i);
  }
}

void MixtureComposer::samplingStepCheck(int i)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::samplingStep, single individual" << std::endl;
#endif
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
#ifdef MC_DEBUG
std::cout << (*it)->idName() << std::endl;
#endif
    (*it)->samplingStepCheck(i);
  }
}

void MixtureComposer::samplingStepNoCheck()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::samplingStep" << std::endl;
#endif

  for (int i = 0; i < nbInd_; ++i)
  {
    samplingStepNoCheck(i);
  }
}

void MixtureComposer::samplingStepNoCheck(int i)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::samplingStep, single individual" << std::endl;
#endif
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
#ifdef MC_DEBUG
std::cout << (*it)->idName() << std::endl;
#endif
    (*it)->samplingStepNoCheck(i);
  }
}

int MixtureComposer::checkSampleCondition(std::string* warnLog) const {
  if (warnLog == NULL) { // if no description of the error is expected, to speed the treatment
    if (checkNbIndPerClass() == 0) {
#ifdef MC_DEBUG
      std::cout << "MixtureComposer::checkSampleCondition, checkNbIndPerClass() == 0" << std::endl;
#endif

      return 0;
    }
    for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
      if ((*it)->checkSampleCondition() == 0) {
#ifdef MC_DEBUG
        std::cout << "MixtureComposer::checkSampleCondition, idName: " << (*it)->idName() << ", checkSampleCondition() == 0" << std::endl;
#endif

        return 0; // no need for log generation -> faster evaluation of checkSampleCondition
      }
    }
  }
  else { // if error description is expected
    int probaCondition = 1; // proba of condition on data given the completed data
    std::string indLog;
    probaCondition *= checkNbIndPerClass(&indLog);
    *warnLog += indLog;
    for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
      std::string mixtLog;
      int currProba = (*it)->checkSampleCondition(&mixtLog); // the global warnLog is not passed directly to the mixture, to avoid accidental wiping
      probaCondition *= currProba;
      *warnLog += mixtLog;
    }

    return probaCondition;
  }

  return 1;
}

int MixtureComposer::checkNbIndPerClass(std::string* warnLog) const
{
  for (int k = 0; k < nbClass_; ++k) {
    if (zClassInd_.classInd()(k).size() > 0) {
      continue;
    }
    else {
      if (warnLog != NULL) {
        std::stringstream sstm;
        sstm << "MixtureComposer::checkNbIndPerClass, at least one class is empty. Did you provide more individuals "
             << "that the number of classes ?" << std::endl;
        *warnLog += sstm.str();
      }

      return 0;
    }
  }

  return 1;
}

void MixtureComposer::storeSEMRun(int iteration,
                                  int iterationMax)
{
  paramStat_.sampleParam(iteration,
                         iterationMax);
  if (iteration == iterationMax)
  {
    // reinject the SEM estimated parameters into the mixture
    paramStat_.setExpectationParam();
  }
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->storeSEMRun(iteration,
                       iterationMax);
  }
}

void MixtureComposer::storeGibbsRun(int ind,
                                    int iteration,
                                    int iterationMax) {
  dataStat_.sampleVals(ind,
                       iteration,
                       iterationMax);

  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
    (*it)->storeGibbsRun(ind,
                         iteration,
                         iterationMax);
  }

  if (iteration == iterationMax) {
    dataStat_.imputeData(ind); // impute the missing values using empirical mean or mode, depending of the model
  }
}

void MixtureComposer::registerMixture(IMixture* p_mixture)
{
  v_mixtures_.push_back(p_mixture);
  ++nbVar_;
}

void MixtureComposer::gibbsSampling(int nbGibbsIter,
                                    int group,
                                    int groupMax) {
  Timer myTimer;
  myTimer.setName("Gibbs: run (individuals count as iterations)");
  for (int i = 0; i < nbInd_; ++i) {
    myTimer.iteration(i, nbInd_ - 1);
    writeProgress(group,
                  groupMax,
                  i,
                  nbInd_ - 1);

    for (int iterGibbs = 0; iterGibbs < nbGibbsIter; ++iterGibbs) {
      eStep(i);
      sStepNoCheck(i);
      samplingStepNoCheck(i);
      storeGibbsRun(i,
                    iterGibbs,
                    nbGibbsIter - 1);
    }
  }
}

std::vector<std::string> MixtureComposer::paramName() const
{
  std::vector<std::string> names(nbClass_);
  for (int k = 0; k < nbClass_; ++k)
  {
    std::stringstream sstm;
    sstm << "k: "
         << k + minModality;
    names[k] = sstm.str();
  }

  return names;
}

std::vector<std::string> MixtureComposer::mixtureName() const
{
  std::vector<std::string> names(nbVar_);
  for (int j = 0; j < nbVar_; ++j)
  {
    names[j] = v_mixtures_[j]->idName();
  }

  return names;
}

void MixtureComposer::removeMissing(initParam algo) {
  initializeTik();
  sStepNoCheck();

  for(MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it) {
    (*it)->removeMissing(algo);
  }
}

void MixtureComposer::E_kj(Matrix<Real>& ekj) const
{
  ekj.resize(nbClass_, nbVar_);
  ekj = 0.;

  for (int i = 0; i < nbInd_; ++i)
  {
    for(int j = 0; j < nbVar_; ++j)
    {
      Vector<Real> lnP(nbClass_); // ln(p(z_i = k, x_i^j))
      Vector<Real> t_ik_j(nbClass_); // p(z_i = k / x_i^j)
      for (int k = 0; k < nbClass_; ++k)
      {
        lnP(k) = std::log(prop_(k)) + v_mixtures_[j]->lnObservedProbability(i, k);
      }
      t_ik_j.logToMulti(lnP);
      Vector<Real> t_ink_j = 1. - t_ik_j; // The nj means: "all classes but k".

#ifdef MC_DEBUG
      std::cout << "MixtureComposer::E_kj, i: " << i << ", v_mixtures_[j]->idName(): " << v_mixtures_[j]->idName() << std::endl
                << "t_ik_j :" << t_ik_j.transpose() << std::endl
                << "t_ink_j:" << t_ink_j.transpose() << std::endl;
#endif

      for (int k = 0; k < nbClass_; ++k)
      {
        Real p, nP;

        if (epsilon < t_ik_j(k))
        {
          p = - t_ik_j(k) * std::log(t_ik_j (k));
        }
        else
        {
          p = 0.;
        }

        if (epsilon < t_ink_j(k))
        {
          nP = - t_ink_j(k) * std::log(t_ink_j(k));
        }
        else
        {
          nP = 0.;
        }

        ekj(k, j) += p + nP;
      }
    }
  }
}

void MixtureComposer::IDClass(Matrix<Real>& idc) const
{
  idc.resize(nbClass_, nbVar_);
  Matrix<Real> ekj;
  E_kj(ekj);

  for (int k = 0; k < nbClass_; ++k)
  {
    Real min = ekj.row(k).minCoeff();
    Real max = ekj.row(k).maxCoeff();

    for(int j = 0; j < nbVar_; ++j)
    {
      idc(k, j) = (max - ekj(k, j)) / (max - min);
//      idc(k, j) = 1. - ekj(k, j) / ekj.row(k).sum();
    }
  }
}

void MixtureComposer::printClassInd() const {
  zClassInd_.printState();
}

} /* namespace mixt */
