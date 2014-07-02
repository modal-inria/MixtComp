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
#include "Clustering/include/STK_IMixtureComposerBase.h"

namespace mixt
{

class IMixture;

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
class MixtureComposer : public STK::IMixtureComposerBase
{
  public:
    typedef std::vector<IMixture*>::const_iterator ConstMixtIterator;
    typedef std::vector<IMixture*>::iterator MixtIterator;
    /** Constructor.
     * @param nbCluster,nbSample,nbVariable number of clusters, samples and Variables
     */
    MixtureComposer( int nbSample, int nbVariable, int nbCluster);
    /** copy constructor.
     *  @param composer the composer to copy
     */
    MixtureComposer(MixtureComposer const& composer);
    /** The registered mixtures will be deleted there.*/
    virtual ~MixtureComposer();

    /** @return a constant reference on the vector of mixture */
    inline std::vector<IMixture*> const& v_mixtures() const { return v_mixtures_;}

    /** Create a composer, but reinitialize the mixtures parameters. */
    virtual MixtureComposer* create() const;
    /** Create a clone of the current model, with mixtures parameters preserved. */
    virtual MixtureComposer* clone() const;

    /** initialize randomly the parameters of the components of the model */
    virtual void randomInit();
    /** Compute the proportions and the model parameters given the current tik
     *  mixture parameters.
     **/
    virtual void mStep();
    /** @return the value of the probability of the i-th sample in the k-th component.
     *  @param i index of the sample
     *  @param k index of the component
     **/
    virtual STK::Real lnComponentProbability(int i, int k);
    /** write the parameters of the model in the stream os. */
    virtual void writeParameters(STK::ostream& os) const;
    /** @brief compute the number of free parameters of the model.
     *  lookup on the mixtures and sum the nbFreeParameter.
     **/
    virtual int computeNbFreeParameters() const;
    /**@brief This step can be used by developer to initialize any thing which
     * is not the model. It will be called before running the estimation
     * algorithm. In this class, the @c initializeSterp method
     *  - set the number of free parameters using the pure virtual function @¢ computeNbFreeParameters()
     *  - Compute the
     *  - call the initializeStep() of all the mixtures,
     *  First initialization of the parameters of the model.
     *  This method is called in order to initialize the parameters.
     *
     **/
    virtual void initializeStep();
    /** @brief Impute the missing values.
     **/
    virtual void imputationStep();
    /** @brief Simulation of all the latent variables and/or missing data
     *  excluding class labels.
     */
    virtual void samplingStep();
    /**@brief This step can be used to signal to the mixtures that they must
     * store results. This is usually called after a burn-in phase.
     **/
    virtual void storeIntermediateResults(int iteration);
    /**@brief This step can be used by developer to finalize any thing. It will
     *  be called only once after we finish running the estimation algorithm.
     **/
    virtual void finalizeStep();
    /** register a mixture to the composer.
     *  When a mixture is registered, the composer:
     *  - assign composer pointer (itself) to the mixture
     *  - add it to v_mixtures_
     *  @note the mixture is not initialized, so don't forget to call
     **/
    void registerMixture(IMixture* mixture);
    /** Utility method allowing to create all the mixtures using the DataHandler
     *  info of the manager.
     **/
    template<class MixtureManager>
    void createMixtures(MixtureManager& manager)
    { manager.createMixtures(*this, nbCluster());}
    /** Create a specific mixture and register it.
     *  @param manager the manger with the responsibility of the creation
     *  @param idModel the id of the mixture we want to create
     *  @param idName the name of the mixture
     **/
    template<class MixtureManager>
    void createMixture(MixtureManager& manager, STK::Clust::Mixture idModel, STK::String const& idName)
    {
      IMixture* p_mixture = manager.createMixture(idModel, idName, nbCluster_);
      if (p_mixture) registerMixture(p_mixture);
    }

  protected:
    /** @brief Create the composer using existing data handler and mixtures.
     * This method is essentially used by the create() method and can be
     * reused in derived classes.
     * @sa MixtureComposerFixedProp
     **/
    void createComposer( std::vector<IMixture*> const& v_mixtures_);
    /** vector of pointers to the mixtures components */
    std::vector<IMixture*> v_mixtures_;
};

} /* namespace mixt */

#endif /* STK_MIXTURECOMPOSER_H */
