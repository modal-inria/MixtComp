/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2011  Serge Iovleff

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
 * Project:  stkpp::Model
 * created on: 22 juil. 2011
 * Purpose: define the class IMultiStatModel.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IMultiStatModel.h
 *  @brief In this file we define the class IMultiStatModel.
 **/

#ifndef STK_IMULTISTATMODEL_H
#define STK_IMULTISTATMODEL_H

#include <cmath>

#include "STK_IStatModelBase.h"
#include "STK_IMultiParameters.h"
#include "Sdk/include/STK_IRunner.h"
#include "Sdk/include/STK_Macros.h"
#include "STatistiK/include/STK_Law_IMultiLaw.h"

namespace STK
{

/** @ingroup StatModels
 *  @brief Interface base class for all Multivariate Statistical Models.
 *
 *  A Statistical model, \f$ \mathcal{P}\f$, is a collection of multivariate
 *  probability distribution functions or probability density functions
 *  (collectively referred to as ''distributions'' for brevity).
 *  A parametric model is a collection of distributions, each of which is
 *  indexed by a unique finite-dimensional parameter:
 *  \f$\mathcal{P}=\{\mathbb{P}_{\theta} : \theta \in \Theta\}\f$, where
 *  \f$\theta\f$ is a parameter and \f$\Theta \subseteq \mathbb{R}^d\f$ is
 *  the feasible region of parameters, which is a subset of d-dimensional
 *  Euclidean space.
 *
 *  A statistical model may be used to describe the set of
 *  distributions from which one assumes that a particular data set is sampled.
 *  For example, if one assumes that data arise from a multivariate Gaussian
 *  distribution, then one has assumed a Gaussian model:
 *  \f$
 *    \mathcal{P}=\{\mathbb{P}(x; \mu, \Sigma) = \frac{1}{\sqrt{2 \pi |\Sigma|} }
 *    \exp\left\{ -\frac{1}{2}(x-\mu)'\Sigma^{-1}(x-\mu)\right\} : \mu \in \mathbb{R}^p, \Sigma > 0\}
 *  \f$.
 *
 *  From a computational point of view a statistical model is defined with
 *  the help of two elements
 *  - A data set where the number of samples is the number of rows and the number
 *  of variable is the number of columns. This data set is stored in a Container
 *  of type @c Array.
 *  - A set of parameters stored in a class of type @c Parameters. These parameters
 *  can be created using the @c createParameters method with the effect to
 *  call the default constructor of the Parameters class, or can be set to this
 *  class using the method @c setParameters.
 *
 *  Derived implementations of this interface have to implement the following
 *  pure virtual methods:
 *  @code
 *    virtual int computeNbFreeParameters() const =0;
 *    virtual Real computeLnLikelihood( RowVector const& rowData) const = 0;
 *    virtual void computeParameters() = 0;
 *    virtual void computeParameters(ColVector const& weights) = 0;
 *  @endcode
 *
 *  @tparam Array can be any kind of vector for the data set. it should at
 *  least derive from the specialization of ITContainer for Arrays::vector_,
 *  @sa ITContainer.
 *  @tparam Parameters any structure encapsulating the parameters of the model.
 *
 *  @note This class is a runner, The parameters are estimated using either
 *  @c run() or @c run(weights) methods. This class can also be used
 *  as a "kitchen" providing tools, in particular if there is latent variables
 *  and one need to use an iterative algorithm.
 *  @sa IMixtureModel.
 **/
template <class Array, class WColVector, class Parameters>
class IMultiStatModel : public IStatModelBase, public IRunnerUnsupervised<Array, WColVector>
{
  public:
    /** Type of the row vector of the container */
    typedef typename Array::Row RowVector;
    /** Type of the column vector of the container */
    typedef typename Array::Col ColVector;
    /** Type of the runner */
    typedef IRunnerUnsupervised<Array, WColVector> Runner;
    using Runner::p_data;

  protected:
    /** default constructor. */
    IMultiStatModel() : IStatModelBase(), Runner()
                      , p_param_(0), isParametersCreated_(false) {}
    /** Constructor with data set. */
    IMultiStatModel( Array const& data)
                   : IStatModelBase(), Runner(data)
                   , p_param_(0) , isParametersCreated_(false)
    { this->initialize(data.sizeRows(), data.sizeCols());}
    /** Constructor with a ptr on the data set. */
    IMultiStatModel( Array const* p_data)
                   : IStatModelBase(), Runner(p_data)
                   , p_param_(0), isParametersCreated_(false)
    {
      if (p_data)
        this->initialize(p_data->sizeRows(), p_data->sizeCols());
    }
    /** Copy constructor.
     *  @param model the model to copy
     **/
    IMultiStatModel( IMultiStatModel const& model)
                   : IStatModelBase(model), Runner(model)
                   , p_param_( (model.isParametersCreated_ && model.p_param_)
                               ? model.p_param_->clone() : model.p_param_)
                   , isParametersCreated_(model.isParametersCreated_)
    {}
    /** destructor */
    ~IMultiStatModel()
    { if (isParametersCreated_ && p_param_) delete p_param_->asPtrDerived();}

  public:
    /** @return the pointer on the parameters */
    inline Parameters* const p_param() const
    { return (p_param_) ? p_param_->asPtrDerived() : 0;}
    /** compute the log Likelihood of the statistical model. */
    Real computeLnLikelihood() const
    {
      Real sum = 0.0;
      for (int i= p_data()->beginRows(); i<= p_data()->lastIdxRows(); i++)
      { sum += computeLnLikelihood(p_data()->row(i));}
      return(sum);
    }
    /** Estimate the parameters of the model and update the model */
    virtual bool run()
    {
#ifdef STK_DEBUG
      if (!p_data())
      { this->msg_error_ = STKERROR_NO_ARG(IMultiStatModel::run,data have not be set);
        return false;
      }
      if (!p_param())
      { this->msg_error_ = STKERROR_NO_ARG(IMultiStatModel::run(weights),parameters have not be set);
        return false;
      }
#endif
      try
      {
        // compute parameters
        computeParameters();
        // compute log-likelihood
        this->setLnLikelihood(computeLnLikelihood());
        // set the number of free parameters
        this->setNbFreeParameter(computeNbFreeParameters());
      }
      catch (Exception const& e)
      { this->msg_error_ = e.error(); return false;}
      return true;
    }
    /** compute the weighted empirical probability of success based on the observed
     *  variables. The NA values are discarded.
     *  @param weights the weights of the observations
     **/
    virtual bool run(WColVector const& weights)
    {
#ifdef STK_DEBUG
      if (!p_data())
      { this->msg_error_ = STKERROR_NO_ARG(IMultiStatModel::run(weights),data have not be set);
        return false;
      }
      if (!p_param())
      { this->msg_error_ = STKERROR_NO_ARG(IMultiStatModel::run(weights),no parameters);
        return false;
      }
#endif
      try
      {
        // compute weighted parameters
        computeParameters(weights);
        // compute log-likelihood
        this->setLnLikelihood(computeLnLikelihood());
        // set the number of free parameters
        this->setNbFreeParameter(computeNbFreeParameters());
      }
      catch (Exception const& e)
      { this->msg_error_ = e.error(); return false;}
      return true;
    }
    /** compute the number of free parameters */
    virtual int computeNbFreeParameters() const =0;
    /** compute the log Likelihood of an observation. */
    virtual Real computeLnLikelihood( RowVector const& rowData) const = 0;
    /** @param p_param the pointer on the parameters to set */
    inline void setParameters(Parameters* p_param)
    { if (isParametersCreated_ && p_param_) delete p_param_->asPtrDerived();
      p_param_ = p_param;
      isParametersCreated_ = false;
    }
    /** crete the parameters of the model using default constructor of Parameters */
    void createParameters()
    {
      if (isParametersCreated_ && p_param_) delete p_param_->asPtrDerived();
      p_param_ = new Parameters; isParametersCreated_ = true;
      if (p_data()) { p_param_->resize(p_data()->cols());}
    }
    /** write the parameters.
     *  @param os the output stream for the parameters
     **/
     virtual void writeParameters(ostream &os)
     {
       if (!p_param())
       { STKRUNTIME_ERROR_NO_ARG(IMultiStatModel::writeParameters(os),no parameters);}
       p_param_->print(os);
     }

  protected:
    /** Pointer on the parameters of the model. */
    IMultiParameters<Parameters>* p_param_;
    /** Pointer on the parameters of the model. */
    bool isParametersCreated_;
    /** update the model if a new data set is set */
    virtual void update()
    { this->initialize(p_data()->sizeRows(), p_data()->sizeCols());
      if (isParametersCreated_ && p_param_) { p_param_->resize(p_data()->cols());}
    }

  private:
    /** compute the parameters */
    virtual void computeParameters() = 0;
    /** compute the weighted parameters */
    virtual void computeParameters(WColVector const& weights) = 0;
};

} // namespace STK

#endif /* STK_IMULTISTATMODEL_H */
