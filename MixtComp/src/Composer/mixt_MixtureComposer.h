/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2014

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
 *  Created on: July 2, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 *
 **/

#ifndef MIXT_MIXTURECOMPOSER_H
#define MIXT_MIXTURECOMPOSER_H

#include <vector>
#include "../Sampler/mixt_ClassSampler.h"
#include "../Mixture/mixt_IMixture.h"
#include "../Various/mixt_Def.h"
#include "../Param/mixt_ConfIntParamStat.h"
#include "../Data/mixt_CategoricalDenseDataStat.h"

namespace mixt
{

typedef std::vector<IMixture*>::const_iterator ConstMixtIterator;
typedef std::vector<IMixture*>::iterator MixtIterator;

class MixtureComposer
{
  public:
    /** Constructor.
     * @param nbCluster,nbSample,nbVariable number of clusters, samples and Variables
     */
    MixtureComposer(int nbInd,
                    int nbClass,
                    Real confidenceLevel);

    /** copy constructor.
     *  @param composer the composer to copy
     */
    MixtureComposer(MixtureComposer const& composer);

    /** The registered mixtures will be deleted there.*/
    ~MixtureComposer();

    /** Create the mixture model parameters. */
    void intializeMixtureParameters();

    int nbInd() const {return nbInd_;}

    /** @return  the zi class label */
    inline Vector<int> const* p_zi() const {return &zi_.data_;}

    /** @return a constant reference on the vector of mixture */
    inline std::vector<IMixture*> const& v_mixtures() const {return v_mixtures_;}

    /** Compute the proportions and the model parameters given the current tik
     *  mixture parameters.
     *  @param[out] worstDeg worst degeneracy type incountered among all mixtures for this mStep
     **/
    std::string mStep();

    /** Compute proportions using the ML estimator, default implementation. Set
     *  as virtual in case we impose fixed proportions in derived model. Only called
     *  by mStep
     **/
    void pStep();

    /**
     * Simulate zi accordingly to tik and replace tik by zik by calling cStep().
     * @return the minimal value of individuals in a class
     **/
    void sStepNoCheck();
    void sStepNoCheck(int i);

    void sStepCheck();
    void sStepCheck(int i);

    /** compute Tik */
    void eStep();
    void eStep(int i);

    /** Compute zi using the Map estimator. */
    void mapStep();
    void mapStep(int i);

    /** @return the value of the probability of the i-th sample in the k-th component.
     *  @param i index of the sample
     *  @param k index of the component
     **/
    Real lnCompletedProbability(int i, int k);

    Real lnObservedProbability(int i, int k);

    /** @return the value of the observed likelihood */
    Real lnObservedLikelihood();

    /** @return the value of the completed likelihood */
    Real lnCompletedLikelihood();

    /** @return the value of the semi-completed likelihood (completion only for latent class) */
    Real lnSemiCompletedLikelihood();

    /** write the parameters of the model in the stream os. */
    void writeParameters() const;

    /** @brief compute the number of free parameters of the model.
     *  lookup on the mixtures and sum the nbFreeParameter.
     **/
    int nbFreeParameters() const;

    /**
     * Perform a sampling step
     * @param checkSampleCondition indicates if there is a need to perform a check on the data or not
     */
    void samplingStepNoCheck();
    void samplingStepNoCheck(int i);

    void samplingStepCheck();
    void samplingStepCheck(int i);

    /**
     * Check if the data conditions are verified for all mixtures. Providing a log is required during
     * initialization. The rejection sampling / Gibbs sampling will ensure that at later steps the
     * data conditions are verified, hence there is no need to provide information to the user, and no
     * log is required.
     * @param[out] warnLog provides information on what condition has not been met
     * */
    int checkSampleCondition(std::string* warnLog = NULL) const;

    /**
     * Check if there are enough individual in each class. Called by checkSampleCondition.
     * @param[out] warnLog provides information on what condition has not been met
     * */
    int checkNbIndPerClass(std::string* warnLog = NULL) const;

    /**@brief This step can be used to signal to the mixtures that they must
     * store results. This is usually called after a burn-in phase.
     **/
    void storeSEMBurnIn(int iteration,
                        int iterationMax);

    /**@brief This step can be used to signal to the mixtures that they must
     * store results. This is usually called after a burn-in phase.
     **/
    void storeSEMRun(int iteration,
                     int iterationMax);

    /** @brief This step can be used to signal to the mixtures that they
     * must store data. This is usually called after the long algo, to
     * store data generated using the estimated parameters during a Gibbs sampling
     */
    void storeGibbsRun(int sample,
                       int iteration,
                       int iterationMax);

    /** DataHandler is injected to take care of setting the values of the latent classes.
     * This avoids templating the whole composer with DataHandler type, as is currently done
     * with the individual IMixtures
     * @param checkInd should be set to 1 if a minimum number of individual per class should be
     * enforced at sampling (true in learning, false in prediction) */
    template<typename ParamSetter,
             typename DataHandler>
    std::string setDataParam(const ParamSetter& paramSetter,
                             const DataHandler& dataHandler,
                             RunMode mode)
    {
      std::string warnLog;
      warnLog += setProportion(paramSetter);
      warnLog += setZi(dataHandler, // dataHandler getData is called to fill zi_
                       mode);
      if (mode == prediction_) // in prediction, paramStatStorage_ will not be modified later during the run
      {
        paramStat_.setParamStorage(); // paramStatStorage_ is set now, and will not be modified further during predict run
      }
      dataStat_.resizeStatStorage(nbInd_);

      for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
      {
        warnLog += (*it)->setDataParam(mode);
      }

      return warnLog;
    }

    /** ParamSetter is injected to take care of setting the values of the proportions.
     * This avoids templating the whole composer with DataHandler type, as is currently done
     * with the individual IMixtures. */
    template<typename ParamSetter>
    std::string setProportion(const ParamSetter& paramSetter)
    {
#ifdef MC_DEBUG
      std::cout << "MixtureComposer::setProportion" << std::endl;
#endif
      std::string warnLog;

      paramSetter.getParam("z_class",
                           prop_);

      return warnLog;
    }

    /** DataHandler is injected to take care of setting the values of the latent classes.
      * This avoids templating the whole composer with DataHandler type, as is currently done
      * with the individual IMixtures.
      * @param checkInd should be set to 1 if a minimum number of individual per class should be
      * enforced at sampling (true in learning, false in prediction) */
    template<typename DataHandler>
    std::string setZi(const DataHandler& dataHandler,
                      RunMode mode)
    {
#ifdef MC_DEBUG
      std::cout << "IMixtureComposerBase::setZi" << std::endl;
#endif
      std::string warnLog;
      std::string dummyParam;

      if (dataHandler.info().find("z_class") == dataHandler.info().end()) // z_class was not provided
      {
        zi_.setAllMissing(nbInd_); // set every value state to missing_
      }
      else // z_class was provided and its value is acquired in zi_
      {
        dataHandler.getData("z_class", // reserved name for the class
                            zi_,
                            nbInd_,
                            dummyParam,
                            -minModality, // an offset is immediately applied to the read data so that internally the classes encoding is 0 based
                            warnLog);
      }

      Vector<bool> at(nb_enum_MisType_); // authorized missing values, should mimic what is found in categorical mixtures
      at(0) = true; // present_,
      at(1) = true;// missing_,
      at(2) = true;// missingFiniteValues_,
      at(3) = false;// missingIntervals_,
      at(4) = false;// missingLUIntervals_,
      at(5) = false;// missingRUIntervals_,

      std::string tempLog = zi_.checkMissingType(at); // check if the missing data provided are compatible with the model
      if(tempLog.size() > 0)
      {
       std::stringstream sstm;
       sstm << "Variable " << idName_ << " contains latent classes and has unsupported missing value types.\n" << tempLog;
       warnLog += sstm.str();
      }
      zi_.computeRange(); // compute effective range of the data for checking, min and max will be set to 0 if data is completely missing
      if (zi_.dataRange_.min_ < 0)
      {
       std::stringstream sstm;
       sstm << "The z_class latent class variable has a lowest provided value of: "
            << minModality + zi_.dataRange_.min_
            << " while the minimal value has to be: "
            << minModality
            << ". Please check the encoding of this variable to ensure proper bounds." << std::endl;
       warnLog += sstm.str();
      }
      if (zi_.dataRange_.hasRange_ == true || zi_.dataRange_.max_ > nbClass_ - 1)
      {
       std::stringstream sstm;
       sstm << "The z_class latent class variable has a highest provided value of: "
            << minModality + zi_.dataRange_.max_
            << " while the maximal value can not exceed the number of class: "
            << minModality + nbClass_ - 1
            << ". Please check the encoding of this variable to ensure proper bounds." << std::endl;
       warnLog += sstm.str();
      }
      zi_.dataRange_.min_ = 0; // real range provided by the parameters is enforced
      zi_.dataRange_.max_ = nbClass_ - 1;
      zi_.dataRange_.range_ = nbClass_;

      return warnLog;
    };

    /**@brief This step can be used to ask each mixture to export its model parameters
     * and data
     **/
    template<typename DataExtractor,
             typename ParamExtractor>
    void exportDataParam(DataExtractor& dataExtractor, ParamExtractor& paramExtractor) const
    {
#ifdef MC_DEBUG
      std::cout << "MixtureComposer::exportDataParam" << std::endl;
      std::cout << "zi_data_" << std::endl;
      std::cout << zi_.data_ << std::endl;
#endif
      dataExtractor.exportVals("z_class",
                               zi_,
                               tik_);
      paramExtractor.exportParam("z_class",
                                 paramStat_.getStatStorage(),
                                 paramStat_.getLogStorage(),
                                 paramNames(),
                                 confidenceLevel_);
      for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
      {
        (*it)->exportDataParam();
      }
    };

    /** register a mixture to the composer.
     *  When a mixture is registered, the composer:
     *  - assign composer pointer (itself) to the mixture
     *  - add it to v_mixtures_
     *  @note the mixture is not initialized, so don't forget to call
     **/
    void registerMixture(IMixture* mixture);

    /** Gibbs sampling, one individual at a time */
    void gibbsSampling(int nbGibbsIter,
                       int group,
                       int groupMax);

    /** @return names of the parameters */
    std::vector<std::string> paramNames() const;

    /**
     * Completion of the data using observed t_ik, to detect impossible observations. The observed tik are computed,
     * impossible individual are detected, and zi_ and missing variable are completed.
     * @return string containing a description of the problematic combinations of data and models */
    std::string eStepObserved();

    void removeMissing();

    void lnObservedLikelihoodDebug();

  private:
    /** name of the latent class variable */
    std::string idName_;

    /** number of classes */
    int nbClass_;

    /** Number of samples */
    int nbInd_;

    /** The proportions of each class */
    Vector<Real> prop_;

    /** The tik probabilities */
    Matrix<Real> tik_;

    /** The zik class label */
    AugmentedData<Vector<int> > zi_;

    /** class sampler */
    ClassSampler sampler_;

    /** vector of pointers to the mixtures components */
    std::vector<IMixture*> v_mixtures_;

    /** computer parameters statistics */
    ConfIntParamStat<Real> paramStat_;

    /** computer of the statistics on latent variables */
    CategoricalDenseDataStat dataStat_;

    /** confidence level used for the computation of statistics */
    Real confidenceLevel_;
};

} /* namespace mixt */

#endif /* MIXT_MIXTURECOMPOSER_H */
