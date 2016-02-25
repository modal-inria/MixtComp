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
#include "mixt_CategoricalBridges.h"
#include "mixt_GaussianBridges.h"
#include "mixt_PoissonBridges.h"
#include "../../IO/mixt_IO.h"
#include "../../Various/mixt_Constants.h"
#include "../../Param/mixt_ConfIntParamStat.h"

namespace mixt
{

template<int Id,
         typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>

class MixtureBridge : public IMixture
{
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
    MixtureBridge(int indexMixture,
                  std::string const& idName,
                  int nbClass,
                  const Vector<int>* p_zi,
                  const Vector<std::set<int> >& classInd,
                  const DataHandler* p_handler,
                  DataExtractor* p_extractor,
                  const ParamSetter* p_paramSetter,
                  ParamExtractor* p_paramExtractor,
                  Real confidenceLevel) :
      IMixture(indexMixture,
               idName),
      p_zi_(p_zi),
      classInd_(classInd),
      nbClass_(nbClass),
      param_(), // must be initialized here, as will immediately be resized in mixture_ constructor
      mixture_(idName,
               nbClass,
               param_,
               classInd),
      augData_(),
      nbInd_(0),
      confidenceLevel_(confidenceLevel),
      sampler_(*this,
               augData_,
               param_,
               nbClass),
      dataStat_(augData_,
                confidenceLevel),
      paramStat_(param_,
                 confidenceLevel),
      likelihood_(param_,
                  augData_,
                  nbClass),
      p_handler_(p_handler),
      p_dataExtractor_(p_extractor),
      p_paramSetter_(p_paramSetter),
      p_paramExtractor_(p_paramExtractor)
    {}

    /** This function will be defined to set the data into your data containers.
     *  To facilitate data handling, framework provide templated functions,
     *  that can be called directly to get the data.
     */
    std::string setDataParam(RunMode mode)
    {
      std::string warnLog;
      warnLog += p_handler_->getData(idName(),
                                     augData_,
                                     nbInd_,
                                     paramStr_,
                                     (mixture_.hasModalities()) ? (-minModality) : (0)); // minModality offset for categorical models

      augData_.computeRange();
      std::string tempLog  = augData_.checkMissingType(mixture_.acceptedType()); // check if the missing data provided are compatible with the model
      if(tempLog.size() > 0) // check on the missing values description
      {
        std::stringstream sstm;
        sstm << "Variable " << idName() << " with model " << mixture_.model() << " has a problem with the descriptions of missing values.\n" << tempLog;
        warnLog += sstm.str();
      }
      else if (mixture_.checkMinVal() && augData_.dataRange_.min_ < mixture_.minVal()) // test the requirement for the data (and bounds) to be above a specified value
      {
        std::stringstream sstm;
        sstm << "Variable: " << idName() << " requires a minimum value of " << mixture_.minVal()
             << " in either provided values or bounds. The minimum value currently provided is : " << augData_.dataRange_.min_ << std::endl;
        warnLog += sstm.str();
      }
      else // minimum value requirements have been met, whether the mode is learning or prediction
      {
        mixture_.setData(augData_.data_);

        if (mode == learning_) // learning mode
        {
          if (mixture_.hasModalities())
          {
            mixture_.setModalities(augData_.dataRange_.max_ + 1); // set the number of modalities
          }
        }
        else // predict mode
        {
          p_paramSetter_->getParam(idName_, // parameters are set using results from previous run
                                   "NumericalParam",
                                   param_);
          int nbModalities = param_.rows() / nbClass_; // number of parameters for each cluster
          if (mixture_.hasModalities()) // all modalities might not be present in the predict set, and as such the real data range from the learning set must be used
          {
            mixture_.setModalities(nbModalities);
          }
          paramStat_.setParamStorage(); // paramStatStorage_ is set now, using dimensions of param_, and will not be modified during predict run by the paramStat_ object
          // for some mixtures, there will be errors if the range of the data in prediction is different from the range of the data in learning
          // in the case of modalities, this can not be performed earlier, as the max val is computed at mixture_.setModalities(nbParam)
          if (mixture_.checkMaxVal() && mixture_.maxVal() < augData_.dataRange_.max_)
          {
            std::stringstream sstm;
            sstm << "Variable: " << idName_ << " requires a maximum value of " << ((mixture_.hasModalities()) ? (minModality) : (0)) + mixture_.maxVal()
                 << " for the data during prediction. This maximum value usually corresponds to the maximum value used during the learning phase."
                 << " The maximum value in the data provided for prediction is : " << ((mixture_.hasModalities()) ? (minModality) : (0)) + augData_.dataRange_.max_ << std::endl;
            warnLog += sstm.str();
          }
          if (mixture_.hasModalities()) // now that predict observed values have been checked, the real data range must be used for all data
          {
            augData_.dataRange_.min_ = 0;
            augData_.dataRange_.max_ = nbModalities - 1;
            augData_.dataRange_.range_ = nbModalities;
          }
        }

        dataStat_.resizeStatStorage(nbInd_);
      }

      return warnLog;
    }

    virtual void samplingStepCheck(int ind)
    {
      sampler_.samplingStepCheck(ind,
                                (*p_zi_)(ind));
    }

    virtual void samplingStepNoCheck(int ind)
    {
      sampler_.samplingStepNoCheck(ind,
                                (*p_zi_)(ind));
    }

    /**
     * Estimate parameters by maximum likelihood
     */
    virtual void mStep(EstimatorType bias) {
      mixture_.mStep(bias);
    }

    /** This function should be used to store any intermediate results during
     *  various iterations after the burn-in period.
     *  @param iteration Provides the iteration number beginning after the burn-in period.
     */
    virtual void storeSEMRun(int iteration,
                             int iterationMax) {
      paramStat_.sampleParam(iteration,
                             iterationMax);
      if (iteration == iterationMax) {
        if (mixture_.hasModalities()) {
          paramStat_.normalizeParam(nbClass_,
                                    mixture_.nbModality()); // enforce that estimated proportions sum to 1
        }
        paramStat_.setExpectationParam(); // set parameter to mode / expectation
      }
    }

    virtual void storeGibbsRun(int sample,
                               int iteration,
                               int iterationMax)
    {
      dataStat_.sampleVals(sample,
                                   iteration,
                                   iterationMax);
      if (iteration == iterationMax)
      {
        dataStat_.imputeData(sample); // impute the missing values using empirical mean or mode, depending of the model
      }
    }

    /**
     * This function must be defined to return the completed likelihood, using the current values for
     * unknown values
     * @return the completed log-likelihood
     */
    virtual Real lnCompletedProbability(int i, int k)
    {
      return likelihood_.lnCompletedProbability(i, k);
    }

    /**
     * This function must be defined to return the observed likelihood
     * @return the observed log-likelihood
     */
    virtual Real lnObservedProbability(int i, int k)
    {
      return likelihood_.lnObservedProbability(i, k);
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
    virtual void writeParameters() const
    {
      mixture_.writeParameters();
    }

    virtual void exportDataParam() const
    {
      p_dataExtractor_->exportVals(indexMixture_,
                                   idName_,
                                   augData_,
                                   dataStat_.getDataStatStorage()); // export the obtained data using the DataExtractor
      p_paramExtractor_->exportParam(idName(),
                                     "NumericalParam",
                                     paramStat_.getStatStorage(),
                                     paramStat_.getLogStorage(),
                                     mixture_.paramNames(),
                                     confidenceLevel_);
    }

    void removeMissing(initParam algo) {augData_.removeMissing();}

    int checkSampleCondition(std::string* warnLog = NULL) const
    {
      if (warnLog == NULL)
      {
        return mixture_.checkSampleCondition();
      }
      else
      {
        std::string warn;
        Real proba = mixture_.checkSampleCondition(&warn);
        if (warn.size() > 0)
        {
          std::stringstream sstm;
          sstm << "checkSampleCondition, error in variable " << idName_ << std::endl
               << warn;
          *warnLog += sstm.str();
        }
        return proba;
      }
    }

  protected:
    /** Pointer to the zik class label */
    const Vector<int>* p_zi_;

    /** Reference to a vector containing in each element a set of the indices of individuals that
     * belong to this class. Can be passed as an alternative to zi_ to a subtype of IMixture. */
    const Vector<std::set<int> >& classInd_;

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
    int nbInd_;

    /** confidence level used in computation of parameters and missing values statistics */
    Real confidenceLevel_;

    /** Sampler to generate values */
    Sampler sampler_;

    /** Statistics computer for missing data */
    DataStat dataStat_;

    /** Statistics storage for parameters */
    ConfIntParamStat<Real> paramStat_;

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
