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

#ifndef STK_JOINTBERNOULLIMODEL_H
#define STK_JOINTBERNOULLIMODEL_H

#include <cmath>

#include "STK_IMultiStatModel.h"
#include "../../STatistiK/include/STK_Law_JointBernoulli.h"

namespace STK
{

/** @ingroup Model
 * A joint Bernoulli model is a statistical model of the form:
 * following form
 * \f[
 *     f(\mathbf{x}_i = x|\theta) =
 *     \prod_{j=1}^p p_{j}^{x_i^j} (1-p_{j})^{1-x_i^j},
 *      \quad x_i^j\in\{0,1\}, \quad j=1,\ldots,p, \quad i=1,\ldots,n.
 * \f]
 *
 **/
template <class Array>
class JointBernoulliModel : public IMultiStatModel<Array, Law::JointBernoulli< typename Array::Row> >
{
  using IRunnerConst<Array>::p_data;
  public:
    /** Type of the data contained in the container */
    typedef typename Array::Type Type;
    /** Type of the data contained in the container */
    typedef typename Array::Col ColVector;
    /** Runner */
    typedef IMultiStatModel<Array, Law::JointBernoulli<typename Array::Row> > Base;

  public:
    /** default constructor. */
    JointBernoulliModel() : Base() {}
    /** Constructor with data set. */
    JointBernoulliModel(Array const& data) : Base(data) {}
    /** Constructor with a ptr on the data set. */
    JointBernoulliModel(Array const* p_data) : Base(p_data) {}
    /** destructor */
    virtual ~JointBernoulliModel() {}
    /** compute the empirical probability of success based on the observed
     * variables. The NA values are discarded.
     **/
    virtual bool run()
    {
      try
      {
        this->law().resize(p_data()->sizeCols());
        for (int j=p_data()->firstIdxCols(); j<=p_data()->lastIdxCols(); ++j)
        {
          Real sum=0.;
          int nbObs=p_data()->sizeRows();
          for (int i=p_data()->firstIdxRows(); i<=p_data()->lastIdxRows(); ++i)
          { (p_data()->elt(i,j) == binaryNA_) ? --nbObs : sum += p_data()->elt(i,j);}
          if (nbObs != 0) { this->law().setProb(j,sum/nbObs);}
                     else { this->law().setProb(j,0.);}
        }
        // compute log-likehood
        this->computeLnLikelihood();
        this->setNbFreeParameter(p_data()->sizeCols());
        return true;
      }
      catch (Exception const& e)
      { this->msg_error_ = e.error();}
      return false;
    }
    /** compute the weighted empirical probability of success based on the observed
     *  variables. The NA values are discarded.
     **/
    virtual bool run(typename Array::Col const& weights)
    {
      try
      {
        this->law().resize(p_data()->sizeCols());
        for (int j=p_data()->firstIdxCols(); j<=p_data()->lastIdxCols(); ++j)
        {
          Real sum=0.;
          int nbObs=p_data()->sizeRows();
          for (int i=p_data()->firstIdxRows(); i<=p_data()->lastIdxRows(); ++i)
          { (p_data()->elt(i,j) == binaryNA_) ? --nbObs : sum += weights[i]*p_data()->elt(i,j);}
          if (nbObs != 0) { this->law().setProb(j,sum/nbObs);}
                     else { this->law().setProb(j,0.);}
        }
        // compute log-likehood
        this->computeLnLikelihood();
        this->setNbFreeParameter(p_data()->sizeCols());
        return true;
      }
      catch (Exception const& e)
      { this->msg_error_ = e.error();}
      return false;
    }
};

} // namespace STK

#endif /* STK_JOINTBERNOULLIMODEL_H */
