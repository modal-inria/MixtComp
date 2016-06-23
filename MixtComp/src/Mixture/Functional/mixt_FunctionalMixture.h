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
                      const Vector<std::set<Index> >& classInd,
                      const DataHandler* p_handler,
                      DataExtractor* p_extractor,
                      const ParamSetter* p_paramSetter,
                      ParamExtractor* p_paramExtractor,
                      Real confidenceLevel) :
        IMixture(indexMixture,
                 idName),
        nClass_(nClass),
        nInd_(0),
        classInd_(classInd),
        p_handler_(p_handler),
        p_dataExtractor_(p_extractor),
        p_paramSetter_(p_paramSetter),
        p_paramExtractor_(p_paramExtractor),
        confidenceLevel_(confidenceLevel) {
      class_    .reserve(nClass_);
      for (Index k = 0; k < nClass_; ++k) {
        class_.emplace_back(data_,
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

    void samplingStepCheck(Index ind) {};
    void samplingStepNoCheck(Index ind) {};
    Index checkSampleCondition(std::string* warnLog = NULL) const {return 0;}
    void mStep(EstimatorType bias) {};
    void storeSEMRun(Index iteration,
                     Index iterationMax) {};
    void storeGibbsRun(Index i,
                       Index iteration,
                       Index iterationMax) {};
    Real lnCompletedProbability(Index i, Index k) {return 0.;}
    Real lnObservedProbability(Index i, Index k)  {return 0.;}
    Index nbFreeParameter() const {return 0;}
    void writeParameters() const {};
    std::string setDataParam(RunMode mode) {return "";}
    void exportDataParam() const {};
    void removeMissing(initParam algo) {};
  private:
    Index nInd_;
    Index nClass_;
    Real confidenceLevel_;

    /** Data */
    Vector<Function> data_;

    const Vector<std::set<Index> >& classInd_;

    const DataHandler* p_handler_;
    DataExtractor* p_dataExtractor_;
    const ParamSetter* p_paramSetter_;
    ParamExtractor* p_paramExtractor_;

    std::vector<FunctionalClass> class_;

    Vector<bool> acceptedType_;
};

} // namespace mixt

#endif // MIXT_FUNCTIONALMIXTURE
