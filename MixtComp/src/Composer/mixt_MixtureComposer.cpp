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

MixtureComposer::MixtureComposer(int nbSample,
                                 int nbVariable,
                                 int nbCluster,
                                 Real confidenceLevel) :
    nbCluster_(nbCluster),
    nbSample_(nbSample),
    prop_(nbCluster),
    tik_(nbSample, nbCluster),
    sampler_(zi_,
            tik_,
            nbCluster),
    idName_("z_class"),
    paramStat_(prop_,
               confidenceLevel),
    dataStat_(zi_,
              confidenceLevel),
    confidenceLevel_(confidenceLevel)
{
  zi_.resizeArrays(nbSample);
  intializeMixtureParameters(); // default values that will be overwritten either by pStep (learning), or eStepObserved (prediction)
}

MixtureComposer::~MixtureComposer()
{
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    delete (*it);
  }
}

/* Create the parameters of the  mixture model. */
void MixtureComposer::intializeMixtureParameters()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::intializeMixtureParameters" << std::endl;
#endif
  prop_     = 1./(Real)nbCluster_;
  tik_      = 1./(Real)nbCluster_;
  zi_.data_ = 0;
}

/** Compute the observed likelihood for a single individual, with the class fixed as a parameter */
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

/** Observed data, and marginalization over classes
 * @return the value of the observed likelihood */
void MixtureComposer::lnObservedLikelihoodDebug()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnObservedLikelihoodDebug() " << std::endl;
#endif

  for (int i = 0; i < nbSample_; ++i)
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

/** Observed data, and marginalization over classes
 * @return the value of the observed likelihood */
Real MixtureComposer::lnObservedLikelihood()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnObservedLikelihood() " << std::endl;
#endif
  Real lnLikelihood = 0.;
  Matrix<Real> lnComp(nbSample_,
                      nbCluster_);

  for (int k = 0; k < nbCluster_; ++k)
  {
    for (int i = 0; i < nbSample_; ++i)
    {
      lnComp(i, k) = lnObservedProbability(i, k);
    }
  }

  for (int i = 0; i < nbSample_; ++i) // sum is inside a log, hence the numerous steps for the computation
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
    std::cout << "lnComp.row(i): " << lnComp.row(i) << std::endl;
#endif
    RowVector<Real> dummy;
    lnLikelihood += lnComp.row(i).logToMulti(dummy);
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

/** Completed latent classes, and marginalization over missing data
 * @return the value of the semi-completed likelihood (completion only for latent class) */
Real MixtureComposer::lnSemiCompletedLikelihood()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnSemiCompletedLikelihood() " << std::endl;
#endif
  Real lnLikelihood = 0.;

  // Compute the completed likelihood for the complete mixture model, using the completed data
  for (int i = 0; i < nbSample_; ++i)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << ", zi_[i]: " << zi_[i] << ", lnComp(i, zi_[i]): " << lnComp(i, zi_[i]) << std::endl;
    std::cout << "lnComp.row(i): " << lnComp.row(i) << std::endl;
#endif
    lnLikelihood += lnObservedProbability(i, zi_.data_(i));
  }

  return lnLikelihood;
}

/** Compute the completed likelihood for a single individual, with the class fixed as a parameter */
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

/** Completed on both missing value and latent classes
 * @return the value of the completed likelihood for all data */
Real MixtureComposer::lnCompletedLikelihood()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::lnCompletedLikelihood() " << std::endl;
#endif
  Real lnLikelihood = 0.;

  // Compute the completed likelihood for the complete mixture model, using the completed data
  for (int i = 0; i < nbSample_; ++i)
  {
#ifdef MC_DEBUG
    std::cout << "i: " << i << std::endl;
    std::cout << "lnComp.row(i): " << lnComp.row(i) << std::endl;
#endif
    lnLikelihood += lnCompletedProbability(i, zi_.data_(i));
  }

  return lnLikelihood;
}

std::string MixtureComposer::mStep()
{
  std::string warn;
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::mStep()" << std::endl;
#endif
  pStep(); // computation of the proportions, mStep for mixture parameters
  for (MixtIterator it = v_mixtures_.begin() ; it != v_mixtures_.end(); ++it)
  {
    warn += (*it)->mStep(); // call mStep on each variable
  }
#ifdef MC_DEBUG
  std::cout << "\twarn: " << warn << std::endl;
#endif
  return warn;
}

std::string MixtureComposer::sStepNbAttempts(int nbSamplingAttempts)
{
  std::string warnLog;
  for (int iterSample = 0; iterSample < nbSamplingAttempts; ++iterSample) // sample until there are enough individuals per class, using default tik from IMixtureComposerBase::intializeMixtureParameters()
  {
    int nbIndPerClass = sStep();
  #ifdef MC_DEBUG
    std::cout << "MixtureComposer::sStepNbAttempts, iterSample: " << iterSample << std::endl;
    std::cout << "nbIndPerClass: " << nbIndPerClass << std::endl;
  #endif
    if (nbIndPerClass > minIndPerClass)
    {
      break; // enough individuals in each class to carry on
    }
    else
    {
      if (iterSample == nbSamplingAttempts - 1) // on last attempt, detail the error in the error message
      {
        std::stringstream sstm;
        sstm << "Sampling step problem in SEM. The class with the lowest number "
             << "of individuals has " << nbIndPerClass << " individuals. Each class must have at least "
             << minIndPerClass << " individuals. There has been " << nbSamplingAttempts
             << " partition samplings before failure. The number of classes might be too important"
             << " relative to the number of individuals. Try decreasing the number of classes." << std::endl;
        warnLog += sstm.str();
      }
    }
  }
  return warnLog;
}

/* simulate zi for all individuals */
int MixtureComposer::sStep()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::sStep" << std::endl;
#endif
  // simulate zi
  for (int i = 0; i < nbSample_; ++i)
  {
    sStep(i);
  }
#ifdef MC_DEBUG
  std::cout << "zi_.data_: " << std::endl;
  std::cout << zi_.data_ << std::endl;
#endif
  Vector<int> indPerClass(nbCluster_);
  indPerClass = 0;
  for (int i = 0; i < nbSample_; ++i)
  {
#ifdef MC_DEBUG
  std::cout << "i: " << i << ", zi_.data_(i): " << zi_.data_(i) << std::endl;
  std::cout << "zi_.data_.size(): " << zi_.data_.size() << ", indPerClass.size(): " << indPerClass.size() << std::endl;
#endif
    indPerClass(zi_.data_(i)) += 1;
  }
  int minIndPerClass = indPerClass.minCoeff();
#ifdef MC_DEBUG
  std::cout << "\tindPerClass: " << indPerClass << std::endl;
  std::cout << "\tminIndPerClass: " << minIndPerClass << std::endl;
#endif
  return minIndPerClass;
}

/* simulate zi for a particular individual */
void MixtureComposer::sStep(int i)
{
  sampler_.sampleIndividual(i);
}

/* compute Tik, for all individuals */
void MixtureComposer::eStep()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::eStep" << std::endl;
  std::cout << "prop_: " << prop_ << std::endl;
#endif
  for (int i = 0; i < nbSample_; ++i)
  {
    eStep(i);
  }
#ifdef MC_DEBUG
  std::cout << "tik_:" << std::endl;
  std::cout << tik_ << std::endl;
#endif
}

/* compute Tik, for a particular individual */
void MixtureComposer::eStep(int i)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::eStep(i), i: " << i << std::endl;
#endif
  RowVector<Real> lnComp(nbCluster_);
  for (int k = 0; k < nbCluster_; k++)
  {
    lnComp[k] = lnCompletedProbability(i, k);
  }

#ifdef MC_DEBUG
    std::cout << "lnComp: " << lnComp << std::endl;
#endif

  lnComp.logToMulti(tik_.row(i));

#ifdef MC_DEBUG
  std::cout << "\tmax: " << max << ", sum2: " << sum2 << std::endl;
  std::cout << "tik_.row(i): " << tik_.row(i) << std::endl;
#endif
}

/* Compute prop using the ML estimator, default implementation. */
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

/* Compute Zi using the Map estimator,default implementation. */
void MixtureComposer::mapStep()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::mapStep" << std::endl;
#endif
  for (int i = 0; i < nbSample_; ++i)
  {
    mapStep(i);
  }
}

/* Compute Zi using the Map estimator, default implementation. */
void MixtureComposer::mapStep(int i)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::mapStep, single individual" << std::endl;
#endif
  int k;
  tik_.row(i).maxCoeff(&k);
  zi_.data_(i) = k;
}

void MixtureComposer::writeParameters(std::ostream& out) const
{
  out << "Composer nbFreeParameter = " << nbFreeParameters() << std::endl;
  out << "Composer proportions = " << prop_ << std::endl;

  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    out << "Parameters of the mixtures: " << (*it)->idName() << "\n";
    (*it)->writeParameters(out);
  }
}

// implement computeNbFreeParameters
int MixtureComposer::nbFreeParameters() const
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::computeNbFreeParameters()" << std::endl;
#endif
  int sum = nbCluster_ - 1; // proportions
  for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    sum+= (*it)->nbFreeParameter();
  }
  return sum;
}

/* @brief Simulation of all the latent variables and/or missing data
 *  excluding class labels.
 */
void MixtureComposer::samplingStep()
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::samplingStep" << std::endl;
#endif
  for (int i = 0; i < nbSample_; ++i)
  {
    samplingStep(i);
  }
}

void MixtureComposer::samplingStep(int i)
{
#ifdef MC_DEBUG
  std::cout << "MixtureComposer::samplingStep, single individual" << std::endl;
#endif
  for (MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
#ifdef MC_DEBUG
std::cout << (*it)->idName() << std::endl;
#endif
    (*it)->samplingStep(i);
  }
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

/* register the mixture in the composer*/
void MixtureComposer::registerMixture(IMixture* p_mixture)
{
  v_mixtures_.push_back(p_mixture);
}

void MixtureComposer::gibbsSampling(int nbGibbsIter,
                                    int group,
                                    int groupMax)
{
  Timer myTimer;
  myTimer.setName("Gibbs: run (individuals count as iterations)");
  for (int i = 0; i < nbSample_; ++i)
  {
    myTimer.iteration(i, nbSample_ - 1);
    writeProgress(group,
                  groupMax,
                  i,
                  nbSample_ - 1);
    for (int iterGibbs = 0; iterGibbs < nbGibbsIter; ++iterGibbs)
    {
      eStep(i);
      sStep(i);
      samplingStep(i);
      storeGibbsRun(i,
                    iterGibbs,
                    nbGibbsIter - 1);
    }
  }
  mapStep(); // z_i estimated by the mode at the end of the Gibbs Sampling
}

std::vector<std::string> MixtureComposer::paramNames() const
{
  std::vector<std::string> names(nbCluster_);
  for (int k = 0; k < nbCluster_; ++k)
  {
    std::stringstream sstm;
    sstm << "k: "
         << k + minModality;
    names[k] = sstm.str();
  }

  return names;
}

std::string MixtureComposer::eStepObserved()
{
  std::string warnLog; // warnlog to be output

  RowVector<Real> lnComp(nbCluster_);
  for (int i = 0; i < nbSample_; ++i)
  {
    std::string temWarnLog; // store the names of potentially problematic variables for this individual
    for (int k = 0; k < nbCluster_; k++)
    {
      lnComp(k) = 0.;
      for(MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
      {
        if ((*it)->possibleNullProbability() == true) // observed likelihood only computed when necessary
        {
          Real lnObservedProbability = (*it)->lnObservedProbability(i, k);
          if (lnObservedProbability == minInf)
          {
            std::stringstream sstm;
            sstm << "Class: " << k << ", variable: " << (*it)->idName() << std::endl;
            temWarnLog += sstm.str();
          }
          lnComp(k) += lnObservedProbability;
        }
      }
    }

    Real lnCompMax = lnComp.maxCoeff();
    if (lnCompMax != minInf) // the individual can be generated by the model
    {
      lnComp -= lnCompMax;
      lnComp = lnComp.exp();
      Real sum = lnComp.sum();
      tik_.row(i) = lnComp / sum;
    }
    else // computation can not continue if the individual remains
    {
      std::stringstream sstm;
      sstm << "Individual: " << i << " (considering 0-based numbering) can not be generated from the models with the current parameters."
           << " In each class, there is at least one variable for which the observed value has a zero probability."
           << " Here is a list of the problematic classe / variable couples: " << std::endl
           << temWarnLog
           << " You can either try a new combination of models for the variables, or remove that particular individual." << std::endl;
      warnLog += sstm.str();
    }
  }

  return warnLog;
}

void MixtureComposer::removeMissing()
{
  for(MixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
  {
    (*it)->removeMissing();
  }
}

} /* namespace mixt */
