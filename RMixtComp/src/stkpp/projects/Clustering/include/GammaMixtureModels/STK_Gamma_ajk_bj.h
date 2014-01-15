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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project: stkpp::Clustering
 * created on: 5 sept. 2013
 * Author:  iovleff, serge.iovleff@stkpp.org
 **/

/** @file STK_Gamma_ajk_bj.h
 *  @brief In this file we define the Gamma_ajk_bj and Gamma_p_ajk_bj mixture classes.
 **/

#ifndef STK_GAMMA_AJK_BJ_H
#define STK_GAMMA_AJK_BJ_H

#include "STK_GammaBase.h"

#include "STK_GammaComponent.h"
#include "STK_Gamma_ajk_bjImpl.h"

namespace STK
{
template<class Array>class Gamma_ajk_bj;

namespace Clust
{
/** @ingroup hidden
 *  Traits class for the Gamma_ajk_bj traits policy. */
template<class _Array>
struct MixtureTraits< Gamma_ajk_bj<_Array> >
{
  typedef _Array Array;
  typedef GammaComponent<_Array, Gamma_ajk_bj_Parameters> Component;
  typedef Gamma_ajk_bj_Parameters        Parameters;
};

} // namespace hidden

/** @ingroup Clustering
 *  Gamma_ajk_bj is a mixture model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) = \sum_{k=1}^K p_k
 *     \prod_{j=1}^p\left(\frac{x_i^j}{b_{j}}\right)^{a_{jk}-1}
 *                   \frac{e^{-x_i^j/b_{j}}} {b_{j} \, \Gamma(a_{jk})},
 *      \quad x_i^j>0, \quad i=1,\ldots,n.
 * \f]
 **/
template<class Array>
class Gamma_ajk_bj : public GammaBase<Gamma_ajk_bj<Array> >
{
  public:
    typedef typename Clust::MixtureTraits< Gamma_ajk_bj<Array> >::Component Component;
    typedef typename Clust::MixtureTraits< Gamma_ajk_bj<Array> >::Parameters Parameters;
    typedef GammaBase<Gamma_ajk_bj<Array> > Base;
    using Base::p_data_;
    using Base::components_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    Gamma_ajk_bj( int nbCluster) : Base(nbCluster), p_scale_(0) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    Gamma_ajk_bj( Gamma_ajk_bj const& model) : Base(model), p_scale_(model.p_scale_->clone())
    {
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { components_[k]->p_param()->p_scale_ = p_scale_;}
    }
    /** destructor */
    ~Gamma_ajk_bj() { if (p_scale_) delete p_scale_;}
    /** Initialize the component of the model.
     *  This function have to be called prior to any used of the class.
     *  In this interface, the @c initializeModel() method call the base
     *  class IMixtureModel::initializeModel() and for all the
     *  components create the parameters.
     **/
    void initializeModel()
    {
      Base::initializeModel();
      // first initialization or cloning ?
      if (!p_scale_)
      { p_scale_ = new Array2DPoint<Real>(p_data_->cols(), 1.);}
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { components_[k]->p_param()->p_scale_ = p_scale_;}
    }
    /** use the default static method initializeStep() for a first initialization
     *  of the parameters using tik values.
     **/
    void initializeStep()
    { MixtureModelImpl< Array, Parameters >::initializeStep(components_,  this->p_tik());}

    /** Write the parameters*/
    void writeParameters(ostream& os) const
    {
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      {
        stk_cout << "---> Component " << k << _T("\n";);
        stk_cout << "shape_ = " << components_[k]->p_param()->shape_;
        stk_cout << "scale_ = " << *p_scale_;
      }
    }
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return this->nbCluster()*this->nbVariable()+ this->nbVariable();}

  protected:
    /** Array of the common scale */
    Array2DPoint<Real>* p_scale_;
};

}  // namespace STK

#endif /* STK_GAMMA_AJK_BJ_H */
