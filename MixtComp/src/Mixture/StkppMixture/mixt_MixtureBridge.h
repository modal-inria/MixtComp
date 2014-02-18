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

#include "stkpp/projects/Clustering/include/STK_Clust_Traits.h"
#include "mixt_IMixture.h"
#include "../Data/mixt_AugmentedData.h"
#include "../Sampler/mixt_Imputer.h"
#include "../Sampler/mixt_SamplerTraits.h"

namespace mixt
{

template<int Id>
class MixtureBridge : public IMixture
{
  public:
    // data type
    typedef typename STK::Clust::MixtureTraits<Id>::Data Data;
    // parameters type to get
    typedef typename STK::Clust::MixtureTraits<Id>::Param Param;
    // type of the data
    typedef typename STK::Clust::MixtureTraits<Id>::Type Type;
    // type of Mixture
    typedef typename STK::Clust::MixtureTraits<Id>::Mixture Mixture;
    // type of Sampler
    typedef typename SamplerTraits<Id>::Sampler Sampler;

    /** constructor.
     *  @param idName id name of the mixture
     *  @param nbCluster number of cluster
     **/
    MixtureBridge( std::string const& idName, int nbCluster)
                 : IMixture( idName, nbCluster)
                 , mixture_(nbCluster), m_augDataij_(), nbVariable_(0)
    {
      mixture_.setData(m_augDataij_.data_);
    }
    /** copy constructor */
    MixtureBridge( MixtureBridge const& mixture)
                 : IMixture(mixture)
                 , mixture_(mixture.mixture_)
                 , m_augDataij_(mixture.m_augDataij_)
                 , nbVariable_(mixture.nbVariable_)
    {
      mixture_.setData(m_augDataij_.data_);
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
      MixtureBridge* p_mixture = new MixtureBridge( idName(), nbCluster());
      p_mixture->m_augDataij_ = m_augDataij_;
      p_mixture->nbVariable_ = nbVariable_;
      // Bug Fix: set the correct data set
      p_mixture->mixture_.setData(p_mixture->m_augDataij_.data_);
      return p_mixture;
    }
    /** @brief Initialize the model before its use by the composer.
     *  The parameters values are set to its default values if the mixture_ is
     *  newly created. if MixtureBridge::initializeModel is used during a
     *  cloning, model class have to take care of the existing values of the
     *  parameters.
     **/
    virtual void initializeModel()
    {
      if (!p_composer()){STKRUNTIME_ERROR_NO_ARG(MixtureBridge::initializeModel,composer is not set);};
      mixture_.setMixtureParameters( p_prop(), p_tik(), p_zi());
      mixture_.initializeModel();
    }
   /** This function will be defined to set the data into your data containers.
    *  To facilitate data handling, framework provide templated functions,
    *  that can be called directly to get the data.
    */
    virtual void setData()
    {
      IMixture::getData<AugmentedData<Data> >(m_augDataij_, nbVariable_);
      removeMissing();
      mixture_.setData(m_augDataij_.data_);
    }
    /** This function must be defined in derived class for initialization of mixture parameters. */
    virtual void initializeStep() { mixture_.initializeStep();}
    /** @brief This function should be used in order to initialize randomly the
     *  parameters of the ingredient.
     */
    virtual void randomInit() { mixture_.randomInit();};
    /** This function should be used for imputation of data.
     *  The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep()
    {
      /* for(ConstIterator it = v_missing_.begin(); it!= v_missing_.end(); ++it)
      { m_dataij_(it->first, it->second) = mixture_.impute(it->first, it->second);} */
    }
    /** This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep()
    {
      /* for(ConstIterator it = v_missing_.begin(); it!= v_missing_.end(); ++it)
      { m_dataij_(it->first, it->second) = mixture_.sample(it->first, it->second);} */
    }
    /** This function is equivalent to Mstep and must be defined to update parameters.
     */
    virtual void paramUpdateStep() { mixture_.mStep();}
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
    { return mixture_.lnComponentProbability(i, k);}
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const { return mixture_.computeNbFreeParameters();}
    /** This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    { mixture_.writeParameters(out);}
    /** Utility function to use in mode debug in order to test that the
     *  model is well initialized. */
    int checkModel() const { return mixture_.checkModel();}

  protected:
    /** The ingredient to bridge with the composer */
    Mixture mixture_;
    /** The augmented data set */
    AugmentedData<Data> m_augDataij_;
    /** number of variables in the data set */
    int nbVariable_;
    
  private:
    typedef std::vector<std::pair<int, int> >::const_iterator ConstIterator;

    /** Imputer used to generate initial values randomly */
    Imputer<Type> imputer_;

    /** Sampler to generate values */
    Sampler sampler_;

    /** Utility function to lookup the data set and remove missing values
     *  coordinates. */
    void removeMissing()
    { 
      // missing value [-inf,+inf] or ?
      for (typename std::vector<pos>::iterator it = m_augDataij_.v_missing_.begin();
           it != m_augDataij_.v_missing_.end();
           ++it)
      {
        m_augDataij_.data_((*it).first,
                           (*it).second) = imputer_.sampleRange(m_augDataij_.dataRanges_[(*it).second].first,
                                                                m_augDataij_.dataRanges_[(*it).second].second);
      }
      
      // missing values [a,b]
      for (typename std::vector<std::pair<pos, std::pair<Type, Type> > >::iterator it = m_augDataij_.v_missingIntervals_.begin();
           it != m_augDataij_.v_missingIntervals_.end();
           ++it)
      {
        m_augDataij_.data_((*it).first.first,
                           (*it).first.second) = imputer_.sampleRange((*it).second.first,
                                                                      (*it).second.second);
      }
      
      // missing values [-inf,b]
      for (typename std::vector<std::pair<pos, Type> >::iterator it = m_augDataij_.v_missingLUIntervals_.begin();
           it != m_augDataij_.v_missingLUIntervals_.end();
           ++it)
      {
        m_augDataij_.data_((*it).first.first,
                           (*it).first.second) = imputer_.sampleRange(m_augDataij_.dataRanges_[(*it).second].first,
                                                                      (*it).second);
      }
      
      // missing values [a,+inf]
      for (typename std::vector<std::pair<pos, Type> >::iterator it = m_augDataij_.v_missingRUIntervals_.begin();
           it != m_augDataij_.v_missingRUIntervals_.end();
           ++it)
      {
        m_augDataij_.data_((*it).first.first,
                           (*it).first.second) = imputer_.sampleRange((*it).second,
                                                                      m_augDataij_.dataRanges_[(*it).second].second);
      }
    }
};

} // namespace mixt

#endif /* MIXT_MIXTUREBRIDGE_H */
