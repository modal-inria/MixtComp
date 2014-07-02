/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: Nov 15, 2013
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              iovleff <S..._Dot_I..._At_stkpp_Dot_org> (see copyright for ...),
 *              Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file mixt_MixtureBridge.h
 *  @brief In this file .
 **/

#ifndef MIXT_MIXTUREBRIDGE_H
#define MIXT_MIXTUREBRIDGE_H

#include "../mixt_IMixture.h"
#include "../../Data/mixt_AugmentedData.h"
#include "mixt_GaussianBridges.h"
#include "mixt_CategoricalBridges.h"
#include "mixt_InitializeMixtureImpl.h"
#include "mixt_RemoveMissing.h"

namespace mixt
{

template<int Id>
class MixtureBridge : public mixt::IMixture
{
  public:
    // data type
    typedef typename BridgeTraits<Id>::Data Data;
    // augmented data type
    typedef typename BridgeTraits<Id>::AugData AugData;
    // statistics on missing values type
    typedef typename BridgeTraits<Id>::DataStats DataStats;
    // parameters type to get
    typedef typename BridgeTraits<Id>::Param Param;
    // type of the data
    typedef typename BridgeTraits<Id>::Type Type;
    // type of Mixture
    typedef typename BridgeTraits<Id>::Mixture Mixture;
    // type of Sampler
    typedef typename BridgeTraits<Id>::Sampler Sampler;
    // type of Sample Iterator
    typedef typename BridgeTraits<Id>::SamplerIterator SamplerIterator;

    /** constructor.
     *  @param idName id name of the mixture
     *  @param nbCluster number of cluster
     **/
    MixtureBridge(std::string const& idName, int nbCluster) :
      mixt::IMixture(idName, nbCluster),
      mixture_(nbCluster),
      m_augDataij_(),
      nbVariable_(0),
      sampler_(getData(),
               getParam())
    {}
    /** copy constructor */
    MixtureBridge(MixtureBridge const& bridge) :
      mixt::IMixture(bridge),
      mixture_(bridge.mixture_),
      m_augDataij_(bridge.m_augDataij_),
      nbVariable_(bridge.nbVariable_),
      sampler_(bridge.sampler_)
    {
      mixture_.setData(m_augDataij_.data_);
      mixture_.initializeModel();
    }
    /** This is a standard clone function in usual sense. It must be defined to
     *  provide new object of your class with values of various parameters
     *  equal to the values of calling object. In other words, this is
     *  equivalent to polymorphic copy constructor.
     *  @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* clone() const
    {
      return new MixtureBridge(*this);
    }
    /** This is a standard create function in usual sense. It must be defined to
     *  provide new object of your class with correct dimensions and state.
     *  In other words, this is equivalent to virtual constructor.
     * @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* create() const
    {
      MixtureBridge* p_bridge = new MixtureBridge(mixture_, idName(), nbCluster());
      p_bridge->m_augDataij_ = m_augDataij_;
      p_bridge->nbVariable_ = nbVariable_;
      // Bug Fix: set the correct data set
      p_bridge->mixture_.setData(p_bridge->m_augDataij_.data_);
      p_bridge->mixture_.initializeModel();
      return p_bridge;
    }
    /** @brief Initialize the model before its use by the composer.
     *  The parameters values are set to their default values if the mixture_ is
     *  newly created. if MixtureBridge::initializeModel is used during a
     *  cloning, mixture class have to take care of the existing values of the
     *  parameters.
     **/
    virtual void initializeStep()
    {
      if (!p_composer())
        STKRUNTIME_ERROR_NO_ARG(MixtureBridge::initializeModel,composer is not set);
      mixture_.setMixtureParameters(p_pk(), p_tik(), p_zi());
      mixture_.initializeStep();
      sampler_.setZi(p_zi()); // at this point the bridge has been registered on the composer and p_zi is valid
    }
    /** This function will be defined to set the data into your data containers.
     *  To facilitate data handling, framework provide templated functions,
     *  that can be called directly to get the data.
     */
    template<class MixtureManager>
    void setData(MixtureManager const* p_manager)
    {
      p_manager->getData(idName(), m_augDataij_, nbVariable_ );

      // data range filling
      for (int currVar = 0; currVar < m_augDataij_.data_.sizeCols(); ++currVar)
      {
        m_augDataij_.dataRanges_.push_back(std::pair<Type, Type>(STK::Stat::minSafe(m_augDataij_.data_.col(currVar)),
                                                                 STK::Stat::maxSafe(m_augDataij_.data_.col(currVar))));
#ifdef MC_DEBUG
      std::cout << "min, " << m_augDataij_.dataRanges_.at(currVar).first << std::endl;
      std::cout << "max, " << m_augDataij_.dataRanges_.at(currVar).second << std::endl;
#endif
      }
      removeMissing(m_augDataij_);
#ifdef MC_DEBUG
      std::cout << "v_missing_.size(): " << m_augDataij_.v_missing_.size() << std::endl;
      std::cout << "v_missingFiniteValues_.size(): " << m_augDataij_.v_missingFiniteValues_.size() << std::endl;
      std::cout << "v_missingIntervals_.size(): " << m_augDataij_.v_missingIntervals_.size() << std::endl;
      std::cout << "After removeMissing, " << idName() << std::endl;
      std::cout << m_augDataij_.data_ << std::endl;
#endif
      initializeMixture();
    }
    /** @brief This function should be used in order to initialize randomly the
     *  parameters of the ingredient.
     */
    virtual void randomInit()
    {
      mixture_.randomInit();
    }
    /** This function should be used for imputation of data.
     *  The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep()
    {
      // imputation based on mean values in classes is not general enough for MixtComp,
      // samplingStep is used instead
    }
    /** This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep()
    {
      mixture_.getParameters(param_); // update the parameters (used by the Sampler)
      SamplerIterator endIt(sampler_.end());
      for (SamplerIterator it = sampler_.begin(); it != endIt; ++it)
      {
        std::pair<std::pair<int, int>, Type> retValue(*it);
        m_augDataij_.data_(retValue.first.first, retValue.first.second) = retValue.second;
      }
    }
    /** This function is equivalent to Mstep and must be defined to update parameters.
     */
    virtual void paramUpdateStep()
    {
      mixture_.mStep();
    }
    /** This function should be used to store any intermediate results during
     *  various iterations after the burn-in period.
     *  @param iteration Provides the iteration number beginning after the burn-in period.
     */
    virtual void storeIntermediateResults(int iteration) {/**Do nothing by default*/}
    /**
     *  This step can be used by developer to finalize any thing. It will be called only once after we
     * finish running the SEM-gibbs algorithm.
     */
    virtual void finalizeStep() {}
    /**
     * This function must be defined to return the posterior probability (PDF)
     * for corresponding sample i and cluster k.
     * @param i Sample number
     * @param k Cluster number
     * @return the log-component probability
     */
    virtual double lnComponentProbability(int i, int k)
    {
      return mixture_.lnComponentProbability(i, k);
    }
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const
    {
      return mixture_.computeNbFreeParameters();
    }
    /** This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    /** This function must return the number of variables.
     *  @return Number of variables
     */
    virtual int nbVariable() const
    {
      return mixture_.nbVariable();
    }
    virtual void writeParameters(std::ostream& out) const
    {
      mixture_.writeParameters(out);
    }
    /** Utility function to use in mode debug in order to test that the
     *  model is well initialized. */
    int checkModel() const
    {
      return mixture_.checkModel();
    }

    virtual AugmentedData<Data> const* getData() const
    {
      return &m_augDataij_;
    }

    virtual Param const* getParam() const
    {
      return &param_;
    }

  protected:
    /** The stkpp mixture to bridge with the composer */
    Mixture mixture_;
    /** The augmented data set */
    AugData m_augDataij_;
    /** Current parameters of the STK Mixture */
    Param param_;
    /** number of variables in the data set */
    int nbVariable_;
    
  private:
    /** This function will be used in order to initialize the mixture model
     *  using informations stored by the data_ container.
     **/
     void initializeMixture()
     { InitializeMixtureImpl<Id>::run(mixture_, m_augDataij_);}
    /** protected constructor to use in order to create a bridge.
     *  @param mixture the mixture to copy
     *  @param idName id name of the mixture
     *  @param nbCluster number of cluster
     **/
    MixtureBridge(Mixture const& mixture, std::string const& idName, int nbCluster) :
      IMixture( idName, nbCluster),
      mixture_(mixture),
      m_augDataij_(),
      sampler_(getData(),
               getParam())
    {}

    /** Sampler to generate values */
    Sampler sampler_;
};

} // namespace mixt

#endif /* MIXT_MIXTUREBRIDGE_H */
