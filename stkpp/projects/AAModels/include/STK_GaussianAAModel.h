/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004  Serge Iovleff

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
 * Purpose:  Interface base class for AA models.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_GaussianAAModel.h
 *  @brief In this file we declare the class GaussianAAModel for the
 *  auto-associative models.
 **/

#ifndef STK_GAUSSIANAAMODEL_H
#define STK_GAUSSIANAAMODEL_H

#include "STK_IAAModel.h"
#include "StatModels/include/STK_IStatModel.h"
#include "../../../include/Arrays.h"

namespace STK
{
/** @ingroup AAModels
 *  @brief Gaussian Auto-Associative models.
 *  A Gaussian Auto-Associative model is a p-dimensional vector \f$\mathbf{y}\f$
 *  with projection function \f$\mathbf{P}\f$ and regression function
 *  \f$\mathbf{R}\f$, if it can be written
 *  \f[
 *    \mathbf{y} = \mathbf{Q}
 *    \left(
 *    \begin{pmatrix}
 *      x_1 \\ \vdots \\
 *      x_d \\
 *      \tilde{r}_{d+1}(\mathbf{x}) \\ \vdots \\
 *      \tilde{r}_p(\mathbf{x})
 *    \end{pmatrix}
 *     + \tilde{\varepsilon}
 *    \right)
 *    = R(\mathbf{x})+ \varepsilon,
 *  \f]
 *  where the \f$\tilde{r}_j(\mathbf{x})\f$, \f$ d+1 \leq j \leq p\f$, are
 *  arbitrary real functions from \f$\mathbb{R}^d\f$ to \f$\mathbb{R}\f$.
 *
 *  The vector \f$\mathbf{x}\f$ is a \f$d\f$-dimensional Gaussian random vector:
 *  \f[
 *    \mathbf{x} \sim \mathcal{N}(\mu_x, \Sigma_x)
 *  \f]
 *  with covariance matrix \f$\Sigma_x = \mathrm{Diag}(\sigma_1^2, \ldots, \sigma_d^2)\f$.
 *
 *  The Gaussian noise \f$\tilde{\varepsilon}\f$ is centered with the following
 *  covariance matrix \f$\Sigma_\varepsilon =
 *  \mathrm{diag}(0,\ldots,0,\sigma^2,\ldots,\sigma^2)\f$.
 *
 *  The GaussianModel class is a factory class which compute the covariance
 *  matrix of x, the residual covariance and the number of free parameters
 *  of the model. It can be sub-classed or used by any class.
 **/
class GaussianAAModel : public IAAModel
                      , public IStatModel<ArrayXX>
{
  public:
    /** Constructor.
     *  @param p_workData a pointer on the data set to process
     **/
    GaussianAAModel( ArrayXX* p_workData);
    /** Constructor.
     *  @param workData a reference on the data set to process
     **/
    GaussianAAModel( ArrayXX& workData);

    /** virtual destuctor. */
    virtual ~GaussianAAModel();

    /** get the ln-likelihood of the projected data set
     * @return the ln-likelihood of the projected data set
     **/
    inline Real const& projectedLnLikelihood() const
    { return projectedLnLikelihood_;}

    /** get the ln-likelihood of the residuals
     * @return the ln-likelihood of the residuals
     **/
    inline Real const& residualLnLikelihood() const
    { return residualLnLikelihood_;}

    /** get the covariance of the projected the data set
     * @return the projected the data set
     */
    inline ArraySquareX const& projectedCovariance() const { return projectedCovariance_;}
    /** get the covariance of the residuals
     * @return the residuals
     */
    inline ArraySquareX const& residualCovariance() const { return residualCovariance_;}
    /** get the variance of the residuals
     * @return the variance of the residuals
     */
    inline Real const& residualVariance() const { return residualVariance_;}

    /** Set a new working data set.
     *  @param workData the working data set to use
     **/
    virtual void setWorkData(ArrayXX& workData);

    /** compute the covariance matrix of the projected data set.
     *  This method is set public as the projected covariance can be computed
     *  only the first time the data set is projected.
     **/
    void computeProjectedCovariance();

    /** compute the ln-likelihood of the model */
    void computeModelParameters();

  protected:
    /** @brief compute the number of free parameter of the model.
     *  It is given by the number of parameter of the regression function,
     *  the number of variance and covariance of the projected data set
     *  (d * (d+1))/2 and the variance of the residuals.
     * **/
    void computeNbFreeParameters();
    /** compute the covariance matrix of the residuals. */
    void computeResidualCovariance();

    /** @brief compute the ln-likelihood of the projected data set
     *  The projected data set is assumed Gaussian with an arbitrary
     *  covariance ArrayXX.
     * */
    void computeProjectedLnLikelihood();
    /** @brief compute the ln-likelihood of the projected data set.
     * The residuals are assumed orthogonal to the the projected
     * data set with a single residual variance.
     **/
    void computeResidualLnLikelihood();

  private:
    /** The covariance matrix of the projected data set */
    ArraySquareX projectedCovariance_;
    /** The covariance matrix of the residuals */
    ArraySquareX residualCovariance_;
    /** The total variance of the residuals */
    Real residualVariance_;
    /** likelihood of the projected data set. */
    Real projectedLnLikelihood_;
    /** likelihood of the residuals. */
    Real residualLnLikelihood_;
};

} // namespace STK

#endif //STK_GAUSSIANAAMODEL_H
