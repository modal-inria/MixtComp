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
 * Project:  stkpp::AAModels
 * Purpose:  Implementation of the ILinearReduct interface using the total variance.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/


#ifndef STK_PROJECTEDVARIANCE_H
#define STK_PROJECTEDVARIANCE_H

#include "STK_ILinearReduct.h"
#include "Arrays/include/STK_Array2DSquare.h"

namespace STK
{

/** @ingroup Reduct
 *  @brief A ProjectedVariance is an implementation of the abstract
 *  @c ILinearReduct interface.
 *
 *  ProjectedVariance (PCA) reduce the dimension of data by maximizing the
 *  projected varaince on an affine subspace of dimension d.
**/
class ProjectedVariance : public ILinearReduct
{
  public:
    /** default constructor */
    ProjectedVariance();
    /** Constructor.
     *  @param p_data a pointer on the constant data set to reduce.
     **/
    ProjectedVariance(ArrayXX const* p_data);
    /** Constructor.
     *  @param data a constant reference on the data set to reduce.
     **/
    ProjectedVariance(ArrayXX const& data);
    /** Copy constructor.
     * @param reductor the reductor to copy
     **/
    ProjectedVariance(ProjectedVariance const& reductor);
    /** Destructor */
    virtual ~ProjectedVariance();
    /** clone pattern
     *  @return a pointer on the clone of this
     **/
    inline virtual ProjectedVariance* clone() const
    { return new ProjectedVariance(*this);}

  protected:
    /** the covariance Array */
    ArraySquareX covariance_;

  private:
    /** Find the axis by maximizing the Index. */
    virtual void maximizeCriteria();

    /** Find the axis by maximizing the weighed Index.
     * @param weights the weights of the samples.
     **/
    virtual void maximizeCriteria(Vector const& weights);
    /** compute axis and index. */
    void computeAxis();
    /**  update the class if a new data set is set. */
    virtual void update();
};

} // namespace STK

#endif /* STK_PROJECTEDVARIANCE_H_ */
