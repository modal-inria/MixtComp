#include <iostream>
#include "../src/framework/mixt_CompositeMixtureModel.h"
#include "../src/framework/mixt_Data.h"
#include "../src/mixtures/StkppMixtures/mixt_GammaMixture.h"

int main()
{
  int nbVar = 0;

  mixt::DataHandler dataHandler;
//  dataHandler.readDataFromFile(std::string("./data/gammadata.csv"),',');
  dataHandler.readDataFromFile(std::string("./data/gammadata.csv"),',');
  mixt::Data<double> data(&dataHandler);

  STK::Array2D<double> array;
  array.move(data.getData('G',nbVar));

  return 0;
}
