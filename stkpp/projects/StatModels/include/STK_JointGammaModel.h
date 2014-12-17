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

/** @file STK_JointGammaModel.h
 *  @brief In this file we define the class JointGammaModel.
 **/

#ifndef STK_JOINTGAMMAMODEL_H
#define STK_JOINTGAMMAMODEL_H

#include <cmath>

#include "STK_IMultiStatModel.h"
#include "STK_JointGammaParameters.h"
#include "STatistiK/include/STK_Law_Gamma.h"
#include "Analysis/include/STK_Algo_FindZero.h"
#include "Analysis/include/STK_Funct_raw.h"

namespace STK
{

/** @ingroup StatModels
 * A joint Gamma model is a statistical model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) =
 *     \prod_{j=1}^p\left(\frac{x_i^j}{b_j}\right)^{a_j-1}
 *                   \frac{e^{-x_i^j/b_j}}{b_j \, \Gamma(a_j)},
 *      \quad x_i^j\in\{0,1\}, \quad j=1,\ldots,p, \quad i=1,\ldots,n.
 * \f]
 **/
template <class Array, class WColVector>
class JointGammaModel : public IMultiStatModel<Array, WColVector, JointGammaParameters >
{

  public:
    /** Type of the data contained in the container */
    typedef typename Array::Type Type;
    /** Type of the row vector of the container */
    typedef typename Array::Row RowVector;
    /** Type of the column vector of the container */
    typedef typename Array::Col ColVector;
    /** Base class */
    typedef IMultiStatModel<Array, WColVector, JointGammaParameters > Base;
    using Base::p_data;
    using Base::p_param;
    /** default constructor. */
    JointGammaModel() : Base() {}
    /** Constructor with data set. */
    JointGammaModel(Array const& data) : Base(data){}
    /** Constructor with a ptr on the data set. */
    JointGammaModel(Array const* p_data) : Base(p_data){}
    /** Copy constructor. */
    JointGammaModel(JointGammaModel const& model) : Base(model) {}
    /** destructor */
    virtual ~JointGammaModel(){}
    /** clone pattern. @return a clone of this. */
    JointGammaModel* clone() const { return new JointGammaModel(*this);}

    /** @return the vector of the mean of the observations */
    inline Array2DPoint<Real> const& shape() const {return p_param()->shape_;}
    /** @return the vector of the mean of the observations */
    inline Array2DPoint<Real> const& scale() const {return p_param()->scale_;}
    /** @return the vector of the mean of the observations */
    inline Array2DPoint<Real> const& mean() const {return p_param()->mean_;}
    /** vector of the mean log of the observations */
    inline Array2DPoint<Real> const& meanLog() const {return p_param()->meanLog_;}
    /** vector of the variance of the observations */
    inline Array2DPoint<Real> const& variance() const {return p_param()->variance_;}

    /** compute the number of free parameters */
    virtual int computeNbFreeParameters() const
    { return 2*p_data()->sizeCols();}
    /** compute the log Likelihood of an observation. */
    virtual Real computeLnLikelihood( RowVector const& rowData) const
    {
      Real sum =0.;
      for (Integer j= rowData.begin(); j <= rowData.lastIdx(); ++j)
      { sum += Law::Gamma::lpdf(rowData[j], shape()[j], scale()[j]);}
      return sum;
    }
  protected:
    class dloglikelihood : public IFunction<dloglikelihood >
    {
      public:
        dloglikelihood( Real const& mean, Real const& meanLog)
                      : delta_(meanLog - Real(std::log(mean))) {}
        /** @return the value of the function at a
         * @param a a positive real value
         **/
        inline Real fImpl(Real a) const
        { return (delta_ + std::log(a) - Funct::psi_raw(a));}
        /** @return the minimal value of the function at x */
        inline Real xminImpl() const { return 0;}
      private:
        Real delta_;
    };
    /** compute the parameters */
    virtual void computeParameters()
    {
      for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
      {
        mean()[j] =  p_data()->col(j).safe().mean();
        meanLog()[j] = p_data()->col(j).safe(1.).log().mean();
        variance()[j] = p_data()->col(j).safe().variance();
        Real x0 = (mean()[j]*mean()[j]) / variance()[j];
        Real x1 = 0.9*x0 +  0.05/(mean()[j] - meanLog()[j]);
        dloglikelihood funct(mean()[j], meanLog()[j]);
        Real a =  Algo::findZero(funct, x0, x1);
        // replace with moment estimate if needed
        if (!Arithmetic<Real>::isFinite(a)) { a =  mean()[j]*mean()[j]/variance()[j];}
        shape()[j] = a;
        scale()[j] = mean()[j]/a;
      }
    }
    /** compute the weighted parameters */
    virtual void computeParameters( WColVector const& weights)
    {
      for (int j=p_data()->beginCols(); j < p_data()->endCols(); ++j)
      {
        mean()[j] =  p_data()->col(j).safe().wmean(weights);
        meanLog()[j] = p_data()->col(j).safe(1).log().wmean(weights);
        variance()[j] = p_data()->col(j).safe().wvariance(weights);
        Real x0 = (mean()[j]*mean()[j]) / variance()[j];
        Real x1 = 0.9*x0 +  0.05/(mean()[j] - meanLog()[j]);
        dloglikelihood funct(mean()[j], meanLog()[j]);
        Real a =  Algo::findZero(funct, x0, x1);
        // replace with moment estimate if needed
        if (!Arithmetic<Real>::isFinite(a)) { a =  mean()[j]*mean()[j]/variance()[j];}
        shape()[j] = a;
        scale()[j] = mean()[j]/a;
      }
    }

  private:
    /** @return the vector of the mean of the observations */
    inline Array2DPoint<Real>& shape() {return p_param()->shape_;}
    /** @return the vector of the mean of the observations */
    inline Array2DPoint<Real>& scale() {return p_param()->scale_;}
    /** @return the vector of the mean of the observations */
    inline Array2DPoint<Real>& mean() {return p_param()->mean_;}
    /** vector of the mean log of the observations */
    inline Array2DPoint<Real>& meanLog() {return p_param()->meanLog_;}
    /** vector of the variance of the observations */
    inline Array2DPoint<Real>& variance() {return p_param()->variance_;}
};

} // namespace STK

#endif /* STK_JOINTGAMMAMODEL_H */
