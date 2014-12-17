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

/** @file STK_BernoulliModel.h
 *  @brief In this file we define the class BernoulliModel.
 **/

#ifndef STK_BERNOULLIMODEL_H
#define STK_BERNOULLIMODEL_H

#include <cmath>

#include "STK_IUnivStatModel.h"
#include "STatistiK/include/STK_Law_Bernoulli.h"
#include "Arrays/include/STK_CArrayVector.h"

namespace STK
{

/** @ingroup StatModels
 * A Bernoulli model, is a statistical model of the following form
 * \f[
 *     f(x_i = x|\theta) =
 *       p^{x_i} (1-p)^{1-x_i}, \quad x_i\in\{0,1\}, \quad i=1\ldots,n.
 * \f]
 **/
template <class Array, class WColVector>
class BernoulliModel : public IUnivStatModel<Array, WColVector, Law::Bernoulli>
{
  using IUnivStatModel<Array, WColVector, Law::Bernoulli>::p_data;

  public:
    /** Type of the data contained in the container */
    typedef typename Array::Type Type;
    /** Base class */
    typedef IUnivStatModel<Array, WColVector, Law::Bernoulli> Base;
    /** default constructor. */
    BernoulliModel() : Base() {}
    /** Constructor with data set. */
    BernoulliModel(Array const& data) : Base(data) {}
    /** Constructor with a ptr on the data set. */
    BernoulliModel(Array const* p_data) : Base(p_data) {}
    /** Copy constructor. */
    BernoulliModel(BernoulliModel const& model) : Base(model) {}
    /** clone patern */
    BernoulliModel* clone() const { return new BernoulliModel(*this);}
    /** destructor */
    virtual ~BernoulliModel() {}
    /** compute the number of free parameters */
    virtual int computeNbFreeParameters() const { return 1;}

  protected:
    /** compute the empirical probability of success based on the observed
     * variables. The NA values are discarded.
     **/
    virtual void computeParameters()
    {
        Real sum=0.;
        int nbObs=p_data()->size();
        for (int i=p_data()->begin(); i<=p_data()->lastIdx(); ++i)
        { (p_data()->elt(i) == binaryNA_) ? --nbObs : sum += p_data()->elt(i);}
        if (nbObs != 0) { this->law_.setProb(sum/nbObs);}
                  else  { this->law_.setProb(0.);}
    }
    /** compute the weighted empirical probability of success based on the observed
     *  variables. The NA values are discarded.
     **/
    virtual void computeParameters(WColVector const& weights)
    {
        Real sum=0.;
        int nbObs=p_data()->size();
        for (int i=p_data()->begin(); i<=p_data()->lastIdx(); ++i)
        { (p_data()->elt(i) == binaryNA_) ? --nbObs : sum += weights[i]*p_data()->elt(i);}
        if (nbObs != 0) { this->law_.setProb(sum/nbObs);}
                  else { this->law_.setProb(0.);}
    }
};

} // namespace STK

#endif /* STK_BERNOULLIMODEL_H */
