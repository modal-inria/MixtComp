/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

#ifndef IMIXTURE_H
#define IMIXTURE_H

/**@file IMixture.h
 * @brief define the main interface for linking specific mixture model to the
 * composer.
 */

#include <iostream>

#include "../framework/mixt_DataHandler.h"
#include "../stkpp/include/Arrays.h"

namespace mixt
{
class CompositeMixtureModel;

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
    virtual void setModel(mixt::CompositeMixtureModel* p_model);
    /** @brief Initialize the model before its first use. Will be called after
     *  setData().
     *  This method should create any container needed by the model and/or resize
     *  them.
     *  Since this method can be used when create is called(), its main purpose should
     *  be to reset the mixture parameters, while leaving the data unchanged. */
    virtual void initializeModel() = 0;
    /**
     * @brief This function must be defined in derived class for initialization
     * of the ingredient parameters.
     * This function will be called once the model is created, data is set and
     * model initialized.
     */
    virtual void initializeStep() = 0;
    /**This is a standard clone function in usual sense. It must be defined to
     * provide new object of your class with values of various parameters equal
     * to the values of calling object. In other words, this is equivalent to
     * polymorphic copy constructor.
     * @return New instance of class as that of calling object.
     */
    virtual IMixture* clone() = 0;

    /**This is a standard create function in usual sense. It must be defined to
     * provide new object of your class with correct
     * . In other words, this is equivalent to
     * virtual constructor.
     * @return New instance of class as that of calling object.
     */
//    virtual IMixture* create() = 0;

    /** This function should be used for Imputation of data.
     * The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep() {/**Do nothing by default*/}
    /**
     * This function must be defined for simulation of all the latent variables and/or missing data
     * excluding class labels. The class labels will be simulated by the framework itself because to do so
     * we have to take into account all the mixture laws.
     */
    virtual void samplingStep() {/**Do nothing by default*/};
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
    virtual double lnComponentProbability(int sample_num, int Cluster_num) = 0;
    /**
     * This must be defined to return the current log-likelihood.
     * @return Current log-likelihood.
     */
    virtual double lnLikelihood() = 0;
    /**
     * This function must return the number of free parameters.
     * @return Number of free parameters
     */
    virtual int nbFreeParameters() const = 0;
    /**
     * This function must be defined to set the data into your data containers.
     * To facilitate data handling, framework provide templated functions,
     * that can be called directly to get the data.
     */
    virtual void setData(DataHandler* dataHandler = NULL) = 0;
    /**
     * This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    { std::cout<<"You need to override this method in your mixture!";}
    /** Virtual destructor. */
    virtual ~IMixture(){};

  protected:
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
    /** This function can be used in derived classes to get proportions from the framework.
     *  @return Pointer to proportions.
     */
    double const* proportions() const;
    /** This function can be used in derived classes to get the posterior probabilities
     *  from the framework.
     *  @return Pointer to tik.
     */
    double const** posteriorProbabilities() const;
    /**
     * This function can be used in derived classes to get proportions from the framework.
     * @return Pointer to proportions.
     */
    STK::CArrayPoint<STK::Real> const* p_prop() const;
    /**
     * This function can be used in derived classes to get posterior probabilities from the framework.
     * @return Pointer to tik.
     */
    STK::Array2D<STK::Real> const* p_tik() const;
    /**
     * This function can be used in derived classes to get class labels from the framework.
     * @return Pointer to zi.
     */
    STK::CArrayVector<int> const* p_zi() const;
  protected:
    int nbVariable_;
    char id_;
  private:
    /** Default Constructor not allowed */
    IMixture();
    /** pointer on the main composer model */
    const mixt::CompositeMixtureModel * p_compositeModel_;
};

} // namespace mixt

#endif /* IMIXTURE_H */
