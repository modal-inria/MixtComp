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

 Contact : Serge.Iovleff@stkpp.org
 */

/*
 * Project:  stkpp::Regress
 * created on: 23 juin 2010
 * Purpose:  Interface base class for regression methods.
 * Author:   iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_IRegression.h
 *  @brief In this file we define the Interface base class IRegression.
 **/

#ifndef STK_IREGRESSION_H
#define STK_IREGRESSION_H

#include "../../../include/STKernel.h"
#include "../../../include/Arrays.h"

namespace STK
{

/** @ingroup Regress
 * @brief Interface base class for Regression methods.
 *
 * Regression models involve the following variables:
 * - The <em>unknown parameters</em>, denoted as <em> β</em>, which may represent a scalar or a vector.
 * - The <em>independent variables</em>, <em>X</em>.
 * - The <em>dependent variable</em>, <em>Y</em>.
 *
 * In various fields of application, different terminologies are used in place of dependent and independent variables.
 * A regression model relates <em>Y</em> to a function of <em>X</em> and <em>β</em>.
 * \f$ Y \approx f (\mathbf {X}, \boldsymbol{\beta} ) \f$
 *
 * The approximation is usually formalized as \f$ E(Y|X)=f(X,β) \f$.  To carry out regression analysis, the form of the function
 * <em>f</em> must be specified.  Sometimes the form of this function is based on knowledge about the relationship between
 * <em>Y</em> and <em>X</em> that does not rely on the data.  If no such knowledge is available, a flexible or convenient form
 * for <em>f</em> is chosen.
 *
 * Assume now that the vector of unknown parameters <em>β</em> is of length <em>k</em>. In order to perform a regression analysis
 * the user must provide information about the dependent variable <em>Y</em>:
 * -If <em>N</em> data points of the form (<em>Y</em>,<em>X</em>) are observed, where <em>N</em> < <em>k</em>, most classical
 * approaches to regression analysis cannot be performed: since the system of equations defining the regression model is under-determined,
 * there is not enough data to recover <em>β</em>.
 * If exactly <em>N</em>=<em>k</em> data points are observed, and the function <em>f</em> is linear, the equations
 * <em>Y</em>=<em>f</em>(<em>X</em>, <em>β</em>) can be solved exactly rather than approximately. This reduces to solving a set of <em>N</em>
 * equations with <em>N</em> unknowns (the elements of <em>β</em>), which has a unique solution as long as the <em>X</em> are linearly independent.
 * If <em>f</em> is nonlinear, a solution may not exist, or many solutions may exist.
 * -The most common situation is where <em>N</em> > <em>k</em> data points are observed.  In this case, there is enough information in the data
 * to estimate a unique value for <em>β</em> that best fits the data in some sense, and the regression model when applied to the data can be
 * viewed as an overdetermined system in <em>β</em>.
 *
 * In the last case, the regression analysis provides the tools for:
 * #- Finding a solution for unknown parameters <em>β</em> that will, for example, minimize the distance between the measured and predicted values
 * of the dependent variable <em>Y</em> (also known as method of least squares).
 * #- Under certain statistical assumptions, the regression analysis uses the surplus of information to provide statistical information about the
 * unknown parameters <em>β</em> and predicted values of the dependent variable <em>Y</em>.
 *
 * The pure virtual function to implement are
 * @code
 *   virtual void regression();
 *   virtual void regression( WContainer const& weights);
 *   virtual void prediction();
 *   virtual int const& computeNbParameter() const;
 *   virtual void extrapolate( XContainer const& x, YContainer& y) const;
 * @endcode
 * The virtual function
 * @code
 *   virtual void preRun();
 * @endcode
 * can be overloaded.
 */
template <class YContainer, class XContainer, class WContainer>
class IRegression
{
  protected:
    /** Constructor. Initialize the data members.
     * @param p_y container with the observed output
     * @param p_x container with the predictors (inputs) of the model
     */
    IRegression( ArrayBase<YContainer> const* p_y =0, ArrayBase<XContainer> const* p_x =0)
               : p_y_(p_y->asPtrDerived())
               , p_x_(p_x->asPtrDerived())
               , p_predicted_(0)
               , p_residuals_(0)
               , nbParameter_(Arithmetic<int>::NA())
     {}

  public:
    /** virtual destructor. */
    virtual ~IRegression() { clear();}
    /** run the computations. */
    bool run()
    {
      // perform any pre-operation needed before the regression step
      preRun();
      // compute the regression
      regression();
      // Compute the number of parameter of the regression function.
      nbParameter_ = computeNbParameter();
      // predictions
      prediction();
      // compute residuals
      residuals();
      // return the result of the computations
      return true;
    }

    /** run the weighted computations.
     *  @param weights weights of the samples
     **/
    bool run( WContainer const& weights)
    {
      // perform any pre-operation needed before the regression step
      preRun();
      // compute weighted regression
      regression(weights);
      // Compute the number of parameter of the regression function.
      nbParameter_ = computeNbParameter();
      // create container of the predicted value and compute prediction
      prediction();
      // create container of the residuals and compute them
      residuals();
      // return the result of the computations
      return true;
    }
    /** get the last error message.
     * @return the last error message
     **/
    inline String const& error() const { return msg_error_;}

    /** get the pointer of the container of the predicted values.
     * The container @c p_predicted_ will not be deleted by @c this.
     * @return the pointer on the predicted values
     **/
    inline YContainer* p_predicted() const
    { return p_predicted_;}
    /** get the pointer of the container of the residuals. The container
     *  @c p_residuals_ will not be deleted by @c this.
     *  @return the pointer on the residuals
     **/
    inline YContainer* p_residuals() const
    {  return p_residuals_;}

    /** Give the number of parameter of the regression function.
     *  @return the number of parameter of the regression function
     **/
    inline int const& nbParameter() const
    {  return nbParameter_;}

    /** Set the data set the regression method should use.
     * @param p_y data set to adjust
     * @param p_x data set of the predictors
     */
    void setData( YContainer const* p_y, XContainer const* p_x)
    { p_y_ = p_y; p_x_ = p_x;}

    /** Set the Y data set the regression method should use.
     * @param p_y data set to adjust
     */
    void setY( YContainer const* p_y)
    { p_y_ = p_y;}

    /** Set the X data set.
     * @param p_x data set of the predictors
     */
    void setX( XContainer const* p_x)
    { p_x_ = p_x;}

    /** @return the extrapolated values of y from the value @c x.
     *  Given the data set @c x will compute the values \f$ y = \hat{f}(x) \f$.
     *  The regression function @e f have to be estimated previously.
     *  @param x the input data set
     */
    virtual YContainer extrapolate(XContainer const& x) const =0;

  protected:
    /** @brief perform any computation needed before the call of the regression
     *  method.
     *  At this level do nothing.
     */
    virtual void preRun()
    { }

    /** @brief Compute the residuals of the model.
     * The residuals of the model are computed by computing the difference
     * between the observed outputs and the predicted outputs
     * of the model.
     */
    inline void residuals()
    {
      if (p_residuals_) delete p_residuals_;
       p_residuals_ =  p_y_->clone();
      *p_residuals_ = *p_y_ - *p_predicted_;
    }

    /** Container of the output to regress. */
    YContainer const* p_y_;
    /** Container of the regressors. */
    XContainer const* p_x_;
    /** Container of the predicted output. */
    YContainer* p_predicted_;
    /** Container of the residuals. */
    YContainer* p_residuals_;

  private:
    /** number of parameter of the regression method. */
    int nbParameter_;

    /** compute the regression function. */
    virtual void regression() =0;
    /** compute the weighted regression function.
     * @param weights the weights of the samples
     **/
    virtual void regression(WContainer const& weights) =0;
    /** Compute the predicted outputs by the regression function and store the
     * result in the p_predicted_ container. */
    virtual void prediction() =0;
    /** Compute the number of parameter of the regression function.
     * @return the number of parameter of the regression function
     **/
    virtual int computeNbParameter() const =0;
    /** delete allocated memory. */
    void clear()
    {
      if (p_predicted_) delete p_predicted_;
      if (p_residuals_) delete p_residuals_;
      p_predicted_ = 0;
      p_residuals_ = 0;
    }

  protected:
    /** String with the last error message. */
    String msg_error_;
};

}

#endif /* STK_IREGRESSION_H */
