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
 * Purpose: define the class IStatModel.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IStatModel.h
 *  @brief In this file we define the class IStatModel.
 **/

#ifndef STK_ISTATMODEL_H
#define STK_ISTATMODEL_H

#include <cmath>

#include "STK_IStatModelBase.h"
#include "Sdk/include/STK_Macros.h"
#include "STatistiK/include/STK_Law_IMultiLaw.h"

namespace STK
{

/** @ingroup StatModels
 *
 *  @brief Base class for all Statistical Models [Deprecated], have been
 *  replaced by IStatModel.
 *
 *  A Statistical model, \f$ \mathcal{P}\f$, is a collection of
 *  probability distribution functions or probability density functions
 *  (collectively referred to as ''distributions'' for brevity).
 *  A parametric model is a collection of distributions, each of which is
 *  indexed by a unique finite-dimensional parameter:
 *  \f$\mathcal{P}=\{\mathbb{P}_{\theta} : \theta \in \Theta\}\f$, where
 *  \f$\theta\f$ is a parameter and \f$\Theta \subseteq \mathbb{R}^d\f$ is
 *  the feasible region of parameters, which is a subset of d-dimensional
 * Euclidean space.  A statistical model may be used to describe the set of
 * distributions from which one assumes that a particular data set is sampled.
 * For example, if one assumes that data arise from a univariate Gaussian
 * distribution, then one has assumed a Gaussian model:
 * \f$\mathcal{P}=\{\mathbb{P}(x; \mu, \sigma) = \frac{1}{\sqrt{2 \pi} \sigma}
 * \exp\left\{ -\frac{1}{2\sigma^2}(x-\mu)^2\right\} : \mu \in \mathbb{R}, \sigma > 0\}
 * \f$.
 *
 *  From a computational point of view a statistical model is defined with
 *  the help of two elements
 *  - A data set where the number of samples is the number of rows
 *    and the number of variables is the number of column. This data set is
 *    stored in a Container of type @c Data.
 *  - A probability (density/law) which for each row of the data set can compute
 *  a density/probability.
 *
 *  The parameters of the distribution (if any) are directly managed by the
 *  probability law.
 *
 *  @tparam Data can be any kind of container for the data set. it should at
 *  least derive from ITContainer, @sa ITContainer, and provide an access to
 *  a single row like the CArray class or the Array2D class.
 **/
template <class Data>
class IStatModel : public IStatModelBase
{
  public:
    /** Type of the data contained in the container */
    typedef typename Data::Type Type;
    /** Type of the row of the data container (a sample) */
    typedef typename Data::Row Row;
    /** Type of the law : depend of the sample type */
    typedef Law::IMultiLaw<Row> MultivariateLaw;

  protected:
    /** Constructor with data set. */
    IStatModel(Data const& data) : IStatModelBase(data.sizeRows(), data.sizeCols())
                                       , p_data_(&data)
                                       , p_law_(0)
    {}
    /** Constructor with a ptr on the data set. */
    IStatModel(Data const* p_data) : IStatModelBase(), p_data_(p_data), p_law_(0)
    { if (p_data) this->initialize(p_data->sizeRows(), p_data->sizeCols()) ;}

  public:
    /** destructor */
    ~IStatModel() {}
    /** @return the total available observations */
    inline MultivariateLaw const* p_law() const { return p_law_;}
    /** Set the probability law of the model.
     *  @param p_law the probability law of the model
     **/
    inline void setLaw( MultivariateLaw* p_law) { p_law_ = p_law;}
    /** Set the data set of the model.
     *  @param data the data set of the model
     **/
    inline void setData( Data const& data)
    { p_data_ = &data;
      this->setNbSample(p_data_->sizeRows());
      this->setNbVariable(p_data_->sizeCols()) ;
    }
    /** Set the data set of the model.
     *  @param p_data the data set of the model
     **/
    inline void setData( Data const* p_data)
    { p_data_ = p_data;
      if (p_data_)
      {
        this->setNbSample(p_data_->sizeRows());
        this->setNbVariable(p_data_->sizeCols()) ;
      }
    }

  protected:
    /** A pointer on the original data set */
    Data const* p_data_;
    /** a pointer on the probability law. */
    MultivariateLaw* p_law_;
    /** compute the log Likelihood of the statistical model. */
    void compLnLikelihood()
    {
      // no data
      if (!this->p_data_) return;
      // check there exists a law
      if (!p_law_)
        STKRUNTIME_ERROR_NO_ARG(IStatModel::compLnLikelihood,p_law_ is not initialized);
      Real sum = 0.0;
      for (int i= p_data_->beginRows(); i<= p_data_->lastIdxRows(); i++)
      { sum += p_law_->lpdf(p_data_->row(i));}
      setLnLikelihood(sum);
    }
};

} // namespace STK

#endif /* STK_ISTATMODEL_H */
