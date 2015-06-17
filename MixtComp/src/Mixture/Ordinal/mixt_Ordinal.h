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
      // data handler fait son getData et remplit le augData
      // v_path_.resize(nbInd_);

      return warnLog;
    }

    virtual void samplingStep(int ind)
    {
    }

    virtual std::string mStep()
    {
      std::string warnLog;
      return warnLog;
    }

    virtual void storeSEMBurnIn(int iteration,
                               int iterationMax)
    {

    }

    virtual void storeSEMRun(int iteration,
                             int iterationMax)
    {
      // at last iteration, compute the obsersved probability distribution and store it
      // in m_proba_
    }

    virtual void storeGibbsRun(int sample,
                               int iteration,
                               int iterationMax)
    {

    }

    virtual Real lnCompletedProbability(int i, int k)
    {
      return 12.;
    }

    /**
     * This function must be defined to return the observed likelihood
     * @return the observed log-likelihood
     */
    virtual Real lnObservedProbability(int i, int k)
    {
      return 12.;
    }

    virtual int nbFreeParameter() const
    {
      return 12;
    }

    virtual void writeParameters(std::ostream& out) const
    {

    }

    virtual void exportDataParam() const
    {

    }

    bool possibleNullProbability() const {return false;} // a precision of 1 concentrates the probability on the mode ??? Therefore should be true ? -> Very long initialization ...

    void removeMissing()
    {
      // should use BOSPath::initPath()
    };

  protected:
  private:
    /** Pointer to the zik class label */
    Vector<int> const* p_zi_;

    /** Number of classes */
    int nbClass_;

    /** The augmented data set */
    AugmentedData<Vector<int> > augData_;

    /** Vector containing path for individuals */
    Vector<BOSPath> path_;

    /** Matrix containing observed probability distribution */
    Matrix<Real> proba_;

    /** Number of samples in the data set*/
    int nbInd_;

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
