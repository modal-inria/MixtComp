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
 * created on: 3 sept. 2013
 * Author:   iovleff, serge.iovleff@stkpp.org
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_MixtureStrategy.h
 *  @brief In this file we define the strategies to use in order to estimate a
 *  mixture model.
 **/


#ifndef STK_MIXTURESTRATEGY_H
#define STK_MIXTURESTRATEGY_H

#include "Sdk/include/STK_IRunner.h"
#include "STKernel/include/STK_Real.h"

namespace STK
{
// forward declarations
class IMixtureComposer;
class IMixtureAlgo;
class IMixtureInit;

/** @ingroup Clustering
 *  Interface base class for all the strategies */
class IMixtureStrategy : public IRunnerBase
{
  public:
    /** default constructor.
     *  @param p_model the model to estimate
     **/
    inline IMixtureStrategy( IMixtureComposer*& p_model)
                           : IRunnerBase(), nbTry_(1), p_model_(p_model), p_init_(0)
    {}
    /** copy constructor
     *  @param strategy the strategy to copy
     **/
    IMixtureStrategy( IMixtureStrategy const& strategy);
    /** destructor */
    virtual ~IMixtureStrategy();
    /** set the number of tries of each strategies.
     * @param nbTry the number of tries to set */
    void setNbTry(int nbTry) { nbTry_ = nbTry;}
    /** set the initialization method to use
     * @param  p_init the initialization method to use */
    void setMixtureInit(IMixtureInit* p_init) { p_init_ = p_init;}

  protected:
    /** number of tries of each strategies (1 by default) */
    int nbTry_;
    /** reference on the main model */
    IMixtureComposer*& p_model_;
    /** initialization method */
    IMixtureInit* p_init_;
    /** store a model in p_model_ if it is better.
     * @param p_otherModel the model to store
     **/
    void storeModel(IMixtureComposer*& p_otherModel);
};

/** @ingroup Clustering
 *  helper structure encapsulating the parameters of the simple strategy
 **/
struct SimpleStrategyParam
{
  /** Constructor. Set default values */
  inline SimpleStrategyParam() : p_algo_(0) {}
  /** destructor */
  virtual ~SimpleStrategyParam();
  /** number of iterations in the Initialization */
  IMixtureAlgo* p_algo_;
};

/** @ingroup Clustering
 *  A SimpleStrategy is just nbTry long run.
 **/
class SimpleStrategy : public IMixtureStrategy
{
  public:
    /** default constructor.
     * @param p_model a reference pointer on the model to estimate
     **/
    inline SimpleStrategy( IMixtureComposer*& p_model) : IMixtureStrategy(p_model), p_param_(0)
    {}
    /** copy constructor.
     *  @param strategy the strategy to copy
     **/
    inline SimpleStrategy( SimpleStrategy const& strategy) : IMixtureStrategy(strategy), p_param_(0)
    {}
    /** destructor */
    inline virtual ~SimpleStrategy() { if (p_param_) delete p_param_;}
    /** clone pattern */
    inline virtual SimpleStrategy* clone() const { return new SimpleStrategy(*this);}
    /** set the parameters of the strategy
     * @param  p_param  the parameters of the strategy */
    inline void setParam(SimpleStrategyParam* p_param) { p_param_ = p_param;}

    /** run the strategy */
    virtual bool run();

  protected:
    SimpleStrategyParam* p_param_;
};

/** @ingroup Clustering
 *  helper structure encapsulating the parameters of the Xem strategy
 **/
struct XemStrategyParam
{  /** Constructor. Set default values */
    inline XemStrategyParam() : nbShortRun_(0), p_shortAlgo_(0) , p_longAlgo_(0)
    {}
    /** destructor */
    virtual ~XemStrategyParam();
    /** number of short run to perform */
    int nbShortRun_;
    /** algorithm to use in short runs  */
    IMixtureAlgo* p_shortAlgo_;
    /** algorithm to use in long run  */
    IMixtureAlgo* p_longAlgo_;
};

/** @ingroup Clustering
 *  A XemStrategy is based on the following paradigm:
 *  - perform nbShortRun of the shortAlgo with a small number of iterations and
 *   a high tolerance,
 *  - pick the best model obtained,
 *  - on this best model perform a long run.
 *  This strategy is used in Rmixmod R package.
 **/
class XemStrategy: public IMixtureStrategy
{
  public:
    /** default constructor.
     * @param p_model a reference pointer on the model to estimate
     **/
    inline XemStrategy( IMixtureComposer*& p_model) : IMixtureStrategy(p_model), p_param_()
    {}
    /** copy constructor.
     *  @param strategy the strategy to copy
     **/
    inline XemStrategy( XemStrategy const& strategy) : IMixtureStrategy(strategy), p_param_(0)
    {}
    /** destructor */
    inline virtual ~XemStrategy() { if (p_param_) delete p_param_;}
    /** clone pattern */
    inline virtual XemStrategy* clone() const { return new XemStrategy(*this);}
    /** set the parameters of the strategy
     * @param  p_param  the parameters of the Xem strategy
     **/
    void setParam(XemStrategyParam * p_param) { p_param_ = p_param;}

    /** run the strategy */
    virtual bool run();

  protected:
    XemStrategyParam* p_param_;
};

/** @ingroup Clustering
 *  helper structure encapsulating the parameters of the Full strategy
 **/
struct FullStrategyParam
{  /** Constructor. Set default values */
    inline FullStrategyParam() : nbInitRun_(1), nbShortRun_(0), p_shortAlgo_(0) , p_longAlgo_(0)
    {}
    /** destructor */
    virtual ~FullStrategyParam();
    /** number of initialization run to perform */
    int nbInitRun_;
    /** number of short run to perform */
    int nbShortRun_;
    /** algorithm to use in short runs  */
    IMixtureAlgo* p_shortAlgo_;
    /** algorithm to use in long run  */
    IMixtureAlgo* p_longAlgo_;
};

/** @ingroup Clustering
 *  A FullStrategy is based on the following paradigm:
 *  - perform nbInitRun_ of the p_init_ initialization method, select the best initialization
 *  - perform nbShortRun of the shortAlgo with a small number of iterations and
 *   a high tolerance,
 *  - pick the best model obtained,
 *  - on this best model perform a long run.
 *  This strategy is used in Rmixmod R package.
 **/
class FullStrategy: public IMixtureStrategy
{
  public:
    /** default constructor.
     * @param p_model a reference pointer on the model to estimate
     **/
    inline FullStrategy( IMixtureComposer*& p_model) : IMixtureStrategy(p_model), p_param_()
    {}
    /** copy constructor.
     *  @param strategy the strategy to copy
     **/
    inline FullStrategy( FullStrategy const& strategy) : IMixtureStrategy(strategy), p_param_(0)
    {}
    /** destructor */
    inline virtual ~FullStrategy() { if (p_param_) delete p_param_;}
    /** clone pattern */
    inline virtual FullStrategy* clone() const { return new FullStrategy(*this);}
    /** set the parameters of the strategy
     * @param  p_param  the parameters of the Xem strategy
     **/
    void setParam(FullStrategyParam * p_param) { p_param_ = p_param;}

    /** run the strategy */
    virtual bool run();

  protected:
    FullStrategyParam* p_param_;
    /** Perform the Initialization step
     *  Initialize nbInitRun_ (should be  > 0) model and select the best model
     *  among them
     **/
    void initStep(IMixtureComposer*& p_current, IMixtureComposer*& pcurrentBestModel);
};

}  // namespace STK

#endif /* STK_MIXTURESTRATEGY_H */
