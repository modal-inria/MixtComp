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
#ifdef STK_MIXTURE_DEBUG
#include "../../Arrays/include/STK_Display.h"
#endif
#include "../include/STK_IMixtureComposerBase.h"
#include "../../STatistiK/include/STK_Law_Categorical.h"
#include "../../STatistiK/include/STK_Stat_Functors.h"

namespace STK
{
/* default constructor */
IMixtureComposerBase::IMixtureComposerBase( int nbCluster) : IModelBase()
                                    , nbCluster_(nbCluster)
                                    , p_prop_(0), p_tik_(0), p_zi_(0)
                                    , state_(Clust::modelCreated_)
{ createMixtureParameters();}

/* copy constructor */
IMixtureComposerBase::IMixtureComposerBase( IMixtureComposerBase const& model)
                                    : IModelBase(model)
                                    , nbCluster_(model.nbCluster_)
                                    , p_prop_(model.p_prop_->clone())
                                    , p_tik_(model.p_tik_->clone())
                                    , p_zi_(model.p_zi_->clone())
                                    , state_(model.state_)
{}
/* destructor */
IMixtureComposerBase::~IMixtureComposerBase()
{ deleteMixtureParameters();}

/* This function can be overloaded in derived class for initialization of
 *  the mixture parameters.
 **/
void IMixtureComposerBase::initializeModel()
{
  // resize model
  resizeModel();
  // compute number of free parameters
  this->setNbFreeParameter(computeNbFreeParameters());
  state_ = Clust::modelInitialized_;
}

/* First initialization of the parameters of the model.
 *  This method is called in order to initialize the parameters. The
 *  default implementation call mStep() but
 *  this behavior can be overloaded in derived class if an initial value
 *  is needed by the mStep.
 *  @sa IMixtureModel
 **/
void IMixtureComposerBase::initializeStep() { mStep();}

/* initialize randomly the labels zi of the model */
void IMixtureComposerBase::randomClassInit()
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Entering IMixtureComposerBase::randomClassInit()\n");
#endif
  *p_prop_ = 1./Real(nbCluster_);
  Law::Categorical law(*p_prop_);
  for (int i = p_zi_->firstIdx(); i<= p_zi_->lastIdx(); ++i)
  { p_zi_->elt(i) = law.rand();}
  cStep();
  initializeStep();
  eStep();
}

/* initialize randomly the posterior probabilities tik of the model */
void IMixtureComposerBase::randomFuzzyInit()
{
#ifdef STK_MIXTURE_VERY_VERBOSE
  stk_cout << _T("Entering IMixtureComposerBase::randomFuzzyInit()\n");
#endif
  *p_prop_ = 1./Real(nbCluster_);
  RandBase generator;
  for (int i = p_tik_->firstIdxRows(); i<= p_tik_->lastIdxRows(); ++i)
  {
    // create a reference on the i-th row
    Array2DPoint<Real> tikRowi(p_tik_->row(i), true);
    generator.randUnif(tikRowi);
    tikRowi = tikRowi * (*p_prop_);
    tikRowi /= tikRowi.sum();
  }
  initializeStep();
  eStep();
}

/* cStep */
int IMixtureComposerBase::cStep()
{
  (*p_tik_) = 0.;
  for (int i=p_tik_->firstIdxRows(); i<= p_tik_->lastIdxRows(); i++)
  { p_tik_->elt(i, p_zi_->elt(i)) = 1.;}
  // check if all label are presents
  return (Stat::sum(*p_tik_).minElt());
}

/* simulate zi  */
int IMixtureComposerBase::sStep()
{
  // simulate zi
  for (int i = p_zi_->firstIdx(); i<= p_zi_->lastIdx(); ++i)
  { p_zi_->elt(i) = Law::Categorical::rand(p_tik_->row(i));}
  // hard classification, throw an exception if there is no more individuals
  // in a class
  return cStep();
}
/* compute Tik, default implementation. */
void IMixtureComposerBase::eStep()
{
  Real sum = 0.;
  for (int i = p_tik_->firstIdxRows(); i<= p_tik_->lastIdxRows(); ++i)
  {
    Array2DPoint<Real> lnComp(p_tik_->cols());
    for (int k=p_tik_->firstIdxCols(); k<= p_tik_->lastIdxCols(); k++)
    { lnComp[k] = lnComponentProbability(i,k);}
    int kmax;
    Real max = lnComp.maxElt(kmax);
    p_zi_->elt(i) = kmax;
    // compute sum_k pk exp{lnCom_k - lnComp_kmax}
    Real sum2 =  (lnComp -= max).exp().dot(*p_prop_);
    // compute likelihood of each sample for each component
    p_tik_->row(i) = (*p_prop_ * lnComp.exp())/sum2;
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
{ *p_prop_ = Stat::mean(*p_tik_);}

/* Compute Zi using the Map estimator, default implementation. */
void IMixtureComposerBase::mapStep()
{
  for (int i = p_zi_->firstIdx(); i<= p_zi_->lastIdx(); ++i)
  {
    int k;
    p_tik_->row(i).maxElt(k);
    p_zi_->elt(i) = k;
  }
}

/* Create the parameters of the  mixture model. */
void IMixtureComposerBase::createMixtureParameters()
{
  createProp();
  createTik();
  createZi();
}

/* delete  the mixture model parameters. */
void IMixtureComposerBase::deleteMixtureParameters()
{
  if (p_prop_) delete p_prop_;  p_prop_ = 0;
  if (p_tik_) delete p_tik_;  p_tik_ = 0;
  if (p_zi_) delete p_zi_;  p_zi_ = 0;
}

/* create the proportions */
void IMixtureComposerBase::createProp()
{
  if (!p_prop_)
  { p_prop_ = new CArrayPoint<Real>(nbCluster_, 1./(Real)nbCluster_);}
}
/* create the tik probabilities */
void IMixtureComposerBase::createTik()
{
  if (!p_tik_)
  { p_tik_ = new Array2D<Real>(nbSample(), nbCluster_,1./(Real)nbCluster_);}
}
/* create the zi labels */
void IMixtureComposerBase::createZi()
{
  if (!p_zi_)
  { p_zi_ = new CArrayVector<int>(nbSample(), STKBASEARRAYS);}
}

/* create the proportions */
void IMixtureComposerBase::resizeModel()
{
  resizeProp();
  resizeTik();
  resizeZi();
}

/* create the proportions */
void IMixtureComposerBase::resizeProp()
{
  if (p_prop_->size() != nbCluster_)
  {
    p_prop_->resize(nbCluster_);
    p_prop_->setValue(1./(Real)nbCluster_);
  }
}
/* create the tik probabilities */
void IMixtureComposerBase::resizeTik()
{
  if (p_tik_->sizeRows() != nbSample() || p_tik_->sizeRows() != nbCluster_)
  {
    p_tik_->resize(nbSample(), nbCluster_);
    p_tik_->setValue(1./(Real)nbCluster_);
  }
}
/* create the zi labels */
void IMixtureComposerBase::resizeZi()
{
  if (p_zi_->size() != nbSample())
  { p_zi_->resize(nbSample());
    p_zi_->setValue(p_zi_->firstIdx());
  }
}
} // namespace SDTK

