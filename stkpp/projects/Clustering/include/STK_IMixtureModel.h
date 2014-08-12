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

#include "STK_IMixtureModelBase.h"
#include "STK_Clust_Traits.h"
#include "STK_MixtureComponent.h"
#include "Arrays/include/STK_Array1D.h"

namespace STK
{
/**@ingroup Clustering
 * @brief Main interface class for mixture models.
 * At this level we create the array of Components
 *
 * @sa MixtureComponent, IMixtureModelBase, IMultiParameters
 **/
template<class Derived>
class IMixtureModel : public IRecursiveTemplate<Derived>, public IMixtureModelBase
{
  public:
    typedef typename Clust::MixtureModelTraits<Derived>::Array Array;
    typedef typename Clust::MixtureModelTraits<Derived>::Component Component;
    typedef typename Clust::MixtureModelTraits<Derived>::Parameters Parameters;

  protected:
    /** Default constructor.
     * Set the number of cluster and create components with zero pointer on data.
     **/
    IMixtureModel( int nbCluster)
                 : IMixtureModelBase(nbCluster), p_dataij_(0), components_(nbCluster, 0)
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
                 , p_dataij_(model.p_dataij_)
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
    inline IMixtureModel* create() const { return new Derived(this->nbCluster());}
    /** @return a pointer on the current data set */
    inline Array const* p_data() const { return p_dataij_;}
    /** @return the array with the components */
    inline Array1D< Component* > const& components() const { return components_;}
    /** @return a constant reference on the k-th component */
    inline Component const* components(int k) const { return components_[k];}
    /** @return a constant reference on the k-th parameter */
    inline Parameters const* p_param(int k) const { return components_[k]->p_param();}
    /** @brief Initialize the model before its first use.
     *  This function can be overloaded in derived class for initialization of
     *  the specific model parameters. It should be called prior to any used of
     *  the class. In this interface, the @c initializeModel() method
     *  - set the number of samples and variables of the mixture model
     *  - resize the parameters of each component with the range of the variables
     **/
    void initializeModel()
    {
      if (!p_dataij_)
        STKRUNTIME_ERROR_NO_ARG(IMixtureModel::initializeModel,p_data is not set);
      // set dimensions
      this->setNbSample(p_dataij_->sizeRows());
      this->setNbVariable(p_dataij_->sizeCols());
#ifdef MC_DEBUG
        std::cout << "STK::IMixtureModel::initializeModel()" << std::endl;
        std::cout << "\tcomponents_.size(): " << components_.size() << std::endl;
#endif
      // initialize the parameters
      for (int k= components_.firstIdx(); k <= components_.lastIdx(); ++k)
      {
#ifdef MC_DEBUG
        std::cout << "\tk: " << k << std::endl;
        std::cout << "\tcomponents_[k]->p_param(): " << components_[k]->p_param() << std::endl;
#endif
        components_[k]->p_param()->resize(p_dataij_->cols());
      }
    }
    /** set a new data set.
     *  @param data the data set to set*/
    void setData(Array const& data)
    {
      p_dataij_ = &data;
      this->setNbSample(p_dataij_->rows().size());
      this->setNbVariable(p_dataij_->cols().size());
    }
    /** @return the value of the probability of the i-th sample in the k-th component.
     *  @param i,k indexes of the sample and of the component
     **/
    inline Real lnComponentProbability(int i, int k)
    { return components_[k]->computeLnLikelihood(p_dataij_->row(i));}

  protected:
    /** @return the array with the components */
    inline Array1D<Component*>& components() { return components_;}
    /** @return a pointer on the k-th parameter */
    inline Parameters* p_param(int k) { return components_[k]->p_param();}
    /** pointer on the data set */
    Array const* p_dataij_;
    /** Array of the components of the mixture model */
    Array1D< Component* > components_;
};

} // namespace STK

#endif /* STK_IMIXTUREMODEL_H */
