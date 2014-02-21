/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2013 Serge Iovleff

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
 *  @brief In this file we define the bridge between the mixtures and the composer.
 **/

#ifndef STK_MIXTUREBRIDGE_H
#define STK_MIXTUREBRIDGE_H

#include "../STK_Clust_Traits.h"
#include "../STK_IMixture.h"

namespace STK
{

/** @ingroup Clustering
 *  @brief Templated implementation of the IMixture interface allowing
 *  to bridge a stk++ mixture with the composer.
 *
 * @tparam Id is any name of a concrete model deriving from the
 * STK::IMixtureModelBase class
 */
template<int Id>
class MixtureBridge: public IMixture
{
  public:
    // type of Mixture
    typedef typename Clust::MixtureTraits<Id>::Mixture Mixture;
    // data type to set
    typedef typename Clust::MixtureTraits<Id>::Data Data;
    // parameters type to get
    typedef typename Clust::MixtureTraits<Id>::Param Param;
    // type of the data
    typedef typename Clust::MixtureTraits<Id>::Type Type;

    /** constructor.
     *  @param idName id name of the mixture
     *  @param nbCluster number of cluster
     **/
    MixtureBridge( std::string const& idName, int nbCluster)
                 : IMixture( idName, nbCluster)
                 , mixture_(nbCluster), m_dataij_(), nbVariable_(0)
                 , v_missing_()
    { mixture_.setData(m_dataij_);}
    /** copy constructor */
    MixtureBridge( MixtureBridge const& mixture)
                 : IMixture(mixture)
                 , mixture_(mixture.mixture_)
                 , m_dataij_(mixture.m_dataij_)
                 , nbVariable_(mixture.nbVariable_)
                 , v_missing_(mixture.v_missing_)
    { mixture_.setData(m_dataij_);}
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
      p_mixture->mixture_.setData(p_mixture->m_dataij_);
      return p_mixture;
    }
    /** @brief Initialize the model before its use by the composer.
     *  The parameters values are set to their default values if the mixture_ is
     *  newly created. if MixtureBridge::initializeModel is used during a
     *  cloning, mixture class have to take care of the existing values of the
     *  parameters.
     **/
    virtual void initializeModel()
    {
      if (!p_composer())
        STKRUNTIME_ERROR_NO_ARG(MixtureBridge::initializeModel,composer is not set);
      mixture_.setMixtureParameters( p_prop(), p_tik(), p_zi());
      mixture_.initializeModel();
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
      mixture_.setData(m_dataij_);
#ifdef STK_MIXTURE_DEBUG
      stk_cout << "Mixture name= " << this->idName() << "\n";
      stk_cout << "m_dataij_.rows() =" << m_dataij_.rows()<< "\n";
      stk_cout << "m_dataij_.cols() =" << m_dataij_.cols()<< "\n";
//      stk_cout << "m_dataij_ =" << m_dataij_<< "\n";
#endif
    }
    /** This function must be defined in derived class for initialization of mixture parameters. */
    virtual void initializeStep() { mixture_.initializeStep();}
    /** This function must be defined to return the component probability (PDF)
     *  for corresponding sample i and cluster k.
     * @param i Sample number
     * @param k Cluster number
     * @return the log-component probability
     */
    virtual double lnComponentProbability(int i, int k)
    { return mixture_.lnComponentProbability(i, k);}
    /** This function is equivalent to Mstep and must be defined to update
     * parameters.
     */
    virtual void paramUpdateStep() { mixture_.mStep();}
    /** @brief This function should be used in order to initialize randomly the
     *  parameters of the mixture.
     */
    virtual void randomInit() { mixture_.randomInit();};
    /** This function should be used for imputation of data.
     *  The default implementation (in the base class) is to do nothing.
     */
    virtual void imputationStep()
    {
      for(ConstIterator it = v_missing_.begin(); it!= v_missing_.end(); ++it)
      { m_dataij_(it->first, it->second) = mixture_.impute(it->first, it->second);}
    }
    /** This function must be defined for simulation of all the latent variables
     * and/or missing data excluding class labels. The class labels will be
     * simulated by the framework itself because to do so we have to take into
     * account all the mixture laws. do nothing by default.
     */
    virtual void samplingStep()
    {
      for(ConstIterator it = v_missing_.begin(); it!= v_missing_.end(); ++it)
      { m_dataij_(it->first, it->second) = mixture_.sample(it->first, it->second);}
    }
    /** This function must return the number of free parameters.
     *  @return Number of free parameters
     */
    virtual int nbFreeParameter() const { return mixture_.computeNbFreeParameters();}
    /** This function can be used to write summary of parameters to the output stream.
     * @param out Stream where you want to write the summary of parameters.
     */
    virtual void writeParameters(std::ostream& out) const
    { mixture_.writeParameters(out);}
    /** This function can be used in order to the values of the parameters
     *  in an Array2D.
     *  @param param the array with the parameters of the mixture.
     */
    void getParameters(Param& param) const
    { mixture_.getParameters(param);}

  protected:
    /** The Mixture to bridge with the composer */
    Mixture mixture_;
    /** The data set */
    Data m_dataij_;
    /** number of variables in the data set */
    int nbVariable_;
    /** vector with the coordinates of the missing values */
    std::vector<std::pair<int,int> > v_missing_;

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
      Type value;
      int j, old_j = UnknownSize;
      for(ConstIterator it = v_missing_.begin(); it!= v_missing_.end(); ++it)
      {
         j = it->second; // get column
         if (j!=old_j)
         {
           value = mixture_.safeValue(j);
         //  mean = m_dataij_.col(j).safe().mean();
           old_j =j;
         }
         m_dataij_(it->first, it->second) = value;
       }
    }
};

} // namespace mixt

#endif /* MIXTUREBRIDGE_H */
