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

/** @file STK_IMixtureModel.h
 *  @brief In this file we define the classes for mixture algorithms.
 **/

#ifndef STK_MIXTUREALGO_H
#define STK_MIXTUREALGO_H

#include "STK_Clust_Util.h"

#include "Sdk/include/STK_IRunner.h"
#include "STKernel/include/STK_Real.h"

namespace STK
{
// forward declaration
class IMixtureComposerBase;

/** @ingroup Clustering
 * Interface base class for the algorithms.
 * All algorithms are runners applying on a model instance given by pointer
 * and have to implement the run method.
 *
 * All algorithms start with an mStep(), so user have to provide an instance of
 * the model with initial parameters values.
 **/
class IMixtureAlgo : public IRunnerBase
{
  protected:
    /** default constructor */
    inline IMixtureAlgo() : IRunnerBase(), p_model_(0), nbIterMax_(0), epsilon_(0.) {}
    /** Copy constructor.
     *  @param algo the algorithm to copy */
    inline IMixtureAlgo( IMixtureAlgo const& algo) : IRunnerBase(algo)
                       , p_model_(algo.p_model_), nbIterMax_(algo.nbIterMax_), epsilon_(algo.epsilon_)
    {}

  public:
    /** destructor */
    inline virtual ~IMixtureAlgo() {}
    /** set a new model */
    inline void setModel(IMixtureComposerBase* p_model) { p_model_ = p_model; }
    /** set the maximal number of iterations */
    inline void setNbIterMax(int nbIterMax) { nbIterMax_ = nbIterMax; }
    /** set the tolerance value */
    inline void setEpsilon(Real epsilon) { epsilon_ = epsilon; }

  protected:
    /** pointer on the mixture model */
    IMixtureComposerBase* p_model_;
    /** number of iterations of the algorithm */
    int nbIterMax_;
    /** tolerance of the algorithm. */
    Real epsilon_;
};

/** @ingroup Clustering
 *  @ brief Implementation of the EM algorithm.
 *  The EM algorithm call alternatively the steps:
 *  - mStep()
 *  - eStep()
 *  until the maximum number of iterations is reached or the variation of the
 *  ln-likelihood is less than the tolerance.
 **/
class EMAlgo: public IMixtureAlgo
{
  public:
    /** default constructor */
    inline EMAlgo() : IMixtureAlgo() {}
    /** Copy constructor.
     *  @param algo the algorithm to copy */
    inline EMAlgo( EMAlgo const& algo) : IMixtureAlgo(algo) {}
    /** destructor */
    inline virtual ~EMAlgo(){}
    /** clone pattern */
    inline virtual EMAlgo* clone() const { return new EMAlgo(*this);}
    /** run the algorithm on the model calling the eStep and mStep of the model
     *  until the maximal number of iteration is reached or the variation
     *  of the lnLikelihood is less than epsilon.
     * @return @c true if no error occur, @c false otherwise
     **/
    virtual bool run();
};

/** @ingroup Clustering
 *  @brief Implementation of the CEM algorithm.
 *  The CEM algorithm call alternatively the steps:
 *  - cStep()
 *  - mStep()
 *  - eStep()
 *  until the maximum number of iterations is reached or the variation of the
 *  ln-likelihood is less than the tolerance.
 **/
class CEMAlgo: public IMixtureAlgo
{
  public:
    /** default constructor */
    inline CEMAlgo() : IMixtureAlgo() {}
    /** Copy constructor.
     *  @param algo the algorithm to copy */
    inline CEMAlgo( CEMAlgo const& algo) : IMixtureAlgo(algo) {}
    /** destructor */
    inline virtual ~CEMAlgo(){}
    /** clone pattern */
    inline virtual CEMAlgo* clone() const { return new CEMAlgo(*this);}
    /** run the algorithm on the model calling cStep, mStep and eStep of the
     *  model until the maximal number of iteration is reached or the variation
     *  of the lnLikelihood is less than epsilon.
     *  @return @c true if no error occur, @c false otherwise
     **/
    virtual bool run();
};

/** @ingroup Clustering
 *  @brief Implementation of the SEM algorithm.
 *  The CEM algorithm call alternatively the steps:
 *  - sStep()
 *  - mStep()
 *  - eStep()
 *  until the maximum number of iterations is reached.
 **/
class SEMAlgo: public IMixtureAlgo
{
  public:
    /** default constructor */
    inline SEMAlgo() : IMixtureAlgo() {}
    /** Copy constructor.
     *  @param algo the algorithm to copy */
    inline SEMAlgo( SEMAlgo const& algo) : IMixtureAlgo(algo) {}
    /** destructor */
    inline virtual ~SEMAlgo(){}
    /** clone pattern */
    inline virtual SEMAlgo* clone() const { return new SEMAlgo(*this);}
    /** run the algorithm on the model calling sStep, mStep and eStep of the
     *  model until the maximal number of iteration is reached.
     *  @return @c true if no error occur, @c false otherwise.
     **/
    virtual bool run();
};

} // namespace STK

#endif /* STK_MIXTUREALGO_H */
