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
 * created on: 15 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_MixtureBridge.h
 *  @brief In this file we define the bridge class between the mixtures and the composer.
 **/

#ifndef STK_MIXTUREBRIDGE_H
#define STK_MIXTUREBRIDGE_H

#include "../STK_IMixture.h"

#include "STK_Bridges.h"
#include "STK_DataBridge.h"
#include "STK_InitializeMixtureImpl.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief Templated implementation of the IMixture interface allowing
 *  to bridge a stk++ mixture with the composer.
 *
 * @tparam Id is any identifier of a concrete model deriving from the
 * interface STK::IMixtureModel class.
 */
template<int Id>
class MixtureBridge: public IMixture
{
  public:
    // type of Mixture
    typedef typename Clust::BridgeTraits<Id>::Mixture Mixture;
    // data type to set
    typedef typename Clust::BridgeTraits<Id>::Data Data;
    // parameters type to get
    typedef typename Clust::BridgeTraits<Id>::Param Param;
    // type of the data
    typedef typename Clust::BridgeTraits<Id>::Type Type;

    /** default constructor.
     *  @param idName id name of the mixture model
     *  @param nbCluster number of cluster
     **/
    MixtureBridge( std::string const& idName, int nbCluster)
                 : IMixture( idName, nbCluster)
                 , mixture_(nbCluster)
                 , data_()
    {}
    /** copy constructor */
    MixtureBridge( MixtureBridge const& mixture)
                 : IMixture(mixture)
                 , mixture_(mixture.mixture_)
                 , data_(mixture.data_)
    {  mixture_.setData(data_.m_dataij()); mixture_.initializeModel();} /* default implementation of initializeMixture*/
    /** This is a standard clone function in usual sense. It must be defined to
     *  provide new object of your class with values of various parameters
     *  equal to the values of calling object. In other words, this is
     *  equivalent to polymorphic copy constructor.
     *  @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* clone() const
    { return new MixtureBridge(*this);}
    /** This is a standard create function in usual sense. It must be defined to
     *  provide new object of your class with correct dimensions and state.
     *  In other words, this is equivalent to virtual constructor.
     * @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* create() const
    {
      MixtureBridge* p_bridge = new MixtureBridge( mixture_, idName(), nbCluster());
      p_bridge->data_ = data_;
      // Bug Fix: set the correct data set
      p_bridge->mixture_.setData(p_bridge->data_.m_dataij());
      p_bridge->mixture_.initializeModel();
      return p_bridge;
    }
    /** @brief Initialize the mixture model before its use by the composer.
     *  The parameters values are set to their default values if the mixture_
     *  is newly created. if MixtureBridge::initializeStep is used during a
     *  cloning, mixture class have to take care of the existing values of the
     *  parameters.
     **/
    virtual void initializeStep()
    {
      if (!p_composer())
        STKRUNTIME_ERROR_NO_ARG(MixtureBridge::initializeStep,composer is not set);
      mixture_.setMixtureParameters( p_pk(), p_tik(), p_zi());
      mixture_.initializeStep();
    }
    /** This function will be defined to set the data into your data container
     *  (aka DataBridge<Id>). To facilitate data handling, framework provide
     *  templated functions, that can be called directly to get the data.
     **/
    template<class MixtureManager>
    void setData(MixtureManager const* p_manager)
    { data_.setData(p_manager, idName());
      initializeMixture();
    }
     /** This function must be defined to return the component probability (PDF)
     *  for corresponding sample i and cluster k.
     * @param i,k Sample and Cluster numbers
     * @return the log-component probability
     */
    virtual double lnComponentProbability(int i, int k)
    { return mixture_.lnComponentProbability(i, k);}
    /** This function is equivalent to Mstep and must be defined to update
     * parameters.
     */
    virtual void paramUpdateStep() { mixture_.mStep();}
    /** @brief This function should be used in order to initialize randomly the
     *  parameters of the mixture.
     */
    virtual void randomInit() { mixture_.randomInit();};
    /** This function should be used for imputation of data.
     *  The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep()
    {
      typedef std::vector<std::pair<int,int> >::const_iterator ConstIterator;
      for(ConstIterator it = data_.v_missing().begin(); it!= data_.v_missing().end(); ++it)
      { data_.m_dataij_(it->first, it->second) = mixture_.impute(it->first, it->second);}
    }
    /** This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep()
    {
      typedef std::vector<std::pair<int,int> >::const_iterator ConstIterator;
      for(ConstIterator it = data_.v_missing().begin(); it!= data_.v_missing().end(); ++it)
      { data_.m_dataij_(it->first, it->second) = mixture_.sample(it->first, it->second);}
    }
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const { return mixture_.computeNbFreeParameters();}
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbVariable() const { return mixture_.nbVariable();}
    /** This function can be used to write summary of parameters to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    { mixture_.writeParameters(out);}
    /** This function can be used in order to the values of the parameters
     *  in an Array2D.
     *  @param param the array with the parameters of the mixture.
     */
    void getParameters(Param& param) const
    { mixture_.getParameters(param);}


  private:
    /** This function will be used in order to initialize the mixture model
     *  using informations stored by the data_ container.
     **/
     void initializeMixture()
     { InitializeMixtureImpl<Id>::run(mixture_, data_);}
    /** protected constructor to use in order to create a bridge.
     *  @param mixture the mixture to copy
     *  @param idName id name of the mixture
     *  @param nbCluster number of cluster
     **/
    MixtureBridge( Mixture const& mixture, std::string const& idName, int nbCluster)
                 : IMixture( idName, nbCluster)
                 , mixture_(mixture)
                 , data_()
    {}
    /** The Mixture to bridge with the composer */
    Mixture mixture_;
    /** Bridge for the data */
    DataBridge<Id> data_;
};

} // namespace STK

#endif /* MIXTUREBRIDGE_H */
