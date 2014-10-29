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
                                          , state_(STK::Clust::modelCreated_)
{  intializeMixtureParameters(); }

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
  prop_ = 1./STK::Real(nbCluster_);
  STK::Law::Categorical law(prop_);
  for (int i = zi_.firstIdx(); i<= zi_.lastIdx(); ++i)
  { zi_.elt(i) = law.rand();}
  cStep();
  initializeStep();
  eStep();
}

/* initialize randomly the posterior probabilities tik of the model */
void IMixtureComposerBase::randomFuzzyInit()
{
  prop_ = 1./STK::Real(nbCluster_);
  STK::RandBase generator;
  for (int i = tik_.firstIdxRows(); i<= tik_.lastIdxRows(); ++i)
  {
    // create a reference on the i-th row
    STK::Array2DPoint<STK::Real> tikRowi(tik_.row(i), true);
    generator.randUnif(tikRowi);
    tikRowi = tikRowi * prop_;
    tikRowi /= tikRowi.sum();
  }
  initializeStep();
  eStep();
}

/* cStep */
int IMixtureComposerBase::cStep()
{
  tik_ = 0.;
  for (int i=tik_.firstIdxRows(); i<= tik_.lastIdxRows(); i++)
  { tik_.elt(i, zi_[i]) = 1.;}
  // count the minimal number of individuals in a class
  return (STK::Stat::sum(tik_).minElt());
}

/* simulate zi  */
int IMixtureComposerBase::sStep()
{
  // simulate zi
  for (int i = zi_.firstIdx(); i<= zi_.lastIdx(); ++i)
  { zi_.elt(i) = STK::Law::Categorical::rand(tik_.row(i));}
  return cStep();
}
/* compute Tik, default implementation. */
void IMixtureComposerBase::eStep()
{
  STK::Real sum = 0.;
  for (int i = tik_.firstIdxRows(); i<= tik_.lastIdxRows(); ++i)
  {
    STK::Array2DPoint<STK::Real> lnComp(tik_.cols());
    for (int k=tik_.firstIdxCols(); k<= tik_.lastIdxCols(); k++)
    { lnComp[k] = lnComponentProbability(i,k);}
    int kmax;
    STK::Real max = lnComp.maxElt(kmax);
    zi_.elt(i) = kmax;
    // compute sum_k pk exp{lnCom_k - lnComp_kmax}
    STK::Real sum2 =  (lnComp -= max).exp().dot(prop_);
    // compute likelihood of each sample for each component
    tik_.row(i) = (prop_ * lnComp.exp())/sum2;
    // compute lnLikelihood
    sum += max + std::log(sum2);
  }
  setLnLikelihood(sum);
}
/* estimate the proportions and the parameters of the components of the
 *  model given the current tik/zi mixture parameters values.
 **/
void IMixtureComposerBase::mStep()
{ pStep();
  /* implement specific parameters estimation in concrete class. */
}

/* Compute prop using the ML estimator, default implementation. */
void IMixtureComposerBase::pStep()
{ prop_ = STK::Stat::mean(tik_);}

/* Compute Zi using the Map estimator, default implementation. */
void IMixtureComposerBase::mapStep()
{
  for (int i = zi_.firstIdx(); i<= zi_.lastIdx(); ++i)
  {
    int k;
    tik_.row(i).maxElt(k);
    zi_.elt(i) = k;
  }
}

/* Create the parameters of the  mixture model. */
void IMixtureComposerBase::intializeMixtureParameters()
{
  prop_ = 1./(STK::Real)nbCluster_;
  tik_  = 1./(STK::Real)nbCluster_;
  zi_   = STK::baseIdx;
}
} // namespace mixt

