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
 * Purpose:  Create a two blocks Gaussian statistical model.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_Gaussian2BlocksModel.h
 *  @brief In this file we define the Gaussian2BlocksModel class.
 **/

#ifndef STK_GAUSSIAN2BLOCKSMODEL_H
#define STK_GAUSSIAN2BLOCKSMODEL_H

#include "STK_GaussianModel.h"

namespace STK
{

/** @ingroup StatModels
 *  @brief Compute the the maximum likelihood estimates of a Gaussian
 *  tow blocks statistical model.
 *
 *  A random vector \f$ X \in; \mathbb{R}^p \f$ is a Gaussian two blocks
 *  if
 *  \f[
 *  X\ \sim\ \mathcal{N}(\mu,\ \Sigma)
 *  \f]
 * and
 *  \f[
 *  \Sigma =
 *  \left(
 *  \begin{array}{c@{}c@{}}
 *  \begin{array}{|ccc|}
 *  \hline
 *  &            & \\
 *  & \Sigma_{x} & \\
 *  &            & \\
 *  \hline
 *  \end{array}  & \mathbf{0} \\
 *  \mathbf {0}  &
 *                \begin{array}{|cccc|}
 *                \hline
 *                \sigma^2 &                 &                 &  \\
 *                         & \ddots          & \text{\huge{0}} &  \\
 *                         &                 & \ddots          &  \\
 *                         & \text{\huge{0}} &                 & \sigma^2 \\
 *              \hline
 *              \end{array} \\
 * \end{array}
 * \right)
 * \begin{array}{cc}
 * \left.\begin{array}{c} \\ \\ \\ \end{array} \right\}  & d\vspace{1.5ex} \\
 * \left.\begin{array}{c} \\ \\ \\ \\ \end{array} \right\}  & p-d \\
 * \end{array}
 *  \f]
 *  where d is a given dimension.
 **/

class Gaussian2BlocksModel : public GaussianModel
{
  public:
    /** constructor. The dimension of the model will be initialized to
     * the maximal dimension of the data set (only one block).
     * @param p_data pointer on the data set
     */
    Gaussian2BlocksModel( ArrayXX const* p_data);

    /** destructor. */
    virtual ~Gaussian2BlocksModel();

    /** get the dimension of the model.
     *  @return the dimension of the model
     **/
    inline int dim() const { return dim_;}
    /** set the dimension of the model.
     *  @param dim the dimension of the model
     **/
    inline void setDim( int const& dim) { dim_ = dim;}

    /** implementation of the 2 blocks Gaussian statistical model
     * @return @c true if no error occur and @c false otherwise.
     */
    inline bool run( int const& dim)
    {
      setDim(dim);
      return run();
    }
    /** implementation of the weighted  2 blocks Gaussian statistical model.
     *  @param weights the weights of the samples
     *  @param dim the dimension of the first block
     *  @return @c true if no error occur and @c false otherwise.
     **/
    inline bool run(Vector const& weights, int const& dim)
    {
      setDim(dim);
      return run(weights);
    }
  protected:
    /** implementation of the Gaussian statistical model
     * @return @c true if no error occur and @c false otherwise.
     */
    inline virtual bool run() { return GaussianModel::run();}
    /** implementation of the weighted Gaussian statistical model
     * @param weights the weights of the samples
     * @return @c true if no error occur and @c false otherwise.
     */
    inline virtual bool run(Vector const& weights)
    { return GaussianModel::run(weights);}

    /** compute the empirical covariance matrix. */
    virtual void compCovariance();
    /** compute the empirical weighted covariance matrix.
     * @param weights the weights of the samples
     **/
    virtual void compWeightedCovariance(Vector const& weights);
    /** dimension of the 2 block model */
    int dim_;
    /** variance in the second block */
    Real variance2_;
};

} // namespace STK

#endif /* STK_GAUSSIAN2BLOCKSMODEL_H */
