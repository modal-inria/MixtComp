/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

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
 * Project:  stkpp::Clustering
 * created on: 22 juil. 2013
 * Purpose: define the gamma parameters.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_ExponentialParameters.h
 *  @brief In this file we define the parameters class for the Exponential mixture models
 **/

#ifndef STK_EXPONENTIALPARAMETERS_H
#define STK_EXPONENTIALPARAMETERS_H

#include <cmath>

#include "Arrays/include/STK_Const_Arrays.h"
#include "Arrays/include/STK_Array2DPoint.h"
#include "Arrays/include/STK_Display.h"

#include "StatModels/include/STK_IMultiParameters.h"
#include "STatistiK/include/STK_Law_Exponential.h"

namespace STK
{
/** @ingroup Clustering
 *  @brief Interface base class for the parameters of a multivariate model.
  */
template<class Parameters>
class ExponentialParametersBase : public IMultiParameters<Parameters>
{
  protected:
    typedef IMultiParameters<Parameters> Base;
    /** default constructor.*/
    inline ExponentialParametersBase(): Base(), tk_(0) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline ExponentialParametersBase( Range const& range): Base(range), tk_(0) {}
    /** copy constructor.*/
    inline ExponentialParametersBase( ExponentialParametersBase const& param)
                                    : Base(param), tk_(param.tk_), mean_(param.mean_)
    {}
    /** Destructor */
    inline ~ExponentialParametersBase() {}
  public:
    /** @return the j-th scale value */
    inline Real scale(int j) const {return this->asDerived().scaleImpl(j);}
    /** Value of the sum of the k-th column of the tik */
    Real tk_;
    /** compute the log Likelihood of an observation.
     *  @param rowData the observation
     **/
    template<class RowVector>
    Real computeLnLikelihood( RowVector const& rowData) const
    {
      Real sum =0.;
      for (Integer j= rowData.begin(); j < rowData.end(); ++j)
      { sum += Law::Exponential::lpdf(rowData[j], scale(j));}
      return sum;
    }
};


/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ajk_bj model.
 **/
class Exponential_bjk_Parameters: public ExponentialParametersBase<Exponential_bjk_Parameters>
{
  public:
    typedef ExponentialParametersBase<Exponential_bjk_Parameters> Base;
    /** default constructor */
    inline Exponential_bjk_Parameters() : Base(), scale_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Exponential_bjk_Parameters( Range const& range)
                                     : Base(range), scale_(range, 1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Exponential_bjk_Parameters( Exponential_bjk_Parameters const& param)
                                     : Base(param), scale_(param.scale_)
    {}
    /** destructor */
    inline ~Exponential_bjk_Parameters() {}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_[j];}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size) { scale_.resize(size); scale_ = 1.;}
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const { os << scale_ << _T("\n");}
    /** vector of the scale */
    Array2DPoint<Real> scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ajk_bk model.
 **/
class Exponential_bk_Parameters: public ExponentialParametersBase<Exponential_bk_Parameters>
{
  public:
    typedef ExponentialParametersBase<Exponential_bk_Parameters> Base;
    /** default constructor */
    inline Exponential_bk_Parameters() : Base(), scale_(1.) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Exponential_bk_Parameters( Range const& range)
                                    : Base(range), scale_(1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Exponential_bk_Parameters( Exponential_bk_Parameters const& param)
                                    : Base(param), scale_(param.scale_)
    {}
    /** destructor */
    inline ~Exponential_bk_Parameters() {}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_;}
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << scale_ * Const::Vector<Real>(this->range()) << _T("\n");}
    /** scale */
    Real scale_;
};


} // namespace STK

#endif /* STK_EXPONENTIALPARAMETERS_H */
