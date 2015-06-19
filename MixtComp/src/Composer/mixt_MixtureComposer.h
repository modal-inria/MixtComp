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
 *  Authors:    Serge IOVLEFF <serge.iovleff@inria.fr>
 *              Vincent KUBICKI <vincent.kubicki@inria.fr>
 **/

#ifndef MIXT_MIXTURECOMPOSER_H
#define MIXT_MIXTURECOMPOSER_H

#include <vector>
#include "mixt_IMixtureComposerBase.h"
#include "../Param/mixt_SimpleParamStat.h"
#include "../Mixture/mixt_IMixture.h"
#include "../Various/mixt_Def.h"

namespace mixt
{

/** @ingroup Clustering
 *  Main class for handling composed mixture models.
 *  A composed mixture model on some composed space
 *  \f$  \mathbb{X} = \subset \mathcal{X}^1\times \ldots  \times \mathcal{X}^L \f$
 *  is a density of the form
 * \f[
 *     f(\mathbf{x}|\boldsymbol{\theta})
 *     = \sum_{k=1}^K p_k \prod_{l=1}^L f^l(\mathbf{x}^l;\boldsymbol{\lambda}^l_k,\boldsymbol{\alpha}^l)
 *     \quad \mathbf{x} \in \mathbb{X}.
 * \f]
 * The \f$ p_k > 0\f$ with  \f$ \sum_{k=1}^K p_k =1\f$ are the mixing proportions.
 * The density \e f is called the component of the model. The parameters
 * \f$\boldsymbol{\lambda}^l_k, \, k=1,\ldots K \f$ are the cluster specific parameters
 * and the parameters \f$ \boldsymbol{\alpha}^l \f$ are the shared parameters.
 * */

typedef std::vector<IMixture*>::const_iterator ConstMixtIterator;
typedef std::vector<IMixture*>::iterator MixtIterator;

class MixtureComposer : public mixt::IMixtureComposerBase
{
  public:
    /** Constructor.
     * @param nbCluster,nbSample,nbVariable number of clusters, samples and Variables
     */
    MixtureComposer(int nbSample,
                    int nbVariable,
                    int nbCluster,
                    Real confidenceLevel);
    /** copy constructor.
     *  @param composer the composer to copy
     */
    MixtureComposer(MixtureComposer const& composer);
    /** The registered mixtures will be deleted there.*/
    virtual ~MixtureComposer();

    /** @return a constant reference on the vector of mixture */
    inline std::vector<IMixture*> const& v_mixtures() const { return v_mixtures_;}

    /** Compute the proportions and the model parameters given the current tik
     *  mixture parameters.
     **/
    virtual std::string mStep();
    /** @return the value of the probability of the i-th sample in the k-th component.
     *  @param i index of the sample
     *  @param k index of the component
     **/
    virtual Real lnCompletedLikelihood(int i, int k);
    virtual Real lnObservedLikelihood(int i, int k);

    /** @return the value of the observed likelihood */
    virtual Real lnObservedLikelihood();

    /** @return the value of the completed likelihood */
    virtual Real lnCompletedLikelihood();

    /** @return the value of the semi-completed likelihood (completion only for latent class) */
    virtual Real lnSemiCompletedLikelihood();

    /** write the parameters of the model in the stream os. */
    virtual void writeParameters(std::ostream& os) const;

    /** @brief compute the number of free parameters of the model.
     *  lookup on the mixtures and sum the nbFreeParameter.
     **/
    virtual int nbFreeParameters() const;
    /** @brief Simulation of all the latent variables and/or missing data
     *  excluding class labels.
     */
    virtual void samplingStep();
    virtual void samplingStep(int i);
    /** @brief Simulation of latent variables to detect misclassified partially
     * observed data during initialization
     */
    virtual void misClasStep(int iteration);
    /**@brief This step can be used to signal to the mixtures that they must
     * store results. This is usually called after a burn-in phase.
     **/
    virtual void storeSEMBurnIn(int iteration,
                               int iterationMax);
    /**@brief This step can be used to signal to the mixtures that they must
     * store results. This is usually called after a burn-in phase.
     **/
    virtual void storeSEMRun(int iteration,
                             int iterationMax);
    /** @brief This step can be used to signal to the mixtures that they
     * must store data. This is usually called after the long algo, to
     * store data generated using the estimated parameters during a Gibbs sampling
     */
    virtual void storeGibbsRun(int sample,
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
      warnLog += setZi(dataHandler,
                       mode);
      if (mode == prediction_) // in prediction, paramStatStorage_ will not be modified later during the run
      {
        paramStatStorage_.resize(prop_.rows(), // paramStatStorage_ is set now, and will not be modified further during predict run
                                 1); // no quantiles have to be computed for imported parameters, hence the single column
        paramStatStorage_.col(0) = prop_;
      }

      for (ConstMixtIterator it = v_mixtures_.begin(); it != v_mixtures_.end(); ++it)
      {
        warnLog += (*it)->setDataParam(mode);
      }

      return warnLog;
    }

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
                                 paramStatStorage(),
                                 paramLogStorage(),
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

    /** @return a reference on the statistics of the proportions */
    inline Matrix<Real> const& paramStatStorage() const
    {
      return paramStatStorage_;
    };

    /** @return a reference on the statistics of the proportions */
    inline Matrix<Real> const& paramLogStorage() const
    {
      return paramLogStorage_;
    };

    /** @return names of the parameters */
    std::vector<std::string> paramNames() const;

    /**
     * Completion of the data using observed t_ik, to detect impossible observations. The observed tik are computed,
     * impossible individual are detected, and zi_ and missing variable are completed.
     * @return string containing a description of the problematic combinations of data and models */
    std::string eStepObserved();

    void removeMissing();

    void lnObservedLikelihoodDebug();

  protected:
    /** vector of pointers to the mixtures components */
    std::vector<IMixture*> v_mixtures_;

    /** computer parameters statistics */
    SimpleParamStat paramStat_;

    /** storage for proportions statistics */
    Matrix<Real> paramStatStorage_;

    /** Log for sampled parameters */
    Matrix<Real> paramLogStorage_;

    /** storage for number of samples during Gibbs */
    Matrix<Real> nik_;

    /** confidence level used for the computation of statistics */
    Real confidenceLevel_;
};

} /* namespace mixt */

#endif /* MIXT_MIXTURECOMPOSER_H */
