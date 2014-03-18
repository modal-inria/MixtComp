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
#include "Mixtures/STK_Mixtures.h"


namespace STK
{

template<class DataHandler>
class MixtureManager
{
  public:
    typedef IDataHandler::InfoMap InfoMap;
    typedef std::vector<IMixture*>::const_iterator ConstMixtIterator;
    typedef std::vector<IMixture*>::iterator MixtIterator;
   /** */
    MixtureManager(DataHandler const& handler) : handler_(handler) {}
    /** */
    void createMixtures(MixtureComposer& composer, int nbCluster)
    {
      for (InfoMap::const_iterator it=handler_.info().begin(); it!=handler_.info().end(); ++it)
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
          MixtureGamma_ajk_bjk* p_mixture = new MixtureGamma_ajk_bjk(idName, nbCluster);
          p_mixture->setData(this);
          return p_mixture;
        }
        break;
        // gamma_ajk_bj_ model
        case Clust::Gamma_ajk_bj_:
        {
          MixtureGamma_ajk_bj* p_mixture = new MixtureGamma_ajk_bj(idName, nbCluster);
          p_mixture->setData(this);
          return p_mixture;
        }
        break;
        // Gaussian_sjk_ model
        case Clust::Gaussian_sjk_:
        {
          MixtureGaussian_sjk* p_mixture = new MixtureGaussian_sjk(idName, nbCluster);
          p_mixture->setData(this);
          return p_mixture;
        }
        break;
        // Gaussian_sk_ model
        case Clust::Gaussian_sk_:
        {
          MixtureGaussian_sk* p_mixture = new MixtureGaussian_sk(idName, nbCluster);
          p_mixture->setData(this);
          return p_mixture;
        }
        break;
        // Gaussian_sj_ model
        case Clust::Gaussian_sj_:
        {
          MixtureGaussian_sk* p_mixture = new MixtureGaussian_sk(idName, nbCluster);
          p_mixture->setData(this);
          return p_mixture;
        }
        break;
        // Gaussian_s_ model
        case Clust::Gaussian_s_:
        {
          MixtureGaussian_s* p_mixture = new MixtureGaussian_s(idName, nbCluster);
          p_mixture->setData(this);
          return p_mixture;
        }
        break;
        default:
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

  private:
    /** pointer to the dataHandler */
    DataHandler const& handler_;
};

} // namespace STK

#endif /* STK_MIXTUREMANAGER_H */
