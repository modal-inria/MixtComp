/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2015

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
 *  Created on: May 22, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_ORDINAL
#define MIXT_ORDINAL

#include "../mixt_IMixture.h"
#include "mixt_BOSPath.h"

namespace mixt
{

template<typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>
class Ordinal : public IMixture
{
  public:

    /** constructor **/
    Ordinal(std::string const& idName,
            int nbClass,
            Vector<int> const* p_zi,
            const DataHandler* p_handler_,
            DataExtractor* p_extractor,
            const ParamSetter* p_paramSetter,
            ParamExtractor* p_paramExtractor,
            Real confidenceLevel) :
      IMixture(idName),
      p_zi_(p_zi),
      nbClass_(nbClass),
      nbModalities_(0),
      augData_(),
      nbInd_(0), // number of individuals will be set during setDataParam
      confidenceLevel_(confidenceLevel),
      dataStatComputer_(augData_,
                        confidenceLevel),
      muParamStatComputer_(mu_,
                           confidenceLevel),
      piParamStatComputer_(pi_,
                           confidenceLevel),
      p_handler_(p_handler_),
      p_dataExtractor_(p_extractor),
      p_paramSetter_(p_paramSetter),
      p_paramExtractor_(p_paramExtractor)
    {}

    std::string setDataParam(RunMode mode)
    {
      std::string warnLog;

      p_handler_->getData(idName(),
                          augData_,
                          nbInd_,
                          paramStr_,
                          -minModality, // ordinal data are modalities, offset enforces 0-based encoding through the whole mixture
                          warnLog);
      augData_.computeRange();

      std::string tempLog  = augData_.checkMissingType(acceptedType()); // check if the missing data provided are compatible with the model
                  tempLog += augData_.sortAndCheckMissing(); // sort and check for duplicates in missing values descriptions

      if(tempLog.size() > 0) // check on the missing values description
      {
        std::stringstream sstm;
        sstm << "Variable " << idName() << " with Ordinal model has a problem with the descriptions of missing values.\n" << tempLog;
        warnLog += sstm.str();
      }
      else if (augData_.dataRange_.min_ < 0) // modality encoding is 0-based
      {
        std::stringstream sstm;
        sstm << "Variable: " << idName() << " is described by an Ordinal model which requires a minimum value of 0 in either provided values or bounds."
             << " The minimum value currently provided is : " << augData_.dataRange_.min_ << std::endl;
        warnLog += sstm.str();
      }
      else // minimum value requirements have been met, whether the mode is learning or prediction
      {
        if (mode == learning_)
        {
          if (augData_.misCount_(present_) < minNbPresentValues) // Any variable with less than three samples will be rejected as not providing enough information for learning
          {
            std::stringstream sstm;
            sstm << "Variable: " << idName() << " only has " << augData_.misCount_(present_)
                 << " present values. Maybe there is an error in the data encoding. If the variable truly has less than "
                 << minNbPresentValues
                 << " present values, it should be removed from the study as it does not provide enough information." << std::endl;
            warnLog += sstm.str();
          }
          nbModalities_ = augData_.dataRange_.max_ + 1; // since an offset has been applied during getData, modalities are 0-based
          setPath(); // initialize the BOSPath vector elements with data gathered from the AugmentedData
        }
        else // prediction mode
        {
          setParam(); // set nbModalities_, mu_ and pi_ using p_paramSetter_

          muParamStatComputer_.setParamStorage();
          piParamStatComputer_.setParamStorage();

          if (nbModalities_ - 1 < augData_.dataRange_.max_)
          {
            std::stringstream sstm;
            sstm << "Variable: " << idName() << " requires a maximum value of " << minModality + nbModalities_ - 1
                 << " for the data during prediction. This maximum value corresponds to the maximum value used during the learning phase."
                 << " The maximum value in the data provided for prediction is : " << minModality + augData_.dataRange_.max_ << std::endl;
            warnLog += sstm.str();
          }

          // data range from learning is applied
          augData_.dataRange_.min_ = 0;
          augData_.dataRange_.max_ = nbModalities_ - 1;
          augData_.dataRange_.range_ = nbModalities_;

          setPath(); // initialize the BOSPath vector elements with data gathered from the AugmentedData
          computeObservedProba(); // parameters are know, so logProba can be computed immediately
        }

        dataStatComputer_.resizeStatStorage(nbInd_);
      }

      return warnLog;
    }

    /** Use information in AugmentedData to set the values of every path in path_ */
    void setPath()
    {
      for (int i = 0; i < nbInd_; ++i)
      {
        path_(i).setInit(0,
                         augData_.dataRange_.max_); // every initial segment is the same and spans all the modalities
        if (augData_.misData_(i).first == present_) // final value is set
        {
          path_(i).setEnd(augData_.data_(i),
                          augData_.data_(i));
        }
        else if (augData_.misData_(i).first == missing_)
        {
          path_(i).setEnd(0,
                          augData_.dataRange_.max_); // final interval is the same as initial interval
        }
        else if (augData_.misData_(i).first == missingIntervals_)
        {
          path_(i).setEnd(augData_.misData_(i).second[0],
                          augData_.misData_(i).second[1]); // bounds of the interval are provided
        }
      }
    }

    /** get parameters from single table, then dispatch it to mu_ and pi_ */
    void setParam()
    {
      Vector<Real> param;
      p_paramSetter_->getParam(idName(), // parameters are set using results from previous run
                               param);
      nbModalities_ = param.size() / (2 * nbClass_);
      mu_.resize(nbClass_);
      pi_.resize(nbClass_);
      for (int k = 0; k < nbClass_; ++k)
      {
        mu_(k) = param(2 * k    );
        pi_(k) = param(2 * k + 1);
      }
    }

    virtual void samplingStep(int ind)
    {
      if (augData_.misData_(ind).first == missing_) // if individual is completely missing, use samplePathForward instead of samplePath to accelerate computation
      {
        path_(ind).forwardSamplePath(mu_((*p_zi_)(ind)),
                                     pi_((*p_zi_)(ind)));
      }
      else // perform one round of Gibbs sampler for the designated individual
      {
        path_(ind).samplePath(mu_((*p_zi_)(ind)),
                              pi_((*p_zi_)(ind)),
                              sizeTuple);
      }
      augData_.data_(ind) = path_(ind).c_(nbModalities_ - 1).e_(0); // copy of the data from last element of path to augData, which will be useful for the dataStatComputer_ to compute statistics
    }

    virtual std::string mStep()
    {
      std::string warnLog;

      pi_ = 0.; // parameter is reinitialized
      Vector<Real> indPerClass(nbClass_);
      indPerClass = 0;
      for (int i = 0; i < nbInd_; ++i)
      {
        int indClass = (*p_zi_)(i);
        indPerClass(indClass) += 1.;
        pi_(indClass) += path_(i).nbZ();
      }
      pi_ /= indPerClass; // from accounts to frequencies

      Matrix<Real> logLik(nbClass_, nbModalities_);
      logLik = 0.;

      for (int p = 0; p < nbModalities_; ++p) // mu obtained from maximization over all possible values
      {
        for (int i = 0; i < nbInd_; ++i)
        {
          int indClass = (*p_zi_)(i);
          logLik(indClass,
                 p) += path_(i).computeLogProba(mu_(p),
                                                pi_(p));
        }
      }

      for (int k = 0; k < nbClass_; ++k)
      {
        int maxLik;
        logLik.row(k).maxCoeff(&maxLik);
        mu_(k) = maxLik;
      }

      return warnLog;
    }

    virtual void storeSEMBurnIn(int iteration,
                                int iterationMax) {} // nothing to be done here

    virtual void storeSEMRun(int iteration,
                             int iterationMax)
    {
      muParamStatComputer_.sampleParam(iteration, iterationMax);
      piParamStatComputer_.sampleParam(iteration, iterationMax);

      if (iteration == iterationMax) // at last iteration, compute the observed probability distribution logProba_
      {
        muParamStatComputer_.setExpectationParam(); // estimate mu parameter using mode / expectation
        piParamStatComputer_.setExpectationParam(); // estimate pi parameter using mode / expectation
        computeObservedProba(); // compute observed probabilities using estimated parameters
      }
    }

    void computeObservedProba()
    {
      observedProba_.resize(nbClass_, nbModalities_);
      BOSPath samplePath; // BOSPath used for the various samplings
      samplePath.setInit(0, nbModalities_ - 1);
      for (int k = 0; k < nbClass_; ++k)
      {
        RowVector<Real> nbInd(nbModalities_); // observed frequencies
        samplePath.setEnd(k, k);
        nbInd = 0;
        for (int i = 0; i < nbSampleBOS; ++i)
        {
          samplePath.forwardSamplePath(mu_(k), pi_(k)); // complete the individual
          nbInd(samplePath.c_(nbModalities_ - 1).e_(0)) += 1.; // register the x value, for marginalization
        }
        observedProba_.row(k) = nbInd / nbSampleBOS;
      }
    }

    virtual void storeGibbsRun(int ind,
                               int iteration,
                               int iterationMax)
    {
      dataStatComputer_.sampleVals(ind,
                                   iteration,
                                   iterationMax); // ConfIntStat called to sample value
    }

    virtual Real lnCompletedProbability(int i, int k)
    {
      return path_(i).computeLogProba(mu_(k), pi_(k)); // path_(i) contains a completed individual
    }

    /**
     * This function must be defined to return the observed likelihood
     * @return the observed log-likelihood
     */
    virtual Real lnObservedProbability(int i, int k)
    {
      if (augData_.misData_(i).first == present_) //
      {
        return std::log(observedProba_(k, augData_.data_(i))); // marginalized only over c_i
      }
      else if (augData_.misData_(i).first == missing_) // marginalized over c_i and all modalities: proba is 1.
      {
        return 0.;
      }
      else if (augData_.misData_(i).first == missingIntervals_) // marginalized over c_i and the observed interval
      {
        Real proba = 0;
        for (int p = augData_.misData_(i).second[0];
             p < augData_.misData_(i).second[1] + 1;
             ++p)
        {
          proba += observedProba_(k, p);
        }
        return std::log(proba);
      }
      return std::numeric_limits<Real>:: signaling_NaN(); // fail case
    }

    virtual int nbFreeParameter() const
    {
      return 1; // only the continuous pi_ parameter is taken into account, not the discrete mu_ parameter
    }

    virtual void writeParameters(std::ostream& out) const
    {
      for (int k = 0; k < nbClass_; ++k)
      {
        out << "Class: " << k << std::endl;
        out << "mu: " << mu_(k) << std::endl;
        out << "pi_: " << pi_(k) << std::endl;
      }
    }

    virtual void exportDataParam() const
    {
#ifdef MC_DEBUG
      std::cout << "MixtureBridge: exportDataParam, idName(): " << idName() << std::endl;
#endif

      Matrix<Real> paramStatStorage(2 * nbClass_, muParamStatComputer_.getStatStorage().cols()); // aggregates both mu and pi values
      Matrix<Real> paramLogStorage (2 * nbClass_, piParamStatComputer_.getLogStorage() .cols()); // aggregates both mu and pi logs
      for (int k = 0; k < nbClass_; ++k)
      {
        paramStatStorage.row(2 * k    ) = muParamStatComputer_.getStatStorage().row(k).cast<Real>();
        paramStatStorage.row(2 * k + 1) = piParamStatComputer_.getStatStorage().row(k);
        paramLogStorage .row(2 * k    ) = muParamStatComputer_.getLogStorage ().row(k).cast<Real>();
        paramLogStorage .row(2 * k + 1) = piParamStatComputer_.getLogStorage ().row(k);
      }
      p_dataExtractor_->exportVals(idName(),
                                   augData_,
                                   dataStatComputer_.getDataStatStorage()); // export the obtained data using the DataExtractor
      p_paramExtractor_->exportParam(idName(),
                                     paramStatStorage,
                                     paramLogStorage,
                                     paramNames(),
                                     confidenceLevel_);
    }

    std::vector<std::string> paramNames() const
    {
      std::vector<std::string> names(nbClass_ * 2);
      for (int k = 0; k < nbClass_; ++k)
      {
        std::stringstream sstmMean, sstmSd;
        sstmMean << "k: "
                 << k + minModality
                 << ", mu: ";
        sstmSd << "k: "
               << k + minModality
               << ", pi";
        names[2 * k    ] = sstmMean.str();
        names[2 * k + 1] = sstmSd  .str();
      }
      return names;
    }

    bool possibleNullProbability() const
    {
      if (pi_.minCoeff() > epsilon)
      {
        return false;
      }
      else
      {
        return true; // a precision of 0 concentrates the probability on the mode ??? Therefore should be true ? -> Very long initialization ...
      }
    }

    void removeMissing()
    {
      for (int i = 0; i < nbInd_; ++i)
      {
        path_(i).initPath(); // remove missing use to initialize learn, and should therefore use BOSPath::initPath() which is parameters free
      }
    };

    Vector<bool> acceptedType()
    {
      Vector<bool> at(nb_enum_MisType_);
      at(0) = true; // present_,
      at(1) = true;// missing_,
      at(2) = false;// missingFiniteValues_,
      at(3) = true;// missingIntervals_,
      at(4) = false;// missingLUIntervals_,
      at(5) = false;// missingRUIntervals_,
      return at;
    }

  private:

    /** Pointer to the zik class label */
    Vector<int> const* p_zi_;

    /** Number of classes */
    int nbClass_;

    /** Number of modalities */
    int nbModalities_;

    /** The augmented data set */
    AugmentedData<Vector<int> > augData_;

    /** Vector containing path for individuals */
    Vector<BOSPath> path_;

    /** Matrix containing observed log probability distribution
     * Modalities in rows
     * Classes in columns */
    Matrix<Real> observedProba_;

    /** Number of samples in the data set*/
    int nbInd_;

    /** Optional string parameters */
    std::string paramStr_;

    /** Confidence level used in computation of parameters and missing values statistics */
    Real confidenceLevel_;

    /** Mode parameter, one element per class */
    Vector<int> mu_;

    /** Precision parameter, one element per class */
    Vector<Real> pi_;

    /** Compute the statistics on missing data during GibbsRun phase */
    ConfIntDataStat<int> dataStatComputer_;

    /** Compute the statistics on mu parameter */
    ConfIntParamStat<int> muParamStatComputer_;

    /** Compute the statistics on pi parameter */
    ConfIntParamStat<Real> piParamStatComputer_;

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

#endif // MIXT_ORDINAL