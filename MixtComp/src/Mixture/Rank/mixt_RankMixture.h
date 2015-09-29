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
#include "mixt_RankParamStat.h"

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
    RankMixture(std::string const& idName,
                int nbClass,
                const Vector<int>* p_zi,
                const Vector<std::set<int> >& classInd,
                const DataHandler* p_handler,
                DataExtractor* p_extractor,
                const ParamSetter* p_paramSetter,
                ParamExtractor* p_paramExtractor,
                Real confidenceLevel) :
        IMixture(idName),
        nbClass_(nbClass),
        nbInd_(0),
        nbMod_(0),
        facNbMod_(0.),
        p_zi_(p_zi),
        classInd_(classInd),
        p_handler_(p_handler),
        p_extractor_(p_extractor),
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

      acceptedType_ << true,   // present_,
                       false,  // missing_,
                       false,  // missingFiniteValues_,
                       false,  // missingIntervals_,
                       false,  // missingLUIntervals_,
                       false; // missingRUIntervals
    }

    void samplingStepCheck(int ind)
    {
      for (int i = 0; i < nbInd_; ++i)
      {
        data_(i).sampleY(mu_((*p_zi_)(i)),
                         pi_((*p_zi_)(i)));
      }
    }

    void samplingStepNoCheck(int ind)
    {
      for (int i = 0; i < nbInd_; ++i)
      {
        data_(i).sampleY(mu_((*p_zi_)(i)),
                         pi_((*p_zi_)(i)));
      }
    }

    int checkSampleCondition(std::string* warnLog = NULL) const
    {
      return 1;
    }

    void mStep()
    {
      for (int k = 0; k < nbClass_; ++k)
      {
        class_[k].mStep();
      }
    }

    void storeSEMBurnIn(int iteration,
                                int iterationMax)
    {}

    void storeSEMRun(int iteration,
                             int iterationMax)
    {
      for (int k = 0; k < nbClass_; ++k)
      {
        muParamStat_[k].sampleParam(iteration, iterationMax);
      }
      piParamStat_.sampleParam(iteration, iterationMax);

      if (iteration == iterationMax) // at last iteration, compute the observed probability distribution logProba_
      {
        for (int k = 0; k < nbClass_; ++k)
        {
          muParamStat_[k].setExpectationParam(); // estimate mu parameter using mode / expectation
        }
        piParamStat_.setExpectationParam(); // estimate pi parameter using mode / expectation
      }
    }

    void storeGibbsRun(int i,
                       int iteration,
                       int iterationMax)
    {
      rl_.observe(i,
                  (*p_zi_)(i),
                  facNbMod_ * lnCompletedProbability(i,
                                                     (*p_zi_)(i)));
      if (iteration == iterationMax)
      {
//        rl_.getHMean(observedProba_); // harmonic mean computation method

        RankIndividual ri(nbMod_);
        observedProbaSampling_.resize(nbClass_);
        for (int k = 0; k < nbClass_; ++k)
        {
          ri.observedProba(mu_(k),
                           pi_(k),
                           observedProbaSampling_(k));
        }
      }
    }

    Real lnCompletedProbability(int i, int k)
    {
      return class_[k].lnCompletedProbabilityInd(i);
    }

    virtual Real lnObservedProbability(int i, int k)
    {
//      return observedProba_(i, k); // harmonic mean computation method

      std::map<RankVal, Real>::iterator it = observedProbaSampling_(k).find(data_(i).x());
      if (it == observedProbaSampling_(k).end()) // the current individual has not been observed during sampling
      {
        return minInf;
      }
      else
      {
        return std::log(it->second);
      }
    }

    void removeMissing()
    {
      for (int i = 0; i < nbInd_; ++i)
      {
        data_(i).removeMissing();
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

    std::string setDataParam(RunMode mode)
    {
      std::string warnLog;
      // setDataParam, rl_.setDim()
      // in prediction: piParamStatComputer_.setParamStorage();
      Vector<std::string> dataStr;

      p_handler_->getData(idName(),
                          dataStr,
                          nbInd_,
                          paramStr_,
                          warnLog);

      warnLog += checkMissingType();

      return warnLog;
    }

    void exportDataParam() const
    {}

  private:
    std::string checkMissingType()
    {
      std::string warnLog;

      std::list<int> listInd;
      for (int i = 0; i < nbInd_; ++i)
      {
        if (!data_(i).checkMissingType(acceptedType_))
        {
          listInd.push_back(i);
        }
      }

      if (listInd.size() > 0)
      {
        std::stringstream sstm;
        sstm << "Rank variable " << idName_ << " contains individual described by missing data type not implemented yet. "
             << "The list of problematic individuals is: " << itString(listInd) << std::endl;
        warnLog += sstm.str();
      }

      return warnLog;
    }

    int nbClass_;

    /** Number of samples in the data set*/
    int nbInd_;

    int nbMod_;
    Real facNbMod_;

    const Vector<int>* p_zi_;
    const Vector<std::set<int> >& classInd_;

    const DataHandler* p_handler_;
    DataExtractor* p_extractor_;
    const ParamSetter* p_paramSetter_;
    ParamExtractor* p_paramExtractor_;

    Real confidenceLevel_;

    Vector<RankVal> mu_;
    Vector<Real> pi_;
    Vector<RankIndividual> data_;

    /** RankLikelihood object, used to for the harmonic mean estimation of the observed likelihood. */
    RankLikelihood rl_;

    /** Matrix containing observed log probability distribution, used in harmonic mean estimation
     * of the observed probability
     * Individual in rows
     * Classes in columns */
    Matrix<Real> observedProbaHMean_;

    std::vector<RankClass> class_;

    /** Vector of map containing observed probability distribution, used in independent sampling estimation.
     * Each element of the vector corresponds to a class */
    Vector<std::map<RankVal, Real> > observedProbaSampling_;

    /** Each element of the vector keeps track of statistics for one particular mu */
    std::vector<RankParamStat> muParamStat_;

    /** Compute the statistics on pi parameter */
    ConfIntParamStat<Real> piParamStat_;

    Vector<bool> acceptedType_;

    std::string paramStr_;
};

} // namespace mixt

#endif // MIXT_RANKMIXTURE_H
