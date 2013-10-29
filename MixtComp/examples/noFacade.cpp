#include <iostream>
#include "../src/framework/mixt_CompositeMixtureModel.h"
#include "../src/framework/mixt_Data.h"
#include "../src/mixtures/StkppMixtures/mixt_GammaMixture.h"

int main()
{
  int nbClusters = 2;

  // creation of the composer model and associated base-type pointer
  mixt::CompositeMixtureModel composerModel(nbClusters);
  STK::IMixtureModelBase* p_composerModel = &composerModel;

  // DataHandler creation and initialization
  mixt::DataHandler dataHandler;
  dataHandler.readDataFromFile(std::string("./data/gaussiandata.csv"),',');

  // create and register mixtures
  mixt::IMixture* gamma = new mixt::Gamma_pk_ajk_bjk ('G', nbClusters, &composerModel);
  gamma->setData(&dataHandler);
  composerModel.registerMixture(gamma);

  // create the strategy
  STK::StrategyFacade strategy(p_composerModel);
  strategy.createSemStrategy( STK::Clust::randomInit_ // init type
                            , 2 // number of initialization trials
                            , 20 // number of burn-in iterations
                            , 100 ); // number of iterations

  //run the facade
  strategy.run();

  gamma->writeParameters(std::cout);

  // delete individual mixtures
  delete gamma;

  return 0;
}
