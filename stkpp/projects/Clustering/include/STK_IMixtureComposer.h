/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2012  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 16 oct. 2012
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_IMixtureComposer.h
 *  @brief In this file we define the abstract base class for mixture models.
 **/

#ifndef STK_IMIXTURECOMPOSER_H
#define STK_IMIXTURECOMPOSER_H

#include "StatModels/include/STK_IStatModelBase.h"
#include "STK_Clust_Util.h"

#include "Arrays/include/STK_CArrayPoint.h"
#include "Arrays/include/STK_CArrayVector.h"
#include "Arrays/include/STK_Array2D.h"
#ifndef _OPENMP
#include "Arrays/include/STK_Array2DPoint.h"
#endif

namespace STK
{

/** @ingroup Clustering
 *  @brief Base class for Mixture (composed) model.
 *
 * In statistics, a mixture model is a probabilistic model for representing
 * the presence of sub-populations within an overall population, without
 * requiring that an observed data-set should identify the sub-population to
 * which an individual observation belongs. Formally a mixture model
 * corresponds to the mixture distribution that represents the probability
 * distribution of observations in the overall population. However, while
 * problems associated with "mixture distributions" relate to deriving the
 * properties of the overall population from those of the sub-populations,
 * "mixture models" are used to make statistical inferences about the
 * properties of the sub-populations given only observations on the pooled
 * population, without sub-population-identity information.
 *
 * Some ways of implementing mixture models involve steps that attribute
 * postulated sub-population-identities to individual observations (or weights
 * towards such sub-populations), in which case these can be regarded as types
 * unsupervised learning or clustering procedures. However not all inference
 * procedures involve such steps.
 *
 * In this interface we assume there is an underline generative model that will
 * be estimated using either an EM, SEM or CEM algorithm.
 * All mixture parameters are created using the method
 * @code
 *   void initializeMixtureParameters();
 * @endcode
 * in the constructor. They can be accessed from the mixtures using constant
 * accessors.
 *
 * The pure virtual function to implement in derived class are
 * @code
 *   virtual IMixtureComposer* create() const = 0;
 *   virtual IMixtureComposer* clone() const = 0;
 *   virtual bool randomInit() =0;
 *   virtual void mStep() = 0;
 *   virtual Real lnComponentProbability(int i, int k) = 0;
 * @endcode
 *
 * The virtual function that can be re-implemented in derived class for a
 * specific behavior are:
 * @code
 *   virtual void initializeStep();
 *   virtual void pStep();
 *   virtual void imputationStep();
 *   virtual void samplingStep();
 *   virtual void finalizeStep();
 *   virtual void writeParameters(std::ostream& os) const;
 * @endcode
 *
 * @sa IMixture
 *
 * @note the virtual method @c IMixtureComposer::initializeStep is called in all
 * the initialization method.  Don't forget to called it in the randomInit
 * implementation.
 */
class IMixtureComposer : public IStatModelBase
{
  protected:
    /** Constructor.
     * @param nbCluster,nbSample number of clusters and samples
     **/
    IMixtureComposer( int nbSample, int nbCluster);
    /** copy constructor. If the pointer on the mixture parameters are not zero
     *  then they are cloned.
     *  @note if the model have not created the parameters, then the pointer are
     *  initialized to 0.
     *  @param model the model to clone
     **/
    IMixtureComposer( IMixtureComposer const& model);

  public:
    /** destructor */
    virtual ~IMixtureComposer();

    /** @return the number of cluster */
    inline int nbCluster() const { return nbCluster_;}
    /** @return the state of the model*/
    inline Clust::modelState state() const { return state_;}

    /** @return the proportions of each mixtures */
    inline CArrayPoint<Real> const& pk() const { return prop_;};
    /** @return the tik probabilities */
    inline ArrayXX const& tik() const { return tik_;};
    /** @return the estimated proportions of individuals  */
    inline CPointX const& nk() const { return nk_;};
    /** @return the zi class label */
    inline CArrayVector<int> const& zi() const { return zi_;};

    /** @return a pointer on the proportions of each mixtures */
    inline CArrayPoint<Real> const* p_pk() const { return &prop_;};
    /** @return a pointer on the the tik probabilities */
    inline ArrayXX const* p_tik() const { return &tik_;};
    /** @return a pointer on the zi class labels */
    inline CArrayVector<int> const* p_zi() const { return &zi_;};

    /** @return the computed log-likelihood of the i-th sample.
     *  @param i index of the sample
     **/
    Real computeLnLikelihood(int i) const;
    /** @return the computed likelihood of the i-th sample.
     *  @param i index of the sample
     **/
    inline Real computeLikelihood(int i) const
    { return std::exp(computeLnLikelihood(i));}
    /** @return the computed log-likelihood. */
    Real computeLnLikelihood() const;
    /** @return the computed ICL criteria. */
    Real computeICL() const;

    /** set the state of the model : should be used by any strategy*/
    inline void setState(Clust::modelState state) { state_ = state;}

    // pure virtual
    /** create pattern */
    virtual IMixtureComposer* create() const = 0;
    /** clone pattern */
    virtual IMixtureComposer* clone() const = 0;
    /** initialize randomly the parameters of the components of the model */
    virtual void randomInit() = 0;
    /** Compute the proportions and the model parameters given the current tik
     *  mixture parameters.
     **/
    virtual void mStep() = 0;
    /** @return the value of the probability of the i-th sample in the k-th component.
     *  @param i index of the sample
     *  @param k index of the component
     **/
    virtual Real lnComponentProbability(int i, int k) const = 0;

    // virtual with default implementation
    /** write the parameters of the model in the stream os. */
    virtual void writeParameters(ostream& os) const {};
    /** compute the number of free parameters of the model.
     *  This method is used in IMixtureComposer::initializeStep
     *  in order to give a value to IStatModelBase::nbFreeParameter_.
     *  @return the number of free parameters
     **/
    virtual int computeNbFreeParameters() const = 0;
    /** @brief Initialize the model before at its first use.
     *  This function can be overloaded in derived class for initialization of
     *  the specific model parameters. It should be called prior to any used of
     *  the class.
     *  @sa IMixture,MixtureBridge,MixtureComposer
     **/
    virtual void initializeStep();

    /** Compute proportions using the ML estimates, default implementation. Set
     *  as virtual in case we impose fixed proportions in derived Composer.
     **/
    virtual void pStep();
    /** @brief Impute the missing values.
     *  Default behavior is "do nothing".
     **/
    inline virtual void imputationStep() {}
    /** @brief Simulation of all the latent variables and/or missing data
     *  excluding class labels. Default behavior is "do nothing".
     */
    virtual void samplingStep() {}
    /** @brief Utility method allowing to signal to a mixture to set its parameters.
     *  It will be called once enough intermediate results have been stored. */
    virtual void setParameters() {}
    /**@brief This step can be used to signal to the mixtures that they must
     * store results. This is usually called after a burn-in phase. The composer
     * store the current value of the log-Likelihood.
     **/
    virtual void storeIntermediateResults(int iteration) {}
    /**@brief This step can be used to signal to the mixtures that they must
     * release the stored results. This is usually called if the estimation
     * process failed.
     **/
    virtual void releaseIntermediateResults() {}
    /** @brief Finalize the estimation of the model.
     *  The default behavior is compute current lnLikelihood.
     **/
    inline virtual void finalizeStep()
    {
      setLnLikelihood(computeLnLikelihood());
      setState(Clust::modelFinalized_);
    }

    // not virtual
    /** Initialize randomly the labels zi of the model.
     *  Initialize the model parameters using initializeStep()
     *  and compute the tik.
     **/
    void randomClassInit();
    /** Initialize randomly the posterior probabilities tik of the model.
     *  Initialize the model parameters and compute the tik.
     **/
    void randomFuzzyInit();
    /** Replace tik by zik
     *  @return the minimal value of individuals in a class
     **/
    int cStep();
    /** Simulate zi accordingly to tik and replace tik by zik by calling cStep().
     *  @return the minimal value of individuals in a class
     **/
    int sStep();
    /** compute the zi, the lnLikelihood of the current estimates
     *  and the next value of the tik.
     *  @return the minimal value of tk
     **/
    Real eStep();
    /** compute one zi and the next value of the tik for i fixed
     *  @param i the individual
     *  @return the contribution of the individual i to the log-likelihood
     **/
    Real eStep(int i);
    /** Compute zi using the Map estimate. */
    void mapStep();

  protected:
    /** number of cluster. */
    int nbCluster_;
    /** The proportions of each mixtures */
    CArrayPoint<Real> prop_;
    /** The tik probabilities */
    ArrayXX tik_;
    /** The sum of the columns of tik_ */
    CPointX nk_;
    /** The zi class label */
    CArrayVector<int> zi_;
    /** Create the mixture model parameters. */
    void initializeMixtureParameters();
    /** generate random tik_ */
    int randomFuzzyTik();

  private:
    /** state of the model*/
    Clust::modelState state_;
    /** Auxiliary array used in the eStep */
#ifndef _OPENMP
    Array2DPoint<Real> lnComp_;
#endif
};

} // namespace STK

#endif /* STK_IMIXTURECOMPOSER_H */

