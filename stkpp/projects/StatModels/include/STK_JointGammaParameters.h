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

#ifndef STK_JOINTGAMMAPARAMETERS_H
#define STK_JOINTGAMMAPARAMETERS_H

#include <cmath>

#include "STK_IMultiParameters.h"
#include "Arrays/include/STK_Array2DPoint.h"

namespace STK
{

/** @ingroup StatModels
 *  Structure encapsulating the parameters of a Joint Gamma model.
 *  @sa GammaComponent, JointGammaModel
 */
struct JointGammaParameters: public IMultiParameters<JointGammaParameters>
{
  /** default constructor */
  JointGammaParameters() : shape_(), scale_(), mean_(), meanLog_(), variance_() {}
  /** default constructor */
  JointGammaParameters( Range const& range)
                      : shape_(range, 1.), scale_(range, 1.)
                      , mean_(range, 1.), meanLog_(range, 0.), variance_(range, 1.)
  {}
  /** copy constructor. @param param the parameters to copy. */
  JointGammaParameters( JointGammaParameters const& param)
                      : shape_(param.shape_), scale_(param.scale_)
                      , mean_(param.mean_), meanLog_(param.meanLog_), variance_(param.variance_)
  {}
  /** destructor */
  ~JointGammaParameters() {}
  /** @return the j-th shape value */
  inline Real shape(int j) const {return shape_[j];}
  /** @return the j-th scale value */
  inline Real scale(int j) const {return scale_[j];}
  /** resize the set of parameter
   *  @param size range of the parameters
   **/
  inline void resizeImpl(Range const& size)
  {
    shape_.resize(size); shape_ = 1.;
    scale_.resize(size); scale_ = 1.;
    mean_.resize(size); mean_ = 1.;
    meanLog_.resize(size); meanLog_ = 0.;
    variance_.resize(size); variance_ = 1.;
  }
  /** print the parameters shape_ and scale_.
   *  @param os the output stream for the parameters
   **/
  inline void printImpl(ostream &os)
   { os << shape_ << scale_ << _T("\n");}
  /** vector of the shape */
  Array2DPoint<Real> shape_;
  /** vector of the scale */
  Array2DPoint<Real> scale_;
  /** vector of the mean of the observations */
  Array2DPoint<Real> mean_;
  /** vector of the mean log of the observations */
  Array2DPoint<Real> meanLog_;
  /** vector of the variance of the observations */
  Array2DPoint<Real> variance_;
};


} // namespace STK

#endif /* STK_JOINTGAMMAPARAMETERS_H */
