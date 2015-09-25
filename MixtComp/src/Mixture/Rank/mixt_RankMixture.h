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
        pi_(nbClass)
    {
      class_    .reserve(nbClass);
      paramStat_.reserve(nbClass);
      for (int k = 0; k < nbClass; ++k)
      {
        class_.emplace_back(data_,
                            classInd_(k),
                            mu_(k),
                            pi_(k)); // doing that means that classInd_, mu_ and pi_ must not be resized in order to avoid incorrect behaviour at runtime
        paramStat_.emplace_back(mu_(k),
                                pi_(k),
                                confidenceLevel);
      }
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
    {}

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
        rl_.getHMean(observedProba_);
      }
    }

    Real lnCompletedProbability(int i, int k)
    {
      return class_[k].lnCompletedProbabilityInd(i);
    }

    virtual Real lnObservedProbability(int i, int k)
    {
      return observedProba_(i, k);
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
      // setDataParam, rl_.setDim()
      return std::string();
    }

    void exportDataParam() const
    {}

  private:
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

    RankLikelihood rl_;

    /** Matrix containing observed log probability distribution
     * Individual in rows
     * Classes in columns */
    Matrix<Real> observedProba_;

    /** One element per class */
    std::vector<RankClass> class_;
    std::vector<RankParamStat> paramStat_;
};

} // namespace mixt

#endif // MIXT_RANKMIXTURE_H
