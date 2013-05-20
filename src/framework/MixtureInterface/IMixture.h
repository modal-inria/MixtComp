#ifndef IMixture_H_
#define IMixture_H_
/**@file IMixture.h
 * @brief
 */
#include <fstream>
#include <vector>
#include "framework/DataHandling/DataHandler.h"
class Model;
class IMixture
{
  public:
    /**Default Constructor */
    IMixture();
    /**Constructor with identification character
     * @param id Identification character of Mixture allocated by framework.
     */
    IMixture(char id);
    /**copy constructor */
    IMixture(IMixture const& mixture);
    /** Setter function: Can be used to set IMixture::p_model_ member  variable.
     *  @param p_model Pointer to the Model being set for IMixture.
     */
    virtual void setModel(Model* p_model);
    /**
     * This function must be defined in derived class for initialization of mixture parameters.
     */
    virtual void initializeStep() = 0;
    /**
     * This is a standard clone function in usual sense. It must be defined to provide new object of your class
     * with values of various parameters equal to the values of calling object.
     * In other words, this is equivalent to polymorphic copy constructor.
     * @return New instance of class as that of calling object.
     */
    virtual IMixture* clone() = 0;
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
    virtual void writeParameters(std::ostream& out) const {};

    /**
     * This function is used  to set IMixture::id_.
     * @param id Identification character.
     */
    void setID(char id);
    /**
     * Virtual destructor.
     */
    virtual ~IMixture(){};

  protected:
    int nbVariable_;
    char id_;
    Model * p_model_;

    //protected functions
    /**
     * This function can be used in derived classes to get number of clusters.
     * @return Number of clusters.
     */
    int nbCluster() const;
    /**
     * This function can be used in derived classes to get number of samples.
     * @return Number of samples.
     */
    int nbSample() const;
    /**
     * This function can be used in derived classes to get class labels from the framework.
     * @return Pointer to class labels.
     */
    int* classLabels() const;
    /**
     * This function can be used in derived classes to get proportions from the framework.
     * @return Pointer to proportions.
     */
    double* proportions() const;

};

inline void IMixture::setID(char id){
  id_ = id;
}

inline void IMixture::setModel(Model* model){
  p_model_ = model;
}

#endif /* IMixture_H_ */
