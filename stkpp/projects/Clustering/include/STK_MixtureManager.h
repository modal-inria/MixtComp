/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2014  Serge Iovleff

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
 * created on: 15 mars 2014
 * Author:   iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 **/

/** @file STK_MixtureManager.h
 *  @brief In this file .
 **/


#ifndef STK_MIXTUREMANAGER_H
#define STK_MIXTUREMANAGER_H

#include "STK_Clust_Util.h"
#include "./MixturesBridges/STK_Bridges.h"


namespace STK
{

template<class DataHandler>
class MixtureManager
{
  public:
    typedef typename DataHandler::InfoMap InfoMap;
    typedef std::vector<IMixture*>::const_iterator ConstMixtIterator;
    typedef std::vector<IMixture*>::iterator MixtIterator;
   /** */
    MixtureManager(DataHandler const& handler) : handler_(handler) {}
    /** */
    void createMixtures(MixtureComposer& composer, int nbCluster)
    {
      for (typename InfoMap::const_iterator it=handler_.info().begin(); it!=handler_.info().end(); ++it)
      {
        std::string idName = it->first;
        std::string model= it->second;
        Clust::Mixture idModel = Clust::stringToMixture(model);
        // get a mixture fully
        IMixture* p_mixture = createMixture(idModel, idName, nbCluster);
        if (p_mixture) composer.registerMixture(p_mixture);
      }
    }
    /** create a mixture and initialize it*
     *  @param idModel id of the model
     *  @param idName name of the model
     *  @param nbCluster number of cluster of the model
     **/
    IMixture* createMixture(Clust::Mixture idModel, String const& idName, int nbCluster)
    {
      switch (idModel)
      {
        // gamma_ajk_bjk_ model
        case Clust::Gamma_ajk_bjk_:
        {
          GammaBridge_ajk_bjk* p_mixture = new GammaBridge_ajk_bjk(idName, nbCluster);
          p_mixture->setData(this);
          p_mixture->initializeMixture(this);
          return p_mixture;
        }
        break;
        // gamma_ajk_bj_ model
        case Clust::Gamma_ajk_bj_:
        {
          GammaBridge_ajk_bj* p_mixture = new GammaBridge_ajk_bj(idName, nbCluster);
          p_mixture->setData(this);
          p_mixture->initializeMixture(this);
          return p_mixture;
        }
        break;
        // Gaussian_sjk_ model
        case Clust::Gaussian_sjk_:
        {
          GaussianBridge_sjk* p_mixture = new GaussianBridge_sjk(idName, nbCluster);
          p_mixture->setData(this);
          p_mixture->initializeMixture(this);
          return p_mixture;
        }
        break;
        // Gaussian_sk_ model
        case Clust::Gaussian_sk_:
        {
          GaussianBridge_sk* p_mixture = new GaussianBridge_sk(idName, nbCluster);
          p_mixture->setData(this);
          p_mixture->initializeMixture(this);
          return p_mixture;
        }
        break;
        // Gaussian_sj_ model
        case Clust::Gaussian_sj_:
        {
          GaussianBridge_sj* p_mixture = new GaussianBridge_sj(idName, nbCluster);
          p_mixture->setData(this);
          p_mixture->initializeMixture(this);
          return p_mixture;
        }
        break;
        // Gaussian_s_ model
        case Clust::Gaussian_s_:
        {
          GaussianBridge_s* p_mixture = new GaussianBridge_s(idName, nbCluster);
          p_mixture->setData(this);
          p_mixture->initializeMixture(this);
          return p_mixture;
        }
        // Categorical_pjk_ model
        case Clust::Categorical_pjk_:
        {
          CategoricalBridge_pjk* p_mixture = new CategoricalBridge_pjk(idName, nbCluster);
          p_mixture->setData(this);
          p_mixture->initializeMixture(this);
          return p_mixture;
        }
        // Categorical_pjk_ model
        case Clust::Categorical_pk_:
        {
          CategoricalBridge_pk* p_mixture = new CategoricalBridge_pk(idName, nbCluster);
          p_mixture->setData(this);
          p_mixture->initializeMixture(this);
          return p_mixture;
        }
        break;
        default:
          return 0; // 0 if idModel is not implemented
          break;
      }
      return 0; // 0 if idModel is not a stk++ model
    }
    /** Get data from DataHandler
     *  @param idName name of the model
     *  @param data set to fill
     *  @param nbVariable number of variable of the model
     **/
    template<typename Data>
    inline void getData(std::string const& idName, Data& data, int& nbVariable) const
    { handler_.getData(idName, data, nbVariable);}

    /** initialize Gamma_ajk_bjk_ mixture.
     *  @param mixture the Gamma_ajk_bjk_ mixture to initialize
     **/
    inline void initializeMixture( Clust::MixtureTraits<Clust::Gamma_ajk_bjk_>::Mixture& mixture) const
    {}
    /** initialize Gamma_ajk_bj mixture.
     *  @param mixture the Gamma_ajk_bj mixture to initialize
     **/
    inline void initializeMixture(Clust::MixtureTraits<Clust::Gamma_ajk_bj_>::Mixture& mixture) const
    {}
    /** initialize Gaussian_sjk_ mixture.
     *  @param mixture the Gaussian_sjk_ mixture to initialize
     **/
    inline void initializeMixture(Clust::MixtureTraits<Clust::Gaussian_sjk_>::Mixture& mixture) const
    {}
    /** initialize Gaussian_sj_ mixture.
     *  @param mixture the Gaussian_sj_ mixture to initialize
     **/
    inline void initializeMixture(Clust::MixtureTraits<Clust::Gaussian_sj_>::Mixture& mixture) const
    {}
    /** initialize Gaussian_sk_ mixture.
     *  @param mixture the Gaussian_sk_ mixture to initialize
     **/
    inline void initializeMixture(Clust::MixtureTraits<Clust::Gaussian_sk_>::Mixture& mixture) const
    {}
    /** initialize Gaussian_s_ mixture.
     *  @param mixture the Gaussian_s_ mixture to initialize
     **/
    inline void initializeMixture(Clust::MixtureTraits<Clust::Gaussian_s_>::Mixture& mixture) const
    {}
    /** initialize Categorical_pjk_ mixture.
     *  @param mixture the Categorical_pjk_ mixture to initialize
     **/
    inline void initializeMixture(Clust::MixtureTraits<Clust::Categorical_pjk_>::Mixture& mixture) const
    {}
    /** initialize Categorical_pk_ mixture.
     *  @param mixture the Categorical_pk_ mixture to initialize
     **/
    inline void initializeMixture(Clust::MixtureTraits<Clust::Categorical_pk_>::Mixture& mixture) const
    {}

  private:
    /** pointer to the dataHandler */
    DataHandler const& handler_;
};

} // namespace STK

#endif /* STK_MIXTUREMANAGER_H */
