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
 * created on: 23 oct. 2014
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_KernelMixtureBridge.h
 *  @brief In this file we define the bridge class between the mixtures and the composer.
 **/

#ifndef STK_KERNELMIXTUREBRIDGE_H
#define STK_KERNELMIXTUREBRIDGE_H

namespace STK
{
// forward declaration
template<int Id, class Data> class KernelMixtureBridge;

namespace hidden
{
/** @ingroup hidden
 *  Partial  specialization of the MixtureBridgeTraits for the Gamma_ajk_bjk_ model
 **/
template<class Data>
struct MixtureBridgeTraits< KernelMixtureBridge< Clust::Gamma_ajk_bjk_, Data> >
{
  /** Type of the mixture model */
  typedef Gamma_ajk_bjk<Data> Mixture;
  enum
  {
    idMixtureClass_ = Clust::Gamma_
  };
};

} // namespace hidden

} // namespace STK

#include "STK_IMixtureBridge.h"

namespace STK
{
/** @ingroup Clustering
 *  @brief Templated implementation of the IMixture interface allowing
 *  to bridge a STK++ lernel mixture with the composer.
 *
 *  This class inherit from the interface IMixtureBridge.
 *
 * @tparam Id is any identifier of a concrete model deriving from the
 * interface STK::IMixtureModel class.
 */
template<int Id, class Data>
class KernelMixtureBridge: public IMixtureBridge< KernelMixtureBridge<Id,Data> >
{
  public:
  public:
    // type of Mixture
    typedef IMixtureBridge< MixtureBridge<Id,Data> > Base;
    // type of Mixture
    typedef typename hidden::MixtureBridgeTraits< KernelMixtureBridge<Id,Data> >::Mixture Mixture;
    // type of data
    typedef typename Data::Type Type;
    // class of mixture
    enum
    {
      idMixtureClass_ = hidden::MixtureBridgeTraits< KernelMixtureBridge<Id,Data>  >::idMixtureClass_
    };
    // parameters type to get
    typedef typename Clust::MixtureTraits<Mixture>::Param Param;

    typedef std::vector<std::pair<int,int> >::const_iterator ConstIterator;
    using Base::mixture_;

    /** default constructor. Remove the missing values from the data set and
     *  initialize the mixture by setting the data set.
     *  @param p_data pointer on the MixtureData that will be used by the bridge.
     *  @param idData id name of the mixture model
     *  @param nbCluster number of cluster
     **/
    KernelMixtureBridge( MixtureData<Data>* p_data, std::string const& idData, int nbCluster)
                       : Base( idData, nbCluster)
                       , p_data_(p_data)
    { initializeMixture();}
    /** copy constructor */
    KernelMixtureBridge( KernelMixtureBridge const& bridge)
                       : Base(bridge)
                       , p_data_(bridge.p_data_)
                       , dik_(bridge.dik_)
    {  mixture_.setData(dik_);}
    /** destructor */
    virtual ~KernelMixtureBridge() {}
    /** This is a standard clone function in usual sense. It must be defined to
     *  provide new object of your class with values of various parameters
     *  equal to the values of calling object. In other words, this is
     *  equivalent to polymorphic copy constructor.
     *  @return New instance of class as that of calling object.
     */
    virtual KernelMixtureBridge* clone() const { return new KernelMixtureBridge(*this);}
    /** This is a standard create function in usual sense. It must be defined to
     *  provide new object of your class with correct dimensions and state.
     *  In other words, this is equivalent to virtual constructor.
     *  @return New instance of class as that of calling object.
     */
    virtual KernelMixtureBridge* create() const
    {
      KernelMixtureBridge* p_bridge = new KernelMixtureBridge( mixture_, idName(), nbCluster());
      p_bridge->p_data_ = p_data_;
      // Bug Fix: set the correct data set
      p_bridge->mixture_.setData(p_bridge->p_data_->m_dataij());
      return p_bridge;
    }
    /** This function is equivalent to Mstep and must be defined to update
     *  parameters. In a Kernel mixture model, the Mstep is defined by
     *  - an update of the distance from the center of the class
     *  - an usual update of the parameters
     */
    virtual void paramUpdateStep()
    {

      if (!mixture_.mStep()) throw Clust::mStepFail_;
    }
    /** This function can be used in order to the values of the parameters
     *  in an Array2D.
     *  @param param the array with the parameters of the mixture.
     */
    void getParameters(Param& param) const { mixture_.getParameters(param);}

  private:
    /** This function will be used in order to initialize the mixture model
     *  using informations stored by the MixtureData. For example the missing
     *  values in the case of a MixtureData instance.
     **/
    void initializeMixture(){ mixture_.setData(dik_);}
    /** protected constructor to use in order to create a bridge.
     *  @param mixture the mixture to copy
     *  @param idData id name of the mixture
     *  @param nbCluster number of cluster
     **/
    KernelMixtureBridge( Mixture const& mixture, std::string const& idData, int nbCluster)
                       : Base( idData, nbCluster)
                       , p_data_(0)
    {}
    /** Bridge for the data */
    MixtureData<Data>* p_data_;
    /** estimated distances */
    Array2D<Real> dik_;
};

} // namespace STK

#endif /* STK_KERNELMIXTUREBRIDGE_H */
