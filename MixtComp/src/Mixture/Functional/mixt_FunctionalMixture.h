/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2016

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
 *  Created on: June 20, 2016
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_FUNCTIONALMIXTURE
#define MIXT_FUNCTIONALMIXTURE

namespace mixt {

template<typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>
class FunctionalMixture : public IMixture {
  public:
    FunctionalMixture(Index indexMixture,
                      std::string const& idName,
                      Index nClass,
                      const Vector<Index>* p_zi,
                      const Vector<std::set<Index> >& classInd,
                      const DataHandler* p_handler,
                      DataExtractor* p_extractor,
                      const ParamSetter* p_paramSetter,
                      ParamExtractor* p_paramExtractor,
                      Real confidenceLevel) :
        IMixture(indexMixture,
                 idName),
        nInd_(0),
        nClass_(nClass),
        nSub_(0),
        nCoeff_(0),
        confidenceLevel_(confidenceLevel),
        p_zi_(p_zi),
        classInd_(classInd),
        p_handler_(p_handler),
        p_dataExtractor_(p_extractor),
        p_paramSetter_(p_paramSetter),
        p_paramExtractor_(p_paramExtractor) {
      class_.reserve(nClass_);
      for (Index k = 0; k < nClass_; ++k) {
        class_.emplace_back(vecInd_,
                            classInd_(k),
                            confidenceLevel_);
      }

      acceptedType_.resize(nb_enum_MisType_);
      acceptedType_ << true , // present_,
                       false, // missing_,
                       false, // missingFiniteValues_,
                       false, // missingIntervals_,
                       false, // missingLUIntervals_,
                       false; // missingRUIntervals
    }

    //** Dummy  constructor to check compilation */
    FunctionalMixture(const Vector<std::set<Index> >& classInd) :
      IMixture(0, "dummy"),
      nInd_(0),
      nClass_(0),
      nSub_(0),
      nCoeff_(0),
      confidenceLevel_(0.),
      p_zi_(NULL),
      classInd_(classInd),
      p_handler_(NULL),
      p_dataExtractor_(NULL),
      p_paramSetter_(NULL),
      p_paramExtractor_(NULL) {};

    void samplingStepCheck(Index i) {
      class_[(*p_zi_)(i)].samplingStepCheck(i);
    };

    void samplingStepNoCheck(Index i) {
      class_[(*p_zi_)(i)].samplingStepNoCheck(i);
    };

    Index checkSampleCondition(std::string* warnLog = NULL) const {
      for (Index k = 0; k < nClass_; ++k) {
        std::string tempLog;
        Index sampleOK = class_[k].checkSampleCondition();

        if (sampleOK == 0) {
          if (warnLog != NULL) {
            std::stringstream sstm;
            sstm << "Error in variable: " << idName_ << " with Functional model. There are not enough timesteps in at least one subregression "
                 << "in at least one class." << std::endl;
            *warnLog += sstm.str();
          }

          return 0;
        }
      }

      return 1;
    }

    void mStep(EstimatorType bias) {
      for (Index k = 0; k < nClass_; ++k) {
        class_[k].mStep();
      }
    };

    void storeSEMRun(Index iteration,
                     Index iterationMax) {
      for (Index k = 0; k < nClass_; ++k) {
        class_[k].sampleParam(iteration, iterationMax);
      }

      if (iteration == iterationMax) {
        for (Index k = 0; k < nClass_; ++k) {
          class_[k].setExpectationParam();
        }
      }
    };

    void storeGibbsRun(Index i,
                       Index iteration,
                       Index iterationMax) {
      // nothing until missing data is supported
    };

    Real lnCompletedProbability(Index i, Index k) {
      return vecInd_(i).lnCompletedProbability(class_[k].alpha(),
                                               class_[k].beta (),
                                               class_[k].sd   ());
    }

    Real lnObservedProbability(Index i, Index k)  {
      return vecInd_(i).lnObservedProbability(class_[k].alpha(),
                                              class_[k].beta (),
                                              class_[k].sd   ());
    }

    Index nbFreeParameter() const {
      return nClass_ * (  nSub_ * 2// alpha
                        + nSub_ * nCoeff_// beta
                        + nSub_ ); // sigma
    }

    void writeParameters() const {
      std::stringstream sstm;

      for (int k = 0; k < nClass_; ++k) {
        sstm << "Class: " << k << std::endl;
        sstm << "alpha: " << std::endl << class_[k].alpha() << std::endl;
        sstm << "beta: " << std::endl << class_[k].beta() << std::endl;
        sstm << "sigma: " << itString(class_[k].sd()) << std::endl;
      }

      std::cout << sstm.str() << std::endl;
    };

    std::string setDataParam(RunMode mode) {
      std::string warnLog;
      Vector<std::string> dataStr;

      warnLog += p_handler_->getData(idName(), // get the raw vector of strings
                                     dataStr,
                                     nInd_,
                                     paramStr_);

      if (mode == prediction_) { // prediction mode, linearized versions of the parameters are fetched, and then distributed to the classes
        Vector<Real> alpha, beta, sd, pi;

        std::string dummyStr;
        p_paramSetter_->getParam(idName_,
                                 "alpha",
                                 alpha,
                                 dummyStr); // alpha is not parameterized by anything, only order one polynomials are used in the logistical regression

        p_paramSetter_->getParam(idName_,
                                 "beta",
                                 beta,
                                 paramStr_);

        p_paramSetter_->getParam(idName_,
                                 "sd",
                                 sd,
                                 dummyStr);

        p_paramSetter_->getParam(idName_,
                                 "pi",
                                 pi,
                                 dummyStr);

        Matrix<Real> alphaCurr(nSub_, 2);
        Matrix<Real> betaCurr(nSub_, nCoeff_);
        Vector<Real> sdCurr(nSub_);
        for (Index k = 0; k < nClass_; ++k) {
          for (Index s = 0; s < nSub_; ++s) {
            for (Index c = 0; c < 2; ++c) {
              alphaCurr(s, c) = alpha(k * nSub_ * 2 + s * 2 + c);
            }
          }

          for (Index s = 0; s < nSub_; ++s) {
            for (Index c = 0; c < nCoeff_; ++c) {
              betaCurr(s, c) = alpha(k * nSub_ * nCoeff_ + s * nCoeff_ + c);
            }
          }

          for (Index s = 0; s < nSub_; ++s) {
            sdCurr(s) = k * nSub_ + s;
          }

          class_[k].setParam(alphaCurr,
                             betaCurr,
                             sdCurr);
          class_[k].setParamStorage();
        }
      }

      // get the value of nSub_ and nCoeff_ by parsing paramStr_
      std::string paramReStr = std::string("nSub: *") + strPositiveInteger + std::string(", nCoeff: *") + strPositiveInteger;
      boost::regex paramRe(paramReStr);
      boost::smatch matches;
      if (boost::regex_match(paramStr_, matches, paramRe)) { // value is present
        nSub_ = str2type<Index>(matches[1].str());
        nCoeff_ = str2type<Index>(matches[2].str());

        for (Index k = 0; k < nClass_; ++k) { // call setSize on each class
          class_[k].setSize(nSub_, nCoeff_);
        }
      }
      else {
        std::stringstream sstm;
        sstm << "Variable: " << idName_ << " has no parameter description. This description is required, and must take the form "
             << "\"nSub: x, nCoeff: y\"" << std::endl;
        warnLog += sstm.str();
      }

      warnLog += parseFunctionalStr(nSub_,
                                    nCoeff_,
                                    dataStr, // convert the vector of strings to ranks
                                    vecInd_);
      warnLog += checkMissingType();
      if (warnLog.size() > 0) {
        return warnLog;
      }

      // datastat will be setup here when partially observed value will be supported
      // should lnObservedProbability be computed here in prediction, as it is done for Ordinal and Rank ?

      return warnLog;
    }

    void exportDataParam() const { // linearize and format the information provided by each class, and send it to the usual extractors, nothing fancy here ...

      // export the missing values here, when they will be support for them

      Index sizeClassAlpha = nSub_ * 2;
      Index sizeClassBeta  = nSub_ * nCoeff_;
      Index sizeClassSd = nSub_;

      Index nObs = class_[0].alphaParamStat().getLogStorage().cols();

      Matrix<Real> alphaStat(nClass_ * sizeClassAlpha, 3); // linearized and concatenated version of alpha
      Matrix<Real> betaStat (nClass_ * sizeClassBeta , 3);
      Matrix<Real> sdStat   (nClass_ * sizeClassSd   , 3);

      Matrix<Real> alphaLog(nClass_ * sizeClassAlpha, nObs); // linearized and concatenated version of alpha
      Matrix<Real> betaLog (nClass_ * sizeClassBeta , nObs);
      Matrix<Real> sdLog   (nClass_ * sizeClassSd   , nObs);


      for (Index k = 0; k < nClass_; ++k) {
        alphaStat.block(k * sizeClassAlpha, 0, sizeClassAlpha, 3   ) = class_[k].alphaParamStat().getStatStorage();
        alphaLog .block(k * sizeClassAlpha, 0, sizeClassAlpha, nObs) = class_[k].alphaParamStat().getLogStorage();

        betaStat.block(k * sizeClassBeta, 0, sizeClassBeta, 3   ) = class_[k].betaParamStat().getStatStorage();
        betaLog .block(k * sizeClassBeta, 0, sizeClassBeta, nObs) = class_[k].betaParamStat().getLogStorage();

        sdStat.block(k * sizeClassSd, 0, sizeClassSd, 3   ) = class_[k].sdParamStat().getStatStorage();
        sdLog .block(k * sizeClassSd, 0, sizeClassSd, nObs) = class_[k].sdParamStat().getLogStorage();
      }

      p_paramExtractor_->exportParam(indexMixture_,
                                     idName_,
                                     "alpha",
                                     alphaStat,
                                     alphaLog,
                                     alphaParamNames(),
                                     confidenceLevel_,
                                     paramStr_);

      p_paramExtractor_->exportParam(indexMixture_,
                                     idName_,
                                     "beta",
                                     betaStat,
                                     betaLog,
                                     betaParamNames(),
                                     confidenceLevel_,
                                     paramStr_);

      p_paramExtractor_->exportParam(indexMixture_,
                                     idName_,
                                     "sd",
                                     sdStat,
                                     sdLog,
                                     sdParamNames(),
                                     confidenceLevel_,
                                     paramStr_);
    };

    void removeMissing(initParam algo) {
      for (Vector<Function>::iterator it  = vecInd_.begin(),
                                      itE = vecInd_.end();
           it != itE;
           ++it) {
        it->removeMissing();
      }
    };

  private:
    std::string checkMissingType() {
      std::string warnLog;
      // to be populated with checks. Each Function object must have a checkMissingType
      return warnLog;
    }

    std::vector<std::string> alphaParamNames() const {
      std::vector<std::string> names(nClass_ * nSub_ * 2);
      for (Index k = 0; k < nClass_; ++k) {
        for (Index s = 0; s < nSub_; ++s) {
          std::stringstream sstm0;
          sstm0 << "k: " << k << ", s: " << s << ", alpha0";
          names[k * nSub_ * 2 + s * 2    ] = sstm0.str();

          std::stringstream sstm;
          sstm << "k: " << k << ", s: " << s << ", alpha1";
          names[k * nSub_ * 2 + s * 2 + 1] = sstm.str();
        }
      }
      return names;
    }

    std::vector<std::string> betaParamNames() const {
      std::vector<std::string> names(nClass_ * nSub_ * nCoeff_);
      for (Index k = 0; k < nClass_; ++k) {
        for (Index s = 0; s < nSub_; ++s) {
          for (Index c = 0; c < nCoeff_; ++c) {
            std::stringstream sstm;
            sstm << "k: " << k << ", s: " << s << ", c: " << c;
            names[k * nSub_ * nCoeff_ + s * nCoeff_ + c] = sstm.str();
          }
        }
      }
      return names;
    }

    std::vector<std::string> sdParamNames() const {
      std::vector<std::string> names(nClass_ * nSub_);
      for (Index k = 0; k < nClass_; ++k) {
        for (Index s = 0; s < nSub_; ++s) {
          std::stringstream sstm;
          sstm << "k: " << k << ", s: " << s;
          names[k * nSub_ + s] = sstm.str();
        }
      }
      return names;
    }

    Index nInd_;
    Index nClass_;
    Index nSub_;
    Index nCoeff_;
    Real confidenceLevel_;

    /** Data */
    Vector<Function> vecInd_;

    const Vector<Index>* p_zi_;
    const Vector<std::set<Index> >& classInd_;

    const DataHandler* p_handler_;
    DataExtractor* p_dataExtractor_;
    const ParamSetter* p_paramSetter_;
    ParamExtractor* p_paramExtractor_;

    std::vector<FunctionalClass> class_;

    Vector<bool> acceptedType_;

    std::string paramStr_;
};

} // namespace mixt

#endif // MIXT_FUNCTIONALMIXTURE
