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
#include "../../Various/mixt_IO.h"

namespace mixt
{

template<int Id,
         typename DataHandler,
         typename DataExtractor,
         typename ParamExtractor>

class MixtureBridge : public mixt::IMixture
{
  public:
    // data type
    typedef typename BridgeTraits<Id>::Data Data;
    // augmented data type
    typedef typename BridgeTraits<Id>::AugData AugData;
    // statistics on DataStat computer
    typedef typename BridgeTraits<Id>::DataStatComputer DataStatComputer;
    // type of DataStat storage
    typedef typename BridgeTraits<Id>::DataStatStorage DataStatStorage;
    // statistics on model parameters
    typedef typename BridgeTraits<Id>::ParamStat ParamStat;
    // type of the data
    typedef typename BridgeTraits<Id>::Type Type;
    // type of Mixture
    typedef typename BridgeTraits<Id>::Mixture Mixture;
    // type of Sampler
    typedef typename BridgeTraits<Id>::Sampler Sampler;
    // type of Likelihood
    typedef typename BridgeTraits<Id>::Likelihood Likelihood;

    /** constructor.
     *  @param idName id name of the mixture
     *  @param nbCluster number of cluster
     **/
    MixtureBridge(std::string const& idName,
                  int nbCluster,
                  const DataHandler* p_handler_,
                  DataExtractor* p_extractor,
                  ParamExtractor* p_paramExtractor,
                  STK::Real confidenceLevel) :
      mixt::IMixture(idName, nbCluster),
      mixture_(nbCluster),
      m_augDataij_(),
      nbVariable_(0),
      sampler_(&m_augDataij_,
               getParam()),
      dataStatComputer_(getData(),
                        &dataStatStorage_,
                        confidenceLevel),
      paramStat_(getParam()),
      likelihood_(getParam(),
                  getData()),
      p_handler_(p_handler_),
      p_dataExtractor_(p_extractor),
      p_paramExtractor_(p_paramExtractor)
      // dataStatStorage_ is an empty std::map at construction
    {}
    /** copy constructor */
    MixtureBridge(MixtureBridge const& bridge) :
      mixt::IMixture(bridge),
      mixture_(bridge.mixture_),
      m_augDataij_(bridge.m_augDataij_),
      nbVariable_(bridge.nbVariable_),
      sampler_(bridge.sampler_),
      dataStatComputer_(bridge.dataStatComputer_),
      paramStat_(bridge.paramStat_),
      likelihood_(bridge.likelihood_),
      p_handler_(bridge.p_handler_),
      p_dataExtractor_(bridge.p_dataExtractor_),
      p_paramExtractor_(bridge.p_paramExtractor_),
      dataStatStorage_(bridge.dataStatStorage_)
    {
      mixture_.setData(m_augDataij_.data_);
      mixture_.initializeModel();
    }
    /** @brief Initialize the model before its use by the composer.
     *  The parameters values are set to their default values if the mixture_ is
     *  newly created. if MixtureBridge::initializeModel is used during a
     *  cloning, mixture class have to take care of the existing values of the
     *  parameters.
     **/
    virtual void initializeStep()
    {
#ifdef MC_DEBUG
      std::cout << "MixtureBridge::initializeStep()"  << std::endl;
      std::cout << "\tidName(): " << idName() << std::endl;
      std::cout << "\tp_composer()" << p_composer() << std::endl;
      std::cout << "\tp_zi(): " << p_zi() << std::endl;
#endif
      if (!p_composer())
        std::cout << "MixtureBridge::initializeStep(), "
                  << idName()
                  << " model is not set." << std::endl;
      mixture_.setMixtureParameters(p_pk(), p_tik(), p_zi());
      mixture_.initializeStep();

      /** get a sample of parameters to initialize the ParamStat object
       * with the right size
       */
      mixture_.getParameters(param_);
      paramStat_.initialize();
    }
    /** This function will be defined to set the data into your data containers.
     *  To facilitate data handling, framework provide templated functions,
     *  that can be called directly to get the data.
     */
    void setData()
    {
#ifdef MC_DEBUG
        std::cout << "MixtureBridge::setData(), idName(): " << idName() << std::endl;
#endif
      p_handler_->getData(idName(), m_augDataij_, nbVariable_ );
      initializeMixture();
    }
    /** This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep(int i)
    {
      mixture_.getParameters(param_); // update the parameters (used by the Sampler)
      sampler_.sampleIndividual(i,
                                p_zi()->elt(i));
    }
    /** This function is equivalent to Mstep and must be defined to update parameters.
     */
    virtual void paramUpdateStep()
    {
      mixture_.mStep();
    }
    /** This function should be used to store any results during the burn-in period
     *  @param iteration Provides the iteration number during the burn-in period
     */
    virtual void storeShortRun(int iteration,
                               int iterationMax)
    {
#ifdef MC_LOG
      std::stringstream fileNameA;
      fileNameA << "out/log/";
      fileNameA << idName();
      fileNameA << "-";
      fileNameA << iteration;
      fileNameA << "-param.csv";
      writeDataCsv(fileNameA.str(), param_);

      std::stringstream fileNameB;
      fileNameB << "out/log/";
      fileNameB << idName();
      fileNameB << "-";
      fileNameB << iteration;
      fileNameB << "-data.csv";
      writeDataCsv(fileNameB.str(), m_augDataij_.data_);
#endif
    }
    /** This function should be used to store any intermediate results during
     *  various iterations after the burn-in period.
     *  @param iteration Provides the iteration number beginning after the burn-in period.
     */
    virtual void storeLongRun(int iteration,
                              int iterationMax)
    {
      mixture_.getParameters(param_);
      paramStat_.sampleParam();
      if (iteration == iterationMax)
      {
        // reinject the SEM estimated parameters into the mixture
        STK::Array2D<STK::Real> param;
        paramStat_.exportCompleteParam(&param);
        mixture_.getParameters(param);
      }
    }

    virtual void storeData(int sample,
                           int iteration,
                           int iterationMax)
    {
#ifdef MC_DEBUG
      std::cout << "MixtureBridge::storeData, for " << idName();
#endif
      dataStatComputer_.sampleVals(sample,
                                   iteration,
                                   iterationMax);
    }

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

    /**
     * This function must be defined to return the observed likelihood
     * @return the observed log-likelihood
     */
    virtual void lnObservedLikelihood(STK::Array2DVector<STK::Real>* lnComp, int k)
    {
      mixture_.getParameters(param_); // update the parameters
#ifdef MC_DEBUG
      std::cout << "MixtureBridge::lnObservedLikelihood(), getParameters" << std::endl;
      std::cout << "\tidName: " << idName() << std::endl;
      std::cout << "\tparam: " << std::endl;
      std::cout << param_ << std::endl;
#endif
      likelihood_.lnLikelihood(lnComp, k);
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

    virtual const STK::Array2D<STK::Real>* getParam() const
    {
      return &param_;
    }

    virtual const DataStatStorage* getDataStatStorage() const
    {
      return &dataStatStorage_;
    }

    virtual void exportDataParam() const
    {
      p_dataExtractor_->exportVals(idName(),
                                   getData(),
                                   getDataStatStorage()); // export the obtained data using the DataExtractor

      //export the parameters
      STK::Array2D<STK::Real> param;
      paramStat_.exportCompleteParam(&param);
#ifdef MC_DEBUG
      std::cout << "MixtureBridge::exportDataParam(), getParameters" << std::endl;
      std::cout << "\tidName: " << idName() << std::endl;
      std::cout << "\tparam: " << std::endl;
      std::cout << param << std::endl;
#endif
      p_paramExtractor_->exportParam(idName(),
                                     &param);
    }

  protected:
    /** The stkpp mixture to bridge with the composer */
    Mixture mixture_;
    /** The augmented data set */
    AugData m_augDataij_;
    /** Current parameters of the STK Mixture */
    STK::Array2D<STK::Real> param_;
    /** number of variables in the data set */
    int nbVariable_;
    /** Sampler to generate values */
    Sampler sampler_;
    /** Statistics computer for missing data */
    DataStatComputer dataStatComputer_;
    /** Statistics storage for parameters */
    ParamStat paramStat_;
    /** Computation of the observed likelihood */
    Likelihood likelihood_;

    /** Pointer to the data handler */
    const DataHandler* p_handler_;
    /** Pointer to the data extractor */
    DataExtractor* p_dataExtractor_;
    /** Pointer to the parameters extractor */
    ParamExtractor* p_paramExtractor_;

    /** Statistics storage for missing data */
    DataStatStorage dataStatStorage_;

  private:
    /** This function will be used in order to initialize the mixture model
     *  using informations stored by the data_ container.
     **/
     void initializeMixture()
     {
       InitializeMixtureImpl<Id>::run(mixture_, m_augDataij_);
     }
};

} // namespace mixt

#endif /* MIXT_MIXTUREBRIDGE_H */
