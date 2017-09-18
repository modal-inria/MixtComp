/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

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
#include "mixt_CategoricalBridges.h"
#include "mixt_GaussianBridges.h"
#include "mixt_PoissonBridges.h"
#include "../../IO/mixt_IO.h"
#include "../../Various/mixt_Constants.h"
#include "../../Param/mixt_ConfIntParamStat.h"

namespace mixt {

template<int Id,
         typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>

class MixtureBridge : public IMixture {
  public:
    // data type
    typedef typename BridgeTraits<Id>::Data Data;
    // augmented data type
    typedef typename BridgeTraits<Id>::AugData AugData;
    // statistics on DataStat computer
    typedef typename BridgeTraits<Id>::DataStat DataStat;
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
    MixtureBridge(
        Index indexMixture,
        std::string const& idName,
        Index nbClass,
        const Vector<Index>* p_zi,
        const Vector<std::set<Index> >& classInd,
        const DataHandler* p_handler,
        DataExtractor* p_extractor,
        const ParamSetter* p_paramSetter,
        ParamExtractor* p_paramExtractor,
        Real confidenceLevel) :
          IMixture(indexMixture, idName),
          p_zi_(p_zi),
          classInd_(classInd),
          nbClass_(nbClass),
          param_(), // must be initialized here, as will immediately be resized in mixture_ constructor
          mixture_(
              idName,
              nbClass,
              param_,
              classInd),
          augData_(),
          nbInd_(0),
          confidenceLevel_(confidenceLevel),
          sampler_(
              *this,
              augData_,
              param_,
              nbClass),
          dataStat_(augData_, confidenceLevel),
          paramStat_(param_, confidenceLevel),
          likelihood_(
              param_,
              augData_,
              nbClass),
          p_handler_(p_handler),
          p_dataExtractor_(p_extractor),
          p_paramSetter_(p_paramSetter),
          p_paramExtractor_(p_paramExtractor) {}

    /** This function will be defined to set the data into your data containers.
     *  To facilitate data handling, framework provide templated functions,
     *  that can be called directly to get the data.
     */
    std::string setDataParam(RunMode mode) {

      std::string warnLog;
      warnLog += p_handler_->getData(idName(),
                                     augData_,
                                     nbInd_,
                                     paramStr_,
                                     (mixture_.hasModalities()) ? (-minModality) : (0)); // minModality offset for categorical models

      if (warnLog.size() > 0) {
        return warnLog;
      }
      augData_.computeRange();
      std::string tempLog  = augData_.checkMissingType(mixture_.acceptedType()); // check if the missing data provided are compatible with the model

      if(tempLog.size() > 0) { // check on the missing values description
        std::stringstream sstm;
        sstm << "Variable " << idName() << " has a problem with the descriptions of missing values." << std::endl << tempLog;
        warnLog += sstm.str();
      }

      if (mode == prediction_) {
        p_paramSetter_->getParam(idName_, // parameters are set using results from previous run
                                 "NumericalParam",
                                 param_,
                                 paramStr_); // note that in the prediction case, the eventual paramStr_ obtained from p_handler_->getData is overwritten by the one provided by the parameter structure from the learning

        paramStat_.setParamStorage(); // paramStatStorage_ is set now, using dimensions of param_, and will not be modified during predict run by the paramStat_ object for some mixtures, there will be errors if the range of the data in prediction is different from the range of the data in learning in the case of modalities, this can not be performed earlier, as the max val is computed at mixture_.setModalities(nbParam)
      }

      warnLog += mixture_.setData(paramStr_, // checks on data bounds are made here
                                  augData_);

      dataStat_.setNbIndividual(nbInd_);

      return warnLog;
    }

    virtual void samplingStepNoCheck(Index ind) {
      sampler_.samplingStepNoCheck(
          ind,
          (*p_zi_)(ind));
    }

    /**
     * Estimate parameters by maximum likelihood
     */
    virtual void mStep() {
      mixture_.mStep();
    }

    /** This function should be used to store any intermediate results during
     *  various iterations after the burn-in period.
     *  @param iteration Provides the iteration number beginning after the burn-in period.
     */
    virtual void storeSEMRun(Index iteration,
                             Index iterationMax) {
      paramStat_.sampleParam(iteration,
                             iterationMax);
      if (iteration == iterationMax) {
        paramStat_.normalizeParam(paramStr_); // enforce that estimated proportions sum to 1, but only if paramStr is of the form "nModality: x"
        paramStat_.setExpectationParam(); // set parameter to mode / expectation
      }
    }

    virtual void storeGibbsRun(Index sample,
                               Index iteration,
                               Index iterationMax) {
      dataStat_.sampleVals(sample,
                           iteration,
                           iterationMax);
      if (iteration == iterationMax) {
        dataStat_.imputeData(sample); // impute the missing values using empirical mean or mode, depending of the model
      }
    }

    /**
     * This function must be defined to return the completed likelihood, using the current values for
     * unknown values
     * @return the completed log-likelihood
     */
    virtual Real lnCompletedProbability(Index i, Index k) {
      return likelihood_.lnCompletedProbability(i, k);
    }

    /**
     * This function must be defined to return the observed likelihood
     * @return the observed log-likelihood
     */
    virtual Real lnObservedProbability(Index i, Index k) {
      return likelihood_.lnObservedProbability(i, k);
    }

    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual Index nbFreeParameter() const {
      return mixture_.computeNbFreeParameters();
    }

    /** This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters() const {
      mixture_.writeParameters();
    }

    virtual void exportDataParam() const {
      p_dataExtractor_->exportVals(indexMixture_,
                                   mixture_.hasModalities(),
                                   idName_,
                                   augData_,
                                   dataStat_.getDataStatStorage()); // export the obtained data using the DataExtractor
      p_paramExtractor_->exportParam(indexMixture_,
                                     idName(),
                                     "NumericalParam",
                                     paramStat_.getStatStorage(),
                                     paramStat_.getLogStorage(),
                                     mixture_.paramNames(),
                                     confidenceLevel_,
                                     paramStr_);
    }

    void initData(Index i) {
      augData_.removeMissing(i);
    }

    void initParam(const Vector<Index>& initObs) {
      mixture_.initParam(initObs);
    };

    Index checkSampleCondition(std::string* warnLog = NULL) const {
      if (warnLog == NULL) {
        return mixture_.checkSampleCondition();
      }
      else {
        std::string warn;
        Real proba = mixture_.checkSampleCondition(&warn);
        if (warn.size() > 0) {
          std::stringstream sstm;
          sstm << "checkSampleCondition, error in variable " << idName_ << std::endl
               << warn;
          *warnLog += sstm.str();
        }
        return proba;
      }
    }

    /**
     * The observed probability is fast to compute for simple models. There is no need to
     * precompute an empirical distribution. */
    virtual void computeObservedProba() {};

    virtual void initializeMarkovChain() {};
  protected:
    /** Pointer to the zik class label */
    const Vector<Index>* p_zi_;

    /** Reference to a vector containing in each element a set of the indices of individuals that
     * belong to this class. Can be passed as an alternative to zi_ to a subtype of IMixture. */
    const Vector<std::set<Index> >& classInd_;

    /** Number of classes */
    int nbClass_;

    /** Current parameters of the mixture_ */
    Vector<Real> param_;

    /** The simple mixture to bridge with the composer */
    Mixture mixture_;

    /** The augmented data set */
    AugData augData_;

    /** Parameters transmitted by the user */
    std::string paramStr_;

    /** number of samples in the data set*/
    Index nbInd_;

    /** confidence level used in computation of parameters and missing values statistics */
    Real confidenceLevel_;

    /** Sampler to generate values */
    Sampler sampler_;

    /** Statistics computer for missing data */
    DataStat dataStat_;

    /** Statistics storage for parameters */
    ConfIntParamStat<Vector<Real> > paramStat_;

    /** Computation of the observed likelihood */
    Likelihood likelihood_;

    /** Pointer to the data handler */
    const DataHandler* p_handler_;

    /** Pointer to the data extractor */
    DataExtractor* p_dataExtractor_;

    /** Pointer to the param setter */
    const ParamSetter* p_paramSetter_;

    /** Pointer to the parameters extractor */
    ParamExtractor* p_paramExtractor_;
};

} // namespace mixt

#endif /* MIXT_MIXTUREBRIDGE_H */
