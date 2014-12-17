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
 * Purpose: define the class IUnivStatModel.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IUnivStatModel.h
 *  @brief In this file we define the class IUnivStatModel.
 **/

#ifndef STK_IUNIVSTATMODEL_H
#define STK_IUNIVSTATMODEL_H

#include "STK_IStatModelBase.h"
#include "Sdk/include/STK_IRunner.h"
#include "Sdk/include/STK_Macros.h"

namespace STK
{

/** @ingroup StatModels
 *  @brief Base class for all Statistical Models.
 *  A Statistical model, \f$ \mathcal{P}\f$, is a collection of
 *  probability distribution functions or probability density functions
 *  (collectively referred to as ''distributions'' for brevity).
 *  A parametric model is a collection of distributions, each of which is
 *  indexed by a unique finite-dimensional parameter:
 *  \f$\mathcal{P}=\{\mathbb{P}_{\theta} : \theta \in \Theta\}\f$, where
 *  \f$\theta\f$ is a parameter and \f$\Theta \subseteq \mathbb{R}^d\f$ is
 *  the feasible region of parameters, which is a subset of d-dimensional
 *  Euclidean space.  A statistical model may be used to describe the set of
 *  distributions from which one assumes that a particular data set is sampled.
 *  For example, if one assumes that data arise from a univariate Gaussian
 *  distribution, then one has assumed a Gaussian model:
 *  \f$
 *    \mathcal{P}=\{\mathbb{P}(x; \mu, \sigma) = \frac{1}{\sqrt{2 \pi} \sigma}
 *    \exp\left\{ -\frac{1}{2\sigma^2}(x-\mu)^2\right\} : \mu \in \mathbb{R}, \sigma > 0\}
 *  \f$.
 *
 *  From a computational point of view a statistical model is defined with
 *  the help of two elements
 *  - A data set where the number of samples is the number of rows. This data
 *    set is stored in a Container of type @c ColVector.
 *  - A probability (density/law) which for each row of the data set can compute
 *  a density/probability.
 *
 *  The parameters of the distribution (if any) are directly managed by the
 *  probability law.
 *
 *  Further implementation of this interface have to implement the @c run() and
 *  the @c run(weights) methods in order to estimate the parameters of the
 *  univariate law.
 *
 *  @tparam ColVector can be any kind of vector for the data set. it should at
 *  least derive from the specialization of ITContainer for Arrays::vector_,
 *  @sa ITContainer.
 *  @tparam UnivariateLaw any probabilistic law deriving from the interface
 *  class IUnivLaw, @sa IUnivLaw.
 **/
template <class ColVector, class WColVector, class UnivariateLaw>
class IUnivStatModel : public IStatModelBase, public IRunnerUnsupervised<ColVector, WColVector>
{
  public:
    using IRunnerUnsupervised<ColVector, WColVector>::p_data;
    /** Type of the data contained in the container */
    typedef typename ColVector::Type Type;
    /** Type of the Runner */
    typedef IRunnerUnsupervised<ColVector, WColVector> Runner;

  protected:
    /** default constructor. */
    IUnivStatModel() : IStatModelBase(), Runner() {}
    /** Constructor with data set. */
    IUnivStatModel(ColVector const& data) : IStatModelBase(data.size(), 1), Runner(data)
    { this->initialize(data.size(), 1);}
    /** Constructor with a ptr on the data set. */
    IUnivStatModel(ColVector const* p_data) : IStatModelBase(), Runner(p_data)
    { if (p_data) this->initialize(p_data->size(), 1) ;}
    /** Copy constructor. */
    IUnivStatModel( IUnivStatModel const& model)
                  : IStatModelBase(model), Runner(model), law_(model.law_) {}

  public:
    /** destructor */
    virtual ~IUnivStatModel() {}
    /** compute the log Likelihood of the statistical model. */
    Real computeLnLikelihood() const
    {
      Real sum = 0.0;
      for (int i= p_data()->begin(); i<= p_data()->lastIdx(); i++)
      { if (!Arithmetic<Type>::isNA(p_data()->elt(i))) sum += law_.lpdf(p_data()->elt(i));}
      return sum;
    }
    /** @return the total available observations */
    inline UnivariateLaw const& law() const { return law_;}
    /** compute the number of free parameters */
    virtual int computeNbFreeParameters() const =0;
    /** Estimate the parameters of the model and update the
     **/
    virtual bool run()
    {
      if (!p_data())
      { this->msg_error_ = STKERROR_NO_ARG(IUnivStatModel::run(),data have not be set);
        return false;
      }
      try
      {
        // compute weighted parameters
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
    virtual bool run( WColVector const& weights)
    {
      if (!p_data())
      { this->msg_error_ = STKERROR_NO_ARG(IUnivStatModel::run(weights),data have not be set);
        return false;
      }
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

  protected:
    /** This virtual method will be called if the user set a new data set
     *  @sa IRunnerUnsupervised::setData */
    virtual void update()
    { if (p_data()) this->initialize(p_data()->size(), 1); }
    /** The probability law of the model. */
    UnivariateLaw law_;
    /** compute the parameters */
    virtual void computeParameters() = 0;
    /** compute the weighted parameters */
    virtual void computeParameters(WColVector const& weights) = 0;
};


} // namespace STK

#endif /* STK_IUNIVSTATMODEL_H */
