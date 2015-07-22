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
      mu_(nbClass),
      pi_(nbClass),
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
#ifdef MC_DEBUG
        std::cout << "Ordinal::setDataParam" << std::endl;
#endif

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

          computeObservedProba(); // parameters are know, so logProba can be computed immediately
        }

#ifdef MC_DEBUG
        std::cout << "augData_.data_" << std::endl;
        std::cout << augData_.data_ << std::endl;
#endif

        path_.resize(nbInd_);
        setPath(); // initialize the BOSPath vector elements with data gathered from the AugmentedData

        dataStatComputer_.resizeStatStorage(nbInd_);
      }

      return warnLog;
    }

    /**
     * Use information in AugmentedData to set the values of every path in path_. Called at the end
     * of setDataParam
     * */
    void setPath()
    {
#ifdef MC_DEBUG
      std::cout << "Ordinal::setPath" << std::endl;
      std::cout << "path_.size(): " << path_.size() << std::endl;
#endif
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
#ifdef MC_DEBUG
      std::cout << "Ordinal::samplingStep" << std::endl;
      std::cout << "ind: " << ind << std::endl;
      std::cout << "path_(ind).c_.size(): " << path_(ind).c_.size() << std::endl;
#endif
      if (augData_.misData_(ind).first == missing_) // if individual is completely missing, use samplePathForward instead of samplePath to accelerate computation
      {
        path_(ind).forwardSamplePath(mu_((*p_zi_)(ind)),
                                     pi_((*p_zi_)(ind)));
      }
      else // perform one round of Gibbs sampler for the designated individual
      {
        path_(ind).samplePath(mu_((*p_zi_)(ind)),
                              pi_((*p_zi_)(ind)),
                              sizeTupleBOS);
      }
      augData_.data_(ind) = path_(ind).c_(nbModalities_ - 2).e_(0); // copy of the data from last element of path to augData, which will be useful for the dataStatComputer_ to compute statistics
    }

    virtual std::string mStep()
    {
#ifdef MC_DEBUG
      std::cout << "Ordinal::mStep, idName_: " << idName_ << std::endl;
#endif

      std::string warnLog;

      pi_ = 0.; // pi_ parameter is reinitialized
      Vector<Real> nodePerClass(nbClass_); // total number of nodes in a class
      nodePerClass = 0.; // counting the number of individual per class
      for (int i = 0; i < nbInd_; ++i)
      {

#ifdef MC_DEBUG
        std::cout << "i: " << i << ", (*p_zi_)(i): " << (*p_zi_)(i) << ", path_(i).nbZ(): " << path_(i).nbZ() << std::endl;
#endif

        int indClass = (*p_zi_)(i);
        nodePerClass(indClass) += path_(i).nbNode_; // add all nodes of the individual
        pi_(indClass) += path_(i).nbZ(); // add only z = 1 nodes of the individual
      }

#ifdef MC_DEBUG
        std::cout << "indPerClass" << std::endl;
        std::cout << nodePerClass << std::endl;
#endif
      pi_ /= nodePerClass; // from accounts to frequencies of z -> maximum likelihood estimate of pi

      for (int k = 0; k < nbClass_; ++k) // reboot degenerate classes
      {
        if (pi_(k) < epsilon)
        {

#ifdef MC_DEBUG
          std::cout << "Ordinal::mStep, class " << k << " has 0-degenerated" << std::endl;
#endif
          sampleMuFreq(k, true);
        }

        if (pi_(k) > 1. - epsilon)
        {
#ifdef MC_DEBUG
          std::cout << "Ordinal::mStep, class " << k << " has 1-degenerated" << std::endl;
#endif
          for (int i = 0; i < nbInd_; ++i)
          {
            if ((*p_zi_)(i) == k)
            {
  #ifdef MC_DEBUG
              std::cout << "1-deg, k: " << k << ", i: " << i << ", augData_.data_(i): " << augData_.data_(i) << std::endl;
  #endif
            }
          }
//          std::stringstream sstm;
//          sstm << "Error in variable: " << idName_ << " with Ordinal model. A latent variable (the accuracy z) is uniformly 1 in class " << k << ". Try using a categorical model, "
//               << "if the number of modalities is not too high." << std::endl;
//          warnLog += sstm.str();
          pi_(k) = 1 - epsilon; // forced upper bound for debugging purposes
        }
      }

      Matrix<Real> logLik(nbClass_, nbModalities_);
      logLik = 0.;

      for (int mu = 0; mu < nbModalities_; ++mu) // mu obtained from maximization over all possible values
      {
        for (int i = 0; i < nbInd_; ++i)
        {
          int indClass = (*p_zi_)(i);
          logLik(indClass,
                 mu) += path_(i).computeLogProba(mu,
                                                 pi_(indClass));
        }
      }

      for (int k = 0; k < nbClass_; ++k)
      {
        int maxLik;
        logLik.row(k).maxCoeff(&maxLik);
        mu_(k) = maxLik;
      }

#ifdef MC_DEBUG
      std::cout << "End of Ordinal::mStep" << std::endl;
      std::cout << "logLik: " << std::endl;
      std::cout << logLik << std::endl;
      std::cout << "mu_" << std::endl;
      std::cout << mu_ << std::endl;
      std::cout << "pi_" << std::endl;
      std::cout << pi_ << std::endl;
#endif

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
          nbInd(samplePath.c_(nbModalities_ - 2).e_(0)) += 1.; // register the x value, for marginalization
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
      if (iteration == iterationMax)
      {
        dataStatComputer_.imputeData(ind); // impute the missing values using empirical mean
      }
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
#ifdef MC_DEBUG
      std::cout << "Ordinal::lnobservedProbability" << std::endl;
      std::cout << "observedProba_.rows(): " << observedProba_.rows() << ", observedProba_.cols(): " << observedProba_.cols() << std::endl;
      std::cout << "augData_.data_(i): " << augData_.data_(i) << std::endl;
#endif
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
#ifdef MC_DEBUG
      std::cout << "Ordinal::removeMissing" << std::endl;
#endif
      for (int i = 0; i < nbInd_; ++i)
      {
        path_(i).initPath(); // remove missing use to initialize learn, and should therefore use BOSPath::initPath() which is parameters free. Problem is that z = 0 everywhere.
      }

      for (int k = 0; k < nbClass_; ++k)
      {
        sampleMuFreq(k, false); // mu is sampled from modalities frequencies, without taking current mu value into account
      }

#ifdef MC_DEBUG
      std::cout << "prop: " << std::endl;
      std::cout << prop << std::endl;
      std::cout << "muIni:" << std::endl;
      std::cout << muIni << std::endl;
      std::cout << "piIni:" << std::endl;
      std::cout << piIni << std::endl;
      std::cout << "(*p_zi_):" << std::endl;
      std::cout << (*p_zi_) << std::endl;
#endif

      for (int i = 0; i < nbInd_; ++i)
      {
        for (int n = 0; n < nbGibbsIniBOS; ++n) // n rounds of Gibbs sampling to increase variability on z
        {
#ifdef MC_DEBUG
          if (idName_ == "Ordinal1" && i == 0)
          {
            std::cout << "n: " << n << std::endl;
            BOSDisplayPath(path_(i));
          }
#endif
          path_(i).samplePath(mu_((*p_zi_)(i)), // mu
                              piInitBOS, // pi
                              sizeTupleBOS); // sizeTuple
        }
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
    /**
     * Sample the next mu. Used in removeMissing to initialize, and in
     * @param k class for which the mode must be simulated
     * @param prohibitCurrentMu shall the current value of mu be forbidden, for example if it lead to degeneracy in the mStep ?
     * */
    void sampleMuFreq(int k,
                      bool prohibitCurrentMu)
    {
      Vector<Real> freqMod(nbModalities_); // frequencies of completed values for the current class
      for (int i = 0; i < nbInd_; ++i) // compute distribution of values
      {
        if ((*p_zi_)(i) == k) // among individuals inside the degenerate class
        {
          freqMod(augData_.data_(i)) += 1.; // completed values are used
        }
      }

#ifdef MC_DEBUG
      std::cout << "total freqMod: " << std::endl;
      std::cout << freqMod << std::endl;
#endif

      if (prohibitCurrentMu == true)
      {
        freqMod(mu_(k)) = 0.; // current mu value is prohibited as it lead to degeneracy
      }
      freqMod = freqMod / freqMod.sum(); // frequencies are renormalized to get a probability distribution

#ifdef MC_DEBUG
      std::cout << "effective freqMod: " << std::endl;
      std::cout << freqMod << std::endl;
#endif

      mu_(k) = multi_.sample(freqMod); // mu is sampled from this distribution
      pi_(k) = piInitBOS; // pi is set high enough to avoid an immediate degeneracy

      for (int i = 0; i < nbInd_; ++i) // have some samplePath calls to shake this up, and get some non zero z values inside the path
      {
        if ((*p_zi_)(i) == k)
        {
#ifdef MC_DEBUG
          std::cout << "0-deg, k: " << k << ", i: " << i << ", augData_.data_(i): " << augData_.data_(i) << std::endl;
#endif
          for (int n = 0; n < nbGibbsIniBOS; ++n) // same initialization than used in removeMisssing, to increase variability on z among individuals
          {
            path_(i).samplePath(mu_(k), // mu
                                pi_(k), // pi
                                sizeTupleBOS); // sizeTuple
          }
        }
      }
    }

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

    /** Sampler for reinitialization of parameters after degeneracy */
    MultinomialStatistic multi_;

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
