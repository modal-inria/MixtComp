/** @file MixtureBridge.h
 *  @brief In this file .
 **/

#ifndef MIXTUREBRIDGE_H
#define MIXTUREBRIDGE_H

#include "../../framework/MixtureInterface/IMixture.h"
#include "../../framework/stkpp/include/STKpp.h"

/** @brief Implementation of the IDevelopper interface for stk++ mixtures.
 */
template<class MixtureModel>
class MixtureBridge: public IMixture
{
  public:
    /** constructor. @param id id of the mixture */
    MixtureBridge(char id, int nbCluster ) : IMixture(id), model_(nbCluster) {}
    /**
     * This function must be defined in derived class for initialization of mixture parameters.
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

    /**
     * This function must be defined in derived class to provide copy semantics.
     * @param other Constant reference to the object that is being copied.
     */
    virtual void copy(const IMixture& other) = 0;
    /**
     * This function should be used for Imputation of data.
     * The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep() {/**Do nothing by default*/}
    /**
     * This function must be defined for simulation of all the latent variables and/or missing data
     * excluding class labels. The class labels will be simulated by the framework itself because to do so
     * we have to take into account all the mixture laws.
     */
    virtual void samplingStep() = 0;
    /**
     * This function is equivalent to Mstep and must be defined to update parameters.
     */
    virtual void paramUpdateStep() = 0;
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
    virtual void finalizeStep() {/**Do nothing by default*/}
    /**
     * This function must be defined to return the posterior probability (PDF)
     * for corresponding sample_num and Cluster_num.
     * @param sample_num Sample number
     * @param Cluster_num Cluster number
     * @return
     */
    virtual double posteriorProbability(int sample_num,int Cluster_num) = 0;
    /**
     * This must be defined to return the current log-likelihood.
     * @return Current log-likelihood.
     */
    virtual double logLikelihood() = 0;
    /**
     * This function must return the number of free parameters.
     * @return Number of free parameters
     */
    virtual int freeParameters() const = 0;
    /**
     * This function must be defined to set the data into your data containers.
     * To facilitate data handling, framework provide templated functions,
     * that can be called directly to get the data.
     */
    virtual void setData() = 0;
    /**
     * This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    {
     std::cout<<"You need to override this method in your mixture!";
    }


  protected:
    MixtureModel model_;

};

#endif /* MIXTUREBRIDGE_H */
