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
 *  Created on: September 23, 2015
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_RANKMIXTURE_H
#define MIXT_RANKMIXTURE_H

#include "mixt_RankClass.h"
#include "mixt_RankLikelihood.h"
#include "mixt_RankParser.h"
#include "mixt_RankStat.h"

namespace mixt
{
/** RankMixture contains an array of RankClass. Each RankClass will have the responsibility to perform
 * estimation of parameters and computation of the probability of individuals that belong to it. */
template<typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>
class RankMixture : public IMixture
{
  public:
    typedef std::pair<MisType, std::vector<int> > MisVal;

    RankMixture(int indexMixture,
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
        nbClass_(nbClass),
        nbInd_(0),
        nbPos_(0),
        facNbMod_(0.),
        p_zi_(p_zi),
        classInd_(classInd),
        p_handler_(p_handler),
        p_dataExtractor_(p_extractor),
        p_paramSetter_(p_paramSetter),
        p_paramExtractor_(p_paramExtractor),
        confidenceLevel_(confidenceLevel),
        mu_(nbClass),
        pi_(nbClass),
        piParamStat_(pi_,
                     confidenceLevel)
    {
      class_    .reserve(nbClass);
      muParamStat_.reserve(nbClass);
      for (int k = 0; k < nbClass; ++k)
      {
        class_.emplace_back(data_,
                            classInd_(k),
                            mu_(k),
                            pi_(k)); // doing that means that classInd_, mu_ and pi_ must not be resized in order to avoid incorrect behaviour at runtime
        muParamStat_.emplace_back(mu_(k),
                                  confidenceLevel);
      }

      acceptedType_.resize(nb_enum_MisType_);
      acceptedType_ << true,   // present_,
                       true,  // missing_,
                       true,  // missingFiniteValues_,
                       false,  // missingIntervals_,
                       false,  // missingLUIntervals_,
                       false; // missingRUIntervals
    }

    void samplingStepCheck(int ind) {
      gCondition gCond = allGAuthorized_; // by default, everything will be authorized for the number of correct comparisons

      bool allOtherGO = true; // are all comparisons in other individuals correct ?
      bool allOtherGA = true; // are all comparisons in other individuals incorrect ?

      int currClass = (*p_zi_)(ind);
      for (std::set<int>::const_iterator it = classInd_(currClass).begin(), itE = classInd_(currClass).end();
           it != itE;
           ++it) {
        if (*it != ind) { // check is performed on all in the class but the current ind
          int A, G;
          data_(ind).AG(mu_((*p_zi_)(*it)), A, G);

          if (A == 0) {
            allOtherGA = false;
          }
          else if (A == G) {
            allOtherGO = false;
          }
          else {
            allOtherGA = false;
            allOtherGO = false;
          }

          if (allOtherGO == false && allOtherGA == false) { // all values are authorized for current individual, stop checking
            goto endTest; // goto is overkill is here, but is present to keep the same structure as in BOSPath::GibbsSampling, for example
          }
        }
      }

      if (allOtherGO == true) {
        gCond = Geq0Forbidden_;
      }
      else if (allOtherGA == true) {
        gCond = GeqAForbidden_;
      }

      endTest:;

      data_(ind).sampleY(mu_((*p_zi_)(ind)),
                       pi_((*p_zi_)(ind)),
                       gCond);
      data_(ind).sampleX(mu_((*p_zi_)(ind)),
                       pi_((*p_zi_)(ind)),
                       gCond);
    }

    void samplingStepNoCheck(int i) {
        data_(i).sampleY(mu_((*p_zi_)(i)),
                         pi_((*p_zi_)(i)),
                         allGAuthorized_);
        data_(i).sampleX(mu_((*p_zi_)(i)),
                         pi_((*p_zi_)(i)),
                         allGAuthorized_);
    }

    /** Note that MixtureComposer::checkNbIndPerClass already enforce that there is at least one observation per class */
    int checkSampleCondition(std::string* warnLog = NULL) const {
      for (int k = 0; k < nbClass_; ++k) {
        bool Geq0 = true; // are all comparisons incorrect ? This would lead to pi = 1 in a maximum likelihood estimation and is to be avoided.
        bool GeqA = true; // are all comparisons correct ? This would lead to pi = 1 in a maximum likelihood estimation and is to be avoided.

        for (std::set<int>::const_iterator it = classInd_(k).begin(), itE = classInd_(k).end();
             it != itE;
             ++it) {
          int A, G;
          data_(*it).AG(mu_(k), A, G);
          if (A == 0) {
            GeqA = false;
          }
          else if (A == G) {
            Geq0 = false;
          }
          else {
            GeqA = false;
            Geq0 = false;
          }

          if (Geq0 == false && GeqA == false) { // there is enough variability on the validities of comparisons in this class to ensure that pi will be estimated inside the open support
            goto itKEnd;
          }
        }

        if (warnLog != NULL) {
          std::stringstream sstm;
          sstm << "Error in variable: " << idName_ << " with Rank model. The comparisons are uniformly correct or invalid in at least one class. "
               << "If the number of different observed values is quite low, try using a categorical model instead." << std::endl;
          *warnLog += sstm.str();
        }

        return 0;

        itKEnd:;
      }

      return 1;
    }

    /** One of the peculiarity of the ISR model is that the space of ranks on which to optimize the likelihood is very large. At each mStep, several candidate
     * mu are sampled, and the one which maximizes the likelihood of the observations is used as the estimated parameter. The sampling method used is a
     * Gibbs sampler, which is a Markov Chain sampler that needs to be initialized. This is slightly different to the Ordinal model case, where the initialization
     * of the parameters is only here to ensure that all individuals are valid (not all z at 0). In the Rank model initialization, mu is chosen among all the
     * observed values of the class, while pi is initialized to a "neutral" value. */
    void mStep(EstimatorType bias) {
      for (int k = 0; k < nbClass_; ++k) {
        class_[k].mStep(bias);
      }
    }

    void storeSEMRun(int iteration,
                     int iterationMax)
    {
      for (int k = 0; k < nbClass_; ++k)
      {
        muParamStat_[k].sampleValue(iteration, iterationMax);
      }
      piParamStat_.sampleParam(iteration, iterationMax);

      if (iteration == iterationMax) // at last iteration, compute the observed probability distribution logProba_
      {
        for (int k = 0; k < nbClass_; ++k)
        {
          muParamStat_[k].setExpectationParam(); // estimate mu parameter using mode / expectation
        }
        piParamStat_.setExpectationParam(); // estimate pi parameter using mode / expectation

        computeObservedProba();
      }
    }

    void storeGibbsRun(int i,
                       int iteration,
                       int iterationMax) {
      if (!data_(i).allPresent()) {
        dataStat_[i].sampleValue(iteration, iterationMax);

        if (iteration == iterationMax) { // at last iteration, compute the observed probability distribution logProba_
          dataStat_[i].setExpectationParam();
        }
      }
    }

    Real lnCompletedProbability(int i, int k) {
      return class_[k].lnCompletedProbabilityInd(i);
    }

    virtual Real lnObservedProbability(int i, int k) {
      return class_[k].lnObservedProbability(i);
    }

    void removeMissing(initParam init) {
      for (int i = 0; i < nbInd_; ++i) {
        data_(i).removeMissing();
      }

      if (init == initParam_) { // mu is initialized through direct sampling in each class
        for (int k = 0; k < nbClass_; ++k) {
          MultinomialStatistic multi;
          int sampleIndInClass = multi.sampleInt(0, classInd_[k].size() - 1); // individual sampled inside the class

          int i = 0;
          int sampleInd = -1;
          for (std::set<int>::const_iterator it  = classInd_[k].begin(),
                                             itE = classInd_[k].end();
               it != itE;
               ++it, ++i) {
            if (i == sampleIndInClass) {
              sampleInd = *it;
              break;
            }
          }

          mu_(k) = data_(sampleInd).x();
          pi_(k) = piInitISR;
        }
      }
    }

    int nbFreeParameter() const
    {
      return nbClass_; // only the continuous pi_ parameter is taken into account, not the discrete mu_ parameter
    }

    void writeParameters() const
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

    std::string setDataParam(RunMode mode) {
      std::string warnLog;
      Vector<std::string> dataStr;

      warnLog += p_handler_->getData(idName(), // get the raw vector of strings
                                     dataStr,
                                     nbInd_,
                                     paramStr_);

      warnLog += rankParser_.parseStr(dataStr, // convert the vector of strings to ranks
                                      minModality,
                                      nbPos_,
                                      data_);

      MultinomialStatistic multi;
      Vector<int> vec(nbPos_);
      std::iota(vec.begin(), vec.end(), 0);
      for (int k = 0; k < nbClass_; ++k) {
        mu_[k].setNbPos(nbPos_);
        multi.shuffle(vec);
        mu_[k].setO(vec);
      }

      warnLog += checkMissingType();

      if (warnLog.size() > 0) {
        return warnLog;
      }

      if (mode == prediction_) { // prediction mode
        p_paramSetter_->getParam(idName_, // parameters are set using results from previous run
                                 "mu",
                                 mu_);

        p_paramSetter_->getParam(idName_, // parameters are set using results from previous run
                                 "pi",
                                 pi_);

        for (int k = 0; k < nbClass_; ++k) {
          muParamStat_[k].setParamStorage();
        }
        piParamStat_.setParamStorage();

        computeObservedProba();
      }

      dataStat_.reserve(nbInd_);
      for (int i = 0; i < nbInd_; ++i) {
        dataStat_.emplace_back(data_(i).xModif(), confidenceLevel_);
      }

      return warnLog;
    }

    void exportDataParam() const {
      p_dataExtractor_->exportVals(indexMixture_,
                                   idName(),
                                   data_,
                                   dataStat_);
      p_paramExtractor_->exportParam(indexMixture_,
                                     idName_,
                                     "mu",
                                     muParamStat_,
                                     muParamNames(),
                                     confidenceLevel_);
      p_paramExtractor_->exportParam(indexMixture_,
                                     idName_,
                                     "pi",
                                     piParamStat_.getStatStorage(),
                                     piParamStat_.getLogStorage(),
                                     piParamNames(),
                                     confidenceLevel_);
    }

    void computeObservedProba() {
      for (int k = 0; k < nbClass_; ++k) {
        class_[k].computeObservedProba();
      }
    }
  private:
    std::string checkMissingType() {
      std::string warnLog;

      std::list<int> listInd;
      for (int i = 0; i < nbInd_; ++i) {
        if (!data_(i).checkMissingType(acceptedType_)) {
          listInd.push_back(i);
        }
      }

      if (listInd.size() > 0) {
        std::stringstream sstm;
        sstm << "Rank variable " << idName_ << " contains individual described by missing data type not implemented yet. "
             << "The list of problematic individuals is: " << itString(listInd) << std::endl;
        warnLog += sstm.str();
      }

      return warnLog;
    }

    std::vector<std::string> muParamNames() const
    {
      std::vector<std::string> names(nbClass_);
      for (int k = 0; k < nbClass_; ++k)
      {
        std::stringstream sstm;
        sstm << "k: " << k + minModality << ", mu";
        names[k] = sstm.str();
      }
      return names;
    }

    std::vector<std::string> piParamNames() const
    {
      std::vector<std::string> names(nbClass_);
      for (int k = 0; k < nbClass_; ++k)
      {
        std::stringstream sstm;
        sstm << "k: " << k + minModality << ", pi";
        names[k] = sstm.str();
      }
      return names;
    }

    int nbClass_;

    /** Number of samples in the data set*/
    int nbInd_;

    int nbPos_;
    Real facNbMod_;

    const Vector<int>* p_zi_;
    const Vector<std::set<int> >& classInd_;

    const DataHandler* p_handler_;
    DataExtractor* p_dataExtractor_;
    const ParamSetter* p_paramSetter_;
    ParamExtractor* p_paramExtractor_;

    Real confidenceLevel_;

    Vector<RankVal> mu_;
    Vector<Real> pi_;
    Vector<RankIndividual> data_;

    /** RankLikelihood object, used to for the harmonic mean estimation of the observed likelihood. */
    RankLikelihood rankLikelihood_;

    /** RankParser object, used to transform pitiful Vector<std::string> to glorious Vector<RankIndividual> */
    RankParser rankParser_;

    /** Matrix containing observed log probability distribution, used in harmonic mean estimation
     * of the observed probability
     * Individual in rows
     * Classes in columns */
    Matrix<Real> observedProbaHMean_;

    std::vector<RankClass> class_;

    /** Each element of the vector keeps track of statistics for one particular mu */
    std::vector<RankStat> muParamStat_;

    /** Each element of the vector keeps track of statistics for one particular individual */
    std::vector<RankStat> dataStat_;

    /** Compute the statistics on pi parameter */
    ConfIntParamStat<Real> piParamStat_;

    Vector<bool> acceptedType_;

    std::string paramStr_;
};

} // namespace mixt

#endif // MIXT_RANKMIXTURE_H
