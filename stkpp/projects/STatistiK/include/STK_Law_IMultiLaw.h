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
 * Project:  stkpp::STatistiK::Law
 * created on: 29 juil. 2011
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Law_IMultiLaw.h
 *  @brief In this file we define the Interface base class IMultiLaw
 *  and the JointProbability class.
 **/

#ifndef STK_ITMULTILAW_H
#define STK_ITMULTILAW_H

#include "STKernel/include/STK_Real.h"
#include "Arrays/include/STK_Array1D.h"
#include "STK_Law_ILawBase.h"

namespace STK
{

namespace Law
{

/** @ingroup Laws
 *  @brief Interface base Class for the multivariate distributions.
 *
 *  Given some sample x in some data space, a multivariate law can compute
 *  a probability density function (pdf), a log-pdf. Moreover it is possible
 *  to simulate some random realization of this law.
 *
 *  @tparam RowVector is the type of container containing the sample x.
 */
template<class RowVector>
class IMultiLaw: public ILawBase
{
  protected:
    /** Constructor. */
    IMultiLaw( String const& name) : ILawBase(name) {}

  public:
    /** destructor. */
    virtual ~IMultiLaw() {}
    /** @brief compute the probability distribution function (density) of the
     * multivariate law.
     *  Give the value of the pdf at the point x.
     *  @param x the multivariate value to compute the pdf.
     *  @return the value of the pdf
     **/
    virtual Real pdf( RowVector const& x) const =0;
    /** @brief compute the log probability distribution function
     *  Give the value of the log-pdf at the point x.
     *  @param x the multivariate value to compute the lpdf.
     *  @return the value of the log-pdf
     **/
    virtual Real lpdf( RowVector const& x) const =0;
    /** @brief simulate a realization of the Multivariate Law and store the
     *  result in x.
     *  @param[out] x the simulated value.
     **/
    virtual void rand( RowVector& x) const =0;
};

/** @ingroup Laws
 *  @brief joint density class for the multivariate distributions.
 *
 *  Given some sample x in some data space, a JointProbability will model a
 *  joint probability law. All the marginal densities/probabilities will
 *  belong to the same family.
 *
 *  @tparam RowVector is the type of container containing a sample x.
 *  @tparam Law Any probability law deriving from Law::IUnivLaw.
 */
template<class RowVector, class Law>
class JointProbability: public IMultiLaw<RowVector>
{
  public:
    typedef typename RowVector::Type Type;
    typedef IMultiLaw<RowVector> Base;
    /** Constructor.
     *  @param name name of the joint law
     **/
    JointProbability( String const& name)
                    : Base(name), jointLaw_() {}
    /** Constructor.
     *  @param name name of the joint law
     *  @param nbVar dimension of the sample
     **/
    JointProbability( String const& name, int nbVar)
                    : Base(name), jointLaw_(nbVar) {}
    /** destructor */
    virtual ~JointProbability() {}
    /** @return the array with the marginals laws */
    Array1D<Law> const& JointLaw() const { return jointLaw_;}

    /** @return the jth marginal law */
    Law const& law(int const& j) const { return jointLaw_[j];}
    /** @return the jth marginal law */
    Law& law(int const& j) { return jointLaw_[j];}
    /** @return safely the jth marginal law */
    Law const& atLaw(int const& j) const { return jointLaw_.at(j);}
    /** @return safely the jth marginal law */
    Law& atLaw(int const& j) { return jointLaw_.at(j);}
    /** resize the joint probability */
    void resize(int const& nbVar) { jointLaw_.resize(nbVar);}

    /** @brief compute the probability distribution function (density) of the
     * multivariate law.
     *  Give the value of the pdf at the point x.
     *  @param x the multivariate value to compute the pdf.
     *  @return the value of the pdf
     **/
    virtual Real pdf( RowVector const& x) const
    { return std::exp((double)lpdf(x));}
    /** @brief compute the log probability distribution function
     *  Give the value of the log-pdf at the point x.
     *  @param x the multivariate value to compute the lpdf.
     *  @return the value of the log-pdf
     **/
    virtual Real lpdf( RowVector const& x) const
    {
      if (x.range() != jointLaw_.range())
      {STKRUNTIME_ERROR_NO_ARG(JointProbability::lpdf(x),dimensions mismatch);}
      Real sum = 0.;
      for (int j= x.begin(); j <= x.lastIdx(); ++j)
      { sum+= Arithmetic<Type>::isNA(x[j]) ? 0. : jointLaw_[j].lpdf(x[j]);}
      return sum;
    }
    /** @brief simulate a realization of the joint law and store the
     *  result in x.
     *  @param[out] x the simulated value.
     **/
    virtual void rand( RowVector& x) const
    {
      x.resize(jointLaw_.range());
      for (int j= x.begin(); j <= x.lastIdx(); ++j)
      { x[j] = jointLaw_[j].rand();}
    }
  protected:
    /** Array with the marginal laws */
    Array1D<Law> jointLaw_;
};


} // namespace Law

} // namespace STK

#endif /* STK_ITMULTILAW_H */
