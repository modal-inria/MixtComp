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

    Contact : Serge.Iovleff@stkpp.org
*/

/*
 * Project:  stkpp::Model
 * created on: 22 juil. 2011
 * Purpose: define the class IUnivStatModel.
 * Author:   iovleff, serge.iovleff@stkpp.org
 *
 **/

/** @file STK_IUnivStatModel.h
 *  @brief In this file we define the class IUnivStatModel.
 **/

#ifndef STK_IUNIVSTATMODEL_H
#define STK_IUNIVSTATMODEL_H

#include "STK_IModelBase.h"
#include "../../Sdk/include/STK_IRunner.h"

namespace STK
{

/** @ingroup Model
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
template <class ColVector, class UnivariateLaw>
class IUnivStatModel : public IModelBase, public IRunnerConst<ColVector>
{
  using IRunnerConst<ColVector>::p_data_;

  public:
    /** Type of the data contained in the container */
    typedef typename ColVector::Type Type;
    /** Type of the Runner */
    typedef IRunnerConst<ColVector> Runner;

  protected:
    /** default constructor. */
    IUnivStatModel() : IModelBase(), Runner() {}
    /** Constructor with data set. */
    IUnivStatModel(ColVector const& data) : IModelBase(data.size(), 1)
                                      , Runner(data)
    { this->initialize(data.size(), 1);}
    /** Constructor with a ptr on the data set. */
    IUnivStatModel(ColVector const* p_data) : IModelBase(), Runner(p_data)
    { if (p_data) this->initialize(p_data->size(), 1) ;}

  public:
    /** destructor */
    virtual ~IUnivStatModel() {}
    /** @return the total available observations */
    inline UnivariateLaw const& law() const { return law_;}

  protected:
    /** This virtual method will be called if the user set a new data set
     *  @sa IRunnerConst::setData */
    virtual void update()
    { if (p_data_) this->initialize(p_data_->size(), 1); }
    /** The probability law of the model. */
    UnivariateLaw law_;
    /** compute the log Likelihood of the statistical model. */
    void computeLnLikelihood()
    {
      // no data
      if (!p_data_) return;
      Real sum = 0.0;
      for (int i= p_data_->firstIdx(); i<= p_data_->lastIdx(); i++)
      { if (!Arithmetic<Type>::isNA(p_data_->elt(i))) sum += law_.lpdf(p_data_->elt(i));}
      this->setLnLikelihood(sum);
    }
};

} // namespace STK

#endif /* STK_IUNIVSTATMODEL_H */
