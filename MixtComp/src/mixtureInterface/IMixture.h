#ifndef IMixture_H_
#define IMixture_H_
/**@file IMixture.h
 * @brief
 */
#include <fstream>
#include <vector>
#include <iostream>

#include "DataHandler.h"

namespace mixt
{
class CompositeMixtureModel;
}

class IMixture
{
  public:
    /**Constructor with identification character
     * @param id Identification character of Mixture allocated by framework.
     */
    IMixture(char id, int nbCluster, mixt::CompositeMixtureModel const* const p_model);
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
    virtual double logComponentProbability(int sample_num,int Cluster_num) = 0;
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
    /**Default Constructor */
    IMixture();
    /**
     * This function can be used in derived classes to get number of samples.
     * @return Number of samples.
     */
    int nbSample() const;
    /**
     * This function can be used in derived classes to get class labels from the framework.
     * @return Pointer to class labels.
     */
    int const* classLabels() const;
    /**
     * This function can be used in derived classes to get proportions from the framework.
     * @return Pointer to proportions.
     */
    double const* proportions() const;

    /**
     * This function can be used in derived classes to get class labels from the framework.
     * @return Pointer to class labels.
     */
    int const* classLabels() const;
    /**
     * This function can be used in derived classes to get proportions from the framework.
     * @return Pointer to proportions.
     */
    STK::Array2DPoint<STK::Real> const* const p_prop() const;
    /**
     * This function can be used in derived classes to get posterior probabilities from the framework.
     * @return Pointer to tik.
     */
    STK::Array2D<STK::Real> const* const p_tik() const;
    /**
     * This function can be used in derived classes to get class labels from the framework.
     * @return Pointer to zi.
     */
    STK::Array2DVector<int> const* const p_zi() const;

  private:
    int nbVariable_;
    char id_;

    mixt::CompositeMixtureModel * p_model_;
};

inline void IMixture::setID(char id){
  id_ = id;
}

inline void IMixture::setModel(Model* model){
  p_model_ = model;
}

#endif /* IMixture_H_ */
