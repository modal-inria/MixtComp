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
 * Purpose: define the gamma parameters structures.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_GammaParameters.h
 *  @brief In this file we define the parameters class for the Gamma mixture models
 **/

#ifndef STK_GAMMAPARAMETERS_H
#define STK_GAMMAPARAMETERS_H

#include <cmath>

#include "Arrays/include/STK_Const_Arrays.h"
#include "Arrays/include/STK_Array2DPoint.h"
#include "Arrays/include/STK_Display.h"

#include "StatModels/include/STK_IMultiParameters.h"
#include "STatistiK/include/STK_Law_Gamma.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief Interface base class for the parameters of a multivariate model.
  */
template<class Parameters>
class GammaParametersBase : public IMultiParameters<Parameters>
{
  protected:
    typedef IMultiParameters<Parameters> Base;
    /** default constructor.*/
    inline GammaParametersBase(): Base(), tk_(0) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline GammaParametersBase( Range const& range)
                              : Base(range), tk_(0), mean_(range, 1.), meanLog_(range, 0.), variance_(range, 1.)
    {}
    /** copy constructor.*/
    inline GammaParametersBase( GammaParametersBase const& param)
                              : Base(param), tk_(param.tk_), mean_(param.mean_), meanLog_(param.meanLog_), variance_(param.variance_)
    {}
    /** Destructor */
    inline ~GammaParametersBase() {}
  public:
    /** @return the j-th shape value */
    inline Real shape(int j) const {return this->asDerived().shapeImpl(j);}
    /** @return the j-th scale value */
    inline Real scale(int j) const {return this->asDerived().scaleImpl(j);}
    /** Value of the sum of the k-th column of the tik */
    Real tk_;
    /** vector of the mean of the observations */
    Array2DPoint<Real> mean_;
    /** vector of the mean log of the observations */
    Array2DPoint<Real> meanLog_;
    /** vector of the variance of the observations */
    Array2DPoint<Real> variance_;
    /** compute the log Likelihood of an observation.
     *  @param rowData the observation
     **/
    template<class RowVector>
    Real computeLnLikelihood( RowVector const& rowData) const
    {
      Real sum =0.;
      for (Integer j= rowData.begin(); j < rowData.end(); ++j)
      { sum += Law::Gamma::lpdf(rowData[j], shape(j), scale(j));}
      return sum;
    }
};


/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ajk_bj model.
 **/
class Gamma_ajk_bjk_Parameters: public GammaParametersBase<Gamma_ajk_bjk_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_ajk_bjk_Parameters> Base;
    /** default constructor */
    inline Gamma_ajk_bjk_Parameters() : Base(), shape_(), scale_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_ajk_bjk_Parameters( Range const& range)
                                   : Base(range), shape_(range, 1.), scale_(range, 1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_ajk_bjk_Parameters( Gamma_ajk_bjk_Parameters const& param)
                                   : Base(param), shape_(param.shape_), scale_(param.scale_)
    {}
    /** destructor */
    inline ~Gamma_ajk_bjk_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return shape_[j];}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_[j];}
    /** resize the parameters.
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
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << shape_ << _T("\n") << scale_ << _T("\n");}
    /** vector of the shape */
    Array2DPoint<Real> shape_;
    /** vector of the scale */
    Array2DPoint<Real> scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ajk_bk model.
 **/
class Gamma_ajk_bk_Parameters: public GammaParametersBase<Gamma_ajk_bk_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_ajk_bk_Parameters> Base;
    /** default constructor */
    inline Gamma_ajk_bk_Parameters() : Base(), shape_(), scale_(1.) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_ajk_bk_Parameters( Range const& range)
                                  : Base(range), shape_(range, 1.), scale_(1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_ajk_bk_Parameters( Gamma_ajk_bk_Parameters const& param)
                                  : Base(param), shape_(param.shape_), scale_(param.scale_)
    {}
    /** destructor */
    inline ~Gamma_ajk_bk_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return shape_[j];}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_;}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      shape_.resize(size);    shape_ = 1.;
      mean_.resize(size);     mean_  = 1.;
      meanLog_.resize(size);  meanLog_ = 0.;
      variance_.resize(size); variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << shape_ << _T("\n")
         << scale_ * Const::Point<Real>(shape_.range()) << _T("\n");}
    /** vector of the shape */
    Array2DPoint<Real> shape_;
    /** vector of the scale */
    Real scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ajk_bj model.
 **/
class Gamma_ajk_bj_Parameters: public GammaParametersBase<Gamma_ajk_bj_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_ajk_bj_Parameters> Base;
    /** default constructor */
    inline Gamma_ajk_bj_Parameters() : Base(), shape_(), p_scale_(0) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_ajk_bj_Parameters( Range const& range)
                                  : Base(range), shape_(range, 1.), p_scale_(0)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_ajk_bj_Parameters( Gamma_ajk_bj_Parameters const& param)
                                  : Base(param), shape_(param.shape_), p_scale_(param.p_scale_)
    {}
    /** destructor */
    inline ~Gamma_ajk_bj_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return shape_[j];}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return p_scale_->elt(j);}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      shape_.resize(size); shape_ = 1.;
      mean_.resize(size); mean_ = 1.;
      meanLog_.resize(size); meanLog_ = 0.;
      variance_.resize(size); variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << *p_scale_ << shape_ << _T("\n");}
    /** vector of the shape */
    Array2DPoint<Real> shape_;
    /** vector of the scale */
    Array2DPoint<Real>* p_scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ajk_b model.
 **/
class Gamma_ajk_b_Parameters: public GammaParametersBase<Gamma_ajk_b_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_ajk_b_Parameters> Base;
    /** default constructor */
    inline Gamma_ajk_b_Parameters() : Base(), shape_(), p_scale_(0) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_ajk_b_Parameters( Range const& range)
                                 : Base(range), shape_(range, 1.), p_scale_(0)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_ajk_b_Parameters( Gamma_ajk_b_Parameters const& param)
                                 : Base(param), shape_(param.shape_), p_scale_(param.p_scale_)
    {}
    /** destructor */
    inline~Gamma_ajk_b_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return shape_[j];}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return *p_scale_;}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      shape_.resize(size); shape_ = 1.;
      mean_.resize(size); mean_ = 1.;
      meanLog_.resize(size); meanLog_ = 0.;
      variance_.resize(size); variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << shape_ << _T("\n")
         << *p_scale_ * Const::Point<Real>(shape_.range()) << _T("\n");
    }
    /** vector of the shape */
    Array2DPoint<Real> shape_;
    /** pointer on the scale */
    Real* p_scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ak_bjk model.
 **/
class Gamma_ak_bjk_Parameters: public GammaParametersBase<Gamma_ak_bjk_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_ak_bjk_Parameters> Base;
    /** default constructor */
    inline Gamma_ak_bjk_Parameters() : Base(), shape_(1), scale_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_ak_bjk_Parameters( Range const& range)
                                  : Base(range), shape_(1), scale_(range, 1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_ak_bjk_Parameters( Gamma_ak_bjk_Parameters const& param)
                                  : Base(param), shape_(param.shape_), scale_(param.scale_)
    {}
    /** destructor */
    inline ~Gamma_ak_bjk_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return shape_;}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_[j];}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      scale_.resize(size);   scale_ = 1.;
      mean_.resize(size);    mean_ = 1.;
      meanLog_.resize(size); meanLog_ = 0.;
      variance_.resize(size);variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << shape_  * Const::Point<Real>(range())
         << _T("\n") << scale_ << _T("\n");
    }
    /** vector of the shape */
    Real shape_;
    /** vector of the scale */
    Array2DPoint<Real> scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ak_bk model.
 **/
class Gamma_ak_bk_Parameters: public GammaParametersBase<Gamma_ak_bk_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_ak_bk_Parameters> Base;
    /** default constructor */
    inline Gamma_ak_bk_Parameters() : Base(), shape_(1), scale_(1) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_ak_bk_Parameters( Range const& range)
                                  : Base(range), shape_(1), scale_(1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_ak_bk_Parameters( Gamma_ak_bk_Parameters const& param)
                                  : Base(param), shape_(param.shape_), scale_(param.scale_)
    {}
    /** destructor */
    inline ~Gamma_ak_bk_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return shape_;}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_;}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      mean_.resize(size);    mean_ = 1.;
      meanLog_.resize(size); meanLog_ = 0.;
      variance_.resize(size);variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << shape_  * Const::Point<Real>(range()) << _T("\n")
         << scale_  * Const::Point<Real>(range()) << _T("\n");
    }
    /** vector of the shape */
    Real shape_;
    /** vector of the scale */
    Real scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ak_bj model.
 **/
class Gamma_ak_bj_Parameters: public GammaParametersBase<Gamma_ak_bj_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_ak_bj_Parameters> Base;
    /** default constructor */
    inline Gamma_ak_bj_Parameters() : Base(), shape_(1), p_scale_(0) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_ak_bj_Parameters( Range const& range)
                                  : Base(range), shape_(1), p_scale_(0)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_ak_bj_Parameters( Gamma_ak_bj_Parameters const& param)
                                  : Base(param), shape_(param.shape_), p_scale_(param.p_scale_)
    {}
    /** destructor */
    inline ~Gamma_ak_bj_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return shape_;}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return p_scale_->elt(j);}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      mean_.resize(size);    mean_ = 1.;
      meanLog_.resize(size); meanLog_ = 0.;
      variance_.resize(size);variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << shape_ * Const::Point<Real>(range()) << _T("\n")
         << *p_scale_ << _T("\n");}
    /** vector of the shape */
    Real shape_;
    /** vector of the scale */
    Array2DPoint<Real>* p_scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_ak_b model.
 **/
class Gamma_ak_b_Parameters: public GammaParametersBase<Gamma_ak_b_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_ak_b_Parameters> Base;
    /** default constructor */
    inline Gamma_ak_b_Parameters() : Base(), shape_(1), p_scale_(0) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_ak_b_Parameters( Range const& range)
                                  : Base(range), shape_(1), p_scale_(0)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_ak_b_Parameters( Gamma_ak_b_Parameters const& param)
                                : Base(param), shape_(param.shape_), p_scale_(param.p_scale_)
    {}
    /** destructor */
    inline ~Gamma_ak_b_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return shape_;}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return *p_scale_;}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      mean_.resize(size);    mean_ = 1.;
      meanLog_.resize(size); meanLog_ = 0.;
      variance_.resize(size);variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << shape_    * Const::Point<Real>(range()) << _T("\n")
         << *p_scale_ * Const::Point<Real>(range()) << _T("\n");
    }
    /** vector of the shape */
    Real shape_;
    /** vector of the scale */
    Real* p_scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_aj_bjk model.
 **/
class Gamma_aj_bjk_Parameters: public GammaParametersBase<Gamma_aj_bjk_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_aj_bjk_Parameters> Base;
    /** default constructor */
    inline Gamma_aj_bjk_Parameters() : Base(), p_shape_(0), scale_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_aj_bjk_Parameters( Range const& range)
                                  : Base(range), p_shape_(0), scale_(range, 1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_aj_bjk_Parameters( Gamma_aj_bjk_Parameters const& param)
                                  : Base(param), p_shape_(param.p_shape_), scale_(param.scale_)
    {}
    /** destructor */
    inline~Gamma_aj_bjk_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return p_shape_->elt(j);}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_[j];}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      scale_.resize(size);    scale_ = 1.;
      mean_.resize(size);     mean_ = 1.;
      meanLog_.resize(size);  meanLog_ = 0.;
      variance_.resize(size); variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << *p_shape_  << _T("\n") << scale_ << _T("\n");}
    /** vector of the shape */
    Array2DPoint<Real>* p_shape_;
    /** vector of the scale */
    Array2DPoint<Real> scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_aj_bk model.
 **/
class Gamma_aj_bk_Parameters: public GammaParametersBase<Gamma_aj_bk_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_aj_bk_Parameters> Base;
    /** default constructor */
    inline Gamma_aj_bk_Parameters() : Base(), p_shape_(0), scale_(1.) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_aj_bk_Parameters( Range const& range)
                                 : Base(range), p_shape_(0), scale_(1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_aj_bk_Parameters( Gamma_aj_bk_Parameters const& param)
                                 : Base(param), p_shape_(param.p_shape_), scale_(param.scale_)
    {}
    /** destructor */
    inline ~Gamma_aj_bk_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return p_shape_->elt(j);}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_;}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      mean_.resize(size); mean_ = 1.;
      meanLog_.resize(size); meanLog_ = 0.;
      variance_.resize(size); variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << *p_shape_ << _T("\n")
         << scale_ * Const::Point<Real>(range()) << _T("\n");
    }
    /** vector of the shape */
    Array2DPoint<Real>* p_shape_;
    /** vector of the scale */
    Real scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_a_bjk model.
 **/
class Gamma_a_bjk_Parameters: public GammaParametersBase<Gamma_a_bjk_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_a_bjk_Parameters> Base;
    /** default constructor */
    inline Gamma_a_bjk_Parameters() : Base(), p_shape_(0), scale_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_a_bjk_Parameters( Range const& range)
                                  : Base(range), p_shape_(0), scale_(range, 1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_a_bjk_Parameters( Gamma_a_bjk_Parameters const& param)
                                  : Base(param), p_shape_(param.p_shape_), scale_(param.scale_)
    {}
    /** destructor */
    inline~Gamma_a_bjk_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return *p_shape_;}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_[j];}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      scale_.resize(size);    scale_ = 1.;
      mean_.resize(size);     mean_  = 1.;
      meanLog_.resize(size);  meanLog_ = 0.;
      variance_.resize(size); variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << *p_shape_ * Const::Point<Real>(range()) << _T("\n")
         << scale_ << _T("\n");
    }
    /** vector of the shape */
    Real* p_shape_;
    /** vector of the scale */
    Array2DPoint<Real> scale_;
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the gamma_a_bk model.
 **/
class Gamma_a_bk_Parameters: public GammaParametersBase<Gamma_a_bk_Parameters>
{
  public:
    typedef GammaParametersBase<Gamma_a_bk_Parameters> Base;
    /** default constructor */
    inline Gamma_a_bk_Parameters() : Base(), p_shape_(0), scale_(1.) {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Gamma_a_bk_Parameters( Range const& range)
                                 : Base(range), p_shape_(0), scale_(1.)
    {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Gamma_a_bk_Parameters( Gamma_a_bk_Parameters const& param)
                                 : Base(param), p_shape_(param.p_shape_), scale_(param.scale_)
    {}
    /** destructor */
    inline ~Gamma_a_bk_Parameters() {}
    /** @return the j-th shape value */
    inline Real shapeImpl(int j) const {return *p_shape_;}
    /** @return the j-th scale value */
    inline Real scaleImpl(int j) const {return scale_;}
    /** resize the parameters.
     *  @param size range of the parameters
     **/
    inline void resizeImpl(Range const& size)
    {
      mean_.resize(size);    mean_ = 1.;
      meanLog_.resize(size); meanLog_ = 0.;
      variance_.resize(size); variance_ = 1.;
    }
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) const
    { os << *p_shape_ * Const::Point<Real>(range()) << _T("\n")
         << scale_ * Const::Point<Real>(range()) << _T("\n");
    }
    /** vector of the shape */
    Real* p_shape_;
    /** vector of the scale */
    Real scale_;
};

} // namespace STK

#endif /* STK_GAMMAPARAMETERS_H */
