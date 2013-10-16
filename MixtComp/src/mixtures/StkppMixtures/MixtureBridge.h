/** @file MixtureBridge.h
 *  @brief In this file .
 **/

#ifndef MIXTUREBRIDGE_H
#define MIXTUREBRIDGE_H

#include "../mixtureInterface/IMixture.h"
#include "../stkpp/include/STKpp.h"

/** @brief Implementation of the IMixture interface for stk++ mixtures.
 * @tparam MixtureModel is any model deriving from the STK::IMixtureModelBase class
 */
template<class MixtureModel>
class MixtureBridge: public IMixture
{
  public:
    /** constructor. @param id id of the mixture */
    MixtureBridge( char id, int nbCluster, mixt::CompositeMixtureModel const* p_model )
                   : IMixture(id, nbCluster, p_model)
                   , model_(nbCluster)
    {}

    /** @brief Initialize the model before at its first use. */
    virtual void initializeModel()
    { model_.initializeModel();};

    /** This function must be defined in derived class for initialization of mixture parameters.
     */
    virtual void initializeStep()
    { model_.initializeStep();}
    /**
     * This is a standard clone function in usual sense. It must be defined to provide new object of your class
     * with values of various parameters equal to the values of calling object.
     * In other words, this is equivalent to polymorphic copy constructor.
     * @return New instance of class as that of calling object.
     */
    virtual IMixture* clone() = 0;

    /** This function must be defined in derived class to provide copy semantics.
     * @param other Constant reference to the object that is being copied.
     */
    virtual void copy(const IMixture& other) = 0;
    /** This function should be used for imputation of data.
     *  The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep()
    { model_.imputationStep();}
    /**
     * This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep() {}
    /** This function is equivalent to Mstep and must be defined to update parameters.
     */
    virtual void paramUpdateStep()
    { model_.mStep();}
    /**
     * This function should be used to store any intermediate results during various iterations a
     * fter the burn-in period.
     * @param iteration Provides the iteration number beginning after the burn-in period.
     */
    virtual void storeIntermediateResults(int iteration) {/**Do nothing by default*/}
    /**
     * This step can be used by developer to finalize any thing. It will be called only once after we
     * finish running the SEM-gibbs algorithm.
     */
    virtual void finalizeStep()
    { model_.finalizeStep();}
    /**
     * This function must be defined to return the posterior probability (PDF)
     * for corresponding sample_num and Cluster_num.
     * @param sample_num Sample number
     * @param Cluster_num Cluster number
     * @return
     */
    virtual double lnComponentProbability(int sample_num,int cluster_num)
    { return model_.lnComponentProbability(sample_num, cluster_num);}

    /** This must be defined to return the current log-likelihood.
     * @return Current log-likelihood.
     */
    virtual double lnLikelihood()
    { return model_.lnLikelihood();}
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameters() const
    { return model_.nbFreeParameters();}
    /** This function must be defined to set the data into your data containers.
     *  To facilitate data handling, framework provide templated functions,
     *  that can be called directly to get the data.
     */
    virtual void setData() = 0;

    /** This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    { model_.writeParameters(out);}

  protected:
    MixtureModel model_;
};

#endif /* MIXTUREBRIDGE_H */
