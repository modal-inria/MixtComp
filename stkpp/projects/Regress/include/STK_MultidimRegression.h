/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2010  Serge Iovleff

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
 * Project:  stkpp::regress
 * created on: 27 oct. 2010
 * Purpose:  .
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_MultidimRegression.h
 *  @brief In this file we define the MultidimRegression class.
 **/

#ifndef MULTIDIMREGRESSION_H
#define MULTIDIMREGRESSION_H

#include "Arrays/include/STK_Array2D.h"

#include "STK_IRegression.h"

namespace STK
{

/** @brief The @c MultidimRegression class allows to regress a multidimensional
 *  output variable among a multivariate explanation variable.
 */
class MultidimRegression : public IRegression<ArrayXX, ArrayXX, Vector>
{
  public:
    /** Constructor.
     * @param y Variates to predict
     * @param x co-variates
     */
    MultidimRegression( ArrayXX const* y =0, ArrayXX const* x =0);

    /** Destructor. */
    virtual ~MultidimRegression();

    /** @return the extrapolated values y from the value @c x.
     *  Given the data set @c x will compute the values \f$ y = x.\hat{\beta} \f$.
     *  The coefficients @c coefs_ have to be estimated previously.
     *  @param x the input data set
     */
    virtual ArrayXX extrapolate(ArrayXX const& x) const;

  protected:
    ArrayXX coefs_;

  private:
    /** compute the regression function. */
    virtual void regressionStep();
    /** compute the weighted regression function.
     * @param weights the weights of the samples
     **/
    virtual void regression(VectorX const& weights);
    /** Compute the predicted outputs by the regression function. */
    virtual void predictionStep();
    /** Compute the number of parameter of the regression function.
     * @return the number of parameter of the regression function
     **/
    inline virtual int computeNbFreeParameter() const
    { return coefs_.sizeCols() * coefs_.sizeRows(); }
};

} // namespace STK

#endif /* MULTIDIMREGRESSION_H */
