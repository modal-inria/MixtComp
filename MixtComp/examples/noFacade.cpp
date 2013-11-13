#include <iostream>
#include "../src/framework/mixt_CompositeMixtureModel.h"
#include "../src/dataHandling/mixt_Data.h"
#include "../src/mixtures/StkppMixtures/mixt_GammaMixture.h"

int main()
{
  int nbClusters = 3;

  // DataHandler creation and initialization
  mixt::DataHandler dataHandler;
  dataHandler.readDataFromFile(std::string("./data/gammadata.csv"),',');

  // creation of the composer model and associated base-type pointer
  mixt::CompositeMixtureModel* composerModel = new mixt::CompositeMixtureModel(&dataHandler, nbClusters, dataHandler.nbSamples());
  STK::IMixtureModelBase* p_composerModel = composerModel;

  // populate the composer
  composerModel->createMixtures(std::string("./data/dataDesc.csv"),',');
  composerModel->setData();

  // create the strategy
  STK::StrategyFacade strategy(p_composerModel);
  strategy.createSemStrategy( STK::Clust::randomInit_ // init type
                            , 2 // number of initialization trials
                            , 20 // number of burn-in iterations
                            , 100 ); // number of iterations

  // run the facade
  strategy.run();

  // write the results
  composerModel->writeParameters(std::cout);

  // delete composer
  delete composerModel;
  return 0;
}
