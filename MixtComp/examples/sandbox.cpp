#include <iostream>
#include "../src/framework/mixt_CompositeMixtureModel.h"
#include "../src/framework/mixt_Data.h"
#include "../src/mixtures/StkppMixtures/mixt_GammaMixture.h"

int main()
{

  mixt::DataHandler dataHandler;
//  dataHandler.readDataFromFile(std::string("./data/gammadata.csv"),',');
  dataHandler.readDataFromFile(std::string("./data/gammadata.csv"),',');
  mixt::Data<double> data(&dataHandler);

  STK::Array2D<double> array;

  // check raw read values
  std::cout << dataHandler.completeData().var(0).name() << std::endl;
  std::cout << dataHandler.completeData().var(1).name() << std::endl;

  // array.move(data.getData('G',nbVar));

  return 0;
}
