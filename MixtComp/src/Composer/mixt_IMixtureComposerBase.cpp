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
 *  Created on: Nov 14, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

#include <cmath>
#include "mixt_IMixtureComposerBase.h"
#include "../LinAlg/mixt_LinAlg.h"
#include "../Statistic/mixt_MultinomialStatistic.h"

namespace mixt
{
IMixtureComposerBase::IMixtureComposerBase(int nbSample,
                                           int nbCluster) :
    nbCluster_(nbCluster),
    nbSample_(nbSample),
    prop_(nbCluster),
    tik_(nbSample, nbCluster),
    sampler_(zi_,
             tik_,
             nbCluster),
    idName_("z_class")
{
  zi_.resizeArrays(nbSample);
  intializeMixtureParameters(); // default values that will be overwritten either by pStep (learning), or setDataParam (prediction)
}

/* copy constructor */
IMixtureComposerBase::IMixtureComposerBase( IMixtureComposerBase const& model) :
    nbCluster_(model.nbCluster_),
    prop_(model.prop_),
    tik_(model.tik_),
    zi_(model.zi_),
    sampler_(model.sampler_)
{}
/* destructor */
IMixtureComposerBase::~IMixtureComposerBase() {}

/* simulate zi for all individuals */
int IMixtureComposerBase::sStep()
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::sStep" << std::endl;
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
  std::cout << "i: " << i << ", zi_[i]: " << zi_[i] << std::endl;
#endif
    indPerClass[zi_.data_(i)] += 1;
  }
  int minIndPerClass = indPerClass.minCoeff();
#ifdef MC_DEBUG
  std::cout << "\tindPerClass: " << indPerClass << std::endl;
  std::cout << "\tminIndPerClass: " << minIndPerClass << std::endl;
#endif
  return minIndPerClass;
}

/* simulate zi for a particular individual */
void IMixtureComposerBase::sStep(int i)
{
  sampler_.sampleIndividual(i);
}

/* compute Tik, for all individuals */
void IMixtureComposerBase::eStep()
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::eStep" << std::endl;
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
void IMixtureComposerBase::eStep(int i)
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::eStep(i), i: " << i << std::endl;
#endif
  RowVector<Real> lnComp(nbCluster_);
  for (int k = 0; k < nbCluster_; k++)
  {
    lnComp[k] = lnCompletedLikelihood(i, k);
  }

#ifdef MC_DEBUG
    std::cout << "lnComp: " << lnComp << std::endl;
#endif

  Real lnCompMax = lnComp.maxCoeff();
  lnComp -= lnCompMax;
  lnComp = lnComp.exp();
  Real sum = lnComp.sum();
  tik_.row(i) = lnComp / sum;

#ifdef MC_DEBUG
  std::cout << "\tmax: " << max << ", sum2: " << sum2 << std::endl;
  std::cout << "tik_.row(i): " << tik_.row(i) << std::endl;
#endif
}

/* Compute prop using the ML estimator, default implementation. */
void IMixtureComposerBase::pStep()
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::pStep" << std::endl;
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
void IMixtureComposerBase::mapStep()
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::mapStep" << std::endl;
#endif
  for (int i = 0; i < nbSample_; ++i)
  {
    mapStep(i);
  }
}

/* Compute Zi using the Map estimator, default implementation. */
void IMixtureComposerBase::mapStep(int i)
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::mapStep, single individual" << std::endl;
#endif
  int k;
  tik_.row(i).maxCoeff(&k);
  zi_.data_(i) = k;
}

/* Create the parameters of the  mixture model. */
void IMixtureComposerBase::intializeMixtureParameters()
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::intializeMixtureParameters" << std::endl;
#endif
  prop_     = 1./(Real)nbCluster_;
  tik_      = 1./(Real)nbCluster_;
  zi_.data_ = 0;
}
} // namespace mixt

