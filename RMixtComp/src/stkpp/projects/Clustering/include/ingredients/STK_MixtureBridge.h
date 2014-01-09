/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Vincent KUBICKI

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

    Contact : S..._DOT_I..._AT_stkpp.org (see copyright for ...)
*/

/*
 * Project:  stkpp::Clustering
 * created on: 15 nov. 2013
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 * Originally created by Parmeet Bhatia <b..._DOT_p..._AT_gmail_Dot_com>
 **/

/** @file STK_MixtureBridge.h
 *  @brief In this file .
 **/

#ifndef STK_MIXTUREBRIDGE_H
#define STK_MIXTUREBRIDGE_H

#include "../STK_Clust_Traits.h"
#include "../STK_IMixture.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief Templated implementation of the IMixture interface allowing
 *  to bridge a stk++ ingredient with an Ingredient.
 *
 * @tparam Id is any name of a concrete model deriving from the
 * STK::IMixtureIngredeintBase class
 */
template<int Id>
class MixtureBridge: public IMixture
{
  public:
    // type of Mixture
    typedef typename Clust::IngredientTraits<Id>::Ingredient Ingredient;
    // data type to set
    typedef typename Clust::IngredientTraits<Id>::Data Data;
    // parameters type to get
    typedef typename Clust::IngredientTraits<Id>::Param Param;
    // type of the data
    typedef typename Clust::IngredientTraits<Id>::Type Type;

    /** constructor.
     *  @param idName id name of the mixture
     *  @param nbCluster number of cluster
     **/
    MixtureBridge( std::string const& idName, int nbCluster)
                 : IMixture( idName, nbCluster)
                 , ingredient_(nbCluster), m_dataij_(), nbVariable_(0)
    { ingredient_.setData(m_dataij_);}
    /** copy constructor */
    MixtureBridge( MixtureBridge const& mixture)
                 : IMixture(mixture)
                 , ingredient_(mixture.ingredient_)
                 , m_dataij_(mixture.m_dataij_)
                 , nbVariable_(mixture.nbVariable_)
                 , v_missing_(mixture.v_missing_)
    { ingredient_.setData(m_dataij_);}
    /** This is a standard clone function in usual sense. It must be defined to
     *  provide new object of your class with values of various parameters
     *  equal to the values of calling object. In other words, this is
     *  equivalent to polymorphic copy constructor.
     *  @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* clone() const
    { return new MixtureBridge(*this);}
    /** This is a standard create function in usual sense. It must be defined to
     *  provide new object of your class with correct dimensions and state.
     *  In other words, this is equivalent to virtual constructor.
     * @return New instance of class as that of calling object.
     */
    virtual MixtureBridge* create() const
    {
      MixtureBridge* p_mixture = new MixtureBridge( idName(), nbCluster());
      p_mixture->m_dataij_ = m_dataij_;
      p_mixture->nbVariable_ = nbVariable_;
      p_mixture->v_missing_ = v_missing_;
      // Bug Fix: set the correct data set
      p_mixture->ingredient_.setData(p_mixture->m_dataij_);
      return p_mixture;
    }
    /** @brief Initialize the model before its use by the composer.
     *  The parameters values are set to its default values if the ingredient_ is
     *  newly created. if MixtureBridge::initializeModel is used during a
     *  cloning, model class have to take care of the existing values of the
     *  parameters.
     **/
    virtual void initializeModel()
    {
      if (!p_composer())
        STKRUNTIME_ERROR_NO_ARG(MixtureBridge::initializeModel,composer is not set);
      ingredient_.setMixtureParameters( p_prop(), p_tik(), p_zi());
      ingredient_.initializeModel();
    }
   /** This function will be defined to set the data into your data containers.
    *  To facilitate data handling, framework provide templated functions,
    *  that can be called directly to get the data.
    */
    virtual void setData()
    {
      IMixture::getData<Data>(m_dataij_, nbVariable_ );
      findMissing();
      removeMissing();
      ingredient_.setData(m_dataij_);
#ifdef STK_MIXTURE_DEBUG
      stk_cout << "Ingredient name= " << this->idName() << "\n";
      stk_cout << "m_dataij_.rows() =" << m_dataij_.rows()<< "\n";
      stk_cout << "m_dataij_.cols() =" << m_dataij_.cols()<< "\n";
//      stk_cout << "m_dataij_ =" << m_dataij_<< "\n";
#endif
    }
    /** This function must be defined in derived class for initialization of mixture parameters. */
    virtual void initializeStep() { ingredient_.initializeStep();}
    /** @brief This function should be used in order to initialize randomly the
     *  parameters of the ingredient.
     */
    virtual void randomInit() { ingredient_.randomInit();};
    /** This function should be used for imputation of data.
     *  The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep()
    {
      for(ConstIterator it = v_missing_.begin(); it!= v_missing_.end(); ++it)
      { m_dataij_(it->first, it->second) = ingredient_.impute(it->first, it->second);}
    }
    /** This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep()
    {
      for(ConstIterator it = v_missing_.begin(); it!= v_missing_.end(); ++it)
      { m_dataij_(it->first, it->second) = ingredient_.sample(it->first, it->second);}
    }
    /** This function is equivalent to Mstep and must be defined to update parameters.
     */
    virtual void paramUpdateStep() { ingredient_.mStep();}
    /** This function should be used to store any intermediate results during
     *  various iterations after the burn-in period.
     *  @param iteration Provides the iteration number beginning after the burn-in period.
     */
    virtual void storeIntermediateResults(int iteration) {/**Do nothing by default*/}
    /**
     *  This step can be used by developer to finalize any thing. It will be called only once after we
     * finish running the SEM-gibbs algorithm.
     */
    virtual void finalizeStep() {}
    /**
     * This function must be defined to return the posterior probability (PDF)
     * for corresponding sample i and cluster k.
     * @param i Sample number
     * @param k Cluster number
     * @return the log-component probability
     */
    virtual double lnComponentProbability(int i, int k)
    { return ingredient_.lnComponentProbability(i, k);}
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const { return ingredient_.computeNbFreeParameters();}
    /** This function can be used to write summary of parameters on to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    { ingredient_.writeParameters(out);}
    /** Utility function to use in mode debug in order to test that the
     *  model is well initialized. */
    int checkModel() const { return ingredient_.checkModel();}

  protected:
    /** The ingredient to bridge with the composer */
    Ingredient ingredient_;
    /** The data set */
    Data m_dataij_;
    /** vector with the coordinates of the missing values */
    std::vector<std::pair<int,int> > v_missing_;
    /** number of variables in the data set */
    int nbVariable_;
  private:
    typedef std::vector<std::pair<int,int> >::const_iterator ConstIterator;
    /** utility function for lookup the data set and find missing values
     *  coordinates. */
    void findMissing()
    {
      for (int j=m_dataij_.firstIdxCols(); j<= m_dataij_.lastIdxCols(); ++j)
        for (int i=m_dataij_.firstIdxRows(); i<= m_dataij_.lastIdxRows(); ++i)
        {
          if (Arithmetic<Type>::isNA(m_dataij_(i,j)))
          { v_missing_.push_back(std::pair<int,int>(i,j));}
        }
    }
    /** utility function for lookup the data set and remove missing values
     *  coordinates. */
    void removeMissing()
    {
      Real mean;
      int j, old_j = UnknownSize;
      for(ConstIterator it = v_missing_.begin(); it!= v_missing_.end(); ++it)
      {
         j = it->second; // get column
         if (j!=old_j) { mean = m_dataij_.col(j).meanSafe(); old_j =j;}
         m_dataij_(it->first, it->second) = mean;
       }
    }
};

} // namespace mixt

#endif /* MIXTUREBRIDGE_H */