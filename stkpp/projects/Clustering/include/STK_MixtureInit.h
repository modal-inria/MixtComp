/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 24 août 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_MixtureInit.h
 *  @brief In this file we define the initialization methods.
 **/


#ifndef STK_MIXTUREINIT_H
#define STK_MIXTUREINIT_H

#include "Sdk/include/STK_IRunner.h"
#include "STK_Clust_Util.h"

namespace STK
{
// forward declaration
class IMixtureComposer;
class IMixtureAlgo;

/** @ingroup Clustering
 *  @brief Interface base class for the initializations.
 *  All derived class will apply on a model instance and have to implement the
 *  run method. An initialization determine in some way values for the parameters
 *  of the mixture and perform a given number of iterations of the SEM algorithm.
 **/
class IMixtureInit : public IRunnerBase
{
  protected:
    /** default constructor */
    inline IMixtureInit() : IRunnerBase(), nbTry_(Clust::defaultNbTrialInInit), p_model_(0), p_initAlgo_(0) {}
    /** copy constructor.
     * @param init the initializing method to copy
     **/
    inline IMixtureInit( IMixtureInit const& init)
                       : IRunnerBase(init), nbTry_(init.nbTry_), p_model_(init.p_model_), p_initAlgo_(init.p_initAlgo_)
    {}

  public:
    /** destructor */
    virtual ~IMixtureInit();
    /** clone pattern */
    virtual IMixtureInit* clone() const = 0;
    /** set a the number of try */
    inline IMixtureAlgo const* const p_initAlgo() const { return p_initAlgo_;}
    /** @return the number of try */
    inline int nbTry() const { return nbTry_; }
    /** set a the number of try */
    inline void setNbTry(int nbTry) { nbTry_ = nbTry; }
    /** set a new model */
    inline void setModel(IMixtureComposer* p_model) { p_model_ = p_model; }
    /** set the initial algorithm  */
    inline void setInitAlgo(IMixtureAlgo* p_initAlgo) { p_initAlgo_ = p_initAlgo; }

  protected:
    /** number of retry in initialization */
    int nbTry_;
    /** pointer on the mixture model */
    IMixtureComposer* p_model_;
    /** algorithm to use in the initialization */
    IMixtureAlgo* p_initAlgo_;
    /** launch the initialization algorithm.
     * @return true if there is no initialization algorithm,
     * otherwise return the result of the initialization algorithm.
     **/
    bool runInitAlgo();
};

/** @ingroup Clustering
 *  Implementation of the random initialization. This class will initialize the
 *  parameter by calling the randomInit() method of the model. */
class RandomInit: public IMixtureInit
{
  public:
    /** default constructor */
    inline RandomInit() : IMixtureInit() {}
    /** copy constructor
     * @param init the initialization to copy
     **/
    inline RandomInit(RandomInit const& init) : IMixtureInit(init) {}
    /** destructor */
    inline virtual ~RandomInit(){}
    /** clone pattern */
    inline virtual RandomInit* clone() const { return new RandomInit(*this);}
    /** run the initialization by calling the randomInit method of the model.
     * @return @c true if no error occur, @c false otherwise*/
    virtual bool run();
};

/** @ingroup Clustering
 *  Initialization by simulating a realization of the class labels zi accordingly
 *  to the initial proportions.
 **/
class ClassInit: public IMixtureInit
{
  public:
    /** default constructor */
    inline ClassInit() : IMixtureInit() {}
    /** copy constructor
     * @param init the initialization to copy
     **/
    inline ClassInit(ClassInit const& init) : IMixtureInit(init) {}
    /** destructor */
    inline virtual ~ClassInit(){}
    /** clone pattern */
    inline virtual ClassInit* clone() const { return new ClassInit(*this);}
    /** run the initialization by calling the randomClassInit method of the model.
     * @return @c true if no error occur, @c false otherwise*/
    virtual bool run();
};

/** @ingroup Clustering
 *  Initialization by simulating the tik accordingly to the initial
 *  proportions. */
class FuzzyInit: public IMixtureInit
{
  public:
    /** default constructor */
    inline FuzzyInit() : IMixtureInit() {}
    /** copy constructor
     *   @param init the initialization to copy
     **/
    inline FuzzyInit(FuzzyInit const& init) : IMixtureInit(init) {}
    /** destructor */
    inline virtual ~FuzzyInit(){}
    /** clone pattern */
    inline virtual FuzzyInit* clone() const { return new FuzzyInit(*this);}
    /** run the algorithm on the model calling E step and M step.
     * @return @c true if no error occur, @c false otherwise*/
    virtual bool run();
};

} // namespace STK

#endif /* STK_MIXTUREINIT_H */
