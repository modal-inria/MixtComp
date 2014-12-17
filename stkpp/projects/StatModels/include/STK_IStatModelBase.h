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
 * Project:  stkpp::
 * created on: 15 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_IStatModelBase.h
 *  @brief In this file we define the interface base class IStatModelBase.
 **/


#ifndef STK_ISTATMODELBASE_H
#define STK_ISTATMODELBASE_H

#include <cmath>
#include "STKernel/include/STK_Real.h"

namespace STK
{
/** @ingroup StatModels
 *  @brief Interface base class for all Statistical Models.
 *  This Interface can be used for any kind of data set.
 *  However, it is not allowed to instantiate directly
 *  this class. For a concrete class, use a derived class of the IStatModel
 *  class @sa IStatModel.
 *  When using a model selection criterion like AIC or BIC, this class is
 *  self-sufficient.
 *   @sa AICCriterion, BICCriterion, IStatModel, IMixtureComposer.
 **/
class IStatModelBase
{
  protected:
    /** Default constructor. */
    inline IStatModelBase() { initialize(0, 0);}
    /** Constructor with specified dimension.
     *  @param nbSample Number of samples of the model
     **/
    inline IStatModelBase( int nbSample)
    { initialize(nbSample, 0);}
    /** Constructor with specified dimension.
     *  @param nbSample, nbVariable Number of samples and variables of the model
     **/
    inline IStatModelBase( int nbSample, int nbVariable)
    { initialize(nbSample, nbVariable);}
    /** Copy constructor.
     *  @param model the model to copy
     **/
    inline IStatModelBase( IStatModelBase const& model)
                         : nbSample_(model.nbSample_)
                         , nbVariable_(model.nbVariable_)
                         , lnLikelihood_(model.lnLikelihood_)
                         , nbFreeParameter_(model.nbFreeParameter_)
    {}
    /** destructor */
    inline ~IStatModelBase() {}

  public:
    /** @return the total number of observations */
    inline int nbSample() const { return nbSample_;}
    /** @return the log of the total number of observations */
    inline Real lnNbSample() const
    { return (nbSample_ <= 0) ? -Arithmetic<Real>::infinity() : std::log((Real)nbSample_);}
    /** @return the number of variables */
    inline int nbVariable() const { return nbVariable_;}
    /** @return The ln-likelihood */
    inline Real lnLikelihood() const { return lnLikelihood_;}
    /** @return The likelihood */
    inline Real likelihood() const
    { return (Arithmetic<Real>::isFinite(lnLikelihood_)) ? std::exp((Real)lnLikelihood_) : 0.;}
    /** @return the total number of free parameters */
    inline int nbFreeParameter() const { return nbFreeParameter_;}
    /** @return the computed ICL criteria. */
    inline Real computeBIC() const
    { return (- 2. * lnLikelihood() + nbFreeParameter() * lnNbSample());}
    /** @return the computed ICL criteria. */
    inline Real computeAIC() const
    { return (2.*(-lnLikelihood()+nbFreeParameter()));}

  protected:
    /** set the number of free parameters of the model
     *  @param nbFreeParameter number of free parameters of the model
     * */
    inline void setNbFreeParameter( int nbFreeParameter)
    { nbFreeParameter_ = nbFreeParameter;}
    /** set the number of samples of the model
     *  @param nbSample number of samples of the model
     * */
    inline void setNbSample( int nbSample) { nbSample_ = nbSample;}
    /** set the number of variables of the model
     *  @param nbVariable number of variables of the model
     * */
    inline void setNbVariable( int nbVariable) { nbVariable_ = nbVariable;}
    /** set the log-likelihood of the model
     *  @param lnLikelihood the log-likelihood of the model
     * */
    inline void setLnLikelihood( Real lnLikelihood) { lnLikelihood_ = lnLikelihood;}
    /** set the dimensions of the parameters of the model
     *  @param nbSample number of samples of the model
     *  @param nbVariable number of variables of the model
     * */
    inline void initialize( int nbSample, int nbVariable)
    {
      nbSample_ = nbSample;
      nbVariable_ = nbVariable;
      lnLikelihood_ = -Arithmetic<Real>::infinity();
      nbFreeParameter_ = 0;
    }

  private:
    /** total available samples */
    int nbSample_;
    /** total available variables */
    int nbVariable_;
    /** likelihood of the samples */
    Real lnLikelihood_;
    /** number of free parameters of the model */
    int nbFreeParameter_;
};

} // namespace STK

#endif /* STK_ISTATMODELBASE_H */
