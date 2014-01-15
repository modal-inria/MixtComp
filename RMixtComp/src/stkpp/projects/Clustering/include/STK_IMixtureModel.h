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

/** @file STK_IMixtureModel.h
 *  @brief In this file we define the main class for mixture models.
 **/


#ifndef STK_IMIXTUREMODEL_H
#define STK_IMIXTUREMODEL_H

#include "../../Sdk/include/STK_IRecursiveTemplate.h"
#include "STK_IMixtureModelBase.h"
#include "STK_Clust_Traits.h"
#include "../../Arrays/include/STK_Array1D.h"

namespace STK
{
/**@ingroup Clustering
 * @brief Main interface class for mixture models.
 * At this level we add the array of Components and a
 * pointer on the data set. The components are created in this class.
 *
 * @tparam Array can be any kind of container for the observable variables.
 * It should at least derive from ITContainer and provide an
 * access to a single row. @sa ITContainer, ICArray, IArray2DBase
 *
 * @tparam Components any structure encapsulating the components
 * and deriving from IMixtureComponent.
 * @sa IMixtureComponent, IMultiStatModel, IMultiParameters
 **/
template<class Derived>
class IMixtureModel : public IRecursiveTemplate<Derived>, public IMixtureModelBase
{
  public:
    typedef typename Clust::MixtureTraits<Derived>::Array Array;
    typedef typename Clust::MixtureTraits<Derived>::Component Component;
    typedef typename Clust::MixtureTraits<Derived>::Parameters Parameters;
    using IMixtureModelBase::p_tik;

  protected:
    /** Default constructor.
     * Set the number of cluster and create components with zero pointer on data.
     **/
    IMixtureModel( int nbCluster)
                 : IMixtureModelBase(nbCluster), p_data_(0), components_(nbCluster, 0)
    {
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { components_[k] = new Component();}
    }
    /** copy constructor.
     *  Call the clone method of the Components class.
     *  The pointer on the data set is copied as-is. Just check if you should no
     *  change it on the copied object.
     *  @param model the model to copy
     **/
    IMixtureModel( IMixtureModel const& model)
                 : IMixtureModelBase(model)
                 , p_data_(model.p_data_)
                 , components_(model.components_)
    {
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { components_[k] = model.components_[k]->clone();}
    }

  public:
    /** destructor */
    ~IMixtureModel()
    {
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { if (components_[k]) delete components_[k];}
    }
    /** create pattern.  */
    IMixtureModel* create() const
    { return new Derived(this->nbCluster());}
    /** @return a pointer on the current data set */
    Array const* p_data() const { return p_data_;}
    /** @return the array with the components */
    Array1D< Component* > const& components() const { return components_;}
    /** @return a constant reference on the k-th component */
    Component* const& components(int k) const { return components_[k];}
    /** @brief Initialize the model before its first use.
     *  This function can be overloaded in derived class for initialization of
     *  the specific model parameters. It should be called prior to any used of
     *  the class. In this interface, the @c initializeModel() method
     *  - set the number of variables of the mixture model
     *  - set the range of the samples in the base class
     *  - resize the parameters of each component
     **/
    void initializeModel()
    {
      if (!p_data_)
        STKRUNTIME_ERROR_NO_ARG(IMixtureModel::initializeModel,p_data is not set);
      // set dimensions
      this->setNbSample(p_data_->rows().size());
      this->setNbVariable(p_data_->cols().size());
      // initialize the parameters
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { components_[k]->p_param()->resize(p_data_->cols());}
    }
    /** @brief This function must be defined in derived class for initialization
     *  of the ingredient parameters.
     *  This function will be called once the model is created, data is set and
     *  model initialized.
     */
    void initializeStep()
    { MixtureModelImpl< Array, Parameters>::initializeStep(components_, p_tik());}

    /** set a new data set.
     *  @param data the data set to set*/
    void setData(Array const& data)
    {
      p_data_ = &data;
      this->setNbSample(p_data_->rows().size());
      this->setNbVariable(p_data_->cols().size());
      // update components
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      { components_[k]->setData(p_data_);}
    }
    /** @return the value of the probability of the i-th sample in the k-th component.
     *  @param i index of the sample
     *  @param k index of the component
     **/
    Real lnComponentProbability(int i, int k)
    { return components_[k]->computeLnLikelihood(p_data_->row(i));}

    /** Call static method mStep() implemented by end-user */
    void mStep()
    { MixtureModelImpl< Array, Parameters>::mStep(components_, p_tik());}
    /** use the default static method randomInit() implemented by end-users
     *  get an initial value of the parameters and compute tik and zi.
     **/
    void randomInit()
    { MixtureModelImpl< Array, Parameters>::randomInit(components_);}
    /** Utility function to use in mode debug in order to test that the
     *  model is well initialized. */
    int checkModel() const
    {
      if (!p_data_) { return 1;}
      if ((!this->p_prop_)||(!this->p_tik_)||(!this->p_zi_)) { return 2;}
      if (this->nbSample() != p_data_->sizeRows()) { return 3;}
      if (this->nbVar() != p_data_->sizeCols()) { return 4;}
      if (this->nbSample() != p_tik_->sizeRows()) { return 5;}
      if (this->nbCluster() != p_tik_->sizeCols()) { return 6;}
      return 0;
    }

  protected:
    /** pointer on the data set */
    Array const* p_data_;
    /** Array of the components of the mixture model */
    Array1D< Component* > components_;
};

} // namespace STK

#endif /* STK_IMIXTUREMODEL_H */
