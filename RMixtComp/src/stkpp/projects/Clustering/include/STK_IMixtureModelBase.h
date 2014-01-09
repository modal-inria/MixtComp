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

/** @file STK_IMixtureModelBase.h
 *  @brief In this file we define the abstract base class for mixture models.
 **/

#ifndef STK_IMODELMIXTUREBASE_H
#define STK_IMODELMIXTUREBASE_H

#include "../../STKernel/include/STK_Range.h"
#include "../../StatModels/include/STK_IModelBase.h"
#include "STK_Clust_Util.h"
#include "../../Arrays/include/STK_CArrayPoint.h"
#include "../../Arrays/include/STK_CArrayVector.h"
#include "../../Arrays/include/STK_Array2D.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief Base class for Mixture model.
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
 *
 * All mixture parameters: proportions, Tik, Zi and components are accessed by
 * pointer and can be set to this class  using the method
 * @code
 *   void setMixtureParameters(CArrayPoint<Real>* p_prop, Array2D<Real>* p_tik, CArrayVector<int>* p_zi);
 * @endcode
 * so that they can be used in a composed model.
 *
 * The mixture parameters also can be created using the method
 * @code
 *   void createMixtureParameters();
 * @endcode
 *
 * The pure virtual function to implement in derived class are
 * @code
 *   virtual IMixtureModelBase* create() const = 0;
 *   virtual IMixtureModelBase* clone() const = 0;
 *   virtual bool randomInit() =0;
 *   virtual void mStep() = 0;
 *   virtual Real lnComponentProbability(int i, int k) = 0;
 * @endcode
 *
 * The virtual function that can be re-implemented in derived class for a
 * specific behavior are:
 * @code
 *   virtual void writeParameters(std::ostream& os) const;
 *   virtual void initializeModel();
 *   virtual void initializeStep();
 *   virtual void inputationStep();
 *   virtual void samplingStep();
 *   virtual void pStep();
 *   virtual void finalizeStep();
 * @endcode
 *
 * @note the virtual method @c IMixtureModelBase::initializeModel have to be
 * called before any use of the class as it will create/resize the arrays
 * and initialize the constants of the model. If using external arrays, they
 * should be set by using the IMixtureModelBase::setMixtureParameters before
 * the call to IMixtureModelBase::initializeModel.
 */
class IMixtureModelBase : public IModelBase
{
  protected:
    /** default constructor */
    IMixtureModelBase(int nbCluster);
    /** copy constructor. If the pointer on the mixture parameters are not zero
     *  then they are cloned.
     *  @note if the model have not created the parameters, then the pointer are
     *  initialized to 0.
     *  @param model the model to clone
     **/
    IMixtureModelBase( IMixtureModelBase const& model);

  public:
    /** destructor */
    virtual ~IMixtureModelBase();

    /** @return the number of cluster */
    inline int nbCluster() const { return nbCluster_;}
    /** @return the state of the model*/
    inline Clust::modelState state() const { return state_;}
    /** @return the proportions of each mixtures */
    inline CArrayPoint<Real> const* p_prop() const { return p_prop_;};
    /** @return the tik probabilities */
    inline Array2D<Real> const* p_tik() const { return p_tik_;};
    /** @return  the zi class label */
    inline CArrayVector<int> const* p_zi() const { return p_zi_;};

    /** set the state of the model : should be used by any strategy*/
    inline void setState(Clust::modelState state) { state_ = state;}

    // pure virtual
    /** create pattern */
    virtual IMixtureModelBase* create() const = 0;
    /** clone pattern */
    virtual IMixtureModelBase* clone() const = 0;
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
    virtual Real lnComponentProbability(int i, int k) = 0;

    // virtual with default implementation
    /** write the parameters of the model in the stream os. */
    virtual void writeParameters(ostream& os) const {};
    /** compute the number of free parameters of the model.
     *  This method is used in IMixtureModelBase::initializeModel
     *  in order to give a value to IModelBase::nbFreeParameter_.
     *  @return the number of free parameters
     **/
    virtual int computeNbFreeParameters() const = 0;
    /** @brief Initialize the model before at its first use.
     *  This function can be overloaded in derived class for initialization of
     *  the specific model parameters. It should be called prior to any used of
     *  the class. In this interface, the @c initializeModel method
     *  - check if the mixture parameters have been created and, if not, create them,
     *  - set the number of free parameters using the pure virtual function @¢ computeNbFreeParameters()
     **/
    virtual void initializeModel();
    /** First initialization of the parameters of the model.
     *  This method is called in order to initialize the parameters. The
     *  default implementation compute the proportions and call mStep() but
     *  this behavior can be overloaded in derived class if an initial value
     *  is needed by the mStep.
     *  @sa IMixtureModel
     **/
    virtual void initializeStep();

    /** Compute proportions using the ML estimator, default implementation. Set
     *  as virtual in case we impose fixed proportions in derived model.
     **/
    virtual void pStep();
    /** @brief Finalize the estimation of the model.
     * The default behavior is "do nothing".
     **/
    inline virtual void finalizeStep() {}
    /** @brief Impute the missing values.
     *  Default behavior is "do nothing".
     **/
    inline virtual void imputationStep() {}
    /** @brief Simulation of all the latent variables and/or missing data
     *  excluding class labels. Default behavior is "do nothing".
     */
    virtual void samplingStep() {};

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
    /** replace tik by zik. */
    void cStep();
    /** Simulate zi accordingly to tik and replace tik by zik by calling cStep(). */
    void sStep();
    /** compute the zi and the lnLikelihodd of the current estimators (pk and paramk)
     *  and the next value of the tik.
     **/
    void eStep();
    /** Compute zi using the Map estimator. */
    void mapStep();

  protected:
    /** number of cluster. */
    int nbCluster_;
    /** The proportions of each mixtures */
    CArrayPoint<Real>* p_prop_;
    /** The tik probabilities */
    Array2D<Real>* p_tik_;
    /** The zik class label */
    CArrayVector<int>* p_zi_;
    /** Create the mixture model parameters. */
    void createMixtureParameters();
    /** delete  the mixture model parameters. */
    void deleteMixtureParameters();

  private:
    /** state of the model*/
    Clust::modelState state_;
    /** create the proportions p_prop_ */
    void createProp();
    /** create the p_tik_ probabilities array*/
    void createTik();
    /** create the p_zi_ labels array */
    void createZi();
    /** resize p_prop_, p_zi and p_tik_ and initialize them */
    void resizeModel();
    /** resize the proportions and initialize them with equal values*/
    void resizeProp();
    /** resize the tik probabilities array and initialize them with equal values*/
    void resizeTik();
    /** resize the zi labels array and initialize them with equal values */
    void resizeZi();
};

} // namespace STK

#endif /* IMODEL_H_ */
