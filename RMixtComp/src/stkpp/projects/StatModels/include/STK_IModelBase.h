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

/** @file STK_IModelBase.h
 *  @brief In this file we define the interface base class IModelBase.
 **/


#ifndef STK_IMODELBASE_H
#define STK_IMODELBASE_H

#include <cmath>
#include "../../STKernel/include/STK_Real.h"
#include "../../STKernel/include/STK_Integer.h"

namespace STK
{
/** @ingroup StatModels
 *  @brief Interface base class for all Statistical Models.
 *  This Interface can be used for any kind of data set. In particular when
 *  we use a model selection criterion like AIC or BIC @sa AICCriterion,
 *  @sa BICCriterion. However, it is not allowed to instantiate directly
 *  this class. For a concrete class, use a derived class of the StatisticalModel
 *  class @sa StatisticalModel.
 **/
class IModelBase
{
  protected:
    /** Default constructor. */
    inline IModelBase() { initialize(0, 0);}
    /** Constructor with specified dimension.
     *  @param nbSample number of sample of the model
     *  @param nbVar number of variable of the model
     **/
    inline IModelBase( int const& nbSample, int const& nbVar)
    { initialize(nbSample, nbVar);}
    /** Copy constructor.
     *  @param model the model to copy
     **/
    inline IModelBase( IModelBase const& model)
                     : nbSample_(model.nbSample_)
                     , nbVar_(model.nbVar_)
                     , lnLikelihood_(model.lnLikelihood_)
                     , nbFreeParameter_(model.nbFreeParameter_)
    {}
    /** destructor */
    inline ~IModelBase() {}

  public:
    /** @return the total available observations */
    inline int const& nbSample() const { return nbSample_;}
    /** @return th Log of the total available observations */
    inline Real lnNbSample() const
    { return (nbSample_ <= 0) ? -Arithmetic<Real>::infinity() : std::log((Real)nbSample_);}
    /** @return the total available observations*/
    inline int const& nbVar() const { return nbVar_;}
    /** @return The ln-likelihood */
    inline Real lnLikelihood() const { return lnLikelihood_;}
    /** @return The likelihood */
    inline Real likelihood() const
    { return (Arithmetic<Real>::isFinite(lnLikelihood_)) ? std::exp((Real)lnLikelihood_) : 0.;}
    /** @return the total number of free parameters */
    inline int const& nbFreeParameter() const { return nbFreeParameter_;}

  protected:
    /** set the number of free parameters of the model
     *  @param nbFreeParameter number of free parameters of the model
     * */
    inline void setNbFreeParameter( int const& nbFreeParameter) { nbFreeParameter_ = nbFreeParameter;}
    /** set the number of sample of the model
     *  @param nbSample number of sample of the model
     * */
    inline void setNbSample( int const& nbSample) { nbSample_ = nbSample;}
    /** set the number of variables of the model
     *  @param nbVar number of variables of the model
     * */
    inline void setNbVariable( int const& nbVar) { nbVar_ = nbVar;}
    /** set the log-likelihood of the model
     *  @param lnLikelihood the log-likelihood of the model
     * */
    inline void setLnLikelihood( Real const& lnLikelihood) { lnLikelihood_ = lnLikelihood;}
    /** set the dimensions of the parameters of the model
     *  @param nbSample number of sample of the model
     *  @param nbVar number of variable of the model
     * */
    inline void initialize( int const& nbSample, int const& nbVar)
    {
      nbSample_ = nbSample;
      nbVar_ = nbVar;
      lnLikelihood_ = -Arithmetic<Real>::infinity();
      nbFreeParameter_ = 0;
    }

  private:
    /** total available samples */
    int nbSample_;
    /** total available variables */
    int nbVar_;
    /** likelihood of the sample */
    Real lnLikelihood_;
    /** number of free parameter of the model */
    int nbFreeParameter_;
};

} // namespace STK

#endif /* STK_IMODELBASE_H */
