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
 * created on: 13 août 2011
 * Purpose:  Create a gaussian statistical model.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_GaussianModel.h
 *  @brief In this file we define the GassianModel class.
 **/

#ifndef STK_GAUSSIANMODEL_H
#define STK_GAUSSIANMODEL_H

#include "STK_IGaussianModel.h"

namespace STK
{

/** @ingroup StatModels
 *  @brief Compute the maximum likelihood estimates of a complete Gaussian
 *  statistical model.
 **/
class GaussianModel : public IGaussianModel<ArrayXX>
{
  public:
    /** constructor.
     * @param p_data pointer on the data set
     */
    GaussianModel( ArrayXX const* p_data);
    /** constructor.
     * @param data reference on the data set
     */
    GaussianModel( ArrayXX const& data);
    /** destructor. */
    virtual ~GaussianModel();
    /** implementation of the Gaussian statistical model
     * @return @c true if no error occur and @c false otherwise.
     */
    virtual bool run();
    /** implementation of the weighted Gaussian statistical model
     * @param weights the weights of the samples
     * @return @c true if no error occur and @c false otherwise.
     */
    virtual bool run(Vector const& weights);
    /** get the empirical covariance
     * @return the empirical covariance
     */
    inline ArraySquareX const& covariance() const { return cov_;}
  protected:
    /** ArrayXX of the empirical covaiance */
    ArraySquareX cov_;
    /** compute the empirical covariance matrix. */
    virtual void compCovariance();
    /** compute the empirical weighted covariance matrix.
     * @param weights the weights of the samples
     **/
    virtual void compWeightedCovariance(ArrayXX::Col const& weights);
};

} // namespace STK

#endif /* STK_GAUSSIANMODEL_H */
