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
 * created on: 14 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/


#ifndef STK_IMIXTURE_H
#define STK_IMIXTURE_H

/**@file STK_IMixture.h
 * @brief define the main interface for linking specific mixture model to the
 * composer.
 */
#include "../include/STK_MixtureComposer.h"

namespace STK
{

template<class Type, int SizeCols_, bool Orient_ > class CArrayPoint;
template<class Type> class Array2D;
template<class Type, int SizeCols_, bool Orient_ > class CArrayVector;

class IMixture
{
  public:
    /**Constructor with identification character
     * @param idName Identification string of Mixture allocated by framework.
     * @param nbCluster number of cluster
     */
    IMixture( std::string const& idName, int nbCluster);
    /**copy constructor.
     * @note The pointer on the composer is not copied and is set to 0: it have
     * to be set again.
     * @param mixture the mixture to copy */
    IMixture( IMixture const& mixture);
    /** Virtual destructor. */
    virtual ~IMixture();

    /** return the Id of the mixture */
    inline std::string const& idName() const { return idName_;}
    /** @return Number of cluster. */
    inline int nbCluster() const  { return nbCluster_;}
    /** @return Number of cluster. */
    inline MixtureComposer const* const p_composer() const { return p_composer_;}

    /** set the mixture composer to the mixture */
    void setMixtureComposer( MixtureComposer const* p_model);

    /**This is a standard clone function in usual sense. It must be defined to
     * provide new object of your class with values of various parameters equal
     * to the values of calling object. In other words, this is equivalent to
     * polymorphic copy constructor.
     * @return New instance of class as that of calling object.
     */
    virtual IMixture* clone() const  = 0;
    /**This is a standard create function in usual sense. It must be defined to
     * provide new object of your class with correct behavior.
     * In other words, this is equivalent to virtual constructor.
     * @return New instance of class as that of calling object.
     */
    virtual IMixture* create() const  = 0;
    /** @brief This function must be defined in derived class for initialization
     *  of the mixture parameters.
     *  This method should create any container needed by the model, resize
     *  them and initialize them.
     *  Since this method can be used when create is called, its main
     *  purpose should be to reset the mixture parameters, while leaving the
     *  data unchanged.
     *  This function will be called once the model is created and data is set.
     */
    virtual void initializeStep() = 0;
    /** @brief This function should be used in order to initialize randomly the
     *  parameters of the mixture.
     */
    virtual void randomInit() = 0;
    /** @brief This function should be used for Imputation of data.
     *  The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep() {/**Do nothing by default*/}
    /** @brief This function must be defined for simulation of all the latent
     * variables and/or missing data excluding class labels. The class labels
     * will be simulated by the framework itself because to do so we have to
     * take into account all the mixture laws.
     */
    virtual void samplingStep() {/**Do nothing by default*/};
    /** @brief This function is equivalent to mStep and must be defined to update
     *  parameters.
     */
    virtual void paramUpdateStep() = 0;
    /** @brief This function should be used to store any intermediate results
     * during various iterations after the burn-in period.
     * @param iteration Provides the iteration number beginning after the burn-in
     * period.
     */
    virtual void storeIntermediateResults(int iteration) {/**Do nothing by default*/}
    /** @brief This step can be used by developer to finalize any thing. It will
     *  be called only once after we finish running the estimation algorithm.
     */
    virtual void finalizeStep() {/**Do nothing by default*/}
    /** This function must be defined to return the posterior probability (PDF)
     * for corresponding sample_num and Cluster_num.
     * @param sample_num Sample number
     * @param Cluster_num Cluster number
     * @return the value of component probability in log scale
     */
    virtual double lnComponentProbability(int sample_num, int Cluster_num) = 0;
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const = 0;
    /** This function must return the number of variables (columns).
     *  @return Number of variables
     */
    virtual int nbVariable() const = 0;
    /** This function can be used to write summary of parameters on to the output stream.
     *  @param out Stream where you want to write the summary of parameters.
     */
    inline virtual void writeParameters(std::ostream& out) const
    {
#ifdef STK_MIXTURE_DEBUG
    stk_cout<< _T("You need to override this method in your mixture!");
#endif
    }

  protected:
    /** This function can be used in derived classes to get number of samples.
     *  @return Number of samples.
     */
    int nbSample() const;
    /** This function can be used in derived classes to get class labels from the framework.
     * @return Pointer to class labels.
     */
    int const* classLabels() const;
    /** This function can be used in derived classes to get proportions from the framework.
     *  @return Pointer to proportions.
     */
    Real const* proportions() const;
    /** This function can be used in derived classes to get the posterior probabilities
     *  from the framework.
     *  @return Pointer to tik.
     */
    Real const** posteriorProbabilities() const;
    /** This function can be used in derived classes to get proportions from the framework.
     *  @return Pointer to proportions.
     */
    CArrayPoint<Real> const* p_pk() const;
    /** This function can be used in derived classes to get posterior probabilities from the framework.
     *  @return Pointer to tik.
     */
    Array2D<Real> const* p_tik() const;
    /** This function can be used in derived classes to get class labels from the framework.
     *  @return Pointer to zi.
     */
    CArrayVector<int> const* p_zi() const;

  private:
    /** pointer on the main composer model */
    const MixtureComposer* p_composer_;
    /** Id name of the mixture */
    std::string idName_;
    /** number of cluster */
    int nbCluster_;
};

} // namespace STK

#endif /* STK_IMIXTURE_H */
