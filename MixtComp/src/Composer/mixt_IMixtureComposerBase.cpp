/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 16 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_IMixtureComposerBase.h
 *  @brief In this file we implement the abstract base class for mixture models.
 **/

#include <cmath>
#include "mixt_IMixtureComposerBase.h"
#include "STatistiK/include/STK_Law_Categorical.h"
#include "STatistiK/include/STK_Stat_Functors.h"

namespace mixt
{
IMixtureComposerBase::IMixtureComposerBase( int nbSample, int nbVariable, int nbCluster)
                                          : IModelBase(nbSample, nbVariable)
                                          , nbCluster_(nbCluster)
                                          , prop_(nbCluster), tik_(nbSample, nbCluster), zi_(nbSample)
                                          , state_(modelCreated_)
{
  intializeMixtureParameters();
}

/* copy constructor */
IMixtureComposerBase::IMixtureComposerBase( IMixtureComposerBase const& model)
                                          : IModelBase(model)
                                          , nbCluster_(model.nbCluster_)
                                          , prop_(model.prop_)
                                          , tik_(model.tik_)
                                          , zi_(model.zi_)
                                          , state_(model.state_)
{}
/* destructor */
IMixtureComposerBase::~IMixtureComposerBase() {}

/* initialize randomly the labels zi of the model */
void IMixtureComposerBase::randomClassInit()
{
  STK::Law::Categorical law(prop_);
  for (int i = zi_.firstIdx(); i <= zi_.lastIdx(); ++i)
  {
    zi_.elt(i) = law.rand();
  }
}

/* cStep for all individuals */
int IMixtureComposerBase::cStep()
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::cStep" << std::endl;
#endif
  for (int i = 0; i < nbSample(); ++i)
  {
    cStep(i);
  }
  // count the minimal number of individuals in a class
  return (STK::Stat::sum(tik_).minElt());
}

/* cStep for a single individual */
void IMixtureComposerBase::cStep(int i)
{
  tik_.row(i) = 0.;
  tik_.elt(i, zi_[i]) = 1.;
}

/* simulate zi for all individuals */
int IMixtureComposerBase::sStep()
{
#ifdef MC_DEBUG_NEW
  std::cout << "IMixtureComposerBase::sStep" << std::endl;
#endif
  // simulate zi
  for (int i = 0; i < nbSample(); ++i)
  {
    sStep(i);
  }
  STK::Array2DVector<int> indPerClass(nbCluster_, 0);
  for (int i = 0; i < nbSample(); ++i)
  {
    indPerClass[zi_[i]] += 1;
  }
  int minIndPerClass = indPerClass.minElt();
#ifdef MC_DEBUG_NEW
  std::cout << "\tindPerClass: " << indPerClass << std::endl;
  std::cout << "\tminIndPerClass: " << minIndPerClass << std::endl;
#endif
  return minIndPerClass;
}

/* simulate zi for a particular individual */
void IMixtureComposerBase::sStep(int i)
{
  zi_.elt(i) = STK::Law::Categorical::rand(tik_.row(i));
}

/* compute Tik, for all individuals */
void IMixtureComposerBase::eStep()
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::eStep" << std::endl;
#endif
  STK::Real sum = 0.;
  for (int i = 0; i < nbSample(); ++i)
  {
    sum += eStep(i);
  }
#ifdef MC_DEBUG_NEW
  std::cout << "tik_:" << std::endl;
  std::cout << tik_ << std::endl;
#endif
  setLnLikelihood(sum);
}

/* compute Tik, for a particular individual */
STK::Real IMixtureComposerBase::eStep(int i)
{
  STK::Array2DPoint<STK::Real> lnComp(tik_.cols());
  for (int k = tik_.firstIdxCols(); k <= tik_.lastIdxCols(); k++)
  {
    lnComp[k] = lnComponentProbability(i, k);
  }
  int kmax;
  STK::Real max = lnComp.maxElt(kmax);
  // compute sum_k pk exp{lnCom_k - lnComp_kmax}
  STK::Real sum2 = (lnComp -= max).exp().dot(prop_);
  // compute likelihood of each sample for each component
  tik_.row(i) = (prop_ * lnComp.exp())/sum2;

  return max + std::log(sum2);
}

/* Compute prop using the ML estimator, default implementation. */
void IMixtureComposerBase::pStep()
{
#ifdef MC_DEBUG_NEW
  std::cout << "IMixtureComposerBase::pStep" << std::endl;
#endif
  for (int i = 0; i < zi_.sizeRows(); ++i)
  {
    prop_[zi_[i]] += 1.;
  }
  prop_ = prop_/prop_.sum();
#ifdef MC_DEBUG_NEW
  std::cout << "\tprop_: " << prop_ << std::endl;
#endif
}

/* Compute Zi using the Map estimator,default implementation. */
void IMixtureComposerBase::mapStep()
{
#ifdef MC_DEBUG
  std::cout << "IMixtureComposerBase::mapStep" << std::endl;
#endif
  for (int i = 0; i < nbSample(); ++i)
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
  tik_.row(i).maxElt(k);
  zi_.elt(i) = k;
}

/* Create the parameters of the  mixture model. */
void IMixtureComposerBase::intializeMixtureParameters()
{
#ifdef MC_DEBUG_NEW
  std::cout << "IMixtureComposerBase::intializeMixtureParameters" << std::endl;
#endif
  prop_ = 1./(STK::Real)nbCluster_;
  tik_  = 1./(STK::Real)nbCluster_;
  zi_   = STK::baseIdx;
}
} // namespace mixt
