#include <iostream>
#include "../src/framework/mixt_CompositeMixtureModel.h"
#include "../src/framework/mixt_Data.h"
#include "../src/mixtures/StkppMixtures/mixt_GammaMixture.h"

using namespace mixt;

int main()
{
  int nbClusters = 3;

  // DataHandler creation and initialization
  DataHandler dataHandler;
  dataHandler.readDataFromFile(std::string("./data/gammadata.csv"),',');

  // creation of the composer model and associated base-type pointer
  CompositeMixtureModel* p_composerModel = new CompositeMixtureModel(nbClusters);
  p_composerModel->createMixtureParameters();
  p_composerModel->setDataHandler(&dataHandler);

  // create and register mixtures
  IMixture* gamma = new Gamma_pk_ajk_bjk ('G', nbClusters, p_composerModel);
  gamma->setData(&dataHandler);
  p_composerModel->registerMixture(gamma);
  p_composerModel->initializeModel();

  // create the strategy
  STK::IMixtureModelBase* p_base = p_composerModel;
  STK::StrategyFacade strategy(p_base);
  strategy.createSemStrategy( STK::Clust::randomInit_ // init type
                            , 2 // number of initialization trials
                            , 20 // number of burn-in iterations
                            , 100 ); // number of iterations

  //run the facade
  strategy.run();

  gamma->writeParameters(std::cout);

  // delete individual mixtures
  delete gamma;
  // delete composer
  delete p_composerModel;
  return 0;
}
