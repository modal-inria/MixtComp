#include <iostream>
#include "../src/framework/mixt_CompositeMixtureModel.h"
#include "../src/dataHandling/mixt_Data.h"
#include "../src/mixtures/StkppMixtures/mixt_GammaMixture.h"
// #include "../src/dataHandling/mixt_DataHandler.h"

int main()
{
  mixt::DataHandler dataHandler;
  dataHandler.readDataFromFile(std::string("./data/gammadata.csv"),',');
  std::vector<mixt::MixtureParam> mixtureParams = dataHandler.readMixtureParam(std::string("./data/dataDesc.csv"),',');

  std::cout << "nb variables = " << mixtureParams.size() << std::endl;

  return 0;
}
