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

/** @file STK_Gamma_ajk_bjk.h
 *  @brief In this file we define the Gamma_ajk_bjk and Gamma_p_ajk_bjk models.
 **/

#ifndef STK_GAMMA_AJK_BJK_H
#define STK_GAMMA_AJK_BJK_H

#include "STK_GammaBase.h"

#include "STK_GammaComponent.h"
#include "STK_Gamma_ajk_bjkImpl.h"

namespace STK
{
template<class Array>class Gamma_ajk_bjk;

namespace Clust
{
/** @ingroup hidden
 * Traits class for the Gamma_ajk_bjk traits policy
 **/
template<class _Array>
struct MixtureTraits< Gamma_ajk_bjk<_Array> >
{
  typedef _Array Array;
  typedef GammaComponent<_Array, Gamma_ajk_bjk_Parameters> Component;
  typedef Gamma_ajk_bjk_Parameters        Parameters;
};

} // namespace hidden

/** @ingroup Clustering
 *  Gamma_ajk_bjk is a mixture model of the following form
 * \f[
 *     f(\mathbf{x}_i|\theta) = \sum_{k=1}^K p_k
 *     \prod_{j=1}^p\left(\frac{x_i^j}{b_{jk}}\right)^{a_{jk}-1}
 *                   \frac{e^{-x_i^j/b_{jk}}}{b_{jk} \, \Gamma(a_{jk})},
 *      \quad x_i^j>0, \quad i=1,\ldots,n.
 * \f]
 **/
template<class Array>
class Gamma_ajk_bjk : public GammaBase< Gamma_ajk_bjk<Array> >
{
  public:
    typedef typename Clust::MixtureTraits< Gamma_ajk_bjk<Array> >::Component Component;
    typedef typename Clust::MixtureTraits< Gamma_ajk_bjk<Array> >::Parameters Parameters;
    typedef GammaBase< Gamma_ajk_bjk<Array> > Base;
    using Base::p_data;
    using Base::components_;

    /** default constructor
     * @param nbCluster number of cluster in the model
     **/
    Gamma_ajk_bjk( int nbCluster) : Base(nbCluster) {}
    /** copy constructor
     *  @param model The model to copy
     **/
    Gamma_ajk_bjk( Gamma_ajk_bjk const& model) : Base(model) {}
    /** destructor */
    ~Gamma_ajk_bjk() {}
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
        stk_cout << "scale_ = " << components_[k]->p_param()->scale_;
      }
    }
    /** @return the number of free parameters of the model */
    inline int computeNbFreeParameters() const
    { return 2*this->nbCluster()*this->nbVariable();}
};

}  // namespace STK

#endif /* STK_GAMMA_AJK_BJK_H */
