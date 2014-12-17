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

/** @file STK_DiagGaussianParameters.h
 *  @brief In this file we define the parameters class for the Gaussian mixture models
 **/

#ifndef STK_DIAGGAUSSIANPARAMETERS_H
#define STK_DIAGGAUSSIANPARAMETERS_H

#include <cmath>

#include "Arrays/include/STK_Const_Arrays.h"
#include "Arrays/include/STK_Array2DPoint.h"
#include "Arrays/include/STK_Display.h"

#include "StatModels/include/STK_IMultiParameters.h"
#include "STatistiK/include/STK_Law_Normal.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief Interface base class for the parameters of a diagonal Gaussian
 *  multivariate model.
  */
template<class Parameters>
class DiagGaussianParametersBase : public IMultiParameters<Parameters>
{
  protected:
    typedef IMultiParameters<Parameters> Base;
    /** default constructor.*/
    inline DiagGaussianParametersBase() : Base(), mean_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline DiagGaussianParametersBase( Range const& range): Base(range), mean_(range, 0.)
    {}
    /** copy constructor.*/
    inline DiagGaussianParametersBase( DiagGaussianParametersBase const& param)
                                     : Base(param), mean_(param.mean_)
    {}
    /** destructor */
    inline ~DiagGaussianParametersBase() {}

  public:
    /** @return the j-th mean value */
    inline Real mean(int j) const {return mean_[j];}
    /** @return the j-th sigma value */
    inline Real sigma(int j) const {return this->asDerived().sigmaImpl(j);}
    /** vector of the mean */
    Array2DPoint<Real> mean_;
    /** compute the log Likelihood of an observation.
     *  @param rowData the observation
     **/
    template<class RowVector>
    Real computeLnLikelihood( RowVector const& rowData) const
    {
      Real sum =0.;
      for (Integer j= rowData.begin(); j <= rowData.lastIdx(); ++j)
      { sum += Law::Normal::lpdf(rowData[j], mean(j), sigma(j));}
      return sum;
    }
};


/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gaussian_sjk model.
 **/
class Gaussian_sjk_Parameters: public DiagGaussianParametersBase<Gaussian_sjk_Parameters>
{
  public:
    typedef DiagGaussianParametersBase<Gaussian_sjk_Parameters> Base;
    /** default constructor */
    inline Gaussian_sjk_Parameters() : Base(), sigma_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gaussian_sjk_Parameters( Range const& range)
                                  : Base(range), sigma_(range, 1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gaussian_sjk_Parameters( Gaussian_sjk_Parameters const& param)
                                  : Base(param), sigma_(param.sigma_)
    {}
    /** destructor */
    inline ~Gaussian_sjk_Parameters() {}
    /** @return the j-th sigma value */
    inline Real sigmaImpl(int j) const {return sigma_[j];}
    /** resize the set of parameter
     *  @param range range of the parameters
     **/
    inline void resizeImpl(Range const& range)
    {
      mean_.resize(range); mean_ = 0.;
      sigma_.resize(range); sigma_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) { os << mean_ << sigma_ << _T("\n");}
    /** vector of the standard deviation */
    Array2DPoint<Real> sigma_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gaussian_sj model.
 **/
class Gaussian_sj_Parameters: public DiagGaussianParametersBase<Gaussian_sj_Parameters>
{
  public:
    typedef DiagGaussianParametersBase<Gaussian_sj_Parameters> Base;
    /** default constructor */
    inline Gaussian_sj_Parameters() : Base(), p_sigma_(0) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gaussian_sj_Parameters( Range const& range) : Base(range), p_sigma_(0) {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gaussian_sj_Parameters( Gaussian_sj_Parameters const& param)
                                 : Base(param), p_sigma_(param.p_sigma_)
    {}
    /** destructor */
    inline ~Gaussian_sj_Parameters() {}
    /** @return the j-th sigma value */
    inline Real sigmaImpl(int j) const { return p_sigma_->elt(j);}
    /** resize the set of parameter
     *  @param range range of the parameters
     **/
    inline void resizeImpl(Range const& range)
    { mean_.resize(range); mean_= 0.;}
    /** print the parameters mean_.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) { os << *p_sigma_ << mean_ << _T("\n");}
    /** vector of the standard deviation */
    Array2DPoint<Real>* p_sigma_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gaussian_sk model.
 **/
class Gaussian_sk_Parameters: public DiagGaussianParametersBase<Gaussian_sk_Parameters>
{
  public:
    typedef DiagGaussianParametersBase<Gaussian_sk_Parameters> Base;

    /** default constructor */
    inline Gaussian_sk_Parameters() : Base(), sigma_(1.) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gaussian_sk_Parameters( Range const& range) : Base(range), sigma_(1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gaussian_sk_Parameters( Gaussian_sk_Parameters const& param)
                                 : Base(param), sigma_(param.sigma_)
    {}
    /** destructor */
    inline ~Gaussian_sk_Parameters() {}
    /** @return the j-th sigma value */
    inline Real sigmaImpl(int j) const {return sigma_;}
    /** resize the set of parameter
     *  @param range range of the parameters
     **/
    inline void resizeImpl(Range const& range)
    { mean_.resize(range); mean_ = 0.; sigma_ = 1.;}
    /** print the parameters mean_.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os)
    { os << mean_ << _T("\n") << sigma_ * Const::Point<Real>(mean_.range()) << _T("\n");}
    /** vector of the standard deviation */
    Real sigma_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gaussian_s model.
 **/
class Gaussian_s_Parameters: public DiagGaussianParametersBase<Gaussian_s_Parameters>
{
  public:
    typedef DiagGaussianParametersBase<Gaussian_s_Parameters> Base;
    /** default constructor */
    inline Gaussian_s_Parameters() : Base(), p_sigma_(0) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gaussian_s_Parameters( Range const& range)
                                : Base(range), p_sigma_(0)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gaussian_s_Parameters( Gaussian_s_Parameters const& param)
                                : Base(param), p_sigma_(param.p_sigma_)
    {}
    /** destructor */
    inline~Gaussian_s_Parameters() {}
    /** @return the j-th sigma value */
    inline Real sigmaImpl(int j) const {return *p_sigma_;}
    /** resize the set of parameter
     *  @param range range of the parameters
     **/
    inline void resizeImpl(Range const& range)
    { mean_.resize(range); mean_ = 0.;}
    /** print the parameters mean_.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os)
    { os << mean_ << _T("\n") << *p_sigma_ * Const::Point<Real>(mean_.range()) << _T("\n");}
    /** pointer on the standard deviation */
    Real* p_sigma_;
};

} // namespace STK

#endif /* STK_DIAGGAUSSIANPARAMETERS_H */
