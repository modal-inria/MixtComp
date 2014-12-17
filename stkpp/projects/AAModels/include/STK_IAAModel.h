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
 * Project:  stkpp::AAModels
 * Purpose: Interface bas class for all AA Models.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_IAAModel.h
 *  @brief  In this file we define the Interface Base class for all AA Models
 **/

#ifndef STK_IAAMODEL_H
#define STK_IAAMODEL_H

#include "Arrays/include/STK_Array2D.h"
#include "Arrays/include/STK_Array2DPoint.h"
#include "Arrays/include/STK_Array2DVector.h"

namespace STK
{

// forward declarations
class IReduct;
template<class X, class Y, class W> class IRegression;

/** @ingroup @ingroup AAModels
 * @brief Interface base class for all Auto-Associative models.
 *
 * A function \f$ g \f$ is an auto-associative function
 * of dimension d if it is a map from \f$ \mathbb{R}^p \f$ to
 * \f$ \mathbb{R}^p \f$ that can be written \f$ g=R\circ P \f$ where
 * P (the ``Projecttion'' or the ``Reduction'') is a map from \f$\mathbb{R}^p\f$
 * to \f$ \mathbb{R}^d \f$ and R (the ``Regression'') is a map from
 * \f$\mathbb{R}^d\f$ to \f$\mathbb{R}^p\f$ .
 *
 * The IAAModel class is an abstract base class for all these AAM and a
 * factory method that propose some tools that can be used in derived classes.
 * End user can have to set set a reductor and a regressor in order to
 * implement this interface.
 */
class IAAModel
{
  private:
    /** regression type for the AAModels */
    typedef IRegression<ArrayXX, ArrayXX, Vector > Regressor;

  protected:
    /** Constructor.
     *  @param p_workData A pointer on the the working data set
     **/
    IAAModel( ArrayXX* p_workData);
    /** Constructor.
     *  @param workData the working data set
     **/
    IAAModel( ArrayXX& workData);
    /** destructor. It is protected in order to prevent the use of this class.
     *  use @c GaussianAAModel decorator instead.
     **/
    ~IAAModel();

  public:
    /** get the working data set
     *  @return the working data set
     **/
    inline ArrayXX const& workData() const { return *p_workData_;}
    /** Get the reductor used for the computation of the reduced data
     *  @return a reference on the Index object
     **/
    inline IReduct* const& p_reductor() const { return p_reductor_;}
    /** Get the regressor used for the computation of the regressed data
     *  @return a reference on the Index object
     **/
    inline Regressor* const& p_regressor() const { return p_regressor_;}
    /** get the reduced data set
     * @return the reduced data set
     **/
    inline ArrayXX* const& p_reduced() const { return p_reduced_;}
    /** get the matrix of the predicted values
     * @return A ptr on the predicted values
     **/
    inline ArrayXX* const& p_predicted() const { return p_predicted_;}
    /** get the matrix of the residuals
     * @return A ptr on the residual covariance
     **/
    inline ArrayXX* const& p_residuals() const { return p_residuals_;}
    /** get the dimension of the model
     * @return the dimension of the model
     **/
    inline int dim() const { return dim_;}

    /** is the data set centered ?
     * @return @c true if the data set is centered, @c false otherwise
     */
    inline bool isCentered() const { return isCentered_;}
    /** is the data set standardized ?
     * @return @c true if the data set is standardized, @c false otherwise
     */
    inline bool isStandardized() const { return isStandardized_;}

    /** get the mean of the data set
     * @return the mean of the data set
     */
    inline PointX const& mean() const { return mean_;}
    /** get the standard deviation of the data set
     * @return the standard deviation of the data set
     */
    inline PointX const& std() const { return std_;}
    /** set the dimension of the model.
     * @param dim the dimension of the model to set
     */
    void setDimension( int const& dim);
    /** set the working set with the Data to treat.
     * @param workData the working data set to treat
     */
    void setWorkData( ArrayXX& workData);
    /** set the reduction dimension method.
     * @param p_reductor a pointer on the reduction dimension method to use
     */
    void setReductor( IReduct* p_reductor);
    /** set the regression method.
     * @param p_regressor a pointer on the regresssion method to use
     */
    void setRegressor( Regressor* p_regressor);

    /** delete the reductor set to this model by the method @c setReductor. */
    void freeReductor();
    /** delete the regressor set to this model by the method @c setRegressor.*/
    void freeRegressor();

    /** center the data set in workData_. */
    void center();
    /** weighted centering of the data set.
     * @param weights the weights of the samples
     **/
    void center( Vector const& weights);
    /** standardize the data set. */
    void standardize();
    /** weighted standardization the data set.
     * @param weights the weights of the samples
     **/
    void standardize( Vector const& weights);

    /** compute the dimension reduction of the data set and store the result in
     *  the @c p_reduced_ container. The reductor p_reductor have to be set.
     **/
    void reduction();
    /** compute the weighted dimension reduction of the data set and store the
     *  result in the @c p_reduced_ container.
     **/
    void reduction( Vector const& weights);
    /** compute the regression of the original data set and set the results in
     *  @c p_predicted and @c p_residuals. **/
    void regressionStep();
    /** compute the weighted regression  of the original data set using the
     *  reduced data set as predictor and set the results in @c p_predicted and
     *  @c p_residuals.
     **/
    void regression( Vector const& weights);

    /** decenter the predicted data set. This will invalidate the results
     *  of p_regressor_.  */
    void decenterResults();
    /** destandardize the predicted data set and the residuals.  This will
     *  invalidate the results of p_regressor_. */
    void destandardizeResults();

  protected:
    /** pointer on the regression method to use. The regression method will
     *  be instanced in the derived class.
     **/
    Regressor* p_regressor_;
    /** pointer on the dimension reduction method. The dimension reduction
     *  method will be chosen in the derived class.
     **/
    IReduct* p_reductor_;

    /** Array of the local data set. */
    ArrayXX* p_workData_;
    /** Array of the reduced data set : the data set is shared with
     * @c p_reductor and set when the @c regression method is call.
     **/
    ArrayXX* p_reduced_;
    /** Array of the predicted data set: the data set is shared with
     * @c p_regressor and set when the @c regression method is call. **/
    ArrayXX* p_predicted_;
    /** Array of the residuals: the data set is shared with @c p_regressor
     * and set when the @c regression method is call.
     **/
    ArrayXX* p_residuals_;

  private:
    /** The dimension of the AA Model. */
    int dim_;
    /** vector of the means of the input data set. */
    PointX mean_;
    /** vector of the standard deviation of the input data set. */
    PointX std_;
    /** a boolean @c true if the working data set is centered, @c false
     *  otherwise. */
    bool isCentered_;
    /** a boolean @c true if the working data set is standardized, @c false
     *  otherwise */
    bool isStandardized_;
};

} // namespace STK

#endif /* STK_IAAMODEL_H */
