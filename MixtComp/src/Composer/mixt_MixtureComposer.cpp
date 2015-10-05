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
    classInd_(nbClass),
    sampler_(*this,
             zi_,
             tik_,
             nbClass),
    paramStat_(prop_,
               confidenceLevel),
    dataStat_(zi_),
    confidenceLevel_(confidenceLevel)
{
  zi_.resizeArrays(nbInd);
  intializeMixtureParameters(); // default values that will be overwritten either by pStep (learning), or eStepObserved (prediction)
}

MixtureComposer::~MixtureComposer()
{
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    delete (*it);
  }
}

void MixtureComposer::intializeMixtureParameters()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::intializeMixtureParameters" << std::endl;
#endif
  prop_     = 1./(Real)nbClass_;
  tik_      = 1./(Real)nbClass_;
  zi_.data_ = 0;
  updateListInd();
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

void MixtureComposer::lnObservedLikelihoodDebug()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnObservedLikelihoodDebug() " << std::endl;
#endif

  for (int i = 0; i < nbInd_; ++i)
  {
    for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
    {
      if ((*it)->lnObservedProbability(i, zi_.data_(i)) == minInf)
      {
        std::cout << "MixtureComposer::lnObservedLikelihoodDebug, minInf, i: " << i << ", zi_.data_(i): " << zi_.data_(i) << ", idName: " << (*it)->idName() << std::endl;
      }
    }
  }
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

Real MixtureComposer::lnSemiCompletedLikelihood()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnSemiCompletedLikelihood() " << std::endl;
#endif
  Real lnLikelihood = 0.;

  // Compute the completed likelihood for the complete mixture model, using the completed data
  for (int i = 0; i < nbInd_; ++i)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << ", zi_[i]: " << zi_[i] << ", lnComp(i, zi_[i]): " << lnComp(i, zi_[i]) << std::endl;
    std::cout << "lnComp.row(i): " << lnComp.row(i) << std::endl;
#endif
    lnLikelihood += lnObservedProbability(i, zi_.data_(i));
  }

  return lnLikelihood;
}

Real MixtureComposer::lnCompletedProbability(int i, int k)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnCompletedLikelihood(int i, int k), i: " << i << ", k: " << k << std::endl;
  std::cout << "prop_.size(): " << prop_.size() << std::endl;
#endif
  Real sum = std::log(prop_[k]);

  for (ConstMixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    Real logProba = (*it)->lnCompletedProbability(i, k);
    sum += logProba;
#ifdef MC_DEBUG
    std::cout << (*it)->idName() << ", sum: " << sum << std::endl;
#endif
  }

  return sum;
}

Real MixtureComposer::lnCompletedLikelihood()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnCompletedLikelihood() " << std::endl;
#endif
  Real lnLikelihood = 0.;

  // Compute the completed likelihood for the complete mixture model, using the completed data
  for (int i = 0; i < nbInd_; ++i)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
    std::cout << "lnComp.row(i): " << lnComp.row(i) << std::endl;
#endif
    lnLikelihood += lnCompletedProbability(i, zi_.data_(i));
  }

  return lnLikelihood;
}

void MixtureComposer::mStep()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::mStep()" << std::endl;
#endif
  pStep(); // computation of z_ik frequencies, which correspond to ML estimator of proportions
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    (*it)->mStep(); // call mStep on each variable
  }
#ifdef MC_DEBUG
  std::cout << "\twarn: " << warn << std::endl;
#endif
}

void MixtureComposer::sStepCheck()
{
  for (int i = 0; i < nbInd_; ++i)
  {
    sStepCheck(i);
  }

#ifdef MC_DEBUG
  std::cout << "MixtureComposer::sStep" << std::endl;
  std::cout << "zi_.data_: " << zi_.data_.transpose() << std::endl;
  printClassInd();
#endif
}

void MixtureComposer::sStepCheck(int i)
{
  classInd_(zi_.data_(i)).erase(i);
  sampler_.sStepCheck(i);
  classInd_(zi_.data_(i)).insert(i);
}

void MixtureComposer::sStepNoCheck()
{
  for (int i = 0; i < nbInd_; ++i)
  {
    sStepNoCheck(i);
  }

#ifdef MC_DEBUG
  std::cout << "MixtureComposer::sStepNoCheck" << std::endl;
  std::cout << "tik_: " << tik_.transpose() << std::endl;
  std::cout << "zi_.data_: " << zi_.data_.transpose() << std::endl;
  printClassInd();

#endif
}

void MixtureComposer::sStepNoCheck(int i)
{
  classInd_(zi_.data_(i)).erase(i);
  sampler_.sStepNoCheck(i);
  classInd_(zi_.data_(i)).insert(i);
}

void MixtureComposer::eStep()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::eStep" << std::endl;
  std::cout << "prop_: " << prop_ << std::endl;
#endif
  for (int i = 0; i < nbInd_; ++i)
  {
    eStep(i);
  }
#ifdef MC_DEBUG
  std::cout << "tik_:" << std::endl;
  std::cout << tik_ << std::endl;
#endif
}

void MixtureComposer::eStep(int i)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::eStep(i), i: " << i << std::endl;
#endif
  RowVector<Real> lnComp(nbClass_);
  for (int k = 0; k < nbClass_; k++)
  {
    lnComp[k] = lnCompletedProbability(i, k);
  }

#ifdef MC_DEBUG
    std::cout << "lnComp: " << lnComp << std::endl;
#endif

  tik_.row(i).logToMulti(lnComp);

#ifdef MC_DEBUG
  std::cout << "\tmax: " << max << ", sum2: " << sum2 << std::endl;
  std::cout << "tik_.row(i): " << tik_.row(i) << std::endl;
#endif
}

void MixtureComposer::pStep()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::pStep" << std::endl;
#endif
  for (int i = 0; i < zi_.data_.rows(); ++i)
  {
    prop_[zi_.data_(i)] += 1.;
  }
  prop_ = prop_ / prop_.sum();
#ifdef MC_DEBUG
  std::cout << "\tprop_: " << prop_ << std::endl;
#endif
}

void MixtureComposer::mapStep()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::mapStep" << std::endl;
#endif
  for (int i = 0; i < nbInd_; ++i)
  {
    mapStep(i);
  }
}

void MixtureComposer::mapStep(int i)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::mapStep, single individual" << std::endl;
#endif
  int k;
  tik_.row(i).maxCoeff(&k);
  zi_.data_(i) = k;
  updateListInd();
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
  std::cout << "MixtureComposer::samplingStep" << std::endl;
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

int MixtureComposer::checkSampleCondition(std::string* warnLog) const
{
  int probaCondition = 1; // proba of condition on data given the completed data
  if (warnLog == NULL)
  {
    probaCondition *= checkNbIndPerClass();
    for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
    {
      int currProba = (*it)->checkSampleCondition(); // no need for log generation -> faster evaluation of checkSampleCondition

#ifdef MC_DEBUG
      if (currProba == 0)
      {
        std::cout << "MixtureComposer::checkSampleCondition, (*it)->idName(): " << (*it)->idName() << " has a 0 checkSampleCondition" << std::endl;
      }
#endif

      probaCondition *= currProba;
    }
  }
  else
  {
    std::string indLog;
    probaCondition *= checkNbIndPerClass(&indLog);
    *warnLog += indLog;
    for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
    {
      std::string mixtLog;
      probaCondition *= (*it)->checkSampleCondition(&mixtLog); // the global warnLog is not passed directly to the mixture, to avoid accidental wiping
      *warnLog += mixtLog;
    }
  }
  return probaCondition;
}

int MixtureComposer::checkNbIndPerClass(std::string* warnLog) const
{
  int proba = 1;
  Vector<int> nbIndPerClass(nbClass_, 0);
  for (int i = 0; i < nbInd_; ++i)
  {
#ifdef MC_DEBUG
    if (zi_.data_(i) < 0)
    {
      std::cout << "MixtureComposer::checkNbIndPerClass, i: " << i << ", zi_.data_(i): " << zi_.data_(i) << ", tik(i): " << tik_.row(i) << std::endl;
    }
#endif
    nbIndPerClass(zi_.data_(i)) += 1;
  }
  int min = nbIndPerClass.minCoeff();
  if (min < minIndPerClass)
  {
    if (warnLog == NULL)
    {
      proba = 0;
    }
    else
    {
      std::stringstream sstm;
      sstm << "MixtureComposer::checkNbIndPerClass, there must be at least " << minIndPerClass << " individuals "
           << "per class. This condition has not been met. Did you provide enough individuals in regard to the "
           << "number of classes you required ?" << std::endl;
      *warnLog += sstm.str();
      proba = 0;
    }
  }

  return proba;
}

void MixtureComposer::storeSEMBurnIn(int iteration,
                                     int iterationMax)
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
    (*it)->storeSEMBurnIn(iteration,
                          iterationMax);
  }
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

void MixtureComposer::storeGibbsRun(int sample,
                                    int iteration,
                                    int iterationMax)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::storeGibbsRun" << std::endl;
  std::cout << "sample: " << sample << ", iteration: " << iteration << ", iterationMax: " << iterationMax << std::endl;
#endif

  dataStat_.sampleVals(sample,
                       iteration,
                       iterationMax);

  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
#ifdef MC_DEBUG
    std::cout << (*it)->idName() << std::endl;
#endif
    (*it)->storeGibbsRun(sample,
                         iteration,
                         iterationMax);
  }
}

void MixtureComposer::registerMixture(IMixture* p_mixture)
{
  v_mixtures_.push_back(p_mixture);
  ++nbVar_;
}

void MixtureComposer::gibbsSampling(int nbGibbsIter,
                                    int group,
                                    int groupMax)
{
  Timer myTimer;
  myTimer.setName("Gibbs: run (individuals count as iterations)");
  for (int i = 0; i < nbInd_; ++i)
  {
    myTimer.iteration(i, nbInd_ - 1);
    writeProgress(group,
                  groupMax,
                  i,
                  nbInd_ - 1);
    for (int iterGibbs = 0; iterGibbs < nbGibbsIter; ++iterGibbs)
    {
      eStep(i);
      sStepNoCheck(i);
      samplingStepNoCheck(i);
      storeGibbsRun(i,
                    iterGibbs,
                    nbGibbsIter - 1);
    }
  }
  mapStep(); // z_i estimated by the mode at the end of the Gibbs Sampling
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

void MixtureComposer::removeMissing()
{
  for(MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->removeMissing();
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
        ekj(k, j) += - t_ik_j (k) * std::log(t_ik_j (k))
                     - t_ink_j(k) * std::log(t_ink_j(k));
      }
    }
  }
}

void MixtureComposer::IDClass(Matrix<Real>& idc) const
{
  idc.resize(nbClass_, nbVar_);
  Matrix<Real> ekj;
  E_kj(ekj);
  Vector<Real> sum = ekj.colwise().sum();

  for(int j = 0; j < nbVar_; ++j)
  {
    for (int k = 0; k < nbClass_; ++k)
    {
      idc(k, j) = 1. - ekj(k, j) / sum(k);
    }
  }

#ifdef MC_DEBUG
  std::cout << "MixtureComposer::IDClass" << std::endl;
  std::cout << "ekj" << std::endl;
  std::cout << ekj << std::endl;
  std::cout << "sum" << std::endl;
  std::cout << sum << std::endl;
  std::cout << "idc" << std::endl;
  std::cout << idc << std::endl;
#endif
}

/** Use the zi to compute a vector with one element per class, each element contains
 * the indices of individuals belonging to this class */
void MixtureComposer::updateListInd()
{
  for (int k = 0; k < nbClass_; ++k)
  {
    classInd_(k).clear();
  }

  for (int i = 0; i < nbInd_; ++i)
  {
    classInd_(zi_.data_(i)).insert(i);
  }
}

void MixtureComposer::printClassInd() const
{
  for (int k = 0; k < nbClass_; ++k)
  {
    std::cout << "k: " << k << ",";
    for (std::set<int>::const_iterator it = classInd_(k).begin(), itEnd = classInd_(k).end();
         it != itEnd;
         ++it)
    {
      std::cout << " " << *it;
    }
    std::cout << std::endl;
  }
}

} /* namespace mixt */
