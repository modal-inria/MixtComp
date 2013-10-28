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

  // manage Gaussian data
  int nbVar;
  mixt::DataHandler dataHandler;
  dataHandler.readDataFromFile(std::string("./data/gaussiandata.csv"),',');
  mixt::Data<double> data(dataHandler);
  data.getData('G',nbVar);

  // create and register mixtures
  mixt::IMixture* gamma = new mixt::Gamma_pk_ajk_bjk ('G', nbClusters, &composerModel);
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

  delete gamma;

  return 0;
}
