/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

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

/** @file STK_Gaussian_sj.h
 *  @brief In this file we implement the Gaussian_s class
 **/

#ifndef STK_GAUSSIAN_SJ_H
#define STK_GAUSSIAN_SJ_H

#include "STK_Gaussian_sjImpl.h"
#include "STK_DiagGaussianBase.h"

namespace STK
{

//forward declaration, to allow for recursive template
template<class Array>class Gaussian_sj;

namespace Clust
{
/** @ingroup hidden
 *  Traits class for the Gaussian_s traits policy. */
template<class _Array>
struct MixtureTraits< Gaussian_sj<_Array> >
{
  typedef _Array Array;
  typedef DiagGaussianComponent<_Array, Gaussian_sj_Parameters> Component;
  typedef Gaussian_sj_Parameters        Parameters;
};

} // namespace hidden

/** @ingroup Clustering
 *  The diagonal Gaussian mixture model Gaussian_sj have a density function of the
 *  form
 * \f[
 *  f(\mathbf{x}|\theta) = \sum_{k=1}^K p_k \prod_{j=1}^d
 *    \frac{1}{\sqrt{2\pi}\sigma_j} \exp\left\{-\frac{(x^j-\mu^j_k)^2}{2\sigma_j^2}\right\}.
 * \f]
 **/
template<class Array>
class Gaussian_sj : public DiagGaussianBase<Gaussian_sj<Array> >
{
  public:
    typedef DiagGaussianBase<Gaussian_sj<Array> > Base;
    using Base::p_data_;
    using Base::components_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    Gaussian_sj( int nbCluster) : Base(nbCluster), sigma_(1) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    Gaussian_sj( Gaussian_sj const& model)
               : Base(model), sigma_(model.sigma_)
    {}
    /** destructor */
    ~Gaussian_sj() {}
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
      { components_[k]->p_param()->p_sigma_ = &sigma_;}
    }
    /** Write the parameters*/
    void writeParameters(ostream& os) const
    {
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      {
        stk_cout << _T("---> Component ") << k << _T("\n";);
        stk_cout << _T("mean_ = ") << components_[k]->p_param()->mean_;
        stk_cout << _T("sigma_ = ") << sigma_;
      }
    }
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*this->nbVariable()+this->nbVariable();}

  protected:
    Array2DPoint<Real> sigma_;
};

} // namespace STK

#endif /* STK_GAUSSIAN_SJ_H */
