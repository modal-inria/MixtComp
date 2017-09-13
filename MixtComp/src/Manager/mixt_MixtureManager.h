/* MixtComp version 2.0  - 13 march 2017
 * Copyright (C) Inria - Lille 1 */

/*
 *  Project:    MixtComp
 *  Created on: March 18, 2014
 *  Authors:    Vincent KUBICKI <vincent.kubicki@inria.fr>,
 *              Serge IOVLEFF <serge.iovleff@inria.fr>
 **/

#ifndef MIXT_MIXTUREMANAGER_H
#define MIXT_MIXTUREMANAGER_H

#include "Composer/mixt_MixtureComposer.h"
#include "Mixture/SimpleMixtureBridge/mixt_MixtureBridge.h"
#include "Mixture/Functional/mixt_FunctionalMixture.h"
// #include "Mixture/Ordinal/mixt_Ordinal.h"
#include "Mixture/Rank/mixt_RankMixture.h"

namespace mixt {

template<typename DataHandler,
         typename DataExtractor,
         typename ParamSetter,
         typename ParamExtractor>
class MixtureManager {
  public:
    MixtureManager(
        const DataHandler* handler,
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
      warnLog_(warnLog) {}

    std::string createMixtures(MixtureComposer& composer,
                               Index nbCluster) {
      std::string warnLog;
      Index indexMixture = 0; // index of current variable
      for (std::map<std::string, std::string>::const_iterator it    = p_handler_->info().begin(),
                                                              itEnd = p_handler_->info().end();
           it != itEnd;
           ++it) {
        std::string idName = it->first;
        std::string idModel = it->second;

        if (idModel != "LatentClass") { // LatentClass type is managed directly in the composer
          IMixture* p_mixture = createMixture(
              idModel,
              indexMixture + 1, // +1 to take into account that LatentClass is at position 0 int the data output
              idName,
              composer,
              nbCluster,
              confidenceLevel_);
          if (p_mixture) {
            composer.registerMixture(p_mixture);
            ++indexMixture;
          }
          else {
            std::stringstream sstm;
            sstm << "The model " << idModel << " has been selected to describe the variable " << idName
                 << " but it is not implemented yet. Please choose an available model for this variable." << std::endl;
            warnLog += sstm.str();
          }
        }
      }
      if (indexMixture == 0) {
        std::stringstream sstm;
        sstm << "No valid variable in the input. Please check the descriptor file." << std::endl;
        warnLog += sstm.str();
      }
      return warnLog;
    }

    /** create a mixture and initialize it*
     *  @param idModel id of the model
     *  @param idName name of the model
     *  @param nbCluster number of cluster of the model
     **/
    IMixture* createMixture(
        std::string const& idModel,
        Index indexMixture,
        std::string const& idName,
        MixtureComposer& composer,
        Index nbCluster,
        Real confidenceLevel) {
      if (idModel == "Categorical_pjk") {
        CategoricalBridge_pjk_m<DataHandler,
                                DataExtractor,
                                ParamSetter,
                                ParamExtractor>* p_bridge = new CategoricalBridge_pjk_m<DataHandler,
                                                                                        DataExtractor,
                                                                                        ParamSetter,
                                                                                        ParamExtractor>(indexMixture,
                                                                                                        idName,
                                                                                                        nbCluster,
                                                                                                        composer.p_zi(),
                                                                                                        composer.classInd(),
                                                                                                        p_handler_,
                                                                                                        p_dataExtractor_,
                                                                                                        p_paramSetter_,
                                                                                                        p_paramExtractor_,
                                                                                                        confidenceLevel);
        return p_bridge;
      }
      if (idModel == "Gaussian_sjk") {
        GaussianBridge_sjk_m<DataHandler,
                             DataExtractor,
                             ParamSetter,
                             ParamExtractor>* p_bridge = new GaussianBridge_sjk_m<DataHandler,
                                                                                  DataExtractor,
                                                                                  ParamSetter,
                                                                                  ParamExtractor>(indexMixture,
                                                                                                  idName,
                                                                                                  nbCluster,
                                                                                                  composer.p_zi(),
                                                                                                  composer.classInd(),
                                                                                                  p_handler_,
                                                                                                  p_dataExtractor_,
                                                                                                  p_paramSetter_,
                                                                                                  p_paramExtractor_,
                                                                                                  confidenceLevel);
        return p_bridge;
      }
      if (idModel == "Poisson_k") {
        PoissonBridge_k_m<DataHandler,
                          DataExtractor,
                                   ParamSetter,
                                   ParamExtractor>* p_bridge = new PoissonBridge_k_m<DataHandler,
                                                                                     DataExtractor,
                                                                                     ParamSetter,
                                                                                     ParamExtractor>(indexMixture,
                                                                                                     idName,
                                                                                                     nbCluster,
                                                                                                     composer.p_zi(),
                                                                                                     composer.classInd(),
                                                                                                     p_handler_,
                                                                                                     p_dataExtractor_,
                                                                                                     p_paramSetter_,
                                                                                                     p_paramExtractor_,
                                                                                                     confidenceLevel);
        return p_bridge;
      }
      if (idModel == "Functional") {
        FunctionalMixture<DataHandler,
                          DataExtractor,
                          ParamSetter,
                          ParamExtractor>* p_bridge = new FunctionalMixture<DataHandler,
                                                                            DataExtractor,
                                                                            ParamSetter,
                                                                            ParamExtractor>(indexMixture,
                                                                                            idName,
                                                                                            nbCluster,
                                                                                            composer.p_zi(),
                                                                                            composer.classInd(),
                                                                                            p_handler_,
                                                                                            p_dataExtractor_,
                                                                                            p_paramSetter_,
                                                                                            p_paramExtractor_,
                                                                                            confidenceLevel);
        return p_bridge;
      }
//      if (idModel == "Ordinal") {
//        Ordinal<DataHandler,
//                DataExtractor,
//                ParamSetter,
//                ParamExtractor>* p_bridge = new Ordinal<DataHandler,
//                                                        DataExtractor,
//                                                        ParamSetter,
//                                                        ParamExtractor>(indexMixture,
//                                                                        idName,
//                                                                        nbCluster,
//                                                                        composer.p_zi(),
//                                                                        composer.classInd(),
//                                                                        p_handler_,
//                                                                        p_dataExtractor_,
//                                                                        p_paramSetter_,
//                                                                        p_paramExtractor_,
//                                                                        confidenceLevel);
//        return p_bridge;
//      }
      if (idModel == "Rank") {
        RankMixture<DataHandler,
                    DataExtractor,
                    ParamSetter,
                    ParamExtractor>* p_bridge = new RankMixture<DataHandler,
                                                                DataExtractor,
                                                                ParamSetter,
                                                                ParamExtractor>(indexMixture,
                                                                                idName,
                                                                                nbCluster,
                                                                                composer.p_zi(),
                                                                                composer.classInd(),
                                                                                p_handler_,
                                                                                p_dataExtractor_,
                                                                                p_paramSetter_,
                                                                                p_paramExtractor_,
                                                                                confidenceLevel);
        return p_bridge;
      }

      return 0;
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
