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

#include <algorithm>

#include "../Data/mixt_AugmentedData.h"
#include "../Data/mixt_ConfIntDataStat.h"
#include "../Param/mixt_ConfIntParamStat.h"
#include "../mixt_IMixture.h"
#include "mixt_BOSPath.h"

namespace mixt
{

enum checkSample {
  checkZ_,
  noCheckZ_
};

template<typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>
class Ordinal : public IMixture
{
  public:

    /** constructor **/
    Ordinal(int indexMixture,
            std::string const& idName,
            int nbClass,
            Vector<int> const* p_zi,
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
      nbModality_(0),
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
      p_handler_(p_handler),
      p_dataExtractor_(p_extractor),
      p_paramSetter_(p_paramSetter),
      p_paramExtractor_(p_paramExtractor)
    {}

    /* Debug constructor with direct data set */
    Ordinal(int nbClass,
            int nbInd,
            int nbModalities,
            const Vector<int>* p_zi,
            const Vector<std::set<int> >& classInd,
            int mu,
            Real pi) :
        IMixture(0,
                 "dummy"),
        p_zi_(p_zi),
        classInd_(classInd),
        nbClass_(nbClass),
        nbModality_(nbModalities),
        nbInd_(nbInd),
        mu_(nbClass, mu),
        pi_(nbClass, pi),
        dataStatComputer_(augData_,
                          1.),
        muParamStatComputer_(mu_,
                             1.),
        piParamStatComputer_(pi_,
                             1.)
    {
      path_.resize(nbInd);
      for (int i = 0; i < nbInd; ++i) // initialization of the paths
      {
        path_(i).setInit(0, nbModalities - 1);
        path_(i).setEnd (0, nbModalities - 1); // no constraint on values
        path_(i).initPath(); // random init, with uniform z = 0

        for (int n = 0; n < nbGibbsIniBOS; ++n)
        {
          path_(i).samplePath(mu,
                              pi,
                              sizeTupleBOS,
                              allZAuthorized_);
        }
      }
    }

    const Vector<int>& mu() const {return mu_;}
    const Vector<Real>& pi() const {return pi_;}

    std::string setDataParam(RunMode mode) {
      std::string warnLog;

      warnLog += p_handler_->getData(idName(),
                                     augData_,
                                     nbInd_,
                                     paramStr_,
                                     -minModality); // ordinal data are modalities, offset enforces 0-based encoding through the whole mixture

      if (warnLog.size() > 0) {
        return warnLog;
      }

      augData_.computeRange();

      std::string missingLog = augData_.checkMissingType(acceptedType()); // check if the missing data provided are compatible with the model

      if(missingLog.size() > 0) { // check on the missing values description
        std::stringstream sstm;
        sstm << "Variable " << idName() << " with Ordinal model has a problem with the descriptions of missing values.\n" << missingLog;
        warnLog += sstm.str();
      }
      else if (augData_.dataRange_.min_ < 0) { // modality encoding is 0-based
        std::stringstream sstm;
        sstm << "Variable: " << idName() << " is described by an Ordinal model which requires a minimum value of 0 in either provided values or bounds."
             << " The minimum value currently provided is : " << augData_.dataRange_.min_ << std::endl;
        warnLog += sstm.str();
      }
      else { // minimum value requirements have been met, whether the mode is learning or prediction
        if (mode == learning_) {
          nbModality_ = augData_.dataRange_.max_ + minModality; // since an offset has been applied during getData, modalities are 0-based
        }
        else { // prediction mode
          setParam(); // set nbModalities_, mu_ and pi_ using p_paramSetter_

          muParamStatComputer_.setParamStorage();
          piParamStatComputer_.setParamStorage();

          nbModality_ = augData_.dataRange_.range_;

          if (nbModality_ - 1 < augData_.dataRange_.max_) {
            std::stringstream sstm;
            sstm << "Variable: " << idName() << " requires a maximum value of " << minModality + nbModality_ - 1
                 << " for the data during prediction. This maximum value corresponds to the maximum value used during the learning phase."
                 << " The maximum value in the data provided for prediction is : " << minModality + augData_.dataRange_.max_ << std::endl;
            warnLog += sstm.str();
          }

          computeObservedProba(); // parameters are know, so logProba can be computed immediately
        }

        if (nbModality_ <= 2) {
          std::stringstream sstm;
          sstm << "Variable: " << idName() << " requires a minimum of 3 modalities. If you have less modalities than that, you must use a "
               << "Categorical model."<< std::endl;
          warnLog += sstm.str();
        }

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
      path_.resize(nbInd_);

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
                               "muPi",
                               param);
      mu_.resize(nbClass_);
      pi_.resize(nbClass_);
      for (int k = 0; k < nbClass_; ++k)
      {
        mu_(k) = param(2 * k    );
        pi_(k) = param(2 * k + 1);
      }
    }

    virtual void samplingStepCheck(int ind)
    {
#ifdef MC_DEBUG
      std::cout << "Ordinal::samplingStep" << std::endl;
      std::cout << "ind: " << ind << std::endl;
      std::cout << "path_(ind).c_.size(): " << path_(ind).c_.size() << std::endl;
#endif

      GibbsSampling(ind,
                    mu_((*p_zi_)(ind)),
                    pi_((*p_zi_)(ind)),
                    checkZ_); // in samplingStepCheck, each sampling must result in a valid state
      copyToData(ind);
    }

    virtual void samplingStepNoCheck(int ind)
    {
#ifdef MC_DEBUG
      std::cout << "Ordinal::samplingStep" << std::endl;
      std::cout << "ind: " << ind << std::endl;
      std::cout << "path_(ind).c_.size(): " << path_(ind).c_.size() << std::endl;
#endif

      GibbsSampling(ind,
                    mu_((*p_zi_)(ind)),
                    pi_((*p_zi_)(ind)),
                    noCheckZ_); // in samplingStepCheck, allZOneAuthorized, no check on number of z values during samplingStepNoCheck
      copyToData(ind);
    }

    virtual void mStep(EstimatorType bias) {
      mStepPi(bias);
      mStepMu();
    }

    /** storeSEMRun sets new parameters at the last iteration of the SEM, before the Gibbs. */
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
#ifdef MC_DEBUG
      std::cout << "Ordinal::computeObservedProba, mu: " << itString(mu_) << ", pi_: " << itString(pi_) << std::endl;
#endif

      observedProba_.resize(nbClass_, nbModality_);
      BOSPath path; // BOSPath used for the various samplings
      path.setInit(0, nbModality_ - 1);
      for (int k = 0; k < nbClass_; ++k)
      {
        RowVector<Real> nbInd(nbModality_); // observed frequencies
        path.setEnd(k, k);
        nbInd = 0;
        for (int i = 0; i < nbSampleObserved; ++i)
        {
          path.forwardSamplePath(mu_(k), // complete the individual
                                 pi_(k),
                                 allZAuthorized_); // to estimate probability distribution, all z can be sampled to 1
          nbInd(path.c()(nbModality_ - 2).e_(0)) += 1.; // register the x value, for marginalization
        }
        observedProba_.row(k) = nbInd / Real(nbSampleObserved);
      }

#ifdef MC_DEBUG
      std::cout << "Ordinal::computeObservedProba, out" << std::endl;
#endif
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
      return nbClass_; // only the continuous pi_ parameter is taken into account, not the discrete mu_ parameter
    }

    virtual void writeParameters() const
    {
      std::stringstream sstm;
      for (int k = 0; k < nbClass_; ++k)
      {
        sstm << "Class: " << k << std::endl;
        sstm << "mu: " << mu_(k) << std::endl;
        sstm << "pi: " << pi_(k) << std::endl;
      }

#ifdef MC_VERBOSE
      std::cout << sstm.str() << std::endl;
#endif
    }

    virtual void exportDataParam() const
    {
#ifdef MC_DEBUG
      std::cout << "MixtureBridge: exportDataParam, idName(): " << idName() << std::endl;
#endif

      int nbColStat = muParamStatComputer_.getStatStorage().cols();
      Matrix<Real> paramStatStorage(2 * nbClass_, nbColStat); // aggregates both mu and pi values
      for (int j = 0; j < nbColStat; ++j) {
        for (int k = 0; k < nbClass_; ++k) {
          paramStatStorage(2 * k    , j) = muParamStatComputer_.getStatStorage()(k, j);
          paramStatStorage(2 * k + 1, j) = piParamStatComputer_.getStatStorage()(k, j);
        }
      }

      int nbColLog = muParamStatComputer_.getLogStorage().cols();
      Matrix<Real> paramLogStorage (2 * nbClass_, nbColLog); // aggregates both mu and pi logs
      for (int j = 0; j < nbColLog; ++j) {
        for (int k = 0; k < nbClass_; ++k) {
          paramLogStorage (2 * k    , j) = muParamStatComputer_.getLogStorage ()(k, j);
          paramLogStorage (2 * k + 1, j) = piParamStatComputer_.getLogStorage ()(k, j);
        }
      }

      p_dataExtractor_->exportVals(indexMixture_,
                                   idName(),
                                   augData_,
                                   dataStatComputer_.getDataStatStorage()); // export the obtained data using the DataExtractor
      p_paramExtractor_->exportParam(idName(),
                                     "muPi",
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

    /** removeMissing is usually called at the beginning of the SEMStrategy. All data are completed by sampling using dummy parameters, since no mStep has
     * been performed. mStep in turns requires complete data, hence the need to bootstrap the process in some way. A similar initialization is to be found
     * in the rank model, who also describe each observation with latent variables. Since BOSPath::initPath initializes all BOSPath with z = 0 to enforce
     * validity, it is necessary to perform nbGibbsIniBOS iterations of GibbsSampling with a pi at piInitBOS to generate variability in z. This will ensure
     * that pi will not likely be equal to 0 at the first mStep estimation. In any case, should this occur, the initialization will ultimately be
     * rejected by a call to checkSampleCondition. */
    void removeMissing(initParam algo) {
      Vector<int> tempMu(nbClass_);

      if (algo == initParam_) {
        for (int k = 0; k < nbClass_; ++k) {
          tempMu(k) = sampleMuFreq(k); // mu is sampled from modalities frequencies, without taking current mu value into account
        }
      }
      else if (algo == keepParam_) {
        for (int k = 0; k < nbClass_; ++k) {
          tempMu(k) = mu_(k); // during the Gibbs phase, mu is already known and can be used for initialization
        }
      }

      for (int i = 0; i < nbInd_; ++i) {
        path_(i).initPath(); // remove missing use to initialize learn, and should therefore use BOSPath::initPath() which is parameters free. Problem is that z = 0 everywhere.
        for (int n = 0; n < nbGibbsIniBOS; ++n) { // n rounds of Gibbs sampling to increase variability on z
          GibbsSampling(i,
                        tempMu((*p_zi_)(i)),
                        piInitBOS,
                        noCheckZ_);
        }

        copyToData(i);
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

    int checkSampleCondition(std::string* warnLog = NULL) const {
      for (int k = 0; k < nbClass_; ++k) {
        bool allZ0 = true;
        bool allZ1 = true; // are all z = 0 or all z = 1 in the current class ?
        for (std::set<int>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
             it != itE;
             ++it) {
          switch(path_(*it).allZ()) {
            case allZ0_: {
              allZ1 = false;
            }
            break;

            case allZ1_: {
              allZ0 = false;
            }
            break;

            case mixZ0Z1_: {
              allZ0 = false;
              allZ1 = false;
            }
            break;
          }

          if (allZ0 == false && allZ1 == false) { // there is enough variability on z in this class to ensure that pi will be estimated inside the open support
            goto itKEnd;
          }
        }

        if (warnLog != NULL) {
          std::stringstream sstm;
          sstm << "Error in variable: " << idName_ << " with Ordinal model. A latent variable (the accuracy z) is uniformly 0 or 1 in at least one class. "
               << "If the number of modalities is quite low, try using a categorical model instead." << std::endl;
          *warnLog += sstm.str();
        }

        return 0;

        itKEnd:;
      }

      return 1;
    }

  private:
    /**
     * Sample the next mu for the class k and initialize the individuals in the class using it.
     * @param k class for which the mode must be simulated
     * @param prohibitCurrentMu shall the current value of mu be forbidden, for example if it lead to degeneracy in the mStep ?
     * */
    int sampleMuFreq(int k)
    {
      Vector<Real> freqMod(nbModality_, 0.); // frequencies of completed values for the current class
      for (int i = 0; i < nbInd_; ++i) // compute distribution of values
      {
        if ((*p_zi_)(i) == k) // among individuals inside the degenerate class
        {
          freqMod(augData_.data_(i)) += 1.; // completed values are used
        }
      }

      Real sum = freqMod.sum();
      if (sum > epsilon)
      {
        freqMod = freqMod / sum;
      }
      else  // this is just to avoid a crash, as empty class are forbidden and will be detected later for resampling
      {
        freqMod = 1. / Real(nbModality_);
      }

      return multi_.sample(freqMod); // mu is sampled from this distribution
    }

    /**
     * Perform one iteration of Gibbs sampling, insuring proper implementation of allZOneAuthorized flag
     *
     * @param sampleAZ indicate if the condition all z = 0 or all z = 1 is authorized for the class containing ind
     * */
    void GibbsSampling(int ind,
                       int mu,
                       Real pi,
                       checkSample globalCheckSample) {
      zCondition zCond = allZAuthorized_; // by default, everything will be authorized for the z sampled value

      if (globalCheckSample == checkZ_) { // if all z = 0 is not authorized and all other individuals already have z = 0, then current individual can not have z = 0
        bool allOtherZ0 = true; // are all z in other individuals equal to 0 ?
        bool allOtherZ1 = true; // are all z in other individuals equal to 0 ?

        int currClass = (*p_zi_)(ind);
        for (std::set<int>::const_iterator it = classInd_(currClass).begin(), itE = classInd_(currClass).end();
             it != itE;
             ++it) {
          if (*it != ind) { // check is performed on all in the class but the current ind
            switch (path_(*it).allZ()) {
              case allZ0_: {
                allOtherZ1 = false; // ... current individual is authorized to have all its z = 1
              }
              break;

              case allZ1_: {
                allOtherZ0 = false; // ... current individual is authorized to have all its z = 0
              }
              break;

              case mixZ0Z1_: {
                allOtherZ0 = false;
                allOtherZ1 = false;
              }
            }

            if (allOtherZ0 == false && allOtherZ1 == false) { // all values are authorized for current individual, stop checking
              goto endTest;
            }
          }
        }

        if (allOtherZ0 == true) {
          zCond = allZ0Forbidden_;
        }
        else if (allOtherZ1 == true) {
          zCond = allZ1Forbidden_;
        }

        endTest:;
      }

      if (augData_.misData_(ind).first == missing_) { // if individual is completely missing, use samplePathForward instead of samplePath to accelerate computation
        path_(ind).forwardSamplePath(mu,
                                     pi,
                                     zCond);
      }
      else { // perform one round of Gibbs sampler for the designated individual
        path_(ind).samplePath(mu,
                              pi,
                              sizeTupleBOS,
                              zCond);
      }
    }

    void mStepMu() {
      Matrix<Real> logLik(nbClass_, nbModality_, 0.);
      for (int i = 0; i < nbInd_; ++i) {
        int currClass = (*p_zi_)(i);
        Real currPi = pi_(currClass);
        RowVector<Real> probaInd(nbModality_);
        for (int mu = 0; mu < nbModality_; ++mu) { // mu obtained from maximization over all possible values
          probaInd(mu) = path_(i).computeLogProba(mu,
                                                  currPi);
        }
        logLik.row(currClass) += probaInd;
      }

      for (int k = 0; k < nbClass_; ++k) {
        RowVector<Real> proba;
        proba.logToMulti(logLik.row(k));
        mu_(k) = multi_.sample(proba);
      }
    }

    /**
     * Estimation of pi by maximum likelihood in all classes
     * */
    void mStepPi(EstimatorType bias) {
      pi_ = 0.; // pi_ parameter is reinitialized

      Vector<Real> nodePerClass(nbClass_, 0.); // total number of nodes in each class
      Vector<Real> zPerClass   (nbClass_, 0.); // total of nodes with z = 1 in each class

      for (int i = 0; i < nbInd_; ++i) {
        int indClass = (*p_zi_)(i);
        zPerClass   (indClass) += path_(i).nbZ()   ; // add only z = 1 nodes of the individual
        nodePerClass(indClass) += path_(i).nbNode(); // add all nodes of the individual
      }

      pi_ = zPerClass / nodePerClass; // from accounts to frequencies of z -> maximum likelihood estimate of pi

      if (bias == biased_) {
        for (int k = 0; k < nbClass_; ++k) {
          pi_(k) = std::max(epsilon, pi_(k)      );
          pi_(k) = std::min(pi_(k) , 1. - epsilon);
        }
      }
    }

    /**
     * Estimation of pi by maximum likelihood in a particular class
     *
     * @param k class for which the parameter pi should be estimated
     * */
    void mStepPiK(int k)
    {
      pi_(k) = 0.; // pi_ parameter is reinitialized for the current class

      Real nodePerClass = 0.; // total number of nodes in class k
      Real zPerClass    = 0.; // total of nodes with z = 1 in class k

      for (int i = 0; i < nbInd_; ++i)
      {
        if ((*p_zi_)(i) == k)
        {
          zPerClass    += path_(i).nbZ()  ; // add only z = 1 nodes of the individual
          nodePerClass += path_(i).nbNode_; // add all nodes of the individual
        }
      }

      pi_(k) = zPerClass / nodePerClass; // from accounts to frequencies of z -> maximum likelihood estimate of pi
    }

    /** update the data using the last segment in c_. to be used after sampling in the BOSPath. */
    void copyToData(int ind)
    {
      augData_.data_(ind) = path_(ind).c()(nbModality_ - 2).e_(0); // copy of the data from last element of path to augData, which will be useful for the dataStatComputer_ to compute statistics
    }

    /** Pointer to the zik class label */
    Vector<int> const* p_zi_;

    /** Reference to a vector containing in each element a set of the indices of individuals that
     * belong to this class. Can be passed as an alternative to zi_ to a subtype of IMixture. */
    const Vector<std::set<int> >& classInd_;

    /** Number of classes */
    int nbClass_;

    /** Number of modalities */
    int nbModality_;

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
