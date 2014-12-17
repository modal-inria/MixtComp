/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

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
 * Purpose:  Interface base class for all univariate distributions laws.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_Law_IUnivLaw.h
 *  @brief In this file we define the interface base class IUnivLaw for all
 *  probabilities laws.
 **/

#ifndef STK_LAW_IUNIVLAW_H
#define STK_LAW_IUNIVLAW_H

// RandBase header
#include "STK_Law_ILawBase.h"
#include "Arrays/include/STK_ArrayBase.h"
#include "STKernel/include/STK_Real.h"

namespace STK
{

namespace Law
{
/** @ingroup Laws
 *  @brief Interface base class for all the univariate distributions.
 *
 *  A general probability law (discrete or real) possess a
 *  probability density law. It can be simulated.
 *
 * Interface base class for the univariate distributions. Every derived
 * class have to furnish :
 * - a random generator method
 * @code Type rand() const @endcode
 * - a quantile method (inverse cdf)
 * @code Type icdf(Real const& p) const @endcode .
 *
 * The derived objects should also furnish the same static functions.
 * Instantiation of a derived object is interesting when one want to
 * simulate independent identical distributed random variates : the
 * creation of the object initialize all parameter-dependent variables.
 **/
template <class Type>
class IUnivLaw : public ILawBase
{
  protected:
    /** Constructor.
     *  @param name the name of the law
     **/
    IUnivLaw(String const& name) : ILawBase(name) {}
    /** copy Constructor.
     *  @param law the law to copy
     **/
    IUnivLaw(IUnivLaw const& law) : ILawBase(law.name_) {}

  public:
    /** Virtual destructor. **/
    inline virtual ~IUnivLaw() {}
    /** Pseudo-random Real law generator for a two dimensional
     *  container of Real.
     *  @param A the container to store the random numbers
     **/
    template < class Array>
    void randArray( ArrayBase< Array>& A) const
    {
      for (int j=A.beginCols(); j<=A.lastIdxCols(); j++)
        for (int i=A.beginRows(); i<=A.lastIdxRows(); i++)
          A(i, j) = rand();
    }
    /** @return a @c Type random variate . */
    virtual Type rand() const =0;
    /** @brief compute the probability distribution function (density) in the
     *  continuous case and the probability mass function in the discrete case.
     *  @param x the value to compute the pdf.
     *  @return the value of the pdf
     **/
    virtual Real pdf(Type const& x) const =0;
    /** @brief compute the log probability distribution function
     *  Give the value of the log-pdf at the point x.
     *  @param x the value to compute the lpdf.
     *  @return the value of the log-pdf
     **/
    virtual Real lpdf(Type const& x) const = 0;
    /** @brief compute the cumulative distribution function
     *  Give the probability that a random variate is less or equal
     *  to t.
     *  @param t a real number
     *  @return the value of the cdf
     **/
    virtual Real cdf(Real const& t) const =0;
    /** @brief inverse cumulative distribution function
     *  The quantile is defined as the smallest value @e x such that
     *  <em> F(x) >= p </em>, where @e F is the cumulative distribution function.
     *  @param p a probability
     **/
    virtual Type icdf(Real const& p) const=0;
};

} // namespace Law

} //  namespace STK

#endif /*STK_LAW_IUNIVLAW_H*/
