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
 * Purpose: define the Interface base class ICriterion.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_PenCriterion.h
 *  @brief In this file we define the classes for the penalized criterion
 **/

#ifndef STK_PENCRITERION_H
#define STK_PENCRITERION_H

#include "Sdk/include/STK_IRunner.h"
#include "STK_IStatModelBase.h"

namespace STK
{
/** @ingroup StatModels
 * @brief Interface base class for the selection model criterion. The pure
 * virtual function @c run will be implemented in derived class and compute
 * the value_ member.
  */
class ICriterion : public IRunnerBase
{
  protected:
    /** Default Constructor. */
    inline ICriterion() : p_model_(), value_(Arithmetic<Real>::NA()) {}
    /** Constructor.
     *  @param p_model a pointer on the current model
     **/
    inline ICriterion( IStatModelBase const* p_model)
                     : p_model_(p_model), value_(Arithmetic<Real>::NA()){}
    /** Constructor.
     *  @param model the current model
     **/
    inline ICriterion( IStatModelBase const& model)
                     : p_model_(&model), value_(Arithmetic<Real>::NA()) {}
    /** copy Constructor.
     *  @param criterion the criterion to copy
     **/
    inline ICriterion( ICriterion const& criterion)
                     : p_model_(criterion.p_model_), value_(criterion.value_) {}

  public:
    /** Destructor */
    inline virtual ~ICriterion() {}
    /** @return The value of the criterion */
    inline Real const& value() const { return value_;}
    /** @param p_model a pointer on the current model to set */
    void setModel( IStatModelBase const* p_model);
    /** @param model the current model to set */
    void setModel( IStatModelBase const& model);

  protected:
    /** The current statistical model to use*/
    IStatModelBase const* p_model_;
    /** Computed value of the criterion */
    Real value_;
};

/** @ingroup StatModels
 *  @brief Derived class of Criterion for computing the AIC Criterion
 *  The AIC criteria is a penalization of the likelihood given by the formula
 *  \f[
 *  -2 \cdot \ln{p(x|k)} \approx \mathrm{AIC} = {-2 \cdot \ln{L} +2 D }
 *  \f]
 *  where \f$ L \f$ represents the likelihood of the observations and \f$ D \f$
 *  the number of free parameter of the model.
 **/
class AICCriterion : public ICriterion
{
  public:
    /** Default Constructor. */
    inline AICCriterion() : ICriterion() {}
    /** Constructor.
     *  @param p_model apointer on the current model
     **/
    inline AICCriterion( IStatModelBase* const p_model) : ICriterion(p_model) {}
    /** Constructor.
     *  @param model the current model
     **/
    inline AICCriterion( IStatModelBase const& model) : ICriterion(model) {}
    /** copy Constructor.
     *  @param criterion the criterion to copy
     **/
    inline AICCriterion( AICCriterion const& criterion) : ICriterion(criterion) {}
    /** virtual destructor. */
    inline virtual ~AICCriterion() {}
    /** clone pattern */
    inline AICCriterion* clone() const { return new AICCriterion(*this);}
    /** implementation of the virtual method run */
    virtual bool run();
};


/** @ingroup StatModels
 *  @brief Derived class of Criterion for computing the BIC Criterion
 *  The Bic criteria is a penalization of the likelihood given by the formula
 *  \f[
 *  -2 \cdot \ln{p(x|k)} \approx \mathrm{BIC} = {-2 \cdot \ln{L} + D \ln(n) }
 *  \f]
 *  where \f$ L \f$ represents the likelihood of the observations, \f$ D \f$ the
 *  number of free parameter of the model and \f$ n \f$ the number of sample.
 **/
class BICCriterion : public ICriterion
{
  public:
    /** Default Constructor. */
    inline BICCriterion() : ICriterion() {}
    /** Constructor.
     *  @param p_model apointer on the current model
     **/
    inline BICCriterion( IStatModelBase* const p_model) : ICriterion(p_model) {}
    /** Constructor.
     *  @param model the current model
     **/
    inline BICCriterion( IStatModelBase const& model) : ICriterion(model) {}
    /** copy Constructor.
     *  @param criterion the criterion to copy
     **/
    inline BICCriterion( BICCriterion const& criterion) : ICriterion(criterion) {}
    /** virtual destructor. */
    inline virtual ~BICCriterion() {}
    /** clone pattern */
    inline BICCriterion* clone() const { return new BICCriterion(*this);}
    /** implementation of the virtual method run */
    virtual bool run();
};

} // namespace STK

#endif /** STK_PENCRITERION_H */
