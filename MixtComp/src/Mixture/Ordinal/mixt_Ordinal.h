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
      sampler_(&m_augDataij_,
               getParam(),
               nbClass),
      dataStatComputer_(&m_augDataij_,
                        &dataStatStorage_,
                        confidenceLevel),
      paramStat_(param_,
                 paramStatStorage_,
                 paramLogStorage_,
                 confidenceLevel),
      likelihood_(getParam(),
                  getData(),
                  getDataStatStorage(),
                  nbClass),
      p_handler_(p_handler_),
      p_dataExtractor_(p_extractor),
      p_paramSetter_(p_paramSetter),
      p_paramExtractor_(p_paramExtractor)
      // dataStatStorage_ is an empty array at construction
{}

    /** copy constructor */
    Ordinal(Ordinal const& ordinal) :
      IMixture(ordinal),
      p_zi_(ordinal.p_zi_),
      nbClass_(ordinal.nbClass_),
      m_augDataij_(ordinal.m_augDataij_),
      nbInd_(ordinal.nbInd_),
      confidenceLevel_(ordinal.confidenceLevel_),
      sampler_(ordinal.sampler_),
      dataStatComputer_(ordinal.dataStatComputer_),
      paramStat_(ordinal.paramStat_),
      likelihood_(ordinal.likelihood_),
      p_handler_(ordinal.p_handler_),
      p_dataExtractor_(ordinal.p_dataExtractor_),
      p_paramSetter_(ordinal.p_paramSetter_),
      p_paramExtractor_(ordinal.p_paramExtractor_),
      dataStatStorage_(ordinal.dataStatStorage_)
    {
      mixture_.setData(m_augDataij_.data_);
    }

    /** copy constructor */
    Ordinal(MixtureBridge const& bridge);

    /** This function will be defined to set the data into your data containers.
     *  To facilitate data handling, framework provide templated functions,
     *  that can be called directly to get the data.
     */
    std::string setDataParam(RunMode mode)
    {
      // data handler fait son getData et remplit le augData
      v_path_.resize(nbInd_);
    }

    /** This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep(int ind)
    {
    }

    /** This function is equivalent to Mstep and must be defined to update parameters.
     */
    virtual std::string mStep()
    {
    }

    /** This function should be used to store any results during the burn-in period
     *  @param iteration Provides the iteration number during the burn-in period
     */
    virtual void storeSEMBurnIn(int iteration,
                               int iterationMax)
    {

    }

    /** This function should be used to store any intermediate results during
     *  various iterations after the burn-in period.
     *  @param iteration Provides the iteration number beginning after the burn-in period.
     */
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

    /**
     * Completed likelihood, using the current values for unknown values
     * @return the completed log-likelihood
     */
    virtual Real lnCompletedProbability(int i, int k)
    {

    }

    /**
     * This function must be defined to return the observed likelihood
     * @return the observed log-likelihood
     */
    virtual Real lnObservedProbability(int i, int k)
    {

    }

    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const
    {

    }

    /** This function must return the number of variables.
     *  @return Number of variables
     */
    virtual int nbVariable() const
    {

    }

    /** This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    {

    }

    virtual void exportDataParam() const
    {

    }
  protected:
  private:
    /** Pointer to the zik class label */
    Vector<int> const* p_zi_;

    /** Number of classes */
    int nbClass_;

    /** The augmented data set */
    AugmentedData<Matrix<int> > augData_;

    /** Vector containing path for individuals */
    Vector<BOSPath> v_path_;

    /** Matrix containing observed probability distribution */
    Matrix<Real> m_proba_;

    /** Number of samples in the data set*/
    int nbInd_;

    /** Confidence level used in computation of parameters and missing values statistics */
    Real confidenceLevel_;

    /** Mode parameter */
    Vector<int> v_mu_;

    /** Precision parameter */
    Vector<Real> v_pi_;

    /** Compute the statistics on missing data during GibbsRun phase */
    PoissonDataStat dataStatComputer_;

    /** Store the statistics on missing data for export, after the GibbsRun phase */
    Vector<RowVector<int> > dataStatStorage_;

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
