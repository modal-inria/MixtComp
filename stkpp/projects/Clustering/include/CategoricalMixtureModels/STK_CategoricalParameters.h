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
 * Purpose: define the categorical parameters.
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_CategoricalParameters.h
 *  @brief In this file we define the parameters class for the Categorical
 *  mixture models.
 **/

#ifndef STK_CATEGORICALPARAMETERS_H
#define STK_CATEGORICALPARAMETERS_H

#include <cmath>

#include "../../../Arrays/include/STK_Const_Arrays.h"
#include "../../../Arrays/include/STK_Display.h"
#include "../../../StatModels/include/STK_IMultiParameters.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief Interface base class for the parameters of a diagonal categorical
 *  multivariate model.
  */
template<class Parameters>
class CategoricalParametersBase : public IMultiParameters<Parameters>
{
  protected:
    typedef IMultiParameters<Parameters> Base;
    /** default constructor.*/
    inline CategoricalParametersBase() : Base() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline CategoricalParametersBase( Range const& range): Base(range) {}
    /** copy constructor.*/
    inline CategoricalParametersBase( CategoricalParametersBase const& param): Base(param) {}
    /** destructor */
    inline ~CategoricalParametersBase() {}

  public:
    /** @return the j-th probability value of the l-th modality */
    inline Real proba(int j, int l) const {return this->asDerived().probaImpl(j, l);}
    /** @return the j-th probability distribution */
    inline Array2DVector<Real> const& proba(int j) const {return this->asDerived().probaImpl(j);}

    /** compute the log Likelihood of an observation.
     *  @param rowData the observation
     **/
    template<class RowVector>
    Real computeLnLikelihood( RowVector const& rowData) const
    {
      Real sum =0.;
      for (Integer j= rowData.begin(); j < rowData.end(); ++j)
      {
        Real prob = proba(j, rowData[j]);
        if (prob <= 0.) return -Arithmetic<Real>::infinity();
        sum += std::log(prob);
      }
      return sum;
    }
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the categorical_pjk model.
 **/
class Categorical_pjkParameters: public CategoricalParametersBase<Categorical_pjkParameters>
{
  public:
    typedef CategoricalParametersBase<Categorical_pjkParameters> Base;
    /** default constructor */
    inline Categorical_pjkParameters() : Base(), proba_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Categorical_pjkParameters( Range const& range): Base(range), proba_(range) {}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Categorical_pjkParameters( Categorical_pjkParameters const& param)
                                    : Base(param), proba_(param.proba_)
    {}
    /** destructor */
    inline ~Categorical_pjkParameters() {}
    /** @return the j-th probability value of the l-th modality */
    inline Real probaImpl(int j, int l) const {return proba_[j][l];}
    /** @return the j-th probability distribution */
    inline Array2DVector<Real> const& probaImpl(int j) const { return proba_[j];}
    /** resize the set of parameter
     *  @param range range of the parameters
     **/
    inline void resizeImpl(Range const& range) { proba_.resize(range);}
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os)
    {
      for(int j=proba_.begin(); j< proba_.end(); j++)
      { os << proba_[j] << _T("\n");}
    }
    /** Array of the probabilities */
    Array1D< Array2DVector<Real> > proba_;
    /** utility function allowing to resize the probability vector with a
     *  given Range for the modalities.
     *  @param rangeMod the range of the modalities of the categorical distribution
     **/
    inline void initializeParameters(Range const& rangeMod)
    {
      for(int j=proba_.begin(); j< proba_.end(); j++)
      { proba_[j].resize(rangeMod); proba_[j] = 1./rangeMod.size();}
    }
};

/** @ingroup Clustering
 *  Structure encapsulating the parameters of the categorical_pk model.
 **/
class Categorical_pkParameters: public CategoricalParametersBase<Categorical_pkParameters>
{
  public:
    typedef CategoricalParametersBase<Categorical_pkParameters> Base;
    /** default constructor */
    inline Categorical_pkParameters() : Base(), proba_() {}
    /** constructor with specified range
     *  @param range the range of the variables
     **/
    inline Categorical_pkParameters( Range const& range): Base(range), proba_(){}
    /** copy constructor.
     * @param param the parameters to copy.
     **/
    inline Categorical_pkParameters( Categorical_pkParameters const& param)
                                   : Base(param), proba_(param.proba_)
    {}
    /** destructor */
    inline ~Categorical_pkParameters() {}
    /** @return the j-th probability value of the l-th modality (does not depend of j) */
    inline Real probaImpl(int j, int l) const { return proba_[l];}
    /** @return the j-th probability distribution (does not depend of j) */
    inline Array2DVector<Real> const& probaImpl(int j) const {return proba_;}
    /** resize the set of parameter
     *  @param range range of the parameters
     **/
    inline void resizeImpl(Range const& range) {}
    /** print the parameters.
     *  @param os the output stream for the parameters
     **/
    inline void printImpl(ostream &os) { os << proba_ << _T("\n");}
    /** utility function allowing to resize the probability vector with a
     *  given Range for the modalities.
     *  @param rangeMod the range of modalities of the categorical distribution
     **/
    inline void initializeParameters(Range const& rangeMod)
    { proba_.resize(rangeMod); proba_ = 1./rangeMod.size();}
    /** probabilities of each modalities */
    Array2DVector<Real> proba_;
};

} // namespace STK

#endif /* STK_CATEGORICALPARAMETERS_H */
