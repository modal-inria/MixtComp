/*--------------------------------------------------------------------*/
/*  Copyright (C) Inria 2013-2014

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  Project:    MixtComp
 *  Created on: March 18, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_MIXTUREMANAGER_H
#define MIXT_MIXTUREMANAGER_H

#include "../Mixture/SimpleMixtureBridge/mixt_MixtureBridge.h"
#include "../Composer/mixt_MixtureComposer.h"

namespace mixt
{

template<typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>
class MixtureManager
{
  public:
    typedef typename DataHandler::InfoMap InfoMap;

    MixtureManager(const DataHandler* handler,
                   DataExtractor* p_dataExtractor,
                   const ParamSetter* p_paramSetter,
                   ParamExtractor* p_paramExtractor,
                   Real confidenceLevel,
                   std::string& warnLog) :
      p_handler_(handler),
      p_dataExtractor_(p_dataExtractor),
      p_paramSetter_(p_paramSetter),
      p_paramExtractor_(p_paramExtractor),
      confidenceLevel_(confidenceLevel),
      warnLog_(warnLog)
    {}

    void createMixtures(MixtureComposer& composer,
                        int nbCluster)
    {
      for (typename InfoMap::const_iterator it=p_handler_->info().begin(); it!=p_handler_->info().end(); ++it)
      {
        std::string idName = it->first;
        std::string idModel = it->second;

#ifdef MC_DEBUG
        std::cout << "MixtureManager::createMixtures, "
        		      << "idName: " << idName
				          << ", model: " << model
				          << ", idModel: " << idModel << std::endl;
        std::cout << "composer.p_pk(): " << composer.p_pk()
                  << ", *composer.p_pk(): " << *composer.p_pk() << std::endl;
#endif
        // get a mixture fully
        IMixture* p_mixture = createMixture(idModel,
                                            idName,
                                            composer,
                                            nbCluster,
                                            confidenceLevel_);
        if (p_mixture) composer.registerMixture(p_mixture);
      }
    }

    /** create a mixture and initialize it*
     *  @param idModel id of the model
     *  @param idName name of the model
     *  @param nbCluster number of cluster of the model
     **/
    IMixture* createMixture(std::string const& idModel,
                            std::string const& idName,
                            MixtureComposer& composer,
                            int nbCluster,
                            Real confidenceLevel)
    {
#ifdef MC_DEBUG
      std::cout << "MixtureManager::createMixture" << std::endl;
      std::cout << "idName: " << idName << std::endl;
#endif

      if (idModel == "Categorical_pjk")
      {
        typename CategoricalBridge_pjk_m<DataHandler,
                                         DataExtractor,
                                         ParamSetter,
                                         ParamExtractor>::type* p_bridge = new typename CategoricalBridge_pjk_m<DataHandler,
                                                                                                                DataExtractor,
                                                                                                                ParamSetter,
                                                                                                                ParamExtractor>::type(idName,
                                                                                                                                      nbCluster,
                                                                                                                                      composer.p_zi(),
                                                                                                                                      p_handler_,
                                                                                                                                      p_dataExtractor_,
                                                                                                                                      p_paramSetter_,
                                                                                                                                      p_paramExtractor_,
                                                                                                                                      confidenceLevel);
        return p_bridge;
      }
      else if (idModel == "Gaussian_sjk")
      {
        typename GaussianBridge_sjk_m<DataHandler,
                                      DataExtractor,
                                      ParamSetter,
                                      ParamExtractor>::type* p_bridge = new typename GaussianBridge_sjk_m<DataHandler,
                                                                                                          DataExtractor,
                                                                                                          ParamSetter,
                                                                                                          ParamExtractor>::type(idName,
                                                                                                                                nbCluster,
                                                                                                                                composer.p_zi(),
                                                                                                                                p_handler_,
                                                                                                                                p_dataExtractor_,
                                                                                                                                p_paramSetter_,
                                                                                                                                p_paramExtractor_,
                                                                                                                                confidenceLevel);
        return p_bridge;
      }
      else if (idModel == "Poisson_k")
      {
        typename PoissonBridge_k_m<DataHandler,
                                   DataExtractor,
                                   ParamSetter,
                                   ParamExtractor>::type* p_bridge = new typename PoissonBridge_k_m<DataHandler,
                                                                                                    DataExtractor,
                                                                                                    ParamSetter,
                                                                                                    ParamExtractor>::type(idName,
                                                                                                                          nbCluster,
                                                                                                                          composer.p_zi(),
                                                                                                                          p_handler_,
                                                                                                                          p_dataExtractor_,
                                                                                                                          p_paramSetter_,
                                                                                                                          p_paramExtractor_,
                                                                                                                          confidenceLevel);
        return p_bridge;
      }
      else
      {
        return 0;
      }
    }

  private:
    /** pointer to the dataHandler */
    const DataHandler* p_handler_;

    /** pointer to the dataExtractor */
    DataExtractor* p_dataExtractor_;

    /** pointer to the paramSetter */
    const ParamSetter* p_paramSetter_;

    /** pointer to the parameter extractor */
    ParamExtractor* p_paramExtractor_;

    /** confidence interval, to be transmitted to the mixtures at creation */
    Real confidenceLevel_;

    /** reference to the warnings log */
    std::string& warnLog_;
};

} // namespace mixt

#endif /* MIXT_MIXTUREMANAGER_H */
