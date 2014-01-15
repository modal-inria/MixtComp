/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: Oct 24, 2013
 * Author:   Serge Iovleff
 **/

/** @file STK_Gaussian_sk.h
 *  @brief In this file we define the Gaussian_sk model
 **/

#ifndef STK_GAUSSIAN_SK_H
#define STK_GAUSSIAN_SK_H

#include "STK_Gaussian_skImpl.h"
#include "STK_DiagGaussianBase.h"

namespace STK
{

//forward declaration, to allow for recursive template
template<class Array>class Gaussian_sk;

namespace Clust
{
/** @ingroup Clustering
 *  Traits class for the Gaussian_sk traits policy. */
template<class _Array>
struct MixtureTraits< Gaussian_sk<_Array> >
{
  typedef _Array Array;
  typedef DiagGaussianComponent<_Array, Gaussian_sk_Parameters> Component;
  typedef Gaussian_sk_Parameters        Parameters;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Gaussian mixture model @c Gaussian_sk is
 *  the most general diagonal Gaussian model and have a density function of the
 *  form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma^j_{k}} \exp\left\{-\frac{(x^j-\mu^j_k)^2}{2(\sigma^j_{k})^2}\right\}.
 * \f]
 **/
template<class Array>
class Gaussian_sk : public DiagGaussianBase<Gaussian_sk<Array> >
{
  public:
    typedef DiagGaussianBase<Gaussian_sk<Array> > Base;
    using Base::p_data_;
    using Base::components_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    Gaussian_sk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    Gaussian_sk( Gaussian_sk const& model) : Base(model) {}
    /** destructor */
    ~Gaussian_sk() {}
    /** Initialize the component of the model.
     *  This function have to be called prior to any used of the class.
     *  In this interface, the @c initializeModel() method call the base
     *  class IMixtureModel::initializeModel() and for all the
     *  components initialize the shared parameter sigma_.
     **/
    void initializeModel()
    {
      Base::initializeModel();
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { components_[k]->p_param()->p_sigma_ = &sigma_[k];}
    }
    /** Write the parameters*/
    void writeParameters(ostream& os) const
    {
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      {
        stk_cout << _T("---> Component ") << k << _T("\n");
        stk_cout << _T("mean_ = ") << components_[k]->p_param()->mean_;
        stk_cout << _T("sigma_ = ")<< sigma_[k]*Const::Point<Real>(this->nbVariable());
      }
    }
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*this->nbVariable() + this->nbCluster();}

  protected:
    Array2DPoint<Real> sigma_;
};

} // namespace STK

#endif /* STK_GAUSSIAN_SJK_H */
