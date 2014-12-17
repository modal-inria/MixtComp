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

/** @file STK_JointBernoulliModel.h
 *  @brief In this file we define the class JointBernoulliModel.
 **/

#ifndef STK_JOINTBERNOULLIMODEL_H
#define STK_JOINTBERNOULLIMODEL_H

#include <cmath>

#include "STK_IMultiStatModel.h"
#include "STatistiK/include/STK_Law_Bernoulli.h"

namespace STK
{

/** @ingroup StatModels
 *  Structure encapsulating the parameters of a Joint Bernoulli model.
 */
class JointBernoulliParameters: public IMultiParameters<JointBernoulliParameters>
{
  public:
    /** default constructor */
    JointBernoulliParameters() {}
    /** constructor with fixed size */
    JointBernoulliParameters( Range const& size)
                            : prob_(size, 0.5)
                            , lnProb_(size, -Const::_LN2_)
                            , ln1mProb_(size, -Const::_LN2_)
    {}
    /** copy constructor. @param param the parameters to copy. */
    JointBernoulliParameters( JointBernoulliParameters const& param)
                            : prob_(param.prob_)
                            , lnProb_(param.lnProb_)
                            , ln1mProb_(param.ln1mProb_)
    {}
    /** destructor */
    ~JointBernoulliParameters() {}
    /** @return the probability of success of the jth law */
    inline Real const prob(int const& j) const { return prob_[j];}
    /** @return the probability of success of the jth law */
    inline Real const lnProb(int const& j) const { return lnProb_[j];}
    /** @return the probability of success of the jth law */
    inline Real const ln1mProb(int const& j) const { return ln1mProb_[j];}
    /** set the probability of success of the jth law */
    inline void setProb(int const& j, Real const& prob)
    { prob_[j] = prob;
      if (prob>0) { lnProb_[j] = std::log(prob);}
      else        { lnProb_[j] = -Arithmetic<Real>::infinity();}
      if (prob<1) { ln1mProb_[j] = std::log(1.-prob);}
      else        { ln1mProb_[j] = -Arithmetic<Real>::infinity();}
    }
    /** resize the set of parameter */
    inline void resizeImpl(Range const& size)
    { prob_.resize(size); lnProb_.resize(size); ln1mProb_.resize(size);}
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os)
    { os << prob_ << _T("\n");}

  protected:
    Array2DPoint<Real> prob_;
    Array2DPoint<Real> lnProb_;
    Array2DPoint<Real> ln1mProb_;
};

/** @ingroup StatModels
 * A joint Bernoulli model is a statistical model of the form:
 * following form
 * \f[
 *     f(\mathbf{x}_i = x|\theta) =
 *     \prod_{j=1}^p p_{j}^{x_i^j} (1-p_{j})^{1-x_i^j},
 *      \quad x_i^j\in\{0,1\}, \quad j=1,\ldots,p, \quad i=1,\ldots,n.
 * \f]
 *
 **/
template <class Array, class WColVector = CVectorX>
class JointBernoulliModel : public IMultiStatModel<Array, WColVector, JointBernoulliParameters >
{
  public:
    /** Type of the data contained in the container */
    typedef typename Array::Type Type;
    /** Type of the row vector of the container */
    typedef typename Array::Row RowVector;
    /** Type of the column vector of the container */
    typedef typename Array::Col ColVector;
    /** Base class */
    typedef IMultiStatModel<Array, WColVector, JointBernoulliParameters > Base;
    using Base::p_data;
    using Base::p_param;
    /** default constructor. */
    JointBernoulliModel() : Base() {}
    /** Constructor with data set. */
    JointBernoulliModel(Array const& data) : Base(data) {}
    /** Constructor with a ptr on the data set. */
    JointBernoulliModel(Array const* p_data) : Base(p_data) {}
    /** Copy constructor. */
    JointBernoulliModel(JointBernoulliModel const& model) : Base(model) {}
    /** destructor */
    virtual ~JointBernoulliModel() {}
    /** clone pattern. @return a clone of this. */
    JointBernoulliModel* clone() const { return new JointBernoulliModel(*this);}

    /** compute the number of free parameters */
    virtual int computeNbFreeParameters() const
    { return p_data()->sizeCols();}
    /** compute the log Likelihood of an observation. */
    virtual Real computeLnLikelihood( RowVector const& rowData) const
    {
      Real sum =0.;
      for (Integer j= rowData.begin(); j <= rowData.lastIdx(); ++j)
      {
        sum += rowData[j] * p_param()->lnProb(j)
             + (1-rowData[j] * p_param()->ln1mProb(j) );
      }
      return sum;
    }
  protected:
    /** compute the parameters */
    virtual void computeParameters()
    {
      for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
      {
        Real sum=0.;
        int nbObs=p_data()->sizeRows();

        for (int i=p_data()->beginRows(); i<=p_data()->lastIdxRows(); ++i)
        { (p_data()->elt(i,j) == binaryNA_) ? --nbObs : sum += p_data()->elt(i,j);}
        if (nbObs != 0) { p_param()->setProb(j,sum/nbObs);}
                   else { p_param()->setProb(j, Arithmetic<Real>::NA());}
      }
    }
    /** compute the weighted parameters
     *  @param weights the weights of the samples
     **/
    virtual void computeParameters(WColVector const& weights)
    {
      // compute
      for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
      {
        Real sum=0., wsum = 0.;
        for (int i=p_data()->beginRows(); i<=p_data()->lastIdxRows(); ++i)
        { if (p_data()->elt(i,j) != binaryNA_)
          { sum  += weights[i]*p_data()->elt(i,j);
            wsum += weights[i];
          }
        }
        if (wsum != 0) { p_param()->setProb(j, sum/wsum);}
                  else { p_param()->setProb(j, Arithmetic<Real>::NA());}
      }
    }
};

} // namespace STK

#endif /* STK_JOINTBERNOULLIMODEL_H */
