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
        nClass_(nClass),
        nSub_(0),
        nInd_(0),
        p_zi_(p_zi),
        classInd_(classInd),
        p_handler_(p_handler),
        p_dataExtractor_(p_extractor),
        p_paramSetter_(p_paramSetter),
        p_paramExtractor_(p_paramExtractor),
        confidenceLevel_(confidenceLevel) {
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

    void samplingStepCheck(Index i) {
      samplingStepNoCheck(i); // until check conditions are properly defined to avoid problems on every parameters
    };

    void samplingStepNoCheck(Index i) {
      vecInd_(i).sampleW(class_[(*p_zi_)(i)].alpha(),
                         class_[(*p_zi_)(i)].beta (),
                         class_[(*p_zi_)(i)].sd   ());
    };

    Index checkSampleCondition(std::string* warnLog = NULL) const {return 0;}

    void mStep(EstimatorType bias) {
      for (std::vector<FunctionalClass>::iterator it  = class_.begin(),
                                                  itE = class_.end();
           it != itE;
           ++it) {
        it->mStep();
      }
    };

    void storeSEMRun(Index iteration,
                     Index iterationMax) {};

    void storeGibbsRun(Index i,
                       Index iteration,
                       Index iterationMax) {};

    Real lnCompletedProbability(Index i, Index k) {
      return vecInd_(i).lnCompletedProbability(class_[(*p_zi_)(i)].alpha(),
                                               class_[(*p_zi_)(i)].beta (),
                                               class_[(*p_zi_)(i)].sd   ());
    }

    Real lnObservedProbability(Index i, Index k)  {
      return vecInd_(i).lnObservedProbability(class_[(*p_zi_)(i)].alpha(),
                                              class_[(*p_zi_)(i)].beta (),
                                              class_[(*p_zi_)(i)].sd   ());
    }

    Index nbFreeParameter() const {return 0;}

    void writeParameters() const {};

    std::string setDataParam(RunMode mode) {
      // once the number of subregressions is known, loop over the class_ and apply setSize
      // also loop on the individuals and call Function::setSize
      // do not forget to call Function::computeVandermonde once t vectors are known

      std::string warnLog;
      Vector<std::string> dataStr;

      warnLog += p_handler_->getData(idName(), // get the raw vector of strings
                                     dataStr,
                                     nInd_,
                                     paramStr_);

      // get the value of nSub by parsing paramStr_

      warnLog += parseFunctionalStr(nSub_,
                                    dataStr, // convert the vector of strings to ranks
                                    vecInd_);

      warnLog += checkMissingType();

      if (warnLog.size() > 0) {
        return warnLog;
      }

      if (mode == prediction_) { // prediction mode, linearized versions of the parameters are fetched, and then distributed to the classes
        Vector<Real> alphaLinearized;
        p_paramSetter_->getParam(idName_, // parameters are set using results from previous run
                                 "alpha",
                                 alphaLinearized);

//        p_paramSetter_->getParam(idName_, // parameters are set using results from previous run
//                                 "pi",
//                                 pi_);

        // call setParam here, to set everything
      }

      // datastat will be setup here when partially observed value will be supported

      return warnLog;
    }

    void exportDataParam() const {
      // linearize and format the information provided by each class, and send it to the usual extractors, nothing fancy here ...
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
      // to be populated with checks
      return warnLog;
    }

    Index nInd_;
    Index nClass_;
    Index nSub_;
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
