#include <iostream>
#include "framework/facade/MixtCompFacade.h"
#include "mixtures/RankMixture/RankCluster.h"
//#include "mixtures/StkppMixtures/GaussianMixture.h"
//#include "mixtures/StkppMixtures/BernouilliMixture.h"
using namespace std;

int main(){
  //set framework information
  FrameworkInfo myinfo;
  myinfo.nbIterations_ = 100;
  myinfo.burnin_ = 20;
  myinfo.nbtry_ = 1;
  myinfo.nbCluster_ = 2;
  myinfo.filesep_ = ' ';
  myinfo.datafilename_ = "./data/words_rank.txt";
  myinfo.modalitiesfilename_ = "./data/words_modality.txt";

  //create object of framework facade
  MixtCompFacade myfacade(myinfo);

  //create various mixtures
  IMixture* rank = new RankCluster('R',myinfo.nbIterations_,myinfo.burnin_);

  //Register various mixtures
  myfacade.registerMixture(*rank);

  //instantiate framework
  if(!myfacade.instantiateFramework())
  {
    std::cerr<<"\n\nCannot instantiate framework.\n";
    exit(0);
  }

  //run the facade
  myfacade.run();

  rank->writeParameters(std::cout);

  delete rank;
  rank = NULL;
  return 0;
}
