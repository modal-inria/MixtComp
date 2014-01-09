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


#ifndef STK_MIXTURECOMPOSITER_H
#define STK_MIXTURECOMPOSITER_H

#include <vector>

#include "STK_IMixtureModelBase.h"
#include "../../DManager/include/STK_IDataHandler.h"

namespace STK
{

class IMixture;

/**
 *
 */
class MixtureComposer : public IMixtureModelBase
{
  public:
    typedef IDataHandler::InfoMap InfoMap;
    typedef std::vector<IMixture*>::const_iterator ConstMixtIterator;
    typedef std::vector<IMixture*>::iterator MixtIterator;
    /** Constructor.
     * @param nbCluster number of cluster
     */
    MixtureComposer( int nbCluster);
    /** Constructor.
     * @param nbCluster number of cluster
     * @param nbSample number of sample
     */
    MixtureComposer( int nbCluster, int nbSample);
    /** copy constructor.
     *  @param composer the composer to copy
     */
    MixtureComposer(MixtureComposer const& composer);
    /** The individual ingredient will be deleted there.*/
    virtual ~MixtureComposer();
    /** Create a composer, but reinitialize the ingredients parameters. */
    virtual MixtureComposer* create() const;
    /** Create a clone of the current model, with ingredients parameters preserved. */
    virtual MixtureComposer* clone() const;

    /** @param p_handler the DataHandler to set */
    void setDataHandler(IDataHandler const* p_handler);
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
    virtual Real lnComponentProbability(int i, int k);
    /** write the parameters of the model in the stream os. */
    virtual void writeParameters(ostream& os) const;
    /** @brief compute the number of free parameters of the model.
     *  lookup on the ingredients and sum the nbFreeParameter.
     **/
    virtual int computeNbFreeParameters() const;
    /** @brief Initialize the model before its first use. */
    virtual void initializeModel();
    /**@brief This step can be used by developer to initialize any thing which
     * is not the model. It will be called just before running the estimation
     * algorithm.
     **/
    void initializeStep();
    /** @brief Impute the missing values.
     **/
    virtual void imputationStep();
    /** @brief Simulation of all the latent variables and/or missing data
     *  excluding class labels.
     */
    virtual void samplingStep();
    /**@brief This step can be used by developer to finalize any thing. It will
     *  be called only once after we finish running the estimation algorithm.
     **/
    virtual void finalizeStep();
    /** create the ingredients using DataHandler info.
     * @note if the idModel is not a Clust::Ingredient, it is not an error as
     * other ingredients with other id can have been implemented outside stk++.
     * @sa Clust::Ingredient */
    void createIngredients();
    /** create a specific ingredient.
     *  @param idModel the id of the ingredient we want to create
     *  @param idName the name of the ingredient
     **/
    void createIngredient(Clust::Ingredient idModel, String const& idName);
    /** add an ingredient to the composer */
    void registerIngredient(IMixture* mixture);
    /** call setData for all ingredients */
    void setData();
    /** get data from DataHandler  */
    template<typename Data>
    inline void getData(std::string const& id, Data& data, int& nbVariable) const
    { p_handler_->getData(id, data, nbVariable);}

  protected:
    /** @return a constant pointer on the data handler */
    inline IDataHandler const* p_handler() const { return p_handler_;}
    /** @return a constant reference on the array of mixture */
    inline std::vector<IMixture*> const& v_mixtures() const { return v_mixtures_;}
    /** @brief Create the composer using existing data handler and ingredients.
     * This method is essentially used by the create() method and can be
     * reused in derived classes.
     * @sa MixtureComposerFixedProp
     **/
    void createComposer(IDataHandler const* p_handler, std::vector<IMixture*> const& v_mixtures_);

  private:
    /** pointer to the dataHandler */
    IDataHandler const* p_handler_;
    /** vector of pointers to the mixtures components */
    std::vector<IMixture*> v_mixtures_;
};

/** @brief specialization of the composer for the fixed proportion case.
 **/
class MixtureComposerFixedProp : public MixtureComposer
{
  public:
    /** Constructor.
     * @param nbCluster number of cluster
     */
    inline MixtureComposerFixedProp( int nbCluster) :MixtureComposer(nbCluster) {}
    /** Constructor.
     * @param nbCluster number of cluster
     * @param nbSample number of sample
     */
    inline MixtureComposerFixedProp( int nbCluster, int nbSample)
                                   : MixtureComposer(nbCluster, nbSample) {}
    /** copy constructor.
     *  @param model the model to copy
     */
    inline MixtureComposerFixedProp( MixtureComposer const& model)
                                   : MixtureComposer(model) {}
    /** The individual ingredient will be deleted there.*/
    virtual ~MixtureComposerFixedProp() {}
    /** Create a composer, but reinitialize the ingredients parameters. */
    virtual MixtureComposerFixedProp* create() const;
    /** Create a clone of the current model, with ingredients parameters preserved. */
    virtual MixtureComposerFixedProp* clone() const;
    /** @return the number of free parameters of the mixture */
    int computeNbFreeParameters() const;
    /** overloading of the computePropotions() method.
     * Let them initialized to 1/K. */
    virtual void pStep();
};

} /* namespace STK */

#endif /* STK_MIXTURECOMPOSITER_H */
